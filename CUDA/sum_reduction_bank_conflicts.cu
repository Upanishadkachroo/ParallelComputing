#include<bits/stdc++.h>
#include<cuda_runtime.h>
using namespace std;

#define size 256
#define SHMEM_SIZE 256*4

__global__ void sum_reduction(int *v, int *v_r){
    //allocate shared memory
    __shared__ int partial_sum[SHMEM_SIZE];

    int tid=blockIdx.x * blockDim.x + threadIdx.x;

    partial_sum[threadIdx.x]=v[tid];
    __syncthreads();

    //start at 1/2 block stride and divide by two each iteration
    for(int s=blockDim.x /2; s>0; s>>=1){
        //each thread does work unless it is further than the stride
        if(threadIdx.x < s){
            partial_sum[threadIdx.x]+=partial_sum[threadIdx.x + s];
        }
        __syncthreads();
    }

    //first index is the answer
    if(threadIdx.x == 0){
        v_r[blockIdx.x] = partial_sum[0];
    }
}

void initialize_vector(int *v, int n){
    for(int i=0; i<n; i++){
        v[i]=1;
    }
}

int main(){
    int n=1<<16;
    size_t bytes=n * sizeof(int);

    int *h_v, *h_v_r;
	int *d_v, *d_v_r;

	h_v = (int*)malloc(bytes);
	h_v_r = (int*)malloc(bytes);
	cudaMalloc(&d_v, bytes);
	cudaMalloc(&d_v_r, bytes);

	initialize_vector(h_v, n);

	cudaMemcpy(d_v, h_v, bytes, cudaMemcpyHostToDevice);

	int TB_SIZE = size;

	int GRID_SIZE = n / TB_SIZE;

	sum_reduction <<<GRID_SIZE, TB_SIZE >>> (d_v, d_v_r);

	sum_reduction <<<1, TB_SIZE >>> (d_v_r, d_v_r);f

	cudaMemcpy(h_v_r, d_v_r, bytes, cudaMemcpyDeviceToHost);

	assert(h_v_r[0] == 65536);

	printf("COMPLETED SUCCESSFULLY\n");
}