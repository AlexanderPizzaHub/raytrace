#include "levelset.hpp"
#include <array>
#include <iostream>
#include <cmath>
#include <fstream>

namespace LevelSet
{

#pragma region RefArea

    RefArea::RefArea(Const::vecDd &center, scalar radius, Const::vecDd &normal)
        : Line(center, radius, normal)
    {
        weightstore_ = 0.0;
        rate_ = 0.0;
    }

    RefArea::RefArea(Const::vecDd &start, Const::vecDd &end)
        : Line(start, end)
    {
        weightstore_ = 0.0;
        rate_ = 0.0;
    }

    RefArea::~RefArea()
    {
    }

    scalar RefArea::Intersect(Ray &ray)
    {
        scalar dt_ = -(normal_[0] * (ray.pos_[0] - center_[0]) + normal_[1] * (ray.pos_[1] - center_[1])) / (normal_[0] * ray.dir_[0] + normal_[1] * ray.dir_[1]);
        Const::vecDd hitPoint = {ray.pos_[0] + dt_ * ray.dir_[0], ray.pos_[1] + dt_ * ray.dir_[1]};
        if (dt_ < 0 || dt_ > Const::MAXIMUM_LIGHT_TIME ||
            (hitPoint[0] - center_[0]) * (hitPoint[0] - center_[0]) +
                    (hitPoint[1] - center_[1]) * (hitPoint[1] - center_[1]) >
                radius_ * radius_)
        {
            return -1.0; // no hit
        }
        else
        {
            return dt_;
        }
    }

    void RefArea::WeightedHit(scalar dWeight)
    {
        // std::cout << "hit " << dWeight  << std::endl;
        weightstore_ += dWeight;
        // std::cout <<getweightstore() <<std::endl;
    }

    /*
    void RefArea::WeightedHit(scalar dWeight, Const::vecDd position)
    {
        //TODO
    }
    */

    void RefArea::SetWeight(scalar weight)
    {
        weightstore_ = weight;
    }

    void RefArea::WeightAdd(scalar dweight)
    {
        weightstore_ += dweight;
    }

    scalar RefArea::getweightstore()
    {
        return weightstore_;
    }

    Const::vecDd RefArea::getnormal()
    {
        return normal_;
    }

    void RefArea::AddRate(scalar rate)
    {
        rate_ += rate;
    }

    void RefArea::SetRate(scalar rate)
    {
        rate_ = rate;
    }

    scalar RefArea::getrate()
    {
        return rate_;
    }

#pragma endregion

#pragma region LevelSetFunction
    LevelSetFunction::LevelSetFunction(hrle::HRLE *hrle)
    {
        hrle_ = hrle;
        marchingcube_ = new MarchingCube2D();
        velocityfield_.resize(hrle_->GetNumActiveGrids(), 0.0);
        levelsetfunction_.resize(hrle_->GetNumActiveGrids(), 0.0);
        // std::cout<<"lsf constructed "<<meshptr_->getnumGrids()<<std::endl;
    }

    LevelSetFunction::~LevelSetFunction()
    {
    }

    void LevelSetFunction::Evolve(scalar dt)
    {
        // std::cout << "evolve" << std::endl;
        label numgrids_active = levelsetfunction_.size();
        for (label i = 0; i < numgrids_active; i++)
        {
            // std::cout << "velocity" << velocityfield_[i] << std::endl;
            levelsetfunction_[i] -= velocityfield_[i] * dt;
        }
    }

    void LevelSetFunction::ClearWeights()
    {
        for (RefArea refarea : refareas_)
        {
            refarea.SetWeight(0.0);
        }
    }

    void LevelSetFunction::ClearVelocity()
    {
        for (label i = 0; i < velocityfield_.size(); i++)
        {
            velocityfield_[i] = 0.0;
        }
    }

    void LevelSetFunction::VelocityExtension()
    {
        for (RefArea refarea : refareas_)
        {
            Square& square = *refarea.linked_square_.at(0);

            Const::vecDd center = refarea.getcenter();
            label gridul = square.linked_grids_[0];
            label gridll = square.linked_grids_[1];
            label gridlr = square.linked_grids_[2];
            label gridur = square.linked_grids_[3];

            scalar h, w; // height and width of left lower sub square
            if(gridll > 0)
            {
                w = center[0] - hrle_->GetActiveGrid(gridll)[0];
                h = center[1] - hrle_->GetActiveGrid(gridll)[1];
            }
            else
            {
                w = hrle_->GetActiveGrid(gridur)[0] - center[0];
                h = hrle_->GetActiveGrid(gridur)[1] - center[1];
            }

            scalar areaul = (1.0-h)*w;
            scalar areall = h*w;
            scalar arealr = h*(1.0-w);
            scalar areaur = (1.0-w)*(1.0-h);

            //scalar area = areaul + areall + arealr + areaur; 

            // Square area fixed to zero
            velocityfield_[gridul] += arealr * refarea.getrate();
            velocityfield_[gridll] += areaur * refarea.getrate();
            velocityfield_[gridlr] += areaul * refarea.getrate();
            velocityfield_[gridur] += areall * refarea.getrate();
        }
    }

    scalar LevelSetFunction::getlsf(label index)
    {
        return levelsetfunction_[index];
    }

    void LevelSetFunction::setlsf(label index, scalar value)
    {
        // std::cout <<"setting lsf..." << index <<std::endl;
        levelsetfunction_[index] = value;
    }

    scalar LevelSetFunction::getvf(label index)
    {
        return velocityfield_[index];
    }

    void LevelSetFunction::setvf(label index, scalar value)
    {
        velocityfield_[index] = value;
    }
#pragma endregion

    bool LevelSetFunction::IdentifyEffectiveCube(Square &square)
    {
        // 这里的Square应该被修改过：四个角没有undifiend的情况
        bool ispos = getlsf(square.linked_grids_[0]) > 0;
        // std::cout << getlsf(square.linked_grids_[0]->index) << " "
        //<< getlsf(square.linked_grids_[1]->index) << " "
        //  << getlsf(square.linked_grids_[2]->index) << " "
        //  << getlsf(square.linked_grids_[3]->index) << std::endl;
        if (ispos)
        {
            if (
                getlsf(square.linked_grids_[1]) < 0 ||
                getlsf(square.linked_grids_[2]) < 0 ||
                getlsf(square.linked_grids_[3]) < 0)
            {
                // std::cout << "Identified" << std::endl;
                // std::cout << getlsf(square.linked_grids_[1]->index) << " " << getlsf(square.linked_grids_[2]->index) << " " << getlsf(square.linked_grids_[3]->index) << std::endl;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (
                getlsf(square.linked_grids_[1]) > 0 ||
                getlsf(square.linked_grids_[2]) > 0 ||
                getlsf(square.linked_grids_[3]) > 0)
            {
                // std::cout << "Identified" << std::endl;
                // std::cout << getlsf(square.linked_grids_[1]->index) << " " << getlsf(square.linked_grids_[2]->index) << " " << getlsf(square.linked_grids_[3]->index) << std::endl;
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    bool LevelSetFunction::IdentifyEffectiveCube(std::vector<int> squareindexlist)
    {
        bool ispos = getlsf(squareindexlist[0]) > 0;
        if (ispos)
        {
            if (
                getlsf(squareindexlist[1]) < 0 ||
                getlsf(squareindexlist[2]) < 0 ||
                getlsf(squareindexlist[3]) < 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (
                getlsf(squareindexlist[1]) > 0 ||
                getlsf(squareindexlist[2]) > 0 ||
                getlsf(squareindexlist[3]) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    void LevelSetFunction::ConstructAllActiveSquares()
    {
        activatesqures_.clear();
        Const::vecDi nextpoint;
        nextpoint[0] = 0; // test
        nextpoint[1] = 0; // test
        std::vector<int> squareindexlist;
        while(hrle_->DenseIterator(nextpoint))
        {
            squareindexlist.clear();
            hrle_->GetSquareNeighbours(nextpoint, squareindexlist);
            if (IdentifyEffectiveCube(squareindexlist))
            {
                activatesqures_.emplace_back(Square(squareindexlist[0], squareindexlist[1], squareindexlist[2], squareindexlist[3]));
            }
            // std::cout << "get square" << std::endl;
            //CreateSquare(nextpoint);
            
        }
    }

    void LevelSetFunction::AddRefArea(Const::vecDd &start, Const::vecDd &end)
    {
        // refareas_.emplace_back(start, end); // 这个报错是类继承的原因，有些讲究
        refareas_.emplace_back(RefArea(start, end));
    }

    void LevelSetFunction::SetMarchingCube(Square &square)
    {
        marchingcube_->SetRefArea(*this, square);
    }

    void LevelSetFunction::ConstructAllRefAreas()
    {
        // std::cout <<"here" << std::endl;
        refareas_.clear();
        label nsquares = activatesqures_.size();
        for (label i = 0; i < nsquares; i++)
        {
            Square& squareptr = activatesqures_[i];
            //if (IdentifyEffectiveCube(squareptr)) // 默认只有当effective square才会被插入activesquarelist
            //{
                // std::cout << "Identified" << std::endl;
            SetMarchingCube(squareptr);
            //}
        }
    }

    RefArea *LevelSetFunction::getRefArea(label index)
    {
        return &(refareas_[index]);
    }

    label LevelSetFunction::getnumRefAreas()
    {
        return refareas_.size();
    }

    std::array<int, Const::D> LevelSetFunction::GetActiveGrid(label index)
    {
        return hrle_->GetActiveGrid(index);
    }

    void LevelSetFunction::ToSurface(std::string outputpath_)
    {
        std::ofstream ofs(outputpath_, std::ios::trunc);
        Const::vecDd start;
        Const::vecDd end;
        for (auto &refarea : refareas_)
        {
            refarea.getendpoints(start, end);
            ofs << start[0] << " "
                << end[0] << " "
                << start[1] << " "
                << end[1] << " "
                << refarea.getweightstore()
                << std::endl;
        }
        ofs.close();
    }

} // namespace LevelSet
