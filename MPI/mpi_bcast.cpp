#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;

    if(rank==0){
        n=50;
        cout<<"Process 0 initialized a number "<<n<<endl;
    }

    //MPI Bcast
    //MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    cout<<"Process "<<rank<<" received number"<<n<<endl;

    MPI_Finalize();
}