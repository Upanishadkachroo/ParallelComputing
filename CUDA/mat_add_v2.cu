#include<bits/stdc++.h>
using namespace std;

const int N=10;//[10 x 10]

//kernel function and uses 2D grid and block
__global__ void mat_add(int *A, int *B, int *C, int size){
    //index= index of the block in the grid * number of threads in the block + index of the current thread in the block 
    int i=blockIdx.x * blockDim.x + threadIdx.x;//column index
    int j=blockIdx.y * blockDim.y + threadIdx.y;//row index 

    if(i<size && j<size){
        int index=i* size + j;//use row major order
        C[index]=A[index]+B[index];
    }
}

int main(){
    const int matrix_size=N*N*sizeof(int);

    int *h_a=new int[N*N];
    int *h_b=new int[N*N];
    int *h_c=new int[N*N];

    for(int i=0; i<N*N; i++){
        h_a[i]=1;
        h_b[i]=2;
        h_c[i]=0;
    }

    int *d_a, *d_b, *d_c;
    cudaMalloc((void**)&d_a, matrix_size);
    cudaMalloc((void**)&d_b, matrix_size);
    cudaMalloc((void**)&d_c, matrix_size);

    cudaMemcpy(d_a, h_a, matrix_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, matrix_size, cudaMemcpyHostToDevice);

    dim3 threadsperblock(32,16);
    dim3 numblocks((int)ceil((float)N/threadsperblock.x), (int)ceil((float)N/threadsperblock.y));// 10/32, 10/16 = (1,1)

    mat_add<<<numblocks, threadsperblock>>>(d_a, d_b, d_c, N);

    cudaDeviceSynchronize();
    cudaMemcpy(h_c, d_c, matrix_size, cudaMemcpyDeviceToHost);

    for(int i=0; i<N*N; i++){
        cout<<h_c[i]<<", ";
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}