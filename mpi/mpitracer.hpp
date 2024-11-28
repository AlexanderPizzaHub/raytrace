#pragma once

#include "mpi.h"

namespace MpiTracer
{
    void CommunicateRefArea();
    void WrapRefArea();
    void UnwrapRefArea();

    void CommunicateGeometry();


}