#include<bits/stdc++.h>
#include<cuda_runtime.h>
using namespace std;

__global__ void vectoradd(int * a, int* b, int* c, int n){
    int tid=blockIdx.x * blockDim.x + threadIdx.x;

    if(tid<n){
        c[tid]=a[tid]+b[tid];
    }
}

int main(){
    const int n= 1<<16;// 2^16 elements
    size_t bytes=n * sizeof(int);

    int *a, *b, *c;

    //unified memory for these pointers
    cudaMallocManaged(&a, bytes);
    cudaMallocManaged(&b, bytes);
    cudaMallocManaged(&c, bytes);

    for(int i=0; i<n; i++){
        a[i]=rand() % 100;
        b[i]=rand() % 100;
    }

    int threadsperblock=1<<10; //1024 threads
    int numblocks=(n + threadsperblock - 1)/threadsperblock;

    vectoradd<<<numblocks, threadsperblock>>>(a, b, c, n);

    cudaDeviceSynchronize();

    for(int i=0; i<n; i++){
        assert(c[i]== a[i] + b[i]);
    }

    cudaFree(a);
    cudaFree(b);
    cudaFree(c);

    cout<<"Completed successfully"<<endl;

    return 0;
}