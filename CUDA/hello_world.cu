#include <iostream>
#include <cuda_runtime.h>

using namespace std;

// Kernel function (runs on GPU)
__global__ void helloFromGPU() {
    printf("Hello World from GPU thread %d!\n", threadIdx.x);
}

int main() {
    cout << "Hello World from CPU!" << endl;

    // Launch kernel with 1 block and 5 threads
    helloFromGPU<<<1, 5>>>();

    // Wait for GPU to finish
    cudaDeviceSynchronize();

    return 0;
}