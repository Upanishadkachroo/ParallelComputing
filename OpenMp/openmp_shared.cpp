#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){
    int thread_id, total_threads;

    #pragma omp parallel
    {
        // int i;
        thread_id=omp_get_thread_num();
        total_threads=omp_get_num_threads();

        //to make it fail almost every time
        // for(i=0; i<100000; i++);
        
        cout<<"Hello World "<<thread_id<<" out of "<<total_threads<<endl;
    }
    return 0;
}