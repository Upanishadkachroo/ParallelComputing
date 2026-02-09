#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

//convert csv into M[0], M[1], M[2] ....
vector<vector<double>> readCSV(string file){
    //convert the csv file into 2D vectors
    //file input stream
    ifstream fin(file);
    string line;//reach each row
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

    if(argc < 5){
        cout<<"Usage: ./mm_openmp A.csv B.csv N threads\n";
        return 0;
    }

    string fA = argv[1];
    string fB = argv[2];
    int N = stoi(argv[3]);
    // user controls the number of threads allowed
    int threads = stoi(argv[4]);

    omp_set_num_threads(threads);

    auto A = readCSV(fA);
    auto B = readCSV(fB);

    // result matrix
    vector<vector<double>> C(N, vector<double>(N,0));

    double t1 = omp_get_wtime();

    //create a thread pool
    #pragma omp parallel for collapse(2) //merges loops
    //split i and j among threads
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            //each thread computes independent call
            double sum=0;
            for(int k=0;k<N;k++)
                sum += A[i][k] * B[k][j];
            
            //each thread computes to diffrent c[][] and thus it prevents race condition
            C[i][j] = sum;
        }
    }

    double t2 = omp_get_wtime();

    cout << (t2-t1) << endl;
}
