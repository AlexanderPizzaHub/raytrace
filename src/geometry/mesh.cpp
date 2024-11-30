#include "mesh.hpp"
#include "levelset/levelset.hpp"
#include <fstream>
#include <iostream>
#include <memory>

#pragma region RefArea
RefArea::RefArea(Line *lineptr)
{
    lineptr_ = lineptr;
    weightstore_ = 0.0;
    //std::cout << "score: " << weightstore_ << std::endl;
}

RefArea::~RefArea()
{
    delete lineptr_;
}

scalar RefArea::Intersect(Ray& ray)
{
    return lineptr_->Intersect(ray);
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

void RefArea::set_object(Line *lineptr)
{
    lineptr_ = lineptr;
}

Const::vecDd RefArea::getnormal()
{
    return lineptr_->getnormal();
}

Line* RefArea::get_object()
{
    return lineptr_;
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

#pragma region Mesh
Mesh::Mesh(std::string inputpath, std::string outputpath)
{
    inputpath_ = inputpath;
    outputpath_ = outputpath;
    marchingcube_ = new MarchingCube2D();
}
Mesh::~Mesh()
{}

void Mesh::CreateMeshFromTxt()
{
    std::ifstream ifs;
    ifs.open(inputpath_);

    if(!ifs.is_open())
    {
        //std::cout << "File not found" << std::endl;
        return;
    }

    std::string buff;
    while(std::getline(ifs, buff))
    {
        // read the file and construct the mesh
        //TODO
    }
}

void Mesh::CreateTestMesh()
{
    /*
    1.5 1.5 1.5 1.5 1.5
    1.5 0.5 0.5 0.5 1.5
    1.2 0.2 -0.2 0.2 1.2 
    0.2 -0.2 -0.2 -0.2 0.2
    0.2 -0.2 -1.2 -0.2 0.2  
    */

    nx = 5; ny = 5;
    for(label i=0; i< nx; i++)
    {
        gridptrs_.emplace_back(new GridCartesian((scalar)i, 0.0, i));
    }
    gridptrs_.emplace_back(new GridCartesian(0.0, 1.0, 5));
    gridptrs_.emplace_back(new GridCartesian(1.0, 1.0, 6));
    gridptrs_.emplace_back(new GridCartesian(2.0, 1.0, 7));
    gridptrs_.emplace_back(new GridCartesian(3.0, 1.0, 8));
    gridptrs_.emplace_back(new GridCartesian(4.0, 1.0, 9));

    gridptrs_.emplace_back(new GridCartesian(0.0, 2.0, 10));
    gridptrs_.emplace_back(new GridCartesian(1.0, 2.0, 11));
    gridptrs_.emplace_back(new GridCartesian(2.0, 2.0, 12));
    gridptrs_.emplace_back(new GridCartesian(3.0, 2.0, 13));
    gridptrs_.emplace_back(new GridCartesian(4.0, 2.0, 14));

    gridptrs_.emplace_back(new GridCartesian(0.0, 3.0, 15));
    gridptrs_.emplace_back(new GridCartesian(1.0, 3.0, 16));
    gridptrs_.emplace_back(new GridCartesian(2.0, 3.0, 17));
    gridptrs_.emplace_back(new GridCartesian(3.0, 3.0, 18));
    gridptrs_.emplace_back(new GridCartesian(4.0, 3.0, 19));

    gridptrs_.emplace_back(new GridCartesian(0.0, 4.0, 20));
    gridptrs_.emplace_back(new GridCartesian(1.0, 4.0, 21));
    gridptrs_.emplace_back(new GridCartesian(2.0, 4.0, 22));
    gridptrs_.emplace_back(new GridCartesian(3.0, 4.0, 23));
    gridptrs_.emplace_back(new GridCartesian(4.0, 4.0, 24));
}

void Mesh::CreateTestMesh2(label nx,label ny,label mid)
{
    this->nx = nx;
    this->ny = ny;
    for(scalar i=0; i< ny; i++)
    {
        for(scalar j=0;j<nx;j++) gridptrs_.emplace_back(new GridCartesian((scalar)j, (scalar)i, i*ny+j));
    }
}
// Link: line(refarea) <- square(cube) <- grid(mesh) 
void Mesh::ConstructTopo()
{
    // square -> grid
    for(label i=0 ; i<nx-1; i++)
    {
        for(label j=0; j<ny-1; j++)
        {
            //std::cout << i << " " << j << std::endl;
            Square* squareptr = new Square(gridptrs_[i*nx+j], gridptrs_[(i+1)*nx+j], gridptrs_[(i+1)*nx+j+1], gridptrs_[i*nx+j+1]);
            //std::cout << "Square constructed" << std::endl;
            //std::cout << "Square linked" << std::endl;
            squareptrs_.push_back(squareptr);
            //std::cout << "Square pushed" << std::endl;
            //std::cout << nx << " " << ny << std::endl;
        }
    }
    //std::cout << squareptrs_.size() << std::endl;
}

bool Mesh::IdentifyEffectiveCube(LevelSet::LevelSetFunction& levelset, Square& square)
{
    bool ispos = levelset.getlsf(square.linked_grids_[0]->index) > 0;
    //std::cout << levelset.getlsf(square.linked_grids_[0]->index) << " "
    //<< levelset.getlsf(square.linked_grids_[1]->index) << " "
    // << levelset.getlsf(square.linked_grids_[2]->index) << " "
    // << levelset.getlsf(square.linked_grids_[3]->index) << std::endl;
    if(ispos)
    {
        if(
            levelset.getlsf(square.linked_grids_[1]->index) < 0 || 
            levelset.getlsf(square.linked_grids_[2]->index) < 0 || 
            levelset.getlsf(square.linked_grids_[3]->index) < 0)
        {
            //std::cout << "Identified" << std::endl;
            //std::cout << levelset.getlsf(square.linked_grids_[1]->index) << " " << levelset.getlsf(square.linked_grids_[2]->index) << " " << levelset.getlsf(square.linked_grids_[3]->index) << std::endl;
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
            levelset.getlsf(square.linked_grids_[1]->index) > 0 || 
            levelset.getlsf(square.linked_grids_[2]->index) > 0 || 
            levelset.getlsf(square.linked_grids_[3]->index) > 0)
        {
            //std::cout << "Identified" << std::endl;
            //std::cout << levelset.getlsf(square.linked_grids_[1]->index) << " " << levelset.getlsf(square.linked_grids_[2]->index) << " " << levelset.getlsf(square.linked_grids_[3]->index) << std::endl;
            return true;
        }
        else
        {
            return false;
        }
    }
}


void Mesh::SetMarchingCube(LevelSet::LevelSetFunction& levelset, Square& square)
{
    marchingcube_->SetRefArea(levelset, square);
    
    for(auto &refarea : square.linked_refareas_)
    {
        refareaptrs_.emplace_back(refarea);
    }
}

void Mesh::ConstructAllRefAreas(LevelSet::LevelSetFunction& levelset)
{
    //std::cout <<"here" << std::endl;
    refareaptrs_.clear();
    for(auto &square : squareptrs_)
    {   
        //std::cout << "Constructing RefArea" << std::endl;
        //std::cout << gridptrs_[square->linked_upperleft_grid_index_]->index) << std::endl;
        if(IdentifyEffectiveCube(levelset, *square))
        {
            //std::cout << "Identified" << std::endl;
            SetMarchingCube(levelset, *square);
        }
    }
}

RefArea* Mesh::getRefArea(label index)
{
    return refareaptrs_[index];
}

label Mesh::getnumRefAreas()
{
    return refareaptrs_.size();
}

GridCartesian* Mesh::getGrid(label index)
{
    return gridptrs_[index];
}

label Mesh::getnumGrids()
{
    return gridptrs_.size();
}

Square* Mesh::getSquare(label index)
{
    return squareptrs_[index];
}

label Mesh::getnumSquares()
{
    return squareptrs_.size();
}

void Mesh::ToSurface()
{
    std::ofstream ofs(outputpath_,std::ios::trunc);
    Const::vecDd start;
    Const::vecDd end;
    for(auto &refarea : refareaptrs_)
    {
        refarea->get_object()->getendpoints(start, end);
        ofs << start[0] << " " 
            << end[0] << " "
            << start[1] << " " 
            << end[1] << " " 
            << refarea->getweightstore() 
        << std::endl;
    }
    ofs.close();
}

void Mesh::ToSurface(std::string outputpath)
{
    std::ofstream ofs(outputpath,std::ios::trunc);
    Const::vecDd start;
    Const::vecDd end;
    for(auto &refarea : refareaptrs_)
    {
        refarea->get_object()->getendpoints(start, end);
        ofs << start[0] << " " 
            << end[0] << " "
            << start[1] << " " 
            << end[1] << " " 
            << refarea->getweightstore() 
        << std::endl;
    }
    ofs.close();
}

#pragma endregion