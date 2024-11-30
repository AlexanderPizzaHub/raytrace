#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <array>
#include <string>

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


    /*
    以下部分之后替换为yaml
    */
    const std::string ray_direction = "default";
    const vecDd source_plane_xbox = {0.0,5.0};
    const vecDd source_plane_ybox = {1.95,2.05};
    const vecDd source_plane_xbox2 = {5.0,10.0};
    const vecDd source_plane_ybox2 = {7.95,8.05};

    const vecDd substrate_xbox = {0.0,10.0};
    const vecDd substrate_ybox = {-10.0,0.0};

    const std::string boundary_condition = "periodic";

    const scalar source_flux =  1e16 ;
    const scalar process_time = 30.0 ;
}



#endif // CONSTANTS_HPP