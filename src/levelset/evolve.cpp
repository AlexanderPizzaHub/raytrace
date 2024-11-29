#include "evolve.hpp"

LevelSet::Evolve::Evolve(Mesh* meshptr)
{
    meshptr_ = meshptr;
}

LevelSet::Evolve::~Evolve()
{
}

void LevelSet::Evolve::evolve()
{
    //std::cout << "evolve" << std::endl;
    scalar dt = 0.1;
    scalar rate = 0.5;
    label numgrids = meshptr_->getnumGrids();
    for(label i = 0; i<numgrids; i++)
    {
        meshptr_->getGrid(i)->phi -=  rate * dt;
    }
}