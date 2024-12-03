#include <iostream>
#include "raytracer/tracer.hpp"
#include "geometry/mesh.hpp"
#include "constants.hpp"
#include "levelset/levelset.hpp"
#include <chrono>

int testrun()
{
    // 从文件中读grids然后跑程序
    Mesh *mesh = new Mesh("./sparsefieldgrids.txt");

    label nx = 1000;
    mesh->CreateTestMesh2(nx, nx, nx / 2);
    mesh->ConstructTopo();

    LevelSet::LevelSetFunction *levelset = new LevelSet::LevelSetFunction(mesh);
    // std::cout << mesh->getnumSquares() << std::endl;

    Tracer *tracer = new Tracer(mesh, levelset);

    // std::cout <<"000  " <<mesh->getnumGrids() <<std::endl;
    label mid = nx / 2;
    for (label i = 0; i < nx; i++)
    {
        for (label j = 0; j < nx; j++)
        {
            levelset->setlsf(i * nx + j, ((scalar)i - (scalar)mid - 0.5));
            // std::cout << "111"<<std::endl;
            // std::cout << levelset->getlsf(i*10+j) << std::endl;
        }
    }

    scalar dt = 1.0;
    RaySampler *splrptr = new RaySampler(Const::source_plane_xbox, Const::source_plane_ybox);
    RaySampler *splrptr2 = new RaySampler(Const::source_plane_xbox2, Const::source_plane_ybox2);
    tracer->AddNewSource(splrptr);
    tracer->AddNewSource(splrptr2);
    for (scalar t = 0.0; t < 100.0; t += dt)
    {
        std::cout << "time: " << t << std::endl;
        levelset->ConstructAllRefAreas();
        // std::cout << levelset->getnumRefAreas() << std::endl;
        // Const::vecDd center  = levelset->getRefArea(0)->getcenter();
        // std::cout << center[0]<<" "<<center[1] <<std::endl;
        levelset->ClearVelocity();

        tracer->CastAllRays(100000, 0);
        tracer->NormalizeFlux(10000);
        tracer->CalcAllRate();

        tracer->CastAllRays(100000, 1);
        tracer->NormalizeFlux(10000);
        tracer->CalcAllRate();
        // std::cout << "2222" <<std::endl;

        levelset->VelocityExtension();
        levelset->Evolve(dt);
        levelset->ToSurface("../results/output" + std::to_string(t) + ".txt");
        // std::cout << ".results/output"+std::to_string(t)+".txt"<<std::endl;
    }
    return 0;
}

int main()
{
    auto start_time = std::chrono::steady_clock::now();
    testrun();
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "finish time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
    return 0;
}