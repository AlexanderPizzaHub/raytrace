#include "ray.hpp"
#include <algorithm>

Ray::Ray(Const::vecDd pos, Const::vecDd dir) : 
time_(0), weight_(1), isValid_(true)
{
    std::copy(pos, pos + Const::D, pos_);
    std::copy(dir, dir + Const::D, dir_);
}

Ray::Ray(Const::vecDd pos, Const::vecDd dir, scalar weight) :
time_(0), weight_(weight), isValid_(true)
{
    std::copy(pos, pos + Const::D, pos_);
    std::copy(dir, dir + Const::D, dir_);
}

Ray::Ray() :
time_(0), weight_(1), isValid_(true)
{
    std::fill(pos_, pos_ + Const::D, 0);
    std::fill(dir_, dir_ + Const::D, 0);
}

void Ray::CheckValid()
{
    if(weight_ <= Const::SMALL || time_ > Const::MAXIMUM_LIGHT_TIME || time_ < 0)
    {
        isValid_ = false;
    }
}
