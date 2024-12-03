#include "objects.hpp"
#include <cmath>

#pragma region GridCartesian
GridCartesian::GridCartesian(scalar x_, scalar y_, label index_)
{
    x = x_;
    y = y_;
    index = index_;
}

GridCartesian::~GridCartesian()
{
}

#pragma region Line

Line::Line()
{
}
/*
Line::Line(Line& line)
{
    center_ = line.getcenter();
    normal_ = line.getnormal();
    radius_ = line.getradius();
}
*/
Line::Line(Const::vecDd &center, scalar radius, Const::vecDd &normal)
{
    center_ = center;
    normal_ = normal;
    radius_ = radius;
}

Line::Line(const Const::vecDd &start, const Const::vecDd &end)
{
    center_[0] = (start[0] + end[0]) / 2;
    center_[1] = (start[1] + end[1]) / 2;
    radius_ = std::sqrt((start[0] - center_[0]) * (start[0] - center_[0]) + (start[1] - center_[1]) * (start[1] - center_[1]));

    normal_[0] = (end[1] - start[1]) / radius_ / 2.0;
    normal_[1] = (start[0] - end[0]) / radius_ / 2.0;
    // 右手定则
}

Line::~Line()
{
    // delete[] center_;
    // delete[] normal_;
}

Const::vecDd Line::getcenter()
{
    return center_;
}

scalar Line::getradius()
{
    return radius_;
}

Const::vecDd Line::getnormal()
{
    return normal_;
}

void Line::getendpoints(Const::vecDd &start, Const::vecDd &end)
{
    // NEED DOUBLE CHECK
    start[0] = center_[0] - radius_ * normal_[1];
    start[1] = center_[1] + radius_ * normal_[0];
    end[0] = center_[0] + radius_ * normal_[1];
    end[1] = center_[1] - radius_ * normal_[0];
}

#pragma endregion

#pragma region Square

Square::Square(Const::vecDi lowerleft, Const::vecDi upperright)
{
    // TOFO
}

Square::Square(GridCartesian &ul, GridCartesian &ll, GridCartesian &lr, GridCartesian &ur)
{
    linked_grids_.emplace_back(&ul);
    linked_grids_.emplace_back(&ll);
    linked_grids_.emplace_back(&lr);
    linked_grids_.emplace_back(&ur);
}

Square::~Square()
{
}

scalar Square::Intersect(Ray &ray)
{
    // mostly for kdtree. TODO
    return 0.0;
}

#pragma endregion