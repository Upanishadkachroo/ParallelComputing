#include<iostream>
#include<vector> 

#include "include/Statistics.hpp"

int main(){
    std::vector<int> numbers={1,2,3,4,5,6,7,8,9,10};

    Statistics obj(numbers);

    std::cout<<"Input Data\n";

    obj.display();

    std::cout<<"\nGPU Statistics\n";

    obj.calculate();

    return 0;
}