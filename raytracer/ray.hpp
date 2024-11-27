#ifndef RAY_HPP
#define RAY_HPP

#include "../constants.hpp"

class Ray
{
    public:
        Const::vecDd pos_; // current position of Ray
        scalar time_; // current time of Ray
        Const::vecDd dir_; // direction of Ray

        // material properties
        scalar weight_;

        // if still valid
        bool isValid_;

        void CheckValid();

        Ray(Const::vecDd& pos, Const::vecDd& dir);
        Ray(Const::vecDd& pos, Const::vecDd& dir, scalar weight);
        Ray();
};

#endif // RAY_HPP
