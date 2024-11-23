#include <iostream>
#include "raytracer/tracer.hpp"
#include "geometry/mesh.hpp"

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
    std::cout << "finish" << std::endl;
    for(label i=0; i<mesh -> getnumRefAreas(); i++)
    {
        RefArea* refarea = mesh -> getRefArea(i);
        auto hit = refarea->getweightstore();
        std::cout << hit << std::endl;
    }
    */
    
    Tracer* tracer = new Tracer(mesh);
    //std::cout<<"finish 111"<<std::endl;
    tracer -> CastAllRays(1000);
    ///std::cout << "finish 222" << std::endl;
    for(label i=0; i<mesh -> getnumRefAreas(); i++)
    {
        RefArea* refarea = mesh -> getRefArea(i);
        auto hit = refarea->getweightstore();
        std::cout << hit << std::endl;
    }

    delete tracer;
    

   return 0;
}


int main()
{
    testrun();
    std::cout << "finish" << std::endl;
    return 0;
}