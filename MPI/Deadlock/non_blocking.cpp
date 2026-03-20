#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data, recv_data;

    MPI_Request request[2];

    if(rank == 0)
    {
        send_data = 100;

        // Start non-blocking send
        MPI_Isend(&send_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request[0]);

        // Start non-blocking receive
        MPI_Irecv(&recv_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request[1]);

        // Do some computation while communication happens
        cout<<"Process 0 doing computation while message transfers\n";

        for(int i=0;i<100000000;i++);   // dummy work

        // Wait for communication to finish
        MPI_Waitall(2, request, MPI_STATUSES_IGNORE);

        cout<<"Process 0 received "<<recv_data<<endl;
    }

    else if(rank == 1)
    {
        send_data = 200;

        MPI_Isend(&send_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[0]);
        MPI_Irecv(&recv_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request[1]);

        cout<<"Process 1 doing computation while message transfers\n";

        for(int i=0;i<100000000;i++);  // dummy work

        MPI_Waitall(2, request, MPI_STATUSES_IGNORE);

        cout<<"Process 1 received "<<recv_data<<endl;
    }

    MPI_Finalize();
    return 0;
}