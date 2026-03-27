#include <bits/stdc++.h>
#include <cuda_runtime.h>
using namespace std;

#define N 1024

// Kernel to transpose matrix
__global__ void transpose(float *input, float *output)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x; // column
    int y = blockIdx.y * blockDim.y + threadIdx.y; // row

    if (x < N && y < N)
    {
        output[x * N + y] = input[y * N + x]; // FIXED
    }
}

// Matrix multiplication using coalesced access
__global__ void matMulCoalesced(float *A, float *B_T, float *C)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < N && col < N)
    {
        float sum = 0.0f;

        for (int k = 0; k < N; k++)
        {
            // Both accesses are row-wise (coalesced)
            sum += A[row * N + k] * B_T[col * N + k];
        }

        C[row * N + col] = sum; // FIXED ;
    }
}

int main()
{
    size_t size = N * N * sizeof(float); // FIXED

    float *A, *B, *B_T, *C;
    float *d_A, *d_B, *d_B_T, *d_C;

    // Allocate host memory
    A = (float *)malloc(size);
    B = (float *)malloc(size);
    B_T = (float *)malloc(size);
    C = (float *)malloc(size);

    // Initialize matrices
    for (int i = 0; i < N * N; i++)
    {
        A[i] = 1.0f;
        B[i] = 1.0f;
    }

    // Allocate device memory
    cudaMalloc(&d_A, size);
    cudaMalloc(&d_B, size);
    cudaMalloc(&d_B_T, size);
    cudaMalloc(&d_C, size);

    // Copy to device
    cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);

    dim3 threads(16, 16);
    dim3 blocks(N / 16, N / 16);

    // Step 1: Transpose B
    transpose<<<blocks, threads>>>(d_B, d_B_T);

    // Step 2: Multiply using coalesced access
    matMulCoalesced<<<blocks, threads>>>(d_A, d_B_T, d_C);

    cudaDeviceSynchronize();

    // Copy result back
    cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

    cout << "Done!" << endl;

    // Free memory
    free(A);
    free(B);
    free(B_T);
    free(C);
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_B_T);
    cudaFree(d_C);

    return 0;
}