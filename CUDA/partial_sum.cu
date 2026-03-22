#include<bits/stdc++.h>
#include<cmath>
#include<cuda_runtime.h>
using namespace std;

__global__ void partialsumkernel(int *input, int *output, int n){
    extern __shared__ int sharedMemory[];
    
    int tid = threadIdx.x;
    int index = blockIdx.x * blockDim.x * 2 + tid;

    // Safe loading (avoid out-of-bounds)
    if(index < n){
        int val1 = input[index];
        int val2 = (index + blockDim.x < n) ? input[index + blockDim.x] : 0;
        sharedMemory[tid] = val1 + val2;
    } else {
        sharedMemory[tid] = 0;
    }

    __syncthreads();

    // Inclusive scan within block
    for(int stride = 1; stride < blockDim.x; stride *= 2){
        int temp = 0;
        if(tid >= stride){
            temp = sharedMemory[tid - stride];
        }
        __syncthreads();
        sharedMemory[tid] += temp;
        __syncthreads();
    }

    // Write back (safe)
    if(index < n){
        output[index] = sharedMemory[tid];
    }
}

int main(){
    const int n = 16;
    const int blocksize = 8;

    // Fixed size input
    int input[n] = {1,2,3,4,5,6,7,8,9,11,2,23,3,14,0,0};
    int output[n];

    int *d_input, *d_output;
    cudaMalloc((void**)&d_input, n*sizeof(int));
    cudaMalloc((void**)&d_output, n*sizeof(int));

    cudaMemcpy(d_input, input, n*sizeof(int), cudaMemcpyHostToDevice);

    // Correct blocks (2 elements per thread)
    int numBlocks = (int)ceil((float)n / (2 * blocksize));

    partialsumkernel<<<numBlocks, blocksize, blocksize * sizeof(int)>>>(d_input, d_output, n);

    cudaDeviceSynchronize();

    cudaMemcpy(output, d_output, n*sizeof(int), cudaMemcpyDeviceToHost);

    // Print input
    for(int i=0; i<n; i++){
        cout << input[i] << ", ";
    }

    printf("\n");

    // Print output
    for(int i=0; i<n; i++){
        cout << output[i] << ", ";
    }

    cudaFree(d_input);
    cudaFree(d_output);

    return 0;
}