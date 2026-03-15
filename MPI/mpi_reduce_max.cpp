#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n=16;
    int arr[n];

    int chunk=n/size;
    int local_arr[chunk];

    if(rank==0){
        int temp[n]={1,2,3,0,4,5,6,7,8,9,11,12,10,13,14,15};
        for(int i=0; i<n; i++){
            arr[i]=temp[i];
        }
    }

    //scatter the array ele
    MPI_Scatter(arr, chunk, MPI_INT, local_arr, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    //Compute local maxima for each local_arr[chunk]
    int local_max=local_arr[0];
    for(int i=1; i<chunk; i++){
        if(local_arr[i]>local_max){
            local_max=local_arr[i];
        }
    }

    cout<<"Process "<<rank<<" local max= "<<local_max<<endl;

    int global_max=0;

    //reduce to root
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank==0){
        cout<<"global maximum is "<<global_max<<endl;
    }
    MPI_Finalize();
}