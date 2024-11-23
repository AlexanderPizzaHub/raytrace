#include "objects.hpp"
#include <cmath>

#pragma region Line
Line::Line(Line& line)
{
    scalar* center = line.getcenter();
    scalar* normal = line.getnormal();
    //std::copy(center, center + Const::D, center_);
    //std::copy(normal, normal + Const::D, normal_);
    for(label i=0; i<Const::D; i++)
    {
        center_[i] = center[i];
        normal_[i] = normal[i];
    }
    radius_ = line.getradius();

    delete center;
    delete normal;
}

Line::Line(Const::vecDd center, scalar radius, Const::vecDd normal)
{
    //std::copy(center, center + Const::D, center_);
    //std::copy(radius, radius + Const::D, radius_);
    //std::copy(normal, normal + Const::D, normal_);  // 能不能赋地址？
    for(label i=0; i<Const::D; i++)
    {
        center_[i] = center[i];
        normal_[i] = normal[i];
    }
    radius_ = radius;
}

Line::Line(Const::vecDd start, Const::vecDd end)
{
    center_[0] = (start[0] + end[0]) / 2;
    center_[1] = (start[1] + end[1]) / 2;
    radius_ = std::sqrt((start[0] - center_[0]) * (start[0] - center_[0]) + (start[1] - center_[1]) * (start[1] - center_[1]));


    normal_[0] = (end[1] - start[1]) / radius_ / 2.0;
    normal_[1] = (start[0] - end[0]) / radius_ / 2.0;
}

Line::~Line()
{
    delete[] center_;
    delete[] normal_;
}

scalar* Line::getcenter()
{
    return center_;
}

scalar Line::getradius()
{
    return radius_;
}

scalar* Line::getnormal()
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

#pragma endregion

#pragma region Square
Square::Square(Square& square)
{
    label* lowerleft = square.getlowerleft();
    label* upperright = square.getupperright();
    std::copy(lowerleft, lowerleft + Const::D, lowerleft_);
    std::copy(upperright, upperright + Const::D, upperright_);

    delete lowerleft;
    delete upperright;
}

Square::Square(Const::vecDi lowerleft, Const::vecDi upperright)
{
    std::copy(lowerleft, lowerleft + Const::D, lowerleft_);
    std::copy(upperright, upperright + Const::D, upperright_);
}

Square::~Square()
{
    delete[] lowerleft_;
    delete[] upperright_;
}

label* Square::getlowerleft()
{
    return lowerleft_;
}

label* Square::getupperright()
{
    return upperright_;
}

scalar Square::Intersect(Ray& ray)
{
    // mostly for kdtree. TODO
    return 0.0;
}

#pragma endregion