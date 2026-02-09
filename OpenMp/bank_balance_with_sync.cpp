#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){
    int balance=0;

    #pragma omp parallel
    {
        // CPU performs update as one indivisible operation - hardware instruction
        // #pragma omp atomic
        // balance+=100;

        //mutex lock
        #pragma omp critical
        {
            balance+=100;
        }
    }

    //two threads may read the old value
    cout<<"FInal balance "<<balance<<endl;
}