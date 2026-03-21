#include<bits/stdc++.h>
#include<cmath>
using namespace std;

__global__ void partialsumkernel(int *input, int *output, int n){
    extern __shared__  int 
    
    int tid=threadIdx.x; //threadindex
    int index=blockIdx.x * blockDim.x * 2 + tid; // global index




}

int main(){

}