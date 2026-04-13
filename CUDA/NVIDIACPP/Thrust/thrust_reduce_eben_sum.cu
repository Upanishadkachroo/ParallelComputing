#include <thrust/device_vector.h>
#include <thrust/transform_reduce.h>
#include <iostream>

struct even_sum {
    __device__
    int operator()(int x) {
        return (x % 2 == 0) ? x : 0;
    }
};

int main() {
    thrust::device_vector<int> v{1,2,3,4,5,6};

    int result = thrust::transform_reduce(
        v.begin(), v.end(),
        even_sum(),   // transform
        0,            // initial value
        thrust::plus<int>() // reduce
    );

    std::cout << "Sum of even numbers = " << result << std::endl;

    return 0;
}