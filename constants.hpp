#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <array>

typedef double scalar;
typedef unsigned int label;

namespace Const
{
    const int D = 2; // dimension
    //typedef scalar vecDd[D];
    //typedef label vecDi[D];
    typedef std::array<scalar, D> vecDd;
    typedef std::array<label, D> vecDi;

    const scalar SMALL = 1e-20;
    const scalar MAXIMUM_LIGHT_TIME = 100.0;
}



#endif // CONSTANTS_HPP