#include <iostream>
#include "raytracer/tracer.hpp"
#include "geometry/mesh.hpp"
#include "constants.hpp"
#include "levelset/evolve.hpp"
#include "raytracer/mpitracer.hpp"
#include "mpi.h"
#include <chrono>

int testrun()
{
    // 从文件中读grids然后跑程序
    Mesh* mesh =new Mesh("./sparsefieldgrids.txt", "./output.txt");
    //std::cout <<"1" << std::endl;
    mesh -> CreateTestMesh();
    //std::cout <<"2" << std::endl;
    mesh -> ConstructTopo();
    //std::cout <<"3" << std::endl;

    LevelSet::Evolve* evolve = new LevelSet::Evolve(mesh);
    Tracer* tracer = new Tracer(mesh);
    RaySampler* splrptr = new RaySampler(Const::source_plane_xbox,Const::source_plane_ybox);

    for(scalar t=0.0; t<4.0; t+=0.1)
    {
        std::cout << "time: " << t << std::endl;
        mesh -> ConstructAllRefAreas();
        for(label i=0; i<mesh -> getnumRefAreas(); i++)
        {
            RefArea* refarea = mesh -> getRefArea(i);
            auto center = refarea->get_object()->getcenter();
            //std::cout << center[0] << " "<<center[1] << std::endl;
        }
        tracer->AddNewSource(splrptr);
        tracer -> CastAllRays(10000,0);
        //std::cout << "finish 222" << std::endl;
        for(label i=0; i<mesh -> getnumRefAreas(); i++)
        {
            RefArea* refarea = mesh -> getRefArea(i);
            auto hit = refarea->getweightstore();
        }

        evolve -> evolve();
        mesh -> ToSurface("results/output"+std::to_string(t)+".txt");
        //std::cout << ".results/output"+std::to_string(t)+".txt"<<std::endl;
    }
    delete evolve;
    delete tracer;
   return 0;
}

int testmpi()
{
    // 从文件中读grids然后跑程序
    MPI_Init(NULL, NULL);

    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Mesh* mesh =new Mesh("./sparsefieldgrids.txt", "./output.txt");
    //std::cout <<"1" << std::endl;
    mesh -> CreateTestMesh();
    //std::cout <<"2" << std::endl;
    mesh -> ConstructTopo();
    //std::cout <<"3" << std::endl;
    mesh -> ConstructAllRefAreas();

    MpiTracer::MpiTracer* mpitracer = new MpiTracer::MpiTracer(*mesh);
    LevelSet::Evolve* evolve = new LevelSet::Evolve(mesh);
    Tracer* tracer = new Tracer(mesh);
    RaySampler* splrptr = new RaySampler(Const::source_plane_xbox,Const::source_plane_ybox);

    tracer->AddNewSource(splrptr);
    tracer -> CastAllRays(20000,0);

    mpitracer -> CommunicateRefArea();
    //evolve -> evolve();
    mesh -> ToSurface("results/output.txt");
    //std::cout << ".results/output"+std::to_string(t)+".txt"<<std::endl;
    delete evolve;
    delete tracer;

    MPI_Finalize();
    return 0;
}


int main()
{
    auto start_time = std::chrono::steady_clock::now();
    testmpi();
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "finish time: "<< std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
    return 0;
}