#ifndef GEOMETRY_MESH_HPP
#define GEOMETRY_MESH_HPP

#include "constants.hpp"
#include "objects.hpp"
#include "marchingcube.hpp"
#include "levelset/levelset.hpp"
#include <string>
#include <vector>

class Ray;
class Line;
class Square;
class GridCartesian;
class MarchingCube2D;
// 组件还是继承？ 组件会导致多一层跳转
class RefArea
{
    // 参考面不仅可以用于光追，还可以把界面转显式。这里不实现新的基础几何物体，所以把它放在mesh下
    // 如果参考面仅用于光追而不可用于重建界面，例如把它中心点放在网格点上时，建议把它放在raytracer下，和几何模块解耦
    public:

        RefArea(Line* lineptr);
        //RefArea(std::unique_ptr<Line> lineptr);
        ~RefArea(); 
        
        scalar Intersect(Ray& ray);

        //void Hit(); // increase the hit counter
        void WeightedHit(scalar dWeight); // increase the weight
        void WeightAdd(scalar dweight);
        //void WeightedHit(scalar dWeight, Const::vecDd position); // increase the weight. Hitpoint information is used
        void SetWeight(scalar weight); 
        
        void SetRate(scalar rate);
        scalar getrate();
        
        Const::vecDd getnormal();
        scalar getweightstore();
        
        void set_object(Line *lineptr);
        Line* get_object();

        

    private:
        Line* lineptr_;
        scalar weightstore_;
        scalar rate_;
        //label hitcounter_; // --> integers storing the number of hits
         // --> store the weight of the ray
};

class Mesh
{
    public:
        
        Mesh(std::string inputpath, std::string outputpath); //read from file. currently use .txt
        ~Mesh();

        void CreateMeshFromTxt();
        
        void CreateTestMesh(); // for testing
        void CreateTestMesh2(label nx,label ny,label mid); // for testing

        // related to refarea construction
        bool IdentifyEffectiveCube(LevelSet::LevelSetFunction& levelset, Square& square);
        void SetMarchingCube(LevelSet::LevelSetFunction& levelset, Square& square);

        //void ConstructRefArea(Square& square);

        // geometry and topology set up
        void ConstructTopo(); // implement geometry topology
        void ConstructAllRefAreas(LevelSet::LevelSetFunction& levelset); // construct all reference area and set the linkings.

        RefArea* getRefArea(label index);
        label getnumRefAreas();

        Square* getSquare(label index);
        label getnumSquares();

        GridCartesian* getGrid(label index);
        label getnumGrids();

        void ToSurface(); // convert the mesh to surface and save it to file
        void ToSurface(std::string outputpath); // convert the mesh to surface and save it to file


    private:
        std::string inputpath_;
        std::string outputpath_;
        MarchingCube2D* marchingcube_;

        label nx, ny;
        std::vector<Square*> squareptrs_;
        std::vector<RefArea*> refareaptrs_;
        std::vector<GridCartesian*> gridptrs_;

        
};


#endif // GEOMETRY_HPP