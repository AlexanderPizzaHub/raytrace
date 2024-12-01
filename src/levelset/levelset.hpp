#ifndef LS_EVOLVE_HPP
#define LS_EVOLVE_HPP

#include "geometry/mesh.hpp"
#include "geometry/objects.hpp"
#include "raytracer/ray.hpp"
#include "constants.hpp"
#include "levelset/marchingcube.hpp"
#include <vector>

class Mesh;
class Line;

namespace LevelSet
{
    class MarchingCube2D;

    class RefArea: public Line
    {
        public:
            //using Line::Line; // use the constructor of the base class
            RefArea(Const::vecDd& start, Const::vecDd& end);
            RefArea(Const::vecDd& center, scalar radius, Const::vecDd& normal);

            ~RefArea(); 
            
            scalar Intersect(Ray& ray); //implement ray-line intersection. Return hit time

            std::vector<Square*> linked_square_; //lengh=1

            //void Hit(); // increase the hit counter
            void WeightedHit(scalar dWeight); // increase the weight
            void WeightAdd(scalar dweight);
            //void WeightedHit(scalar dWeight, Const::vecDd position); // increase the weight. Hitpoint information is used
            void SetWeight(scalar weight); 
            
            void SetRate(scalar rate);
            scalar getrate();
            
            Const::vecDd getnormal();
            scalar getweightstore();
            
            void set_object(Line *lineptr);
            Line* get_object();

            

        private:
            //Line* lineptr_;
            scalar weightstore_;
            scalar rate_;
            //label hitcounter_; // --> integers storing the number of hits
            // --> store the weight of the ray
    };


    class LevelSetFunction
    {
        public:
            LevelSetFunction(Mesh* meshptr);
            ~LevelSetFunction();

                    // related to refarea construction
            bool IdentifyEffectiveCube(Square& square);
            void SetMarchingCube(Square& square);

            void ConstructAllRefAreas(); // construct all reference area and set the linkings.

            void AddRefArea( Const::vecDd& start, Const::vecDd& end);
            RefArea* getRefArea(label index);
            label getnumRefAreas();


            void ClearVelocity();
            void VelocityExtension();
            void Evolve(scalar dt);

            scalar getlsf(label index);
            void setlsf(label index, scalar value);
            scalar getvf(label index);
            void setvf(label index, scalar value);

            void ToSurface(std::string outputpath); // convert the mesh to surface and save it to file



        private:
            Mesh* meshptr_;        
            MarchingCube2D* marchingcube_;
            std::vector<scalar> velocityfield_;
            std::vector<scalar> levelsetfunction_;

            std::vector<RefArea> refareas_;
            
    };
}; // namespace LevelSet


#endif // LS_EVOLVE_HPP 