#ifndef GEOMETRY_MESH_HPP
#define GEOMETRY_MESH_HPP

#include "constants.hpp"
#include "objects.hpp"

#include <string>
#include <vector>

class Square;
class GridCartesian;

class Mesh
{
public:
    Mesh(std::string inputpath); // read from file. currently use .txt
    ~Mesh();

    void CreateMeshFromTxt();

    void CreateTestMesh();                               // for testing
    void CreateTestMesh2(label nx, label ny, label mid); // for testing

    // void ConstructRefArea(Square& square);

    // geometry and topology set up
    void ConstructTopo(); // implement geometry topology

    Square *getSquare(label index);
    label getnumSquares();

    GridCartesian *getGrid(label index);
    label getnumGrids();

    label nx, ny;

private:
    std::string inputpath_;
    std::vector<GridCartesian> grids_;
    std::vector<Square> squares_;
};

#endif // GEOMETRY_HPP