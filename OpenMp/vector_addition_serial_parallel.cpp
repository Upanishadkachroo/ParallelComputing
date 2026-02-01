#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){
    int n=10000000;
    vector<int> a(n), b(n), c(n);

    for(int i=0; i<n; i++){
        a[i]=i;
        b[i]=i*2;
    }

    //serial addition
    double s1=omp_get_wtime();

    for(int i=0; i<n; i++){
        c[i]=a[i]+b[i];
    }

    double e1=omp_get_wtime();

    //parallel execution
    double s2=omp_get_wtime();

    #pragma omp parallel for
    for(int i=0; i<n; i++){
        c[i]=b[i]+a[i];
    }

    double e2=omp_get_wtime();

    cout<<"Serial time is: "<<e1-s1<<endl;
    cout<<"Parallel time is: "<<e2-s2<<endl;//Time ≈ N / threads
}