#include "marchingcube.hpp"
#include "levelset.hpp"
#include <iostream>

using namespace LevelSet;
/*
    从左上起，逆时针动，0表示正，1表示负，二进制数映射
    共16种情况分别写函数算
    每个函数接收refareaptrlist_，产生的新参考面加入到refareaptrlist_中
    */

scalar interpolant_zero(scalar pos0, scalar pos1, scalar phi0, scalar phi1)
{
    scalar zero = pos0 - phi0 * (pos1 - pos0) / (phi1 - phi0);
    // std::cout <<"check: " << pos1-pos0 << " "<<phi1 - phi0<< std::endl;
    // std::cout << pos0 << " " << pos1 << " " << phi0 << " " << phi1 << " " << zero << std::endl;
    return zero;
}

void AddRefArea(Const::vecDd &start, Const::vecDd &end, LevelSet::LevelSetFunction &levelset, Square &square)
{
    levelset.AddRefArea(start, end);
    LevelSet::RefArea *refarea = levelset.getRefArea(levelset.getnumRefAreas() - 1);
    refarea->linked_square_.emplace_back(&square);
}

void case0000_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    return;
}

void case0001_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |     \  |
    |      \ |
    |        |
    ----------
    */
    Const::vecDd start;
    Const::vecDd end;
    // std::cout <<"case0001" << std::endl;
    start[1] = square.linked_grids_[3]->y;
    // assert grid size =1
    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, levelset.getlsf(square.linked_grids_[3]->index), levelset.getlsf(square.linked_grids_[0]->index));

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, levelset.getlsf(square.linked_grids_[2]->index), levelset.getlsf(square.linked_grids_[3]->index));

    // change normal by swapping x and y

    AddRefArea(end, start, levelset, square);
}

void case0010_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |        |
    |      / |
    |     /  |
    ----------
    */
    Const::vecDd start;
    Const::vecDd end;

    // std::cout<<"case0010" << std::endl;
    //  assert grid size =1
    start[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, levelset.getlsf(square.linked_grids_[1]->index), levelset.getlsf(square.linked_grids_[2]->index));
    start[1] = square.linked_grids_[2]->y;

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[3]->y, square.linked_grids_[2]->y, levelset.getlsf(square.linked_grids_[3]->index), levelset.getlsf(square.linked_grids_[2]->index));

    AddRefArea(start, end, levelset, square);
}

void case0011_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    -----------
    |    |    |
    |    |    |
    |    |    |
    -----------
    */
    Const::vecDd start;
    Const::vecDd end;

    // std::cout << "case0011" << std::endl;
    //  assert grid size =1
    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, levelset.getlsf(square.linked_grids_[3]->index), levelset.getlsf(square.linked_grids_[0]->index));
    start[1] = square.linked_grids_[3]->y;

    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, levelset.getlsf(square.linked_grids_[1]->index), levelset.getlsf(square.linked_grids_[2]->index));
    end[1] = square.linked_grids_[2]->y;

    AddRefArea(end, start, levelset, square);
}

void case0100_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |        |
    |\       |
    | \      |
    ----------
    */
    Const::vecDd start;
    Const::vecDd end;

    /// std::cout << "case0100" << std::endl;
    // assert grid size =1
    start[0] = square.linked_grids_[0]->x;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[1]->index));

    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, levelset.getlsf(square.linked_grids_[1]->index), levelset.getlsf(square.linked_grids_[2]->index));
    end[1] = square.linked_grids_[1]->y;

    AddRefArea(start, end, levelset, square);
}

void case0101_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |     \  |
    | \    \ |
    |  \     |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case0101" << std::endl;
}

void case0110_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |        |
    |--------|
    |        |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case0110" << std::endl;

    Const::vecDd start;
    Const::vecDd end;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[1]->index));
    start[0] = square.linked_grids_[0]->x;

    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, levelset.getlsf(square.linked_grids_[2]->index), levelset.getlsf(square.linked_grids_[3]->index));
    end[0] = square.linked_grids_[2]->x;

    AddRefArea(start, end, levelset, square);
}

void case0111_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    -----------
    |  /      |
    | /       |
    |/        |
    -----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case0111" << std::endl;

    Const::vecDd start;
    Const::vecDd end;
    start[0] = interpolant_zero(square.linked_grids_[0]->x, square.linked_grids_[3]->x, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[3]->index));
    start[1] = square.linked_grids_[0]->y;

    end[0] = square.linked_grids_[1]->x;
    end[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[1]->index));

    AddRefArea(end, start, levelset, square);
}

void case1000_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    | /      |
    |/       |
    |        |
    ----------
    */
    // TODO
    // std::cout << "case1000" << std::endl;

    Const::vecDd start;
    Const::vecDd end;
    start[0] = interpolant_zero(square.linked_grids_[0]->x, square.linked_grids_[3]->x, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[3]->index));
    start[1] = square.linked_grids_[0]->y;

    end[0] = square.linked_grids_[1]->x;
    end[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[1]->index));

    AddRefArea(start, end, levelset, square);
}

void case1001_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |        |
    |--------|
    |        |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case1001" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    start[0] = square.linked_grids_[0]->x;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[1]->index));

    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, levelset.getlsf(square.linked_grids_[2]->index), levelset.getlsf(square.linked_grids_[3]->index));
    end[0] = square.linked_grids_[2]->x;

    AddRefArea(end, start, levelset, square);
}

void case1010_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |        |
    |      / |
    |     /  |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case1010" << std::endl;
}

void case1011_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    -----------
    |         |
    |\        |
    | \       |
    -----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case1011" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    // assert grid size =1
    start[0] = square.linked_grids_[0]->x;
    start[1] = interpolant_zero(square.linked_grids_[0]->y, square.linked_grids_[1]->y, levelset.getlsf(square.linked_grids_[0]->index), levelset.getlsf(square.linked_grids_[1]->index));

    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, levelset.getlsf(square.linked_grids_[1]->index), levelset.getlsf(square.linked_grids_[2]->index));
    end[1] = square.linked_grids_[1]->y;

    AddRefArea(end, start, levelset, square);
}

void case1100_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |    |   |
    |    |   |
    |    |   |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case1100" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, levelset.getlsf(square.linked_grids_[3]->index), levelset.getlsf(square.linked_grids_[0]->index));
    start[1] = square.linked_grids_[3]->y;

    end[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, levelset.getlsf(square.linked_grids_[1]->index), levelset.getlsf(square.linked_grids_[2]->index));
    end[1] = square.linked_grids_[2]->y;

    AddRefArea(start, end, levelset, square);
}

void case1101_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |       /|
    |      / |
    |     /  |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case1101" << std::endl;
    Const::vecDd start;
    Const::vecDd end;

    // assert grid size =1
    start[0] = interpolant_zero(square.linked_grids_[1]->x, square.linked_grids_[2]->x, levelset.getlsf(square.linked_grids_[1]->index), levelset.getlsf(square.linked_grids_[2]->index));
    start[1] = square.linked_grids_[2]->y;

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[3]->y, square.linked_grids_[2]->y, levelset.getlsf(square.linked_grids_[3]->index), levelset.getlsf(square.linked_grids_[2]->index));

    AddRefArea(end, start, levelset, square);
}

void case1110_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    /*
    ----------
    |      \ |
    |       \|
    |        |
    ----------
    */
    // TODO NEED TO DEAL AMBIGUOUITY
    // std::cout << "case1110" << std::endl;
    Const::vecDd start;
    Const::vecDd end;
    start[1] = square.linked_grids_[3]->y;
    // assert grid size =1
    start[0] = interpolant_zero(square.linked_grids_[3]->x, square.linked_grids_[0]->x, levelset.getlsf(square.linked_grids_[3]->index), levelset.getlsf(square.linked_grids_[0]->index));

    end[0] = square.linked_grids_[2]->x;
    end[1] = interpolant_zero(square.linked_grids_[2]->y, square.linked_grids_[3]->y, levelset.getlsf(square.linked_grids_[2]->index), levelset.getlsf(square.linked_grids_[3]->index));

    // change normal by swapping x and y

    AddRefArea(start, end, levelset, square);
}

void case1111_(LevelSet::LevelSetFunction &levelset, Square &square)
{
    return;
}

LevelSet::MarchingCube2D::MarchingCube2D()
{
    methodptrs_[0] = case0000_;
    methodptrs_[1] = case0001_;
    methodptrs_[2] = case0010_;
    methodptrs_[3] = case0011_;
    methodptrs_[4] = case0100_;
    methodptrs_[5] = case0101_;
    methodptrs_[6] = case0110_;
    methodptrs_[7] = case0111_;
    methodptrs_[8] = case1000_;
    methodptrs_[9] = case1001_;
    methodptrs_[10] = case1010_;
    methodptrs_[11] = case1011_;
    methodptrs_[12] = case1100_;
    methodptrs_[13] = case1101_;
    methodptrs_[14] = case1110_;
    methodptrs_[15] = case1111_;
}

LevelSet::MarchingCube2D::~MarchingCube2D()
{
    // do nothing
}

void LevelSet::MarchingCube2D::SetRefArea(LevelSet::LevelSetFunction &levelset, Square &square)
{
    label ul, ll, lr, ur;
    ul = (label)(levelset.getlsf(square.linked_grids_[0]->index) < 0);
    ll = (label)(levelset.getlsf(square.linked_grids_[1]->index) < 0);
    lr = (label)(levelset.getlsf(square.linked_grids_[2]->index) < 0);
    ur = (label)(levelset.getlsf(square.linked_grids_[3]->index) < 0);

    // std::cout << ul << " " << ll << " " << lr << " " << ur << std::endl;
    label index = ul * 8 + ll * 4 + lr * 2 + ur;

    methodptrs_[index](levelset, square);
}
