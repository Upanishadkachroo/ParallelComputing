#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char * argv[]){
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int msg;

    if(rank==0){
        msg=10;

        MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"Process 0 recieved"<<msg<<endl;
    }
    else if(rank==1){
        
        msg=20;

        MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout<<"Process 1 recieved"<<msg<<endl;
    }
    MPI_Finalize();
}