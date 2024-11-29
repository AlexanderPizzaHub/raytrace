#ifndef LS_EVOLVE_HPP
#define LS_EVOLVE_HPP

#include "mesh.hpp"

namespace LevelSet
{
    class Evolve
    {
        public:
            Evolve(Mesh* meshptr);
            ~Evolve();

            void evolve();

        private:
            Mesh* meshptr_;        
            
    };
}; // namespace LevelSet


#endif // LS_EVOLVE_HPP 