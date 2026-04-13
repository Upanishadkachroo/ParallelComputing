#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <thrust/transform.h>
#include <iostream>

struct odd_functor {
    __device__
    int operator()(int x) {
        return 2 * x + 1;
    }
};

int main() {
    thrust::device_vector<int> v(10);

    thrust::sequence(v.begin(), v.end(), 0);  // [0,1,...,9]

    thrust::transform(v.begin(), v.end(), v.begin(), odd_functor());

    for (int i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
}