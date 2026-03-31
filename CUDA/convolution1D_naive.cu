#include <bits/stdc++.h>
#include <cuda_runtime.h>
using namespace std;

__global__ void convolution(int *array, int *mask, int *result, int n, int m)
{
    // n -> elements of the array
    // m -> number of elements in mask

    int tid = blockIdx.x * blockDim * x + threadIdx.x;

    // radius of mask
    int r = m / 2;

    // start point of the element
    int temp = 0;

    // iterate for each element in the mask
    for (int j = 0; j < m; j++)
    {
        if (((start + j) >= 0) && (start + j < n))
        {
            temp += array[start + j] * mask[j];
        }
    }

    result[tid] = temp;
}

// Verify the result on the CPU
void verify_result(int *array, int *mask, int *result, int n, int m)
{
    int radius = m / 2;
    int temp;
    int start;
    for (int i = 0; i < n; i++)
    {
        start = i - radius;
        temp = 0;
        for (int j = 0; j < m; j++)
        {
            if ((start + j >= 0) && (start + j < n))
            {
                temp += array[start + j] * mask[j];
            }
        }
        assert(temp == result[i]);
    }
}

int main()
{
    int n = 1 << 20;

    // size of array in bytes
    int bytes_n = n * sizeof(int);

    // no. of elements in convolution mask
    int m = 7;

    // size of mask
    int bytes_m = m * sizeof(int);

    // allocate the array
    vector<int> h_array(n);

    generate(begin(h_array), end(h_array), []()
             { return rand() % 100; });

    vector<int> h_mask(m);
    generate(begin(h_mask), end(h_mask), []()
             { return rand() % 10; });

    vector<int> h_result(n);

    int *d_array, *d_mask, *d_result;
    cudaMalloc(&d_array, bytes_n);
    cudaMalloc(&d_mask, bytes_m);
    cudaMalloc(&d_result, bytes_n);

    // Copy the data to the device
    cudaMemcpy(d_array, h_array.data(), bytes_n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_mask, h_mask.data(), bytes_m, cudaMemcpyHostToDevice);

    // Threads per TB
    int THREADS = 256;

    // Number of TBs
    int GRID = (n + THREADS - 1) / THREADS;

    // Call the kernel
    convolution_1d<<<GRID, THREADS>>>(d_array, d_mask, d_result, n, m);

    // Copy back the result
    cudaMemcpy(h_result.data(), d_result, bytes_n, cudaMemcpyDeviceToHost);

    // Verify the result
    verify_result(h_array.data(), h_mask.data(), h_result.data(), n, m);

    std::cout << "COMPLETED SUCCESSFULLY\n";

    // Free allocated memory on the device and host
    cudaFree(d_result);
    cudaFree(d_mask);
    cudaFree(d_array);
}