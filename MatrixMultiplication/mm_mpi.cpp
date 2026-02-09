#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

//convert csv into M[0], M[1], M[2] ....
vector<vector<double>> readCSV(string file){
    //file input stream
    ifstream fin(file);
    string line;
    vector<vector<double>> M;

    // read the line one by one
    while(getline(fin,line)){
        //convert line to stream, comma separated value
        stringstream ss(line);
        string cell;
        vector<double> row;

        //split by comma
        while(getline(ss, cell, ','))
            //convert string to double
            row.push_back(stod(cell));

        //add row to mat
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
    int N = stoi(argv[3]);//convrt size of mat in command from string to int

    vector<vector<double>> A,B;

    //only rank 0 reads, others now have empty matrices
    if(rank==0){
        A = readCSV(fA);
        B = readCSV(fB);
    }

    //falttening converting it to 2D mat.
    vector<double> Aflat, Bflat;

    if(rank==0){
        for(auto &r:A) for(double x:r) Aflat.push_back(x);
        for(auto &r:B) for(double x:r) Bflat.push_back(x);
    }

    if(rank!=0){
        // allocate buffer for incoming brodcast
        Aflat.resize(N*N);
        Bflat.resize(N*N);
    }

    //rank 0 sends to all prcoess both A and B
    MPI_Bcast(Aflat.data(), N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(Bflat.data(), N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    vector<double> Cflat(N*N,0);

    //here the real computation starts
    double t1 = MPI_Wtime();

    //data parttioning - row wise cyclic distribution
    for(int i=rank;i<N;i+=size){
        for(int j=0;j<N;j++){

            //each process computes C[i][j] for its rows only
            double sum=0;
            for(int k=0;k<N;k++)
                sum += Aflat[i*N+k] * Bflat[k*N+j];

            Cflat[i*N+j] = sum;
        }
    }

    vector<double> Cglobal(N*N);

    //global combine - collect partial results from each process
    MPI_Allreduce(Cflat.data(), Cglobal.data(), N*N, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    double t2 = MPI_Wtime();

    if(rank==0)
        cout << (t2-t1) << endl;

    MPI_Finalize();
}
