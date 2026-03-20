#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[]){
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int n=4;

    int a[n][n], b[n][n], c[n][n];

    int local_a[n];
    int local_b[n];
    int local_c[n];

    if(rank==0){
        cout<<"Matrix A"<<endl;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                a[i][j]=i+j;
                cout<<a[i][j]<<" ";
            }
            cout<<endl;
        }

        cout<<"Matrix B"<<endl;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                b[i][j]=i*j;
                cout<<b[i][j]<<" ";
            }
            cout<<endl;
        }
    }

    MPI_Scatter(a, n, MPI_INT, local_a, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, n, MPI_INT, local_b, n, MPI_INT, 0, MPI_COMM_WORLD);

    for(int i=0; i<n; i++){
        local_c[i]=local_b[i]+local_a[i];
    }

    MPI_Gather(local_c, n, MPI_INT, local_b, n, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank==0){
        cout<<"Result matrix"<<endl;
        for(int i=0; i<n; i++){
            for(int j=0; j<n; j++){
                cout<<c[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    MPI_Finalize();
}