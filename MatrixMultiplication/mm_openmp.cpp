#include <bits/stdc++.h>
#include <omp.h>
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

    if(argc < 5){
        cout<<"Usage: ./mm_openmp A.csv B.csv N threads\n";
        return 0;
    }

    string fA = argv[1];
    string fB = argv[2];
    int N = stoi(argv[3]);
    int threads = stoi(argv[4]);

    omp_set_num_threads(threads);

    auto A = readCSV(fA);
    auto B = readCSV(fB);

    vector<vector<double>> C(N, vector<double>(N,0));

    double t1 = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            double sum=0;
            for(int k=0;k<N;k++)
                sum += A[i][k] * B[k][j];

            C[i][j] = sum;
        }
    }

    double t2 = omp_get_wtime();

    cout << (t2-t1) << endl;
}
