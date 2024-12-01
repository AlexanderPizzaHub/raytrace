#include <iostream>
#include "raytracer/tracer.hpp"
#include "geometry/mesh.hpp"
#include "constants.hpp"
#include "levelset/levelset.hpp"
#include <chrono>

int testrun()
{
    // 从文件中读grids然后跑程序
    Mesh* mesh =new Mesh("./sparsefieldgrids.txt");
    mesh -> CreateTestMesh2(10,10,5);
    mesh -> ConstructTopo();

    LevelSet::LevelSetFunction* levelset = new LevelSet::LevelSetFunction(mesh);
    //std::cout << mesh->getnumSquares() << std::endl;

    Tracer* tracer = new Tracer(mesh,levelset);
    
    

    //std::cout <<"000  " <<mesh->getnumGrids() <<std::endl;
    label mid = 5;
    for(label i=0; i<10;i++)
    {
        for(label j=0; j<10;j++)
        {
            levelset -> setlsf(i*10+j, ((scalar)i-(scalar)mid-0.5));
            //std::cout << "111"<<std::endl;
            //std::cout << levelset->getlsf(i*10+j) << std::endl;
        }
    }
    
    scalar dt = 1.0;
    RaySampler* splrptr = new RaySampler(Const::source_plane_xbox,Const::source_plane_ybox);
    RaySampler* splrptr2 = new RaySampler(Const::source_plane_xbox2,Const::source_plane_ybox2);
    tracer->AddNewSource(splrptr);
    tracer->AddNewSource(splrptr2);
    for(scalar t=0.0; t<200.0; t+=dt)
    {
        std::cout << "time: " << t << std::endl;
        levelset->ConstructAllRefAreas();
        tracer -> CastAllRays(100000,0);
        //tracer -> CastAllRays(10000,1);
        tracer -> NormalizeFlux(100000);
        //std::cout << "2222" <<std::endl;
        tracer -> CalcAllRate();
        levelset -> VelocityExtension();
        levelset -> Evolve(dt);
        levelset -> ToSurface("../results/output"+std::to_string(t)+".txt");
        //std::cout << ".results/output"+std::to_string(t)+".txt"<<std::endl;
    }
   return 0;
}


int main()
{
    auto start_time = std::chrono::steady_clock::now();
    testrun();
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "finish time: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
    return 0;
}