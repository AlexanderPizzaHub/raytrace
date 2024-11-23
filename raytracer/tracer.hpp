#ifndef RAYTRACER_TRACER_HPP
#define RAYTRACER_TRACER_HPP

#include "../geometry/mesh.hpp"
#include "../raytracer/ray.hpp"
#include "../constants.hpp"

#include <random>

// 两个提供给光线的享元
class RaySampler
{
    public:
        void SamplePos(const Const::vecDd &xBound,const Const::vecDd &yBound, Const::vecDd &pos);
        void SampleDir(Const::vecDd &dir);

        RaySampler();
        ~RaySampler();

    private:
        std::mt19937 generator;
        std::uniform_real_distribution<scalar> distribution;
};

class RayOperator
{
    public:
        void Reflect(Ray &ray,Const::vecDd &normal);

        void Decay(Ray &ray, const scalar dweight);
        void UpdatePos(Ray &ray, const scalar dt);


        RayOperator();
        ~RayOperator();
};


class Tracer
{
    public:
        Tracer(Mesh* meshptr);
        ~Tracer();

        Ray InitNewRay();

        // upon tracing
        void ItsctAllRefAreas(Ray &ray, RefArea* &hitRefArea, scalar &dt);

        // upon intersection
        void ReInit(Ray& ray, Const::vecDd& xBound, Const::vecDd& yBound);
        scalar CalcDWeight(Ray &ray, RefArea &refarea);
        void UpdateAfterHit(Ray &ray, RefArea &refarea,scalar &dt);

        // trace single ray
        void CastOneRay(Ray &ray);
        
        // trace all
        void CastAllRays(label numrays); // cast rays to the Geometry
        
        // after tracing
        void NormalizeFlux(); // align the flux on surface with flux on source plane
        void CalcAllRate(); // distribute flux on refarea to grids. This will use the linking information of refarea


    private:
        Mesh* meshptr_;

        RaySampler splr_;
        RayOperator optr_;
};

#endif 