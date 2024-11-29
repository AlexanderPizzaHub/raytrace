#include "mpitracer.hpp"
#include <iostream>

namespace MpiTracer
{
    MpiTracer::MpiTracer(Mesh& mesh)
    {
        meshptr_ = &mesh;
    }
    MpiTracer::~MpiTracer()
    {}

    void MpiTracer::WrapRefArea()
    {
        int nrefarea = meshptr_->getnumRefAreas();

        delete[] sendbuffer_;
        sendbuffer_ = new double[nrefarea];
        for(int i = 0; i< nrefarea;i++)
        {   
            sendbuffer_[i] = (meshptr_->getRefArea(i)->getweightstore());
        }
    }

    void MpiTracer::UnwrapRefAreaAggregate()
    {
        int nrefarea = meshptr_->getnumRefAreas();
        //assert(nrefarea == meshptr_->getnumRefAreas());
        if(nrefarea != meshptr_->getnumRefAreas())
        {
            std::cout << "Error: number of refareas mismatch" << std::endl;
        }
        for(int i=0;i<nrefarea;i++)
        {
            meshptr_->getRefArea(i)->WeightAdd(recvbuffer_[i]); 
        }
        delete[] recvbuffer_;
    }

    void MpiTracer::CommunicateRefArea()
    {
        int rank;
        int size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        if(rank == 0)
        {
            for(int source = 1; source < size; source++)
            {
                MPI_Recv(recvbuffer_, meshptr_->getnumRefAreas(), MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                UnwrapRefAreaAggregate();
            }
        }
        else
        {
            WrapRefArea();
            MPI_Send(sendbuffer_, meshptr_->getnumRefAreas(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        }
    }

    

    void CommunicateGeometry()
    {
        //TODO
    }
} // namespace MpiTracer