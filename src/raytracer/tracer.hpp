#ifndef RAYTRACER_TRACER_HPP
#define RAYTRACER_TRACER_HPP

#include "geometry/mesh.hpp"
#include "raytracer/ray.hpp"
#include "constants.hpp"

#include <random>

// 两个提供给光线的享元, 代表产生光线的光源
class RaySampler
{
    public:
        void SamplePos(Const::vecDd &pos);
        void SampleDir(Const::vecDd &dir);

        RaySampler(const Const::vecDd& xbound,const Const::vecDd& ybound);
        ~RaySampler();

    private:
        Const::vecDd xbound_;
        Const::vecDd ybound_;
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

        void AddNewSource(RaySampler* splrptr);

        Ray InitNewRay(RaySampler* splrptr);
        Ray InitNewRay(label splrindex);

        // upon tracing
        void ItsctAllRefAreas(Ray &ray, RefArea* &hitRefArea, scalar &dt);

        // upon intersection
        void ReInit(Ray& ray, RaySampler* splrptr);
        void ReInit(Ray& ray, label splrindex);
        scalar CalcDWeight(Ray &ray, RefArea &refarea);
        void UpdateAfterHit(Ray &ray, RefArea &refarea,scalar &dt);

        // trace single ray
        void CastOneRay(Ray &ray);
        
        // trace all
        void CastAllRays(label numrays, label sourceID); // cast rays to the Geometry
        
        // after tracing
        void NormalizeFlux(scalar numrays); // align the flux on surface with flux on source plane
        void CalcAllRate(); // distribute flux on refarea to grids. This will use the linking information of refarea


    private:
        Mesh* meshptr_;

        std::vector<RaySampler*> splr_;
        RayOperator optr_;

};

#endif 