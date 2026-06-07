#include "StatisticsCUDA.cuh"

#include<bits/stdc++.h>

#include <thrust/device_vector.h>
#include <thrust/reduce.h>
#include <thrust/extrema.h>

void computeStatistics(int* arr, int size){
    thrust::device_vector<int> d(arr, arr+size);

    int sum=thrust::reduce(d.begin(), d.end(), 0);

    int maximum=*thrust::max_element(d.begin(), d.end());

    int minimum=*thrust::min_element(d.begin(), d.end());

    double average=(double)sum/size;

    std::cout<<"Sum: "<<sum<<std::endl;

    std::cout<<"Max: "<<maximum<<std::endl;

    std::cout<<"Min: "<<minimum<<std::endl;

    std::cout<<"Average: "<<average<<std::endl;
}