#include<bits/stdc++.h>

struct counting_iterator{
    int operator[](int i){
        return i;
    }
};

int main(){
    counting_iterator it;

    std::printf("it[0]: %d\n", it[0]);
}