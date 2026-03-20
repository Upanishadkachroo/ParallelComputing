#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc,char* argv[])
{
    MPI_Init(&argc,&argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int send_data = rank*10;
    int recv_data;

    MPI_Request req[2];

    int partner = (rank==0)?1:0;

    MPI_Irecv(&recv_data,1,MPI_INT,partner,0,MPI_COMM_WORLD,&req[0]);

    MPI_Isend(&send_data,1,MPI_INT,partner,0,MPI_COMM_WORLD,&req[1]);

    MPI_Waitall(2,req,MPI_STATUSES_IGNORE);

    cout<<"Process "<<rank<<" received "<<recv_data<<endl;

    MPI_Finalize();
}