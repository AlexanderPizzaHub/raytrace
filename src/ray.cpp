#include "ray.hpp"
#include <algorithm>

Ray::Ray(Const::vecDd& pos, Const::vecDd& dir) : 
time_(0), weight_(1), isValid_(true)
{
   pos_ = pos;
   dir_ = dir;
}

Ray::Ray(Const::vecDd& pos, Const::vecDd& dir, scalar weight) :
time_(0), weight_(weight), isValid_(true)
{
    pos_ = pos;
    dir_ = dir;
}

Ray::Ray() :
time_(0), weight_(1), isValid_(true)
{
    pos_.fill(0);
    dir_.fill(0);
}

void Ray::CheckValid()
{
    if(weight_ <= Const::SMALL || time_ > Const::MAXIMUM_LIGHT_TIME || time_ < 0)
    {
        isValid_ = false;
    }
}
