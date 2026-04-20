%%writefile Sources/row-symmetry-check.cpp
#include "ach.cuh"

__global__ void symmetry_check_kernel(ach::temperature_grid_f temp, int row)
{
  // Each thread handles one column
  int column = blockIdx.x * blockDim.x + threadIdx.x;

  // Boundary check
  if (column >= temp.extent(1)) return;

  if (abs(temp(row, column) - temp(temp.extent(0) - 1 - row, column)) > 0.1)
  {
    printf("Error: asymmetry in %d / %d\n", column, temp.extent(1));
  }
}

void symmetry_check(ach::temperature_grid_f temp, cudaStream_t stream)
{
  int width = temp.extent(1);

  int threads_per_block = 256;
  int num_blocks = (width + threads_per_block - 1) / threads_per_block;

  int target_row = 0;

  // Launch enough threads so each column is checked
  symmetry_check_kernel<<<num_blocks, threads_per_block, 0, stream>>>(temp, target_row);
}