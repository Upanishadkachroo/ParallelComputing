#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size<2){
        cout<<"less processor alloted"<<endl;
        MPI_Finalize();
        return 0;
    }

    int number;
    int tag=100;

    if(rank==0){
        number=69;
        MPI_Send(&number, 1, MPI_INT, 1, tag, MPI_COMM_WORLD);
        cout<<"Process 0 send number "<<number<<endl;
    }
    else if(rank==1){
        MPI_Recv(&number, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout<<"Process 1 recieves number "<<number<<endl;
    }

    MPI_Finalize();
}