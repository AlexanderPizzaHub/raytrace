#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

typedef double scalar;
typedef unsigned int label;

namespace Const
{
    const int D = 2; // dimension
    typedef scalar vecDd[D];
    typedef label vecDi[D];

    const scalar SMALL = 1e-20;
    const scalar MAXIMUM_LIGHT_TIME = 100.0;
}



#endif // CONSTANTS_HPP