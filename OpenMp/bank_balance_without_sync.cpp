#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(){
    int balance=0;

    #pragma omp parallel
    {
        balance=balance+100; //race condition 
    }

    //two threads may read the old value
    cout<<"FInal balance "<<balance<<endl;
}

// if the balance was declared inside the parallel region, then the each thread will have its own copy of 
// balance and we get output as 100 for each only