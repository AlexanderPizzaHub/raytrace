#pragma once

#include "constants.hpp"
#include "objects.hpp"
#include "mesh.hpp"
#include "levelset/levelset.hpp"

#include <vector>

class Square;
namespace LevelSet
{
    class LevelSetFunction;
}
class MarchingCube2D
{
    public:
        MarchingCube2D();
        ~MarchingCube2D();

        void (*methodptrs_[16])(LevelSet::LevelSetFunction& levelset, Square& square);

        void SetRefArea(LevelSet::LevelSetFunction& levelset, Square& square);

    private:

    /*
    从左上起，逆时针动，0表示正，1表示负，二进制数映射
    共16种情况分别写函数算
    每个函数接收refareaptrlist_，产生的新参考面加入到refareaptrlist_中
    */

    void (*case0000)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case0001)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case0010)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case0011)(LevelSet::LevelSetFunction& levelset, Square& square);

    void (*case0100)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case0101)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case0110)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case0111)(LevelSet::LevelSetFunction& levelset, Square& square);

    void (*case1000)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case1001)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case1010)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case1011)(LevelSet::LevelSetFunction& levelset, Square& square);

    void (*case1100)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case1101)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case1110)(LevelSet::LevelSetFunction& levelset, Square& square);
    void (*case1111)(LevelSet::LevelSetFunction& levelset, Square& square);
    
};