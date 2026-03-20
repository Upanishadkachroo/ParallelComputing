#include <iostream>
#include <cuda_runtime.h>
using namespace std;

#define N (1024*1024)
#define Threads_per_block 128

// kernel
__global__ void convolve(int n, float *input, float* output){
    int index = blockIdx.x * blockDim.x + threadIdx.x;

    if(index < n){
        float res = 0.0f;

        for(int i = 0; i < 3; i++){
            res += input[index + i];//global memory access
        }

        output[index] = res / 3.0f;
    }
}

int main(){
    int inputsize = (N + 2) * sizeof(float); // padding
    int outputsize = N * sizeof(float);

    // host memory
    float *host_input = new float[N+2];
    float *host_output = new float[N];

    // initialize
    for(int i = 0; i < N+2; i++){
        host_input[i] = i % 10;
    }

    // device memory
    float *device_input, *device_output;
    cudaMalloc((void**)&device_input, inputsize);
    cudaMalloc((void**)&device_output, outputsize);

    // copy to device
    cudaMemcpy(device_input, host_input, inputsize, cudaMemcpyHostToDevice);

    // execution config
    int numblocks = (N + Threads_per_block - 1) / Threads_per_block;

    // launch
    convolve<<<numblocks, Threads_per_block>>>(N, device_input, device_output);

    cudaDeviceSynchronize();

    cudaMemcpy(host_output, device_output, outputsize, cudaMemcpyDeviceToHost);

    // print
    for(int i = 0; i < 10; i++){
        cout << host_output[i] << " ";
    }
    cout << endl;

    // free
    delete[] host_input;
    delete[] host_output;

    cudaFree(device_input);
    cudaFree(device_output);

    return 0;
}