#include <iostream>
#include <cuda_runtime.h>
using namespace std;

#define N 1024
#define M 1024

// Kernel
__global__ void matrixadd(int *A, int *B, int *C, int n, int m){
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < n && col < m){
        int idx = row * m + col;
        C[idx] = A[idx] + B[idx];
    }
}

int main(){
    int size = N * M * sizeof(int);

    // Host memory
    int *mat_a = new int[N*M];
    int *mat_b = new int[N*M];
    int *mat_c = new int[N*M];

    // Initialize
    for(int i = 0; i < N*M; i++){
        mat_a[i] = i;
        mat_b[i] = 2*i;
    }

    // Device memory
    int *device_mat_a, *device_mat_b, *device_mat_c;
    cudaMalloc((void**)&device_mat_a, size);
    cudaMalloc((void**)&device_mat_b, size);
    cudaMalloc((void**)&device_mat_c, size);

    // Copy Host → Device
    cudaMemcpy(device_mat_a, mat_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(device_mat_b, mat_b, size, cudaMemcpyHostToDevice);

    // Execution config
    dim3 threadsPerBlock(16, 16);
    dim3 numBlocks(
        (M + threadsPerBlock.x - 1) / threadsPerBlock.x,
        (N + threadsPerBlock.y - 1) / threadsPerBlock.y
    );

    // Launch kernel
    matrixadd<<<numBlocks, threadsPerBlock>>>(device_mat_a, device_mat_b, device_mat_c, N, M);

    // Important
    cudaDeviceSynchronize();

    // Copy Device → Host (FIXED)
    cudaMemcpy(mat_c, device_mat_c, size, cudaMemcpyDeviceToHost);

    // Verify
    bool check = true;
    for(int i = 0; i < 10; i++){
        if(mat_c[i] != mat_a[i] + mat_b[i]){
            check = false;
            break;
        }
    }

    if(check){
        cout << "success" << endl;
    } else {
        cout << "error" << endl;
    }

    // Free memory
    delete[] mat_a;
    delete[] mat_b;
    delete[] mat_c;

    cudaFree(device_mat_a);
    cudaFree(device_mat_b);
    cudaFree(device_mat_c); 

    return 0;
}