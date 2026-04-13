#include <thrust/device_vector.h>
#include <thrust/gather.h>
#include <iostream>

int main() {
    // Input data
    thrust::device_vector<int> input{10, 20, 30, 40, 50};

    // Map indices
    thrust::device_vector<int> map{3, 0, 4, 1};

    // Output vector
    thrust::device_vector<int> output(map.size());

    // Gather operation
    thrust::gather(map.begin(), map.end(), input.begin(), output.begin());

    // Print result
    for (int i = 0; i < output.size(); i++) {
        std::cout << output[i] << " ";
    }

    return 0;
}