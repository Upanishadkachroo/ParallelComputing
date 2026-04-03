#include "ach.h"
#include <thrust/universal_vector.h>  // unified memory vector (CPU + GPU access)
#include <thrust/transform.h>         // for thrust::transform (parallel apply)
#include <thrust/sort.h>              // for thrust::sort (GPU sorting)
#include <cstdio>                    // for printf

// Function to compute median
// NOTE: We pass by value → creates a COPY so original data is not modified
float median(thrust::universal_vector<float> vec)
{
    // Sort the vector on GPU
    // thrust::device → ensures execution on GPU
    thrust::sort(thrust::device, vec.begin(), vec.end());

    // Return middle element (median)
    // For odd size: exact middle
    // For even size: this gives upper-middle (not average)
    return vec[vec.size() / 2];
}

int main()
{
    // Cooling constant (controls rate of convergence)
    float k = 0.5;

    // Ambient/environment temperature
    float ambient_temp = 20;

    // Initial temperatures stored in unified memory
    // Accessible from both CPU and GPU
    thrust::universal_vector<float> temp{ 42, 24, 50 };

    // Lambda transformation (cooling formula)
    // [=] → capture k and ambient_temp by value
    // __host__ __device__ → usable on both CPU and GPU
    auto transformation = [=] __host__ __device__ (float temp) {
        // Formula:
        // new_temp = current_temp + k * (ambient_temp - current_temp)
        return temp + k * (ambient_temp - temp);
    };

    // Print header
    std::printf("step  median\n");

    // Run simulation for 3 steps
    for (int step = 0; step < 3; step++) {

        // Step 1: Update temperatures on GPU
        // Each element is processed in parallel
        thrust::transform(
            thrust::device,          // execution policy → GPU
            temp.begin(), temp.end(),// input range
            temp.begin(),            // output (in-place update)
            transformation           // function applied to each element
        );

        // Step 2: Compute median (on GPU via thrust::sort)
        float median_temp = median(temp);

        // Print result
        std::printf("%d     %.2f\n", step, median_temp);
    }

    return 0;
}