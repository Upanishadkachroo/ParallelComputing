#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){
    int thread_id=0, total_threads;

    #pragma omp parallel private(thread_id) shared(total_threads)
    {
        thread_id=omp_get_thread_num();
        total_threads=omp_get_num_threads();

        
        cout<<"Hello World "<<thread_id<<" out of "<<total_threads<<endl;
    }

    //the final value of the thread_id does not get overwritten as it was earlier in openmp_shared
    cout<<"Final value of thread_id "<<thread_id<<endl;
    return 0;
}