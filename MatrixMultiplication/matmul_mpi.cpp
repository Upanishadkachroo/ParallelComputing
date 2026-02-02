#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char** argv){

    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(argc < 3){
        if(rank==0)
            cout<<"Usage: mpirun -np P ./matmul_mpi N iterations\n";
        MPI_Finalize();
        return 0;
    }

    int N = stoi(argv[1]);
    int ITER = stoi(argv[2]);

    vector<double> A(N*N);
    vector<double> B(N*N);
    vector<double> C(N*N,0);

    if(rank==0){
        for(int i=0;i<N;i++)
            for(int j=0;j<N;j++){
                A[i*N+j]=(i+j)%10;
                B[i*N+j]=(i-j)%10;
            }
    }

    MPI_Bcast(A.data(),N*N,MPI_DOUBLE,0,MPI_COMM_WORLD);
    MPI_Bcast(B.data(),N*N,MPI_DOUBLE,0,MPI_COMM_WORLD);

    int rows = N/size;
    int start = rank*rows;
    int end = (rank==size-1)?N:start+rows;

    double t1 = MPI_Wtime();

    for(int it=0; it<ITER; it++){

        for(int i=start;i<end;i++){
            for(int j=0;j<N;j++){

                double sum=0;
                for(int k=0;k<N;k++)
                    sum += A[i*N+k]*B[k*N+j];

                C[i*N+j]=sum;
            }
        }
    }

    double t2 = MPI_Wtime();

    if(rank==0)
        cout<<"TIME_SECONDS "<<(t2-t1)<<endl;

    MPI_Finalize();
}
