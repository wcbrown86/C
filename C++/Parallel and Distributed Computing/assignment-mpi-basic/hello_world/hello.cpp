#include <mpi.h>
#include <unistd.h>
#include <iostream>
#include <limits.h>
#include <chrono>

int main(int argc, char*argv[]) {

    //starts the MPI functions
    MPI_Init(&argc, &argv);

    //tracks the number of the processors total
    int size; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //tracks the currnt process rank.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //gets the current host name
    char host[HOST_NAME_MAX];
    gethostname(host, HOST_NAME_MAX);

    //prints the information the stdout
    std::cout << "I am node " << rank << " out of " << size << ". I am running on " << host << std::endl;

    //ends the MPI program. 
    MPI_Finalize();

    return 0;


}
