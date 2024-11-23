#include "Ray.hpp"
#include <random>

#pragma region consAndDes
Ray::Ray(RaySampler *splr_,RayOperator *optr_) : 
time(0.0), weight(1.0), splr(splr_),optr(optr_), isValid(true)
{};
Ray::~Ray() {};

RaySampler::RaySampler() : distribution(0.0,1.0)
{
    std::random_device rd;
    generator.seed(rd());
};
RaySampler::~RaySampler() {};

RayOperator::RayOperator() {};
RayOperator::~RayOperator() {};
#pragma endregion


#pragma region rayMethods 
void Ray::ReInit(vecDd &xBound, vecDd &yBound)
{
    time = 0.0;
    weight = 1.0;
    splr->SamplePos(xBound,yBound,pos);
    splr->SampleDir(dir);
}

#pragma endregion

#pragma region RaySamplerMethods
void RaySampler::SamplePos(const vecDd &xBound,const vecDd &yBound, vecDd &pos)
{
    pos[0] = distribution(generator) * (xBound[1] - xBound[0]) + xBound[0];
    pos[1] = distribution(generator) * (yBound[1] - yBound[0]) + yBound[0];
}
void RaySampler::SampleDir(vecDd &dir)
{
    scalar theta = distribution(generator) * 2 * M_PI;
    dir[0] = cos(theta);
    dir[1] = sin(theta);
    if(dir[1]>0)
    {
        dir[1] = -dir[1];
    }
}
#pragma endregion

#pragma region RayOperatorMethods
void RayOperator::Reflect(Ray &ray_, const RefArea &RefArea)
{   
    // dir_new[0] = dir[0] - 2dot_with_normal * normal[0]
    scalar dot = ray_.dir[0] * RefArea.normal[0] + ray_.dir[1] * RefArea.normal[1];
    ray_.dir[0] -= 2 * dot * RefArea.normal[0];
    ray_.dir[1] -= 2 * dot * RefArea.normal[1];
}
scalar RayOperator::RayRefItsct(Ray &ray_, const RefArea &RefArea)
{
    
    scalar dt_ = - (RefArea.normal[0]*(ray_.pos[0]-RefArea.coords[0]) + RefArea.normal[1]*(ray_.pos[1]-RefArea.coords[1])) 
    / (RefArea.normal[0]*ray_.dir[0] + RefArea.normal[1]*ray_.dir[1]);
    vecDd hitPoint = {ray_.pos[0] + dt_ * ray_.dir[0], ray_.pos[1] + dt_ * ray_.dir[1]};
    if (dt_ < 0 || 
    (hitPoint[0]-RefArea.coords[0])*(hitPoint[0]-RefArea.coords[0]) + 
    (hitPoint[1]-RefArea.coords[1])*(hitPoint[1]-RefArea.coords[1]) > RefArea.radius*RefArea.radius
    )
    {
        return -1.0;
    }
    else
    {
        //ray_.time += dt_;
        //ray_.pos[0] = hitPoint[0];
        //ray_.pos[1] = hitPoint[1];
        return dt_;
    }
}