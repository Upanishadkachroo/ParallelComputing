#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;

    //get total number of process
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //get rank of process
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("Hello world from process %d out of %d\n", rank, size);

    MPI_Finalize();
}