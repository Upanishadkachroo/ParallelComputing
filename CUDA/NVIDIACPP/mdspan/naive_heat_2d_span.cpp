// naive_heat_2D_cpu.cpp
// ---------------------------------------------
// Naive 2D Heat Transfer using CPU (loops)
// Same logic as GPU version but sequential
// ---------------------------------------------

#include <vector>

// simulate one timestep of heat diffusion
void simulate_cpu(int height, int width,
                  const std::vector<float> &in,
                  std::vector<float> &out)
{
    // Loop over each cell in 2D grid
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            // Check if cell is NOT on boundary
            if (row > 0 && column > 0 &&
                row < height - 1 && column < width - 1)
            {
                // Compute second derivative in X direction
                float d2tdx2 =
                    in[row * width + (column - 1)]    // left
                    - 2 * in[row * width + column]    // center
                    + in[row * width + (column + 1)]; // right

                // Compute second derivative in Y direction
                float d2tdy2 =
                    in[(row - 1) * width + column]    // up
                    - 2 * in[row * width + column]    // center
                    + in[(row + 1) * width + column]; // down

                // Update temperature
                out[row * width + column] =
                    in[row * width + column] + 0.2f * (d2tdx2 + d2tdy2);
            }
            else
            {
                // Boundary cells remain unchanged
                out[row * width + column] =
                    in[row * width + column];
            }
        }
    }
}