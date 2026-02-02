#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main(int argc, char** argv){

    if(argc < 4){
        cout << "Usage: ./matmul_openmp N threads iterations\n";
        return 0;
    }

    int N = stoi(argv[1]);
    int threads = stoi(argv[2]);
    int ITER = stoi(argv[3]);

    omp_set_num_threads(threads);

    vector<vector<double>> A(N, vector<double>(N));
    vector<vector<double>> B(N, vector<double>(N));
    vector<vector<double>> C(N, vector<double>(N,0));

    // Initialize
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++){
            A[i][j] = (i+j)%10;
            B[i][j] = (i-j)%10;
        }

    auto t1 = chrono::high_resolution_clock::now();

    for(int it=0; it<ITER; it++){

        #pragma omp parallel for collapse(2)
        for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){

                double sum = 0;
                for(int k=0;k<N;k++)
                    sum += A[i][k] * B[k][j];

                C[i][j] = sum;
            }
        }
    }

    auto t2 = chrono::high_resolution_clock::now();

    double time =
        chrono::duration<double>(t2-t1).count();

    cout << "TIME_SECONDS " << time << endl;
}
