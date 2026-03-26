#include <bits/stdc++.h>
#include <cuda_runtime.h>
using namespace std;

const int n = 1 << 10;
const int SHMEM_SIZE = 1 << 10;

__global__ void matmul(const int *a, const int *b, int *c)
{
    // compute each thread's global row and column
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // statiscally allocated shared memory
    __shared__ int s_a[SHMEM_SIZE];
    __shared__ int s_b[SHMEM_SIZE];

    int tmp = 0;

    // sweep tile across matrix
    for (int i = 0; i < n; i += blockDim.x)
    {
        // load in elements for this tile
        // each thread laods one element of a tile
        s_a[threadIdx.y * blockDim.x + threadIdx.x] = a[row * n + i + threadIdx.x];
        s_b[threadIdx.y * blockDim.x + threadIdx.x] = b[i * n + threadIdx.y * n + col];

        __syncthreads();

        for (int j = 0; j < blockDim.x; j++)
        {
            tmp += s_a[threadIdx.y * blockDim.x + j] * s_b[j * blockDim.x + threadIdx.x];
        }

        __syncthreads();
    }

    c[row * n + col] = tmp;
}

void verify_result(vector<int> &a, vector<int> &b, vector<int> &c)
{
    // For every row...
    for (int i = 0; i < n; i++)
    {
        // For every column...
        for (int j = 0; j < n; j++)
        {
            // For every element in the row-column pair
            int tmp = 0;
            for (int k = 0; k < n; k++)
            {
                // Accumulate the partial results
                tmp += a[i * n + k] * b[k * n + j];
            }

            // Check against the CPU result
            assert(tmp == c[i * n + j]);
        }
    }
}

int main()
{
    // Size (in bytes) of matrix
    size_t bytes = n * n * sizeof(int);

    // Host vectors
    vector<int> h_a(n * n);
    vector<int> h_b(n * n);
    vector<int> h_c(n * n);

    // Initialize matrices
    generate(h_a.begin(), h_a.end(), []()
             { return rand() % 100; });
    generate(h_b.begin(), h_b.end(), []()
             { return rand() % 100; });

    // Allocate device memory
    int *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, bytes);
    cudaMalloc(&d_b, bytes);
    cudaMalloc(&d_c, bytes);

    // Copy data to the device
    cudaMemcpy(d_a, h_a.data(), bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b.data(), bytes, cudaMemcpyHostToDevice);

    // Threads per CTA dimension
    int THREADS = 32;

    // Blocks per grid dimension (assumes THREADS divides N evenly)
    int BLOCKS = n / THREADS;

    // Use dim3 structs for block  and grid dimensions
    dim3 threads(THREADS, THREADS);
    dim3 blocks(BLOCKS, BLOCKS);

    // Launch kernel
    matmul<<<blocks, threads>>>(d_a, d_b, d_c);

    // Copy back to the host
    cudaMemcpy(h_c.data(), d_c, bytes, cudaMemcpyDeviceToHost);

    // Check result
    verify_result(h_a, h_b, h_c);

    cout << "COMPLETED SUCCESSFULLY\n";

    // Free memory on device
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;
}