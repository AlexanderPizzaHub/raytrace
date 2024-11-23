#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "../constants.hpp"
#include "../Ray/Ray.hpp"
#include "objects.hpp"

class RefArea;
class SparseFieldGrid;
class Geometry;

class RefArea
{
    public:

        // To describe the Geometry of RefArea
        scalar coords[D];
        scalar normal[D];
        scalar radius;

        // To record the flux
        // 两种方式：记被命中次数，最后再匀给每个grid；或者每次被命中时立刻按照位置匀给grid。
        std::vector<label> linkedGrids; // grids that are linked to this RefArea
        label hitCounter; 

        RefArea();
        ~RefArea(); 

        void hit();
        //void distributeFlux(); // distribute the flux to linked grids
        void refAreaUpdateAfterHit(scalar dWeight, vecDd position);
};

class SparseFieldGrid 
{

    public:
        scalar coords[D];
        label index; // this index is consistent with the mesh info. Is it really necessary?

        // data is attachted to grids
        scalar phi;
        scalar velX;
        scalar velY;

        // reference area
        //const RefArea *refAreaPtr;
        RefArea *refAreaPtr;

        // Geometry and topology
        // here we assert the active grid can be expressed by a single chain
        // anti-clockwisely ordered
        SparseFieldGrid *next; // 怎么算法向量？
        SparseFieldGrid *prev; 
       
       void calcRate(); 
       void constructRefArea(); // just modify the data in RefArea
       void constructRefArea_test();

       SparseFieldGrid(scalar x_, scalar y_, label i, RefArea *refAreaPtr_=nullptr);
       ~SparseFieldGrid();

};

class Geometry
{
    public:
    // the Geometry is a collection of sparse field chains
        SparseFieldGrid *L0head; // represent by sparse field
        std::vector<RefArea*>  refAreaPtrs;
        

        // do global operations
        void constructAllRefArea(); // visit all grids and construct reference area for it
        void calcAllRate(); // visit all grids and calculate rate for it
        void normalizeFlux(); // align the flux on surface with flux on source plane

        // 光线生成是动态还是静态？追踪需要用kdtree，放到以后写
        // 先动态生成光线，遍历做追踪，做个简单的
        void castOneRay(Ray &ray_);
        void updateAfterHit(Ray &ray_, RefArea &RefArea,scalar &dt);
        scalar calcDWeight(Ray &ray_, RefArea &RefArea);
        void itsctAllRefAreas(Ray &ray_, scalar &dt, scalar &dt_, RefArea* refAreaPtr);
        void castAllRays(label numRays); // cast rays to the Geometry
        
        Geometry();
        ~Geometry();

};

#endif // GEOMETRY_HPP