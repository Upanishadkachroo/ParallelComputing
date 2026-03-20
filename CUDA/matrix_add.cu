#include<bits/stdc++.h>
#include<cuda_runtime.h>
using namespace std;

#define N 1024
#define M 1024


//kernel runs on gpu
__global__ void matrixadd(int *A, int *B, int *C, int n, int m){
    int row=blockIdx.y * blockDim.y + threadIdx.y;
    int col=blockIdx.x * blockDim.x + threadIdx.x;

    if(row < n && col < m){
        int idx=row*m + col;
        C[idx]=A[idx] + B[idx];
    } 
}


int main(){
    int size=N*M*sizeof(int);

    //host memeory allocation in CPU
    int *mat_a=new int[N*M];
    int *mat_b=new int[N*M];
    int *mat_c=new int[N*M];

    //initialize the matrix
    for(int i=0; i<N*M; i++){
        mat_a[i]=i;
        mat_b[i]=2*i;
    }

    //device memory allocation in gpu
    int *device_mat_a, *device_mat_b, *device_mat_c;
    cudaMalloc((void**)&device_mat_a, size);
    cudaMalloc((void**)&device_mat_b, size);
    cudaMalloc((void**)&device_mat_c, size);

    //copy data from host to device
    cudaMemcpy(device_mat_a, mat_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(device_mat_b, mat_b, size, cudaMemcpyHostToDevice);

    //define execution config.
    dim3 threadsperblock(16, 16);
    dim3 numblocks(
        (M + threadsperblock.x - 1)/threadsperblock.x,
        (N + threadsperblock.y - 1)/threadsperblock.y
    );

    //launch kernel
    matrixadd<<<numblocks, threadsperblock>>(device_mat_a, device_mat_b, device_mat_c, N, M);

    //copy result from gpu to cpu
    cudaMemcpy(device_mat_c, mat_c, size, cudaMemcpyDeviceToHost);

    //verify the result
    bool check=true;
    for(int i=0; i<10; i++){
        if(mat_a[i] != mat_b[i] + mat_a[i]){
            check=false;
            break;
        }
    }

    if(check){
        cout<<"sucess"<<endl;
    }
    else{
        cout<<"error"<<endl;
    }

    //free memory
    delete[] mat_a;
    delete[] mat_b;
    delete[] mat_c;

    cudaFree(device_mat_a);
    cudaFree(device_mat_b);
    cudaFree(device_mat_b);

    return 0;
    
}