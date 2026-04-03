#include "ach.h"
#include <thrust/for_each.h>
#include <thrust/universal_vector.h>
#include <cstdio>

int main() {
    thrust::universal_vector<int> vec{ 1, 2, 3 };

    // Lambda stored separately
    auto printer = [] __device__ (int val) {
        printf("printing %d on %s\n", val, ach::execution_space());
    };

    thrust::for_each(thrust::device, vec.begin(), vec.end(), printer);
}