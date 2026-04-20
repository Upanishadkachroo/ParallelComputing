%%writefile Sources/histogram-bug.cpp
#include "ach.cuh"

constexpr float bin_width = 10;

__global__ void histogram_kernel(cuda::std::span<float> temperatures,
                                 cuda::std::span<int> histogram)
{
  int cell = blockIdx.x * blockDim.x + threadIdx.x;

  // Boundary check (important!)
  if (cell >= temperatures.size()) return;

  int bin = static_cast<int>(temperatures[cell] / bin_width);

  // Wrap histogram[bin] with atomic_ref
  cuda::std::atomic_ref<int> ref(histogram[bin]);

  // Atomic increment
  ref.fetch_add(1);
}

void histogram(cuda::std::span<float> temperatures,
               cuda::std::span<int> histogram,
               cudaStream_t stream)
{
  int block_size = 256;
  int grid_size = cuda::ceil_div(temperatures.size(), block_size);

  histogram_kernel<<<grid_size, block_size, 0, stream>>>(
    temperatures, histogram);
}