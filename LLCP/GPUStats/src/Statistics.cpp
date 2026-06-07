#include "../include/Statistics.hpp"
#include "../cuda/StatisticsCUDA.cuh"
#include <iostream>

Statistics::Statistics(const std::vector<int> &input){
    data=input;
}

void Statistics::display(){
    for(auto x:data){
        std::cout<<x<<" ";
    }

    std::cout<<std::endl;
}

void Statistics::calculate(){
    computeStatistics(data.data(), data.size());
}