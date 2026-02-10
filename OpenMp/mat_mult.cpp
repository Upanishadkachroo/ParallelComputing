#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){
    int n = 4;

    int A[4][4] = {
        {1,2,3,4},
        {2,1,0,1},
        {3,1,2,2},
        {1,0,1,1}
    };

    int B[4][4] = {
        {1,0,2,1},
        {2,1,1,0},
        {1,2,0,1},
        {0,1,1,2}
    };

    int C[4][4] = {0};

    double t1=omp_get_wtime();

    //openmp merges both loops into one big iteration space
    #pragma omp parallel for collapse(2)
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            int sum=0;
            for(int k=0; k<n; k++)
            {
                sum+=A[i][k]*B[k][j];
            }
            C[i][j]=sum;
        }
    }

    double t2=omp_get_wtime();

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            cout<<C[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"time taken is "<<t2-t1<<endl;
}