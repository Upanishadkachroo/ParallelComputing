#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_sum=rank;
    int global_sum=0;

    cout<<"Process "<<rank<<" has value "<<local_sum<<endl;

    //all reduce operation
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout<<"Process "<<rank<<" recieved total sum "<<global_sum<<endl;

    MPI_Finalize();
}