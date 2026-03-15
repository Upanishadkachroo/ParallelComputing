#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n=8;
    int arr[n];

    int chunk=n/size;
    int local_arr[chunk];

    if(rank==0){
        for(int i=0; i<n; i++){
            arr[i]=i+1;
        }

        cout<<"original array is";
        for(int i=0; i<n; i++){
            cout<<arr[i]<<" ";
        }
        cout<<endl;
    }

    //scatter the data 
    MPI_Scatter(arr, chunk, MPI_INT, local_arr, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    //double the elements
    for(int i=0; i<chunk; i++){
        local_arr[i]*2;
    }

    //gather the results
    MPI_Gather(local_arr, chunk, MPI_INT, arr, chunk, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank==0){
        cout<<"changed array";
        for(int i=0; i<n; i++){
            cout<<arr[i]<<" ";
        }
        cout<<endl;
    }
    MPI_Finalize();
}