#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){
    const int n=100000000;

    vector<int> a(n, 2);
    vector<int> b(n, 4);

    int dot=0;

    double t1=omp_get_wtime();

    #pragma omp parallel for schedule(static, 1000) reduction(+:dot)
    for(int i=0; i<n; i++)
    {
        dot+=a[i]*b[i];
    }

    double t2=omp_get_wtime();

    cout<<"the dot product is "<<dot<<" and "<<"time taken is "<<t2-t1<<endl;
}