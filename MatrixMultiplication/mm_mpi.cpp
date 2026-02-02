#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

vector<vector<double>> readCSV(string file){
    ifstream fin(file);
    string line;
    vector<vector<double>> M;

    while(getline(fin,line)){
        stringstream ss(line);
        string cell;
        vector<double> row;

        while(getline(ss, cell, ','))
            row.push_back(stod(cell));

        M.push_back(row);
    }
    return M;
}

int main(int argc, char *argv[]){

    MPI_Init(&argc,&argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(argc < 4){
        if(rank==0)
            cout<<"Usage: mpirun -np P ./mm_mpi A.csv B.csv N\n";
        MPI_Finalize();
        return 0;
    }

    string fA = argv[1];
    string fB = argv[2];
    int N = stoi(argv[3]);

    vector<vector<double>> A,B;

    if(rank==0){
        A = readCSV(fA);
        B = readCSV(fB);
    }

    vector<double> Aflat, Bflat;

    if(rank==0){
        for(auto &r:A) for(double x:r) Aflat.push_back(x);
        for(auto &r:B) for(double x:r) Bflat.push_back(x);
    }

    if(rank!=0){
        Aflat.resize(N*N);
        Bflat.resize(N*N);
    }

    MPI_Bcast(Aflat.data(), N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(Bflat.data(), N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<double> Cflat(N*N,0);

    double t1 = MPI_Wtime();

    for(int i=rank;i<N;i+=size){
        for(int j=0;j<N;j++){
            double sum=0;
            for(int k=0;k<N;k++)
                sum += Aflat[i*N+k] * Bflat[k*N+j];

            Cflat[i*N+j] = sum;
        }
    }

    vector<double> Cglobal(N*N);

    MPI_Allreduce(Cflat.data(), Cglobal.data(),
                  N*N, MPI_DOUBLE, MPI_SUM,
                  MPI_COMM_WORLD);

    double t2 = MPI_Wtime();

    if(rank==0)
        cout << (t2-t1) << endl;

    MPI_Finalize();
}
