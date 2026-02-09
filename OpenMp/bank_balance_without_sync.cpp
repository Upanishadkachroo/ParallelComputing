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