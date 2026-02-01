#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){

    //since total_threads is same for all we can declare and initialize it outside
    // int total_threads=omp_get_num_threads();

    #pragma omp parallel
    {
        int thread_id=omp_get_thread_num();
        int total_threads=omp_get_num_threads();
        
        cout<<"Hello World "<<thread_id<<" out of "<<total_threads<<endl;
    }
    return 0;
}