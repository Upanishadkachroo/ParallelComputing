#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size!=2){
        if(rank==0){
            cout<<"It works with 2 processors"<<endl;
        }
        MPI_Finalize();
        return 0;
    }


    int msg=100;
    int itr=1000;

    double start, end;

    if(rank==0) start=MPI_Wtime();

    for(int i=0; i<itr; i++){
        if(rank==0){
            MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else if(rank==1){
            MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if(rank==0){
        end=MPI_Wtime();

        double total_time=end-start;
        double latency=total_time/(2*itr);

        cout<<"Total time: "<<total_time<<" seconds"<<endl;
        cout<<"Estimated latency "<<latency<<" seconds"<<endl;
    }
    MPI_Finalize();
    return 0;
}