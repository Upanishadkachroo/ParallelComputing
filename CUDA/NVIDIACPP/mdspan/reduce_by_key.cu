// reduce_by_key_full.cu
// ---------------------------------------------
// Efficient row-wise sum using thrust::reduce_by_key
// ---------------------------------------------

#include <cstdio>
#include <chrono>

#include <thrust/universal_vector.h>
#include <thrust/reduce.h>
#include <thrust/execution_policy.h>
#include <thrust/iterator/discard_iterator.h>

// ---------------------------------------------
// Initialize temperature grid
// ---------------------------------------------
thrust::universal_vector<float> init(int height, int width)
{
    const float low = 15.0f;
    const float high = 90.0f;

    thrust::universal_vector<float> temp(height * width, low);

    // Make first row hot
    thrust::fill(thrust::device, temp.begin(), temp.begin() + width, high);

    return temp;
}

// ---------------------------------------------
// Create row_ids: [0,0,0,...,1,1,1,...]
// ---------------------------------------------
thrust::universal_vector<int> create_row_ids(int height, int width)
{
    thrust::universal_vector<int> row_ids(height * width);

    thrust::tabulate(
        thrust::device,
        row_ids.begin(),
        row_ids.end(),
        [=] __host__ __device__ (int id)
        {
            return id / width;  // row number
        });

    return row_ids;
}

// ---------------------------------------------
// Row-wise sum using reduce_by_key
// ---------------------------------------------
thrust::universal_vector<float> row_temperatures(
    int height, int width,
    thrust::universal_vector<int>& row_ids,
    thrust::universal_vector<float>& temp)
{
    thrust::universal_vector<float> sums(height);

    thrust::reduce_by_key(
        thrust::device,
        row_ids.begin(), row_ids.end(),   // keys
        temp.begin(),                     // values
        thrust::make_discard_iterator(),  // ignore output keys
        sums.begin());                    // output sums

    return sums;
}

// ---------------------------------------------
// Main function
// ---------------------------------------------
int main()
{
    int height = 16;
    int width  = 1 << 24; // ~16M

    auto temp = init(height, width);
    auto row_ids = create_row_ids(height, width);

    auto start = std::chrono::high_resolution_clock::now();

    auto sums = row_temperatures(height, width, row_ids, temp);

    auto end = std::chrono::high_resolution_clock::now();

    double seconds =
        std::chrono::duration<double>(end - start).count();

    double gigabytes =
        (double)(temp.size() * sizeof(float)) / (1024 * 1024 * 1024);

    double throughput = gigabytes / seconds;

    std::printf("Time: %f seconds\n", seconds);
    std::printf("Throughput: %f GB/s\n", throughput);

    // Print first few sums
    for (int i = 0; i < 5; i++) {
        std::printf("Row %d sum = %f\n", i, sums[i]);
    }

    return 0;
}