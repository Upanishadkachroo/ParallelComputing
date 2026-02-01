#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char* argv[]){
    int counter=0;//shared variable by all thread

    // #pragma omp parallel 
    // {
    //     // counter++;
    //     #pragma omp critical
    //     {
    //         counter++;
    //     }
    // }

    #pragma omp parallel reduction(+:counter)
    {
        counter++;

        // How it works:
        // private_counter[thread]
        // counter = sum of all private copies

    }

    //expected should be equal to number of threads
    cout<<"Final counter value "<<counter<<endl;
}