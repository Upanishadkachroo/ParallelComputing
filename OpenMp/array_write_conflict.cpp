#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

int main(int argc, char* argv[]){
    int arr[10]={0};

    #pragma omp critical
    {
        int id=omp_get_thread_num();
        //arr[0]=id; //all threads modify the same location, arr[10] is shared by all
        arr[id]=id;//different index per thread
    }

    // cout<<arr[0]<<endl;
    for(int i=0; i<4; i++){
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}