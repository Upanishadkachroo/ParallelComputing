#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_sum=rank;
    int global_sum=0;

    cout<<"Process "<<rank<<" has value "<<local_sum<<endl;

    //Reduction ooperation
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    //only root node receives the data
    if(rank==0){
        cout<<"total sum is "<<global_sum<<endl;
    }

    MPI_Finalize();

}