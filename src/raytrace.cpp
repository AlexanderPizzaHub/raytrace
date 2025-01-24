#include <iostream>
#include "raytracer/tracer.hpp"
#include "geometry/mesh.hpp"
#include "constants.hpp"
#include "levelset/levelset.hpp"
#include "HRLE/hrle.hpp"
#include <chrono>
/*
int testrun()
{
    // 从文件中读grids然后跑程序
    Mesh *mesh = new Mesh("./sparsefieldgrids.txt");

    label nx = 1000;
    mesh->CreateTestMesh2(nx, nx, nx / 2);
    mesh->ConstructTopo();

    LevelSet::LevelSetFunction *levelset = new LevelSet::LevelSetFunction(mesh);
    std::cout << mesh->getnumSquares() << std::endl;

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
        tracer->CalcAllRateAdd();

        tracer->CastAllRays(100000, 1);
        tracer->NormalizeFlux(10000);
        tracer->CalcAllRateAdd();
        // std::cout << "2222" <<std::endl;

        levelset->VelocityExtension();
        levelset->Evolve(dt);
        levelset->ToSurface("../results/output" + std::to_string(t) + ".txt");
        // std::cout << ".results/output"+std::to_string(t)+".txt"<<std::endl;
    }
    return 0;
}
*/

int testrun2()
{
    using namespace hrle;

    std::array<std::array<int, 2>, Const::D> extents;
    extents[0][0] = 0;
    extents[0][1] = 11;
    extents[1][0] = 0;
    extents[1][1] = 10;

    HRLE hrle(extents);

    using namespace hrle;

    // 生成y轴游程编码
    hrle.AddNewLayer(0, 0, UNDEFINED_POS);
    //hrle.AddUndefinedSection(0, -3, UNDEFINED_POS);
    hrle.AddDefinedSection(0, -1);
    hrle.AddUndefinedSection(0,6, UNDEFINED_POS);
    

    // int dim = 0;
    //  生成x轴游程编码
    // int nbreaks = hrle.rles_[dim].runbreaks_.size();
    // hrle.rles_[1].startindices_.reserve(maxdataindex[0] + rles_[0].runbreaks_[nbreaks - 1] - rles_[0].runbreaks_[nbreaks - 2])

    hrle.AddNewLayer(1, 0, UNDEFINED_POS);

    hrle.AddDefinedSection(1, -4);
    hrle.AddUndefinedSection(1, 3, UNDEFINED_POS);

    hrle.AddNewLayer(1, 1, UNDEFINED_POS);

    hrle.AddDefinedSection(1, -5);
    hrle.AddUndefinedSection(1, 4, UNDEFINED_POS);

    hrle.AddNewLayer(1, 2, UNDEFINED_POS);

    hrle.AddDefinedSection(1, -5);
    hrle.AddUndefinedSection(1, -3, UNDEFINED_NEG);
    hrle.AddDefinedSection(1, 0);
    hrle.AddUndefinedSection(1, 3, UNDEFINED_POS);

    hrle.denseiterator.SetStartPoint();

    /*
    RLE& rle0 = hrle.rles_[0];
    std::cout << "indecies: " << rle0.startindices_[0] << std::endl;
    std::cout << "runbreaks: "  << std::endl;
    for(auto i : rle0.runbreaks_)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    std::cout << "runtypes: " << std::endl;

    for(auto i : rle0.runtypes_)
    {
        std::cout << i.state_ << " " << i.index_ << std::endl;
    }

    RLE& rle1 = hrle.rles_[1];


    std::cout << "indecies: " << std::endl;
    for(auto i : rle1.startindices_)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    std::cout << "runbreaks: "  << std::endl;

    for(auto i : rle1.runbreaks_)
    {
        std::cout << i << " ";
    }

    std::cout << std::endl;

    std::cout << "runtypes: " << std::endl;

    for(auto i : rle1.runtypes_)
    {
        std::cout << i.state_ << " " << i.index_ << std::endl;
    }

    std::cout << "indecies: " << rle1.startindices_[1] << std::endl;
    */


    
    int index;
    Const::vecDi coords;
    /*
    for (int i = -3; i < 1; i++)
    {
        for (int j = -6; j < 5; j++)
        {
            coords[0] = j;
            coords[1] = i;
            index = hrle.CartesianToIndex(coords, 0, 1);

            std::cout << j << " " << " " << i<< " " <<index << std::endl;
        }
    }
    */
   std::cout <<"start"<<std::endl;
   std::cout<<hrle.denseiterator.coord_[0] << " " << hrle.denseiterator.coord_[1] << std::endl;
   for(int i=0;i<10;i++)
   {
    hrle.denseiterator.next();
    std::cout<<std::endl;
    std::cout<<hrle.denseiterator.coord_[0] << " " << hrle.denseiterator.coord_[1] << std::endl;
   }
    

    return 0;
}
int main()
{
    auto start_time = std::chrono::steady_clock::now();
    testrun2();
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "finish time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl;
    return 0;
}