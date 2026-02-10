#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){
    const long n=100000000;
    vector<int> arr(n, 1);

    // long long sum=0;

    // double t1=omp_get_wtime();

    // #pragma omp parallel for reduction(+:sum)
    // for(int i=0; i<n; i++)
    // {
    //     sum+=arr[i];
    // }

    // double t2=omp_get_wtime();
    // cout<<"sumof array is "<<sum<<" and "<<"time taken "<<t2-t1<<endl;


    int i, tid, numt, sum=0;
    double t1, t2;

    t1=omp_get_wtime();
    #pragma omp parallel default(shared) private(i, tid)
    {
        int from, to, psum=0;

        tid=omp_get_thread_num();
        numt=omp_get_num_threads();

        from=(n/numt)*tid;
        to=(n/numt)*(tid+1)-1;
        if(tid == numt-1){
            to=n-1;
        }

        // for(i=from; i<=to; i++){
        //     psum+=arr[i];
        // }

        //not an explicit parallel region just to distribute threads for loop
        #pragma omp for
        for(int i=0; i<n; i++){
            psum+=arr[i];
        }

        #pragma critical
        {
            sum+=psum;
        }
    }

    t2=omp_get_wtime();

    cout<<"sum is "<<sum<<" and "<<"time taken is "<<t2-t1<<endl;
}