#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int token=1;

    if(rank!=0){
        MPI_Recv(&token, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    cout<<"Process"<<rank<<endl;

    if(rank!=size -1){
        MPI_Send(&token, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
    }
}