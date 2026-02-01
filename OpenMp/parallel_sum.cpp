#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){
    int arr[4]={1,3,5,6};
    int sum=0;

    // #pragma omp parallel for
    //     //mutiple independent threads
    //     for(int i=0; i<4; i++){
    //         sum+=arr[i];
    //     }

    #pragma omp parallel for reduction(+:sum)
    for(int i=0; i<4; i++){
        sum+=arr[i];
    }

    cout<<"Sum is "<<sum<<endl;
}