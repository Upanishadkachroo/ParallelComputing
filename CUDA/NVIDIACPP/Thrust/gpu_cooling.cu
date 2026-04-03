#include <thrust/device_vector.h>
#include <thrust/transform.h>
#include <iostream>

//use nvcc command to execute in gpu
int main() {
    float k = 0.5;
    float ambient_temp = 20;

    // Device vector (stored on GPU)
    thrust::device_vector<float> temp{42, 24, 50};

    // Lambda for GPU (must be __device__)
    auto transformation = [=] __device__ (float t) {
        return t + k * (ambient_temp - t);
    };

    for (int step = 0; step < 3; step++) {

        // Runs in parallel on GPU
        thrust::transform(temp.begin(), temp.end(), temp.begin(), transformation);
    }

    // Copy back to CPU for printing
    std::vector<float> host = temp;

    for (float x : host)
        std::cout << x << " ";
}