#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "constants.hpp"
#include "geometry/geometry.hpp"
#include "raytracer/ray.hpp"

int testrun1()
{
    // 从文件中读grids然后跑程序
    std::ifstream ifs;
    ifs.open("./sparsefieldgrids.txt");

    if(!ifs.is_open())
    {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    std::string buff;

    return 0;
}

int testrun2()
{
    // 把grid写到这里
    int numGrids = 64;
    //std::vector<RefArea> refAreas;
    //std::vector<SparseFieldGrid> grids;
    Geometry geom = Geometry();
    SparseFieldGrid *grid = new SparseFieldGrid(0.0,0.0,0);
    geom.L0head = grid;

    grid->refAreaPtr = new RefArea();
    
    for(int i = 1; i < numGrids; i++)
    {   
        SparseFieldGrid *grid_ = new SparseFieldGrid((1.0/64.0)*(double)i,0.0,i); // 不转double全部是0
        grid_->refAreaPtr = new RefArea();
        grid->next = grid_;
        grid_ -> prev = grid;
        grid = grid_;
    }
    grid->next = nullptr;

    SparseFieldGrid* curgrid = geom.L0head;
    while(curgrid != nullptr)
    {
        curgrid->constructRefArea_test();
        curgrid = curgrid->next;
    }
    
    std::cout << "start tracing..."<<std::endl;
    geom.allRaysCast(100000);
    std::cout << "end tracing..."<<std::endl;

    curgrid = geom.L0head;
    while(curgrid != nullptr)
    {
        std::cout << "grid index: "<<curgrid->index << " hit counts: "<< curgrid->refAreaPtr->hitCounter << std::endl;
        curgrid = curgrid->next;
    }

    return 0;
}


int main()
{
    testrun2();

    return 0;
}
