#pragma once

#include "../constants.hpp"
#include "../geometry/objects.hpp"
#include "../geometry/mesh.hpp"

#include <vector>

class Square;
class MarchingCube2D
{
    public:
        MarchingCube2D();
        ~MarchingCube2D();

        void (*methodptrs_[16])(Square& square);

        void SetRefArea(Square& square);

    private:

    /*
    从左上起，逆时针动，0表示正，1表示负，二进制数映射
    共16种情况分别写函数算
    每个函数接收refareaptrlist_，产生的新参考面加入到refareaptrlist_中
    */

    void (*case0000)(Square& square);
    void (*case0001)(Square& square);
    void (*case0010)(Square& square);
    void (*case0011)(Square& square);

    void (*case0100)(Square& square);
    void (*case0101)(Square& square);
    void (*case0110)(Square& square);
    void (*case0111)(Square& square);

    void (*case1000)(Square& square);
    void (*case1001)(Square& square);
    void (*case1010)(Square& square);
    void (*case1011)(Square& square);

    void (*case1100)(Square& square);
    void (*case1101)(Square& square);
    void (*case1110)(Square& square);
    void (*case1111)(Square& square);
    
};