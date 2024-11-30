#include "levelset.hpp"
#include <array>
#include <iostream>
using namespace LevelSet;

LevelSetFunction::LevelSetFunction(Mesh* meshptr)
{
    meshptr_ = meshptr;
    velocityfield_.resize(meshptr_->getnumGrids(),0.0);
    levelsetfunction_.resize(meshptr_->getnumGrids(),0.0);
    //std::cout<<"lsf constructed "<<meshptr_->getnumGrids()<<std::endl;
}

LevelSetFunction::~LevelSetFunction()
{
}

void LevelSetFunction::Evolve(scalar dt)
{
    //std::cout << "evolve" << std::endl;
    label numgrids_active = levelsetfunction_.size();
    for(label i = 0; i<numgrids_active; i++)
    {
        levelsetfunction_[i] -= velocityfield_[i]*dt;
    }
}

void LevelSetFunction::ClearVelocity()
{
    for(label i = 0; i<velocityfield_.size(); i++)
    {
        velocityfield_[i] = 0.0;
    }
}

void LevelSetFunction::VelocityExtension()
{
    label nsquares = meshptr_->getnumSquares();
    for(label squareind = 0; squareind<nsquares; squareind++)
    {
        Square* square = meshptr_->getSquare(squareind);
        for(RefArea* refarea: square->linked_refareas_)
        {
            Const::vecDd center = refarea->get_object()->getcenter();
            GridCartesian* gridul = square->linked_grids_[0];
            GridCartesian* gridll = square->linked_grids_[1];
            GridCartesian* gridlr = square->linked_grids_[2];
            GridCartesian* gridur = square->linked_grids_[3];

            scalar areaul = (center[0]-gridul->x)*(center[1]-gridul->y);
            scalar areall = (center[0]-gridll->x)*(gridll->y-center[1]);
            scalar arealr = (gridlr->x-center[0])*(gridlr->y-center[1]);
            scalar areaur = (gridur->x-center[0])*(center[1]-gridur->y);

            scalar area = areaul + areall + arealr + areaur;

            velocityfield_[gridul->index] += arealr/area * refarea->getrate();
            velocityfield_[gridll->index] += areaur/area * refarea->getrate();
            velocityfield_[gridlr->index] += areaul/area * refarea->getrate();
            velocityfield_[gridur->index] += areall/area * refarea->getrate();
        }
    }
}
scalar LevelSetFunction::getlsf(label index)
{
    return levelsetfunction_[index];
}

void LevelSetFunction::setlsf(label index, scalar value)
{
    //std::cout <<"setting lsf..." << index <<std::endl;
    levelsetfunction_[index] = value;
    
}

scalar LevelSetFunction::getvf(label index)
{
    return velocityfield_[index];
}

void LevelSetFunction::setvf(label index, scalar value)
{
    velocityfield_[index] = value;
}


