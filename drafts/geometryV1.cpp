#include "geometry.hpp"
#include <iostream>

#pragma region consAndDes
SparseFieldGrid::SparseFieldGrid(scalar x_, scalar y_, label i, RefArea *refAreaPtr_) : 
        index(i), phi(0.0), velX(0.0), velY(0.0), refAreaPtr(refAreaPtr_), next(nullptr) 
       {
              coords[0] = x_;
              coords[1] = y_;
              if(refAreaPtr_ == nullptr)
              {
                  refAreaPtr_ = new RefArea();
              }
       };
SparseFieldGrid::~SparseFieldGrid()
{
    if(refAreaPtr != nullptr)
    {
        delete refAreaPtr;
    }
    if(next != nullptr)
    {
        delete next;
    }
};

RefArea::RefArea() : radius(0.0), hitCounter(0) {};
RefArea::~RefArea() {};

Geometry::Geometry() : L0head(nullptr) {};
Geometry::~Geometry()
{
    if(L0head != nullptr)
    {
        delete L0head;
    }
};
#pragma endregion

#pragma region refAreaMethods
void RefArea::hit()
        {
            hitCounter++;
        }

#pragma endregion

#pragma region sparseFieldGridMethods
void SparseFieldGrid::constructRefArea()
{
    // Location: exactly where the grid is 
    // Normal: compute using the next point in chain
    // Radius: defined to be the gradient of phi
    scalar dx = 0.0;
    scalar dy = 0.0;
    scalar phiChange = 0.0;

    if(next != nullptr)
    {
        dx += next->coords[0] - coords[0];
        dy += next->coords[1] - coords[1];
        phiChange += next->phi - phi;
    }
    if(prev != nullptr)
    {
        dx += prev->coords[0] - coords[0];
        dy += prev->coords[1] - coords[1];
    }
}
void SparseFieldGrid::constructRefArea_test()
{
    refAreaPtr->coords[0] = coords[0];
    refAreaPtr->coords[1] = coords[1];
    refAreaPtr->normal[0] = 0.705;
    refAreaPtr->normal[1] = 0.705;
    //refAreaPtr->normal[0] = 0.0;
    //refAreaPtr->normal[1] = 1.0;
    refAreaPtr->radius = 1.0/(64.0)/(2.0);
}
void SparseFieldGrid::calcRate()
{
    scalar tmpFlux = refAreaPtr->hitCounter;
    velX = tmpFlux * refAreaPtr->normal[0];
    velY = tmpFlux * refAreaPtr->normal[1];
}
#pragma endregion

#pragma region geometryMethods
void Geometry::castOneRay(Ray &ray_)
{
    while(ray_.isValid)
    {   
        scalar dt = 9999;
        scalar dt_ = -1.0;
        RefArea* hitAreaPtr = NULL;
        
        itsctAllRefAreas(ray_, dt, dt_, hitAreaPtr);

        if(hitAreaPtr == NULL)
        {
            ray_.isValid = false;
            break;
        }

        
    }
}

scalar Geometry::calcDWeight(Ray &ray_, RefArea &refArea_)
{
    return 0.5;
}

void Geometry::updateAfterHit(Ray &ray_, RefArea &refArea_, scalar &dt)
{
    scalar dWeight = calcDWeight(ray_, refArea_);
    ray_.optr -> Decay(ray_,dWeight);
    ray_.optr -> UpdateLoc(ray_,dt);
    ray_.optr -> Reflect(ray_, refArea_);
    //refArea_.hit(); // simply count
    refArea_.refAreaUpdateAfterHit(dWeight, ray_.pos);
}

void Geometry::itsctAllRefAreas(Ray &ray_, scalar &dt, scalar &dt_, RefArea* hitAreaPtr)
{
    for(RefArea* refAreaPtr: refAreaPtrs)
    {
        dt_ = ray_.optr->RayRefItsct(ray_,*refAreaPtr);
        if(dt_ > 0 && dt_ < dt)
        {
            dt = dt_;
            hitAreaPtr = refAreaPtr;
        }
    }
}


void Geometry::castAllRays(label numRays)
{   
    
    RaySampler sampler = RaySampler();
    RayOperator operator_ = RayOperator();
    vecDd xBound = {0.0,1.0}; vecDd yBound = {0.0,0.1};
    for(label rayInd=0; rayInd<numRays; rayInd++)
    {
        //std::cout << "Ray " << rayInd << " is casted" << std::endl;
        Ray newRay = Ray(&sampler, &operator_);
        newRay.ReInit(xBound, yBound);
        //std::cout <<"Ray " << rayInd << " with pos and dir "<< newRay.pos[0] << " "<<newRay.dir[0] << std::endl;
        SparseFieldGrid* curGrid = L0head;
        while(curGrid != nullptr)
        {   
            //std::cout << "diskchecking: " << curGrid->index << std::endl;
            if(newRay.optr->RayRefItsct(newRay, *(curGrid->refAreaPtr)))
            {
                //std::cout<<"Ray " << rayInd << " hit grid " << curGrid->index << std::endl;
                curGrid->refAreaPtr->hit();
            } // 暂时先不做反射
            //std::cout << "diskchecking: " << curGrid->index <<" done"<< std::endl;
            curGrid = curGrid->next;
        }
        //std::cout << "checking done"<< std::endl;
    }
}
#pragma endregion