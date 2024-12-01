#pragma once

#include "constants.hpp"
#include "geometry/objects.hpp"
#include "geometry/mesh.hpp"
#include "levelset/levelset.hpp"

#include <vector>

class Square;
namespace LevelSet
{

class LevelSetFunction;

class MarchingCube2D
{
    public:
        MarchingCube2D();
        ~MarchingCube2D();

        void (*methodptrs_[16])(LevelSet::LevelSetFunction& levelset, Square& square);

        void SetRefArea(LevelSet::LevelSetFunction& levelset, Square& square);

};
    
    } // namespace LevelSet