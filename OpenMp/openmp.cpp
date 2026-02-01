#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){
    omp_set_num_threads(20);
    // cout is a shared device (terminal/buffer).
    //or we can do at the time of compilation is: export OMP_NUM_THREADS=4
    //critical, 1 thread at a time is executed and prevents race condition
    //single, first thread that arrives only gets executed
    //master, takes zero thread

    #pragma omp parallel
    {
        cout<<"Hello World "<<endl;
    }
    return 0;
}