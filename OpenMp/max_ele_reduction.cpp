#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char *argv[]){
    int arr[5]={0, 4, 2, 8, 9};
    int max_val=INT_MIN;

    // #pragma omp parallel for
    // for(int i=0; i<5; i++){
    //     if(arr[i]>max_val){
    //         max_val=arr[i];
    //     }
    // }

    #pragma omp parallel for reduction(max:max_val)
    for(int i = 0; i < 5; i++) {
        if(arr[i] > max_val)
            max_val = arr[i];
    }

    cout<<"Maximum: "<<max_val<<endl;
}