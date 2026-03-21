#include<bits/stdc++.h>
using namespace std;

__global__ void vector_add(const float* A, const float * B, float* C, int N){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;

    //n of grids -> n. of block -> n. of threads (256,512)
    //to check if no. of threads are less than no. of elements in the vector
    if(idx<N){
        C[idx]=A[idx]+B[idx];
    }
}

int main(){
    int N=10;
    float A[N], B[N], C[N];//inside your host

    for(int i=0; i<N; i++ ){
        A[i]=(float)i + 1.0f;
        B[i]=2.0f;
    }

    float *d_A, *d_B, *d_C;//inside your device

    //alocation of memeory in device
    cudaMalloc(&d_A, N*sizeof(float));
    cudaMalloc(&d_B, N*sizeof(float));
    cudaMalloc(&d_C, N*sizeof(float));

    //copy from host to device
    cudaMemcpy(d_A, A, N*sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, N*sizeof(float), cudaMemcpyHostToDevice);


    //we have used 1D size blocks, threads
    int threadsperblock=256;//blocksize
    int numblocks=(int)ceil((float)N/threadsperblock);//gridsize

    vector_add<<<numblocks, threadsperblock>>>(d_A, d_B, d_C, N);

    //copy from device to host
    cudaMemcpy(C, d_C, N*sizeof(float), cudaMemcpyDeviceToHost);

    printf("vector A: ");
    for(int i=0; i<N; i++){
        cout<<A[i]<<(", ");
    }

    printf("\nvector B: ");
    for(int i=0; i<N; i++){
        cout<<B[i]<<(", ");
    }

    printf("\nvector C: ");
    for(int i=0; i<N; i++){
        cout<<C[i]<<(", ");
    }
  
 
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    return 0;

}