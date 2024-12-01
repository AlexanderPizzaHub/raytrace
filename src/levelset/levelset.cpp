#include "levelset.hpp"
#include <array>
#include <iostream>
#include <cmath>
#include <fstream>

namespace LevelSet
{

#pragma region RefArea

RefArea::RefArea(Const::vecDd& center, scalar radius, Const::vecDd& normal)
: Line(center, radius, normal)
{
    weightstore_ = 0.0;
    rate_ = 0.0;
}

RefArea::RefArea(Const::vecDd& start, Const::vecDd& end)
: Line(start, end)
{
    weightstore_ = 0.0;
    rate_ = 0.0;
}

RefArea::~RefArea()
{}

scalar RefArea::Intersect(Ray& ray)
{
    scalar dt_ = - (normal_[0]*(ray.pos_[0]-center_[0]) + normal_[1]*(ray.pos_[1]-center_[1])) 
    / (normal_[0]*ray.dir_[0] + normal_[1]*ray.dir_[1]);
    Const::vecDd hitPoint = {ray.pos_[0] + dt_ * ray.dir_[0], ray.pos_[1] + dt_ * ray.dir_[1]};
    if (dt_ < 0 || dt_ > Const::MAXIMUM_LIGHT_TIME ||
    (hitPoint[0]-center_[0])*(hitPoint[0]-center_[0]) + 
    (hitPoint[1]-center_[1])*(hitPoint[1]-center_[1]) > radius_*radius_
    )
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
    //std::cout << "hit" << std::endl;
    weightstore_ += dWeight;
    //std::cout <<getweightstore() <<std::endl;
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
LevelSetFunction::LevelSetFunction(Mesh* meshptr)
{
    meshptr_ = meshptr;
    marchingcube_ = new MarchingCube2D();
    velocityfield_.resize(meshptr_->getnumGrids(),0.0);
    levelsetfunction_.resize(meshptr_->getnumGrids(),0.0);
    //std::cout<<"lsf constructed "<<meshptr_->getnumGrids()<<std::endl;
}

LevelSetFunction::~LevelSetFunction()
{
}

void LevelSetFunction::Evolve(scalar dt)
{
    //std::cout << "evolve" << std::endl;
    label numgrids_active = levelsetfunction_.size();
    for(label i = 0; i<numgrids_active; i++)
    {
        levelsetfunction_[i] -= velocityfield_[i]*dt;
    }
}

void LevelSetFunction::ClearVelocity()
{
    for(label i = 0; i<velocityfield_.size(); i++)
    {
        velocityfield_[i] = 0.0;
    }
}

void LevelSetFunction::VelocityExtension()
{
    for(RefArea refarea: refareas_)
    {
        Square* square = refarea.linked_square_.at(0);

        Const::vecDd center = refarea.getcenter();
        GridCartesian* gridul = square->linked_grids_[0];
        GridCartesian* gridll = square->linked_grids_[1];
        GridCartesian* gridlr = square->linked_grids_[2];
        GridCartesian* gridur = square->linked_grids_[3];

        scalar areaul = (center[0]-gridul->x)*(center[1]-gridul->y);
        scalar areall = (center[0]-gridll->x)*(gridll->y-center[1]);
        scalar arealr = (gridlr->x-center[0])*(gridlr->y-center[1]);
        scalar areaur = (gridur->x-center[0])*(center[1]-gridur->y);

        scalar area = areaul + areall + arealr + areaur;

        velocityfield_[gridul->index] += arealr/area * refarea.getrate();
        velocityfield_[gridll->index] += areaur/area * refarea.getrate();
        velocityfield_[gridlr->index] += areaul/area * refarea.getrate();
        velocityfield_[gridur->index] += areall/area * refarea.getrate();
    }
}

scalar LevelSetFunction::getlsf(label index)
{
    return levelsetfunction_[index];
}

void LevelSetFunction::setlsf(label index, scalar value)
{
    //std::cout <<"setting lsf..." << index <<std::endl;
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

bool LevelSetFunction::IdentifyEffectiveCube(Square& square)
{
    bool ispos = getlsf(square.linked_grids_[0]->index) > 0;
    //std::cout << getlsf(square.linked_grids_[0]->index) << " "
    //<< getlsf(square.linked_grids_[1]->index) << " "
    // << getlsf(square.linked_grids_[2]->index) << " "
    // << getlsf(square.linked_grids_[3]->index) << std::endl;
    if(ispos)
    {
        if(
            getlsf(square.linked_grids_[1]->index) < 0 || 
            getlsf(square.linked_grids_[2]->index) < 0 || 
            getlsf(square.linked_grids_[3]->index) < 0)
        {
            //std::cout << "Identified" << std::endl;
            //std::cout << getlsf(square.linked_grids_[1]->index) << " " << getlsf(square.linked_grids_[2]->index) << " " << getlsf(square.linked_grids_[3]->index) << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(
            getlsf(square.linked_grids_[1]->index) > 0 || 
            getlsf(square.linked_grids_[2]->index) > 0 || 
            getlsf(square.linked_grids_[3]->index) > 0)
        {
            //std::cout << "Identified" << std::endl;
            //std::cout << getlsf(square.linked_grids_[1]->index) << " " << getlsf(square.linked_grids_[2]->index) << " " << getlsf(square.linked_grids_[3]->index) << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void LevelSetFunction::AddRefArea(Const::vecDd& start,Const::vecDd& end)
{
    //refareas_.emplace_back(start, end); // 这个报错是类继承的原因，有些讲究
    refareas_.emplace_back(RefArea(start, end));

}

void LevelSetFunction::SetMarchingCube(Square& square)
{
    marchingcube_->SetRefArea(*this, square);
}

void LevelSetFunction::ConstructAllRefAreas()
{
    //std::cout <<"here" << std::endl;
    refareas_.clear();
    label nsquares = meshptr_->getnumSquares();
    for(label i = 0; i <nsquares;i++)
    {   
        Square* squareptr = meshptr_->getSquare(i); 
        if(IdentifyEffectiveCube(*squareptr))
        {
            //std::cout << "Identified" << std::endl;
            SetMarchingCube(*squareptr);
        }
    }
}

RefArea* LevelSetFunction::getRefArea(label index)
{
    return &(refareas_[index]);
}

label LevelSetFunction::getnumRefAreas()
{
    return refareas_.size();
}

void LevelSetFunction::ToSurface(std::string outputpath_)
{
    std::ofstream ofs(outputpath_,std::ios::trunc);
    Const::vecDd start;
    Const::vecDd end;
    for(auto &refarea : refareas_)
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


