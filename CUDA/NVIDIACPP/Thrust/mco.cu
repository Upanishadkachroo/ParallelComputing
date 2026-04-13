#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/extrema.h>
#include <iostream>

int main() {
    // Input array
    thrust::device_vector<int> keys{1,1,0,1,1,1,0,1};

    int n = keys.size();

    // Values array (all 1s)
    thrust::device_vector<int> values(n, 1);

    // Output arrays
    thrust::device_vector<int> out_keys(n);
    thrust::device_vector<int> out_counts(n);

    // Step 1: reduce_by_key
    auto result = thrust::reduce_by_key(
        keys.begin(), keys.end(),
        values.begin(),
        out_keys.begin(),
        out_counts.begin()
    );

    int new_size = result.first - out_keys.begin();

    // Step 2: find max count where key == 1
    int max_ones = 0;

    for(int i = 0; i < new_size; i++) {
        if(out_keys[i] == 1 && out_counts[i] > max_ones) {
            max_ones = out_counts[i];
        }
    }

    std::cout << "Maximum consecutive 1s = " << max_ones << std::endl;

    return 0;
}