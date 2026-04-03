#include<bits/stdc++.h>

struct tranform_iterator{
    int *a;

    int operator[](int i){
        return i*2;
    }
};

int main(){
    std::array<int, 3> a{0, 1, 2};

    transform_iterator it{a.data()};

    std::printf("it[0]: %d\n", it[0]);
}