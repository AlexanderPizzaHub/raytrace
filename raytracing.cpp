#include <iostream>
#include "raytracer/tracer.hpp"
#include "geometry/mesh.hpp"
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
    mesh -> ConstructAllRefAreas();
    //std::cout << "RefAreas: " << mesh->getnumRefAreas() << std::endl;
    for(label i=0; i<mesh -> getnumRefAreas(); i++)
    {
        RefArea* refarea = mesh -> getRefArea(i);
        auto center = refarea->get_object()->getcenter();
        //std::cout << center[0] << " "<<center[1] << std::endl;
    }
    //delete mesh;
   
    /*
    Tracer tracer = Tracer(mesh);

    tracer.CastAllRays(1000);
    //std::cout << "finish" << std::endl;
    for(label i=0; i<mesh -> getnumRefAreas(); i++)
    {
        RefArea* refarea = mesh -> getRefArea(i);
        auto hit = refarea->getweightstore();
        //std::cout << hit << std::endl;
    }
    */
    
    Tracer* tracer = new Tracer(mesh);
    Const::vecDd xbound = {2.0,3.0}; 
    Const::vecDd ybound = {7.0,7.0};
    RaySampler* splrptr = new RaySampler(xbound,ybound);
    tracer->AddNewSource(splrptr);
    //std::cout<<"finish 111"<<std::endl;
    tracer -> CastAllRays(100000,0);
    //std::cout << "finish 222" << std::endl;
    for(label i=0; i<mesh -> getnumRefAreas(); i++)
    {
        RefArea* refarea = mesh -> getRefArea(i);
        auto hit = refarea->getweightstore();
        //std::cout << hit << std::endl;
    }

    mesh -> ToSurface();
    delete tracer;
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