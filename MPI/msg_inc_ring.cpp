#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next=(rank + 1) % size;
    int prev=(rank - 1 + size) % size;

    int val;

    if(rank==0){
        val=100;
        cout<<"Process 0 starts with value: "<<val<<endl;

        //send to the next
        MPI_Send(&val, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

        //receieve from the last process
        MPI_Recv(&val, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"Process 0 receieved final value "<<val<<endl;
    }
    else{
        //recieve from previous one
        MPI_Recv(&val, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"Process "<<rank<<" recieved val: "<<val<<endl;

        val+=rank;

        //send to next
        MPI_Send(&val, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

    }
    MPI_Finalize();
    return 0;
}