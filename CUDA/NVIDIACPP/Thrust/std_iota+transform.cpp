#include<iostream>
#include<vector>
#include<numeric>
#include<algorithm>


int main(){
    std::vector<int> v(10);

    std::iota(v.begin(), v.end(), 0);

    std::transform(v.begin(), v.end(), v.begin(), [](int x){
        return 2 * x + 1;
    });

    for(int x : v) std::cout<<x<<" ";
}