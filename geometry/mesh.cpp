#include "mesh.hpp"
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

void RefArea::ResetWeight()
{
    weightstore_ = 0;
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
    1 1 1 1 1
    1 0.5 0.5 0.5 1
    0.5 0.2 0.2 0.2 0.5 
    0.2 -0.2 -0.2 -0.2 0.2
    0.2 -0.5 -0.5 -0.5 0.2  
    */

    nx = 5; ny = 5;
    for(label i=0; i< nx; i++)
    {
        gridptrs_.emplace_back(new GridCartesian((scalar)i, 0.0, 1.0));
    }
    gridptrs_.emplace_back(new GridCartesian(0.0, 1.0, 1.0));
    gridptrs_.emplace_back(new GridCartesian(1.0, 1.0, 0.5));
    gridptrs_.emplace_back(new GridCartesian(2.0, 1.0, 0.5));
    gridptrs_.emplace_back(new GridCartesian(3.0, 1.0, 0.5));
    gridptrs_.emplace_back(new GridCartesian(4.0, 1.0, 1.0));

    gridptrs_.emplace_back(new GridCartesian(0.0, 2.0, 0.5));
    gridptrs_.emplace_back(new GridCartesian(1.0, 2.0, 0.2));
    gridptrs_.emplace_back(new GridCartesian(2.0, 2.0, -0.2));
    gridptrs_.emplace_back(new GridCartesian(3.0, 2.0, 0.2));
    gridptrs_.emplace_back(new GridCartesian(4.0, 2.0, 0.5));

    gridptrs_.emplace_back(new GridCartesian(0.0, 3.0, 0.2));
    gridptrs_.emplace_back(new GridCartesian(1.0, 3.0, -0.2));
    gridptrs_.emplace_back(new GridCartesian(2.0, 3.0, -0.2));
    gridptrs_.emplace_back(new GridCartesian(3.0, 3.0, -0.2));
    gridptrs_.emplace_back(new GridCartesian(4.0, 3.0, 0.2));

    gridptrs_.emplace_back(new GridCartesian(0.0, 4.0, 0.2));
    gridptrs_.emplace_back(new GridCartesian(1.0, 4.0, -0.5));
    gridptrs_.emplace_back(new GridCartesian(2.0, 4.0, -0.5));
    gridptrs_.emplace_back(new GridCartesian(3.0, 4.0, -0.5));
    gridptrs_.emplace_back(new GridCartesian(4.0, 4.0, 0.2));
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

bool Mesh::IdentifyEffectiveCube(Square& square)
{
    bool ispos = square.linked_grids_[0]->phi > 0;
    //std::cout << gridptrs_[upperleft]->phi << " " 
    //<< gridptrs_[upperleft+1]->phi << " " <<
    // gridptrs_[upperleft+nx]->phi << " " <<
    // gridptrs_[upperleft+nx+1]->phi << " "  <<
    //std::endl;
    if(ispos)
    {
        if(
            square.linked_grids_[1]->phi < 0 || 
            square.linked_grids_[2]->phi < 0 || 
            square.linked_grids_[3]->phi < 0)
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
        if(
            square.linked_grids_[1]->phi > 0 || 
            square.linked_grids_[2]->phi > 0 || 
            square.linked_grids_[3]->phi > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


void Mesh::SetMarchingCube(Square& square)
{
    marchingcube_->SetRefArea(square);
    
    for(auto &refarea : square.linked_refareas_)
    {
        refareaptrs_.emplace_back(refarea);
    }
}

void Mesh::ConstructAllRefAreas()
{
    //std::cout <<"here" << std::endl;
    for(auto &square : squareptrs_)
    {   
        //std::cout << "Constructing RefArea" << std::endl;
        //std::cout << gridptrs_[square->linked_upperleft_grid_index_]->phi << std::endl;
        if(IdentifyEffectiveCube(*square))
        {
            SetMarchingCube(*square);
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

#pragma endregion