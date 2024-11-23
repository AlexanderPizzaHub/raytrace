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
}
Mesh::~Mesh()
{
    // this part is wrong
    /*
    for(auto &square : squareptrs_)
    {
        delete &square;
    }
    std::cout << "Mesh destructed" << std::endl;
    for(auto &refarea : refareaptrs_)
    {
        delete &refarea;
    }
    for(auto &grid : gridptrs_)
    {
        delete &grid;
    }
    */
}

void Mesh::CreateMeshFromTxt()
{
    std::ifstream ifs;
    ifs.open(inputpath_);

    if(!ifs.is_open())
    {
        std::cout << "File not found" << std::endl;
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
        gridptrs_.emplace_back(new GridCartesian{i,0, 1.0});
    }
    gridptrs_.emplace_back(new GridCartesian{0, 1, 1.0});
    gridptrs_.emplace_back(new GridCartesian{1, 1, 0.5});
    gridptrs_.emplace_back(new GridCartesian{2, 1, 0.5});
    gridptrs_.emplace_back(new GridCartesian{3, 1, 0.5});
    gridptrs_.emplace_back(new GridCartesian{4, 1, 1.0});

    gridptrs_.emplace_back(new GridCartesian{0, 2, 0.5});
    gridptrs_.emplace_back(new GridCartesian{1, 2, 0.2});
    gridptrs_.emplace_back(new GridCartesian{2, 2, 0.2});
    gridptrs_.emplace_back(new GridCartesian{3, 2, 0.2});
    gridptrs_.emplace_back(new GridCartesian{4, 2, 0.5});

    gridptrs_.emplace_back(new GridCartesian{0, 3, 0.2});
    gridptrs_.emplace_back(new GridCartesian{1, 3, -0.2});
    gridptrs_.emplace_back(new GridCartesian{2, 3, -0.2});
    gridptrs_.emplace_back(new GridCartesian{3, 3, -0.2});
    gridptrs_.emplace_back(new GridCartesian{4, 3, 0.2});

    gridptrs_.emplace_back(new GridCartesian{0, 4, 0.2});
    gridptrs_.emplace_back(new GridCartesian{1, 4, -0.5});
    gridptrs_.emplace_back(new GridCartesian{2, 4, -0.5});
    gridptrs_.emplace_back(new GridCartesian{3, 4, -0.5});
    gridptrs_.emplace_back(new GridCartesian{4, 4, 0.2});
}

// Link: line(refarea) -> square(cube) -> grid(mesh) 
void Mesh::ConstructTopo()
{
    // square -> grid
    for(label i=0 ; i<nx-1; i++)
    {
        for(label j=0; j<ny-1; j++)
        {
            //std::cout << i << " " << j << std::endl;
            Square* squareptr = new Square(Const::vecDi{i+1,j}, Const::vecDi{i,j+1});
            //std::cout << "Square constructed" << std::endl;
            squareptr -> linked_upperleft_grid_index_ = i*ny + j;
            //std::cout << "Square linked" << std::endl;
            squareptrs_.push_back(squareptr);
            //std::cout << "Square pushed" << std::endl;
            //std::cout << nx << " " << ny << std::endl;
        }
    }
    std::cout << squareptrs_.size() << std::endl;
}

bool Mesh::IdentifyEffectiveCube(Square& square)
{
    label upperleft = square.linked_upperleft_grid_index_;
    bool ispos = gridptrs_[upperleft]->phi > 0;
    //std::cout << gridptrs_[upperleft]->phi << " " 
    //<< gridptrs_[upperleft+1]->phi << " " <<
    // gridptrs_[upperleft+nx]->phi << " " <<
    // gridptrs_[upperleft+nx+1]->phi << " "  <<
    //std::endl;
    if(ispos)
    {
        if(
            gridptrs_[upperleft+1]->phi < 0 || 
            gridptrs_[upperleft+nx]->phi < 0 || 
            gridptrs_[upperleft+nx+1]->phi < 0)
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
            gridptrs_[upperleft+1]->phi > 0 || 
            gridptrs_[upperleft+nx]->phi > 0 || 
            gridptrs_[upperleft+nx+1]->phi > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


void Mesh::MarchingCube(Square& square)
{
    Line* line = new Line(Const::vecDd{-10,-10}, Const::vecDd{10,10}); //TODO
    //std::unique_ptr<Line> line(new Line(Const::vecDd{0.5,0.5}, 0.5, Const::vecDd{0.0,1.0}));
    
    line -> linked_objects_.emplace_back(&square);
    RefArea* refarea = new RefArea(line);
    refareaptrs_.emplace_back(refarea);
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
            MarchingCube(*square);
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

#pragma endregion