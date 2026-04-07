// tabulate_heat_2D.cu
// ---------------------------------------------
// Heat transfer using thrust::tabulate (GPU)
// Cleaner than transform + counting_iterator
// ---------------------------------------------

#include <thrust/universal_vector.h>
#include <thrust/tabulate.h>
#include <thrust/execution_policy.h>

void simulate(int height, int width,
              const thrust::universal_vector<float> &in,
              thrust::universal_vector<float> &out)
{
    // Convert thrust vector to raw pointer
    const float *in_ptr = thrust::raw_pointer_cast(in.data());

    // Launch parallel computation
    thrust::tabulate(
        thrust::device,
        out.begin(),
        out.end(),

        // Each thread computes value for index 'id'
        [in_ptr, height, width] __host__ __device__(int id)
        {
            // Convert 1D index → 2D coordinates
            int column = id % width;
            int row    = id / width;

            // Check if not boundary
            if (row > 0 && column > 0 &&
                row < height - 1 && column < width - 1)
            {
                // X-direction second derivative
                float d2tdx2 =
                    in_ptr[row * width + (column - 1)]
                  - 2 * in_ptr[row * width + column]
                  + in_ptr[row * width + (column + 1)];

                // Y-direction second derivative
                float d2tdy2 =
                    in_ptr[(row - 1) * width + column]
                  - 2 * in_ptr[row * width + column]
                  + in_ptr[(row + 1) * width + column];

                // Return updated value
                return in_ptr[row * width + column]
                     + 0.2f * (d2tdx2 + d2tdy2);
            }
            else
            {
                // Boundary unchanged
                return in_ptr[row * width + column];
            }
        });
}