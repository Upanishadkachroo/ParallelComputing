// cub_demo.cu
#include <iostream>
#include <chrono>
#include <thrust/device_vector.h>
#include <cub/cub.cuh>

// Simple transform kernel using CUB
void cub_compute(int N, thrust::device_vector<float>& out)
{
    auto counting = thrust::make_counting_iterator(0);

    // Operation
    auto op = [=] __device__ (int i) {
        float x = i * 0.001f;
        return x * x + 2.0f;
    };

    cub::DeviceTransform::Transform(
        counting,
        out.begin(),
        N,
        op
    );
}

int main()
{
    int N = 1 << 24;
    thrust::device_vector<float> d_out(N);

    // -------------------------------
    // ❌ Case 1: Without synchronization
    // -------------------------------
    auto start1 = std::chrono::high_resolution_clock::now();

    cub_compute(N, d_out);

    auto end1 = std::chrono::high_resolution_clock::now();

    std::cout << "CUB Time (no sync): "
              << std::chrono::duration<float>(end1 - start1).count()
              << " sec\n";


    // -------------------------------
    // ✅ Case 2: With synchronization
    // -------------------------------
    auto start2 = std::chrono::high_resolution_clock::now();

    cub_compute(N, d_out);

    cudaDeviceSynchronize();  // important

    auto end2 = std::chrono::high_resolution_clock::now();

    std::cout << "CUB Time (with sync): "
              << std::chrono::duration<float>(end2 - start2).count()
              << " sec\n";

    return 0;
}