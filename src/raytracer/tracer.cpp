#include "tracer.hpp"
#include <iostream>

#pragma region RaySampler
RaySampler::RaySampler(const Const::vecDd &xbound,const Const::vecDd &ybound) : distribution(0.0,1.0)
{
    std::random_device rd;
    generator.seed(rd());
    for(int i = 0;i<Const::D;i++)
    {
    xbound_[i] = xbound[i];
    ybound_[i] = ybound[i];
    }
};

RaySampler::~RaySampler() {};

void RaySampler::SamplePos(Const::vecDd &pos)
{
    pos[0] = distribution(generator) * (xbound_[1] - xbound_[0]) + xbound_[0];
    pos[1] = distribution(generator) * (ybound_[1] - ybound_[0]) + ybound_[0];
}
void RaySampler::SampleDir(Const::vecDd &dir)
{
    //default is isotropic
    scalar theta = distribution(generator) * 2 * M_PI;
    dir[0] = cos(theta);
    dir[1] = sin(theta);
    //if(dir[1]>0)
    //{
    //    dir[1] = -dir[1];
    //}
}

#pragma endregion

#pragma region RayOperator
RayOperator::RayOperator() {};
RayOperator::~RayOperator() {};

void RayOperator::Reflect(Ray &ray, Const::vecDd& normal)
{   
    // dir_new[0] = dir[0] - 2dot_with_normal * normal[0]
    scalar dot = ray.dir_[0] * normal[0] + ray.dir_[1] * normal[1];
    ray.dir_[0] -= 2 * dot * normal[0];
    ray.dir_[1] -= 2 * dot * normal[1];
}

void RayOperator::Decay(Ray &ray, const scalar dWeight)
{
    // additive or multiplicative
    ray.weight_ -= dWeight;
    //ray.weight_ *= dWeight;
}

void RayOperator::UpdatePos(Ray &ray, const scalar dt)
{
    ray.pos_[0] += dt * ray.dir_[0];
    ray.pos_[1] += dt * ray.dir_[1];
    ray.time_ += dt;
}

#pragma endregion

#pragma region Tracer
Tracer::Tracer(Mesh* meshptr){
meshptr_ = meshptr;
optr_ = RayOperator();

};
Tracer::~Tracer(){};

Ray Tracer::InitNewRay(RaySampler* splrptr)
{
    Const::vecDd pos;
    Const::vecDd dir;
    splrptr -> SamplePos(pos);
    splrptr -> SampleDir(dir);
    Ray ray(pos, dir, 1.0);
    return ray;
}
Ray Tracer::InitNewRay(label splrindex)
{
    Const::vecDd pos;
    Const::vecDd dir;
    splr_[splrindex] -> SamplePos(pos);
    splr_[splrindex] -> SampleDir(dir);
    Ray ray(pos, dir, 1.0);
    return ray;
}

void Tracer::AddNewSource(RaySampler* splrptr)
{
    splr_.emplace_back(splrptr);
}


void Tracer::ItsctAllRefAreas(Ray &ray, RefArea* &hitrefarea, scalar &dt)
{
    scalar dt_ = Const::MAXIMUM_LIGHT_TIME;
    label numRefAreas = meshptr_->getnumRefAreas();
    //std::cout << hitrefarea << "???" <<std::endl;
    label I = 0;
    for(label i = 0; i < numRefAreas; i++)
    {
        // DOUBLE CHECK THIS PART
        scalar dt_temp = meshptr_->getRefArea(i) -> Intersect(ray);
        if(dt_temp > 0.0 && dt_temp < dt_)
        {
            dt_ = dt_temp;
            I = i;
        }
        
    }

    if(dt_ + ray.time_< Const::MAXIMUM_LIGHT_TIME)
    {
        hitrefarea = (meshptr_->getRefArea(I));
        dt = dt_;
    }

    //std::cout << hitrefarea << "!!!" <<std::endl;
}

void Tracer::ReInit(Ray& ray, RaySampler* splrptr)
{
    ray.time_ = 0;
    ray.weight_ = 1;
    splrptr -> SamplePos(ray.pos_);
    splrptr -> SampleDir(ray.dir_);
    ray.isValid_ = true;
}
void Tracer::ReInit(Ray& ray, label splrindex)
{
    ray.time_ = 0;
    ray.weight_ = 1;
    splr_[splrindex] -> SamplePos(ray.pos_);
    splr_[splrindex] -> SampleDir(ray.dir_);
    ray.isValid_ = true;
}

scalar Tracer::CalcDWeight(Ray &ray, RefArea &RefArea)
{   
    if(ray.weight_ < 1.0)
    {
        return ray.weight_;
    }
    else
    {
        return 1.0;
    }
    
}

void Tracer::UpdateAfterHit(Ray &ray, RefArea &refarea,scalar &dt)
{
    scalar weightdecay = CalcDWeight(ray, refarea);
    optr_.Decay(ray, weightdecay);
    optr_.UpdatePos(ray, dt);
    //std::cout << weightdecay << std::endl;
    refarea.WeightedHit(weightdecay);
    Const::vecDd normal = refarea.getnormal();
    optr_.Reflect(ray, normal );
    //std::cout << "hit ..." << std::endl;
}

void Tracer::CastOneRay(Ray &ray)
{
    RefArea* hitRefArea;
    scalar dt = Const::MAXIMUM_LIGHT_TIME;
    while(ray.isValid_)
    {
        //std::cout << "tracing ..." << std::endl;
        hitRefArea = nullptr;
        dt = Const::MAXIMUM_LIGHT_TIME;
        ItsctAllRefAreas(ray, hitRefArea, dt);
        //std::cout << "hitting ..." << std::endl;
        if(dt + ray.time_ < Const::MAXIMUM_LIGHT_TIME)
        {
            //std::cout<< "hit!!!" << std::endl;
            //std::cout << hitRefArea << std::endl;
            //std::cout << hitRefArea->getweightstore() << std::endl;
            UpdateAfterHit(ray, *hitRefArea, dt);
            
        }
        else
        {
            // in case of no hit: apply boundary condition, or simply destroy ray
            // we destroy ray here
            ray.isValid_ = false;
            //std::cout << "no hit ..." << std::endl;
        }

        // check if the ray exceeds life span, or the weight becomes negative
        ray.CheckValid();
        //std::cout << "done one ..." << std::endl;
        
    }
    //std::cout << "Ray finished" << std::endl;
}

void Tracer::CastAllRays(label numrays, label sourceID)
{
    label i;
    // pragma OMP here?
    //Ray ray = InitNewRay();
    //Const::vecDd xbound{0.0,1.0};
    //Const::vecDd ybound{0.0,1.0};
    for(i = 0; i < numrays; i++)
    {   
        //std::cout << "Ray initializing" << std::endl;
        Ray ray = InitNewRay(sourceID);
        //Tracer::ReInit(ray, xbound, ybound);
        //std::cout << "Ray initialized" << std::endl;
        CastOneRay(ray);
        // delete ray?
    }
}

void Tracer::NormalizeFlux()
{
    // to be implemented
}

void Tracer::CalcAllRate()
{
    // to be implemented
}

#pragma endregion
