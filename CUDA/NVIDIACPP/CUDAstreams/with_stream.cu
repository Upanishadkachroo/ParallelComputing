#include<iostream>
#include<chrono>

__global__ void kernel(float *a, int n){
    int i=blockIdx.x * blockDim.x + threadIdx.x;
    if(i<N){
        a[i]=a[i] * 2.0f;
    }
}

int main(){
    int n=1<<24;
    size_t size=n*sizeof(float);

    float *h, *d;
    cudaMallocHost(&h, size); //pinned memory in DMA
    cudaMalloc(&d, size);

    for(int i=0; i<n; i++) h[i]=1.0f;

    cudaStream_t stream1, stream2;
    cudaStreamCreate(&stream1);
    cudaStreamCreate(&stream2);

    auto start=std::chrono::high_resolution_clock::now();

    cudaMemcpyAsync(d, h, size, cudaMemcpyHostToDevice, stream1);

    int threadsperblock=256;
    int numblocks= (n + (threadsperblock-1)) / threadsperblock;

    cudaMemcpyAsync(h, d, size, cudaMemcpyDeviceToHost, stream1);

    cudaDeviceSynchronize(); // wait for all

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time (no streams): "<< std::chrono::duration<float>(end - start).count() << " sec\n";

    cudaStreamDestroy(stream1);
    cudaStreamDestroy(stream2);
    
    cudaFree(d);
    cudaFreeHost(h);
}