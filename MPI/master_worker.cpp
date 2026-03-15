#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int tasks=20;

    if(rank==0){
        //master process
        int task=1;
        int active_task=size-1;

        while(task<=tasks){
            for(int worker=1; worker<size && task<=tasks; worker++){
                MPI_Send(&task, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
                task++;
            }

            for(int worker=1; worker<size && task<=tasks; worker++){
                int res;
                MPI_Recv(&res, 1, MPI_INT, worker, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                cout<<"Master received result "<<res<<endl;
            }
        }
        //stop workers
        int stop=-1;
        for(int worker=1; worker<size; worker++){
            MPI_Send(&stop, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
        }
    }
    else{
        //worker process
        while(true){
            int task;
            MPI_Recv(&task, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(task==-1){
                break;
            }

            int res=task*task;

            cout<<"worker "<<rank<<" compute square of "<<task<<" = "<<res<<endl;

            MPI_Send(&res, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
}