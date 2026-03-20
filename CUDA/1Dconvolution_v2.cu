#include <iostream>
#include <cuda_runtime.h>
using namespace std;

#define N (1024*1024)
#define Threads_per_block 128

// kernel
__global__ void convolve(int n, float *input, float *output){
    __shared__ float s_data[Threads_per_block + 2];

    int tx = threadIdx.x;
    int index = blockIdx.x * blockDim.x + tx;

    // Load main data
    if(index < n + 2){
        s_data[tx] = input[index];
    }

    // Load halo (right side only, since kernel size = 3)
    if(tx < 2){
        int halo_index = index + blockDim.x;
        if(halo_index < n + 2){
            s_data[tx + blockDim.x] = input[halo_index];
        }
    }

    __syncthreads();

    // Compute convolution
    if(index < n){
        float res = 0.0f;

        // FIXED: correct indexing
        res = s_data[tx] + s_data[tx+1] + s_data[tx+2];

        output[index] = res / 3.0f;
    }
}

int main(){
    int inputsize = (N + 2) * sizeof(float);
    int outputsize = N * sizeof(float);

    float *host_input = new float[N+2];
    float *host_output = new float[N];

    for(int i = 0; i < N+2; i++){
        host_input[i] = i % 10;
    }

    float *device_input, *device_output;
    cudaMalloc((void**)&device_input, inputsize);
    cudaMalloc((void**)&device_output, outputsize);

    cudaMemcpy(device_input, host_input, inputsize, cudaMemcpyHostToDevice);

    int numblocks = (N + Threads_per_block - 1) / Threads_per_block;

    convolve<<<numblocks, Threads_per_block>>>(N, device_input, device_output);

    cudaDeviceSynchronize();

    cudaMemcpy(host_output, device_output, outputsize, cudaMemcpyDeviceToHost);

    for(int i = 0; i < 10; i++){
        cout << host_output[i] << " ";
    }
    cout << endl;

    delete[] host_input;
    delete[] host_output;

    cudaFree(device_input);
    cudaFree(device_output);

    return 0;
}