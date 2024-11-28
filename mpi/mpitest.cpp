#include "mpi.h"
#include <iostream>
#include <string>

int main(int argc,char** argv)
{
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //std::string message;
    char message[100];
    if(rank==0)
    {
        for(int source=1 ;source<size;source++)
        {
            MPI_Recv(message, 100, MPI_BYTE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        strcpy(message, "Hello, MPI");
        MPI_Send(message, 11, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
        std::cout << "rank: " << rank <<std::endl<< " message: " << message << std::endl;
    }

    
    MPI_Finalize();
    return 0;
}