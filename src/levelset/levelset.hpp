#ifndef LS_EVOLVE_HPP
#define LS_EVOLVE_HPP

#include "geometry/mesh.hpp"
#include <vector>

class Mesh;
namespace LevelSet
{
    class LevelSetFunction
    {
        public:
            LevelSetFunction(Mesh* meshptr);
            ~LevelSetFunction();

            void ClearVelocity();
            void VelocityExtension();
            void Evolve(scalar dt);

            scalar getlsf(label index);
            void setlsf(label index, scalar value);
            scalar getvf(label index);
            void setvf(label index, scalar value);

            
            

        private:
            Mesh* meshptr_;        
            std::vector<scalar> velocityfield_;
            std::vector<scalar> levelsetfunction_;
            
    };
}; // namespace LevelSet


#endif // LS_EVOLVE_HPP 