#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N=1000;
    int arr[N];

    int chunk=N/size;
    int local_arr[chunk];

    //root initailizes
    if(rank==0){
        for(int i=0; i<N; i++){
            arr[i]=i+1;
        }
    }

    //Scatter array parts
    //each process gets local_arr[chunk(==25)]
    MPI_Scatter(arr, chunk, MPI_INT, local_arr, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    //local computation
    int local_sum=0;
    for(int i=0; i<chunk; i++){
        local_sum+=local_arr[i];
    }

    cout<<"Process "<<rank<<" has local sum "<<local_sum<<endl;

    int global_sum;

    //Reduce to root
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank==0){
        cout<<"total sum "<<global_sum<<endl;
    }
    MPI_Finalize();
}