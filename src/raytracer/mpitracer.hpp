#pragma once

#include "mpi.h"
#include "geometry/mesh.hpp"
#include "geometry/objects.hpp"

namespace MpiTracer
{
    class MpiTracer
    {
        public:
            MpiTracer(Mesh& mesh);
            ~MpiTracer();

            void WrapRefArea(); // subprocess wrap the data on ref area
            void UnwrapRefAreaAggregate(); // master process unwrap the data on ref area
            void CommunicateRefArea(); // send and receive data

            void CommunicateGeometry(); // Master process broacast the geometry to all subprocesses

        private:
            Mesh* meshptr_;
            double** sendbuffer_;
            double** recvbuffer_;
    };
}
