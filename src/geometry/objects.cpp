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
Line::Line(Line& line)
{
    center_ = line.getcenter();
    normal_ = line.getnormal();
    radius_ = line.getradius();
}

Line::Line(Const::vecDd& center, scalar radius, Const::vecDd& normal)
{
    center_ = center;
    normal_ = normal;
    radius_ = radius;
}

Line::Line(Const::vecDd& start, Const::vecDd& end)
{
    center_[0] = (start[0] + end[0]) / 2;
    center_[1] = (start[1] + end[1]) / 2;
    radius_ = std::sqrt((start[0] - center_[0]) * (start[0] - center_[0]) + (start[1] - center_[1]) * (start[1] - center_[1]));


    normal_[0] = (end[1] - start[1]) / radius_ / 2.0;
    normal_[1] = (start[0] - end[0]) / radius_ / 2.0;
}

Line::~Line()
{
    //delete[] center_;
    //delete[] normal_;
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

scalar Line::Intersect(Ray& ray)
{
    scalar dt_ = - (normal_[0]*(ray.pos_[0]-center_[0]) + normal_[1]*(ray.pos_[1]-center_[1])) 
    / (normal_[0]*ray.dir_[0] + normal_[1]*ray.dir_[1]);
    Const::vecDd hitPoint = {ray.pos_[0] + dt_ * ray.dir_[0], ray.pos_[1] + dt_ * ray.dir_[1]};
    if (dt_ < 0 || dt_ > Const::MAXIMUM_LIGHT_TIME ||
    (hitPoint[0]-center_[0])*(hitPoint[0]-center_[0]) + 
    (hitPoint[1]-center_[1])*(hitPoint[1]-center_[1]) > radius_*radius_
    )
    {
        return -1.0; // no hit
    }
    else
    {
        return dt_;
    }
}

void Line::getendpoints(Const::vecDd& start, Const::vecDd& end)
{
    // NEED DOUBLE CHECK
    start[0] = center_[0] - radius_ * normal_[1];
    start[1] = center_[1] + radius_ * normal_[0];
    end[0] = center_[0] + radius_ * normal_[1];
    end[1] = center_[1] - radius_ * normal_[0];
}

#pragma endregion

#pragma region Square
Square::Square(Square& square)
{
    //TODO
}

Square::Square(Const::vecDi lowerleft, Const::vecDi upperright)
{
    // TOFO
}

Square::Square(GridCartesian* ul, GridCartesian* ll, GridCartesian* lr, GridCartesian* ur)
{
    linked_grids_.emplace_back(ul);
    linked_grids_.emplace_back(ll);
    linked_grids_.emplace_back(lr);
    linked_grids_.emplace_back(ur);

}

Square::~Square()
{
}


scalar Square::Intersect(Ray& ray)
{
    // mostly for kdtree. TODO
    return 0.0;
}

#pragma endregion