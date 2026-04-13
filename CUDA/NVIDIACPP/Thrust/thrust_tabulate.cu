#include<thrust/device_vector.h>
#include<thrust/tabulate.h>
#include<iostream>

struct odd_functor{
    __device__
    int operator()(int i){
        return 2 * i + 1;
    }
};

int main(){
    thrust::device_vector<int> v(10);

    thrust::tabulate(v.begin(), v.end(), odd_functor());

    for(int i=0; i<v.size(); i++){
        std::cout<<v[i]<<" ";
    }
}