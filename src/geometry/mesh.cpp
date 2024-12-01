#include "mesh.hpp"

#include <fstream>
#include <iostream>
#include <memory>

#pragma region Mesh
Mesh::Mesh(std::string inputpath)
{
    inputpath_ = inputpath;
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
        grids_.emplace_back(GridCartesian((scalar)i, 0.0, i));
    }
    grids_.emplace_back(GridCartesian(0.0, 1.0, 5));
    grids_.emplace_back(GridCartesian(1.0, 1.0, 6));
    grids_.emplace_back(GridCartesian(2.0, 1.0, 7));
    grids_.emplace_back(GridCartesian(3.0, 1.0, 8));
    grids_.emplace_back(GridCartesian(4.0, 1.0, 9));

    grids_.emplace_back(GridCartesian(0.0, 2.0, 10));
    grids_.emplace_back(GridCartesian(1.0, 2.0, 11));
    grids_.emplace_back(GridCartesian(2.0, 2.0, 12));
    grids_.emplace_back(GridCartesian(3.0, 2.0, 13));
    grids_.emplace_back(GridCartesian(4.0, 2.0, 14));

    grids_.emplace_back(GridCartesian(0.0, 3.0, 15));
    grids_.emplace_back(GridCartesian(1.0, 3.0, 16));
    grids_.emplace_back(GridCartesian(2.0, 3.0, 17));
    grids_.emplace_back(GridCartesian(3.0, 3.0, 18));
    grids_.emplace_back(GridCartesian(4.0, 3.0, 19));

    grids_.emplace_back(GridCartesian(0.0, 4.0, 20));
    grids_.emplace_back(GridCartesian(1.0, 4.0, 21));
    grids_.emplace_back(GridCartesian(2.0, 4.0, 22));
    grids_.emplace_back(GridCartesian(3.0, 4.0, 23));
    grids_.emplace_back(GridCartesian(4.0, 4.0, 24));
}

void Mesh::CreateTestMesh2(label nx,label ny,label mid)
{
    this->nx = nx;
    this->ny = ny;
    for(scalar i=0; i< ny; i++)
    {
        for(scalar j=0;j<nx;j++) grids_.emplace_back(GridCartesian((scalar)j, (scalar)i, i*ny+j));
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
  
            squares_.emplace_back(Square(grids_[i*nx+j], grids_[(i+1)*nx+j], grids_[(i+1)*nx+j+1], grids_[i*nx+j+1]));
            //std::cout << "Square pushed" << std::endl;
            //std::cout << nx << " " << ny << std::endl;
        }
    }
    //std::cout << squareptrs_.size() << std::endl;
}



GridCartesian* Mesh::getGrid(label index)
{
    return &(grids_[index]);
}

label Mesh::getnumGrids()
{
    return grids_.size();
}

Square* Mesh::getSquare(label index)
{
    return &(squares_[index]);
}

label Mesh::getnumSquares()
{
    return squares_.size();
}

#pragma endregion