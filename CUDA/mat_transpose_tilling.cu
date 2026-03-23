#include<bits/stdc++.h>
#include<cmath>
using namespace std;

#define TILE_DIM 32

__global__ void matrixtransposetilling(const float* input, float* ouptut, int width, int height){
    // 1. Shared Memory Declaration
    // We add +1 to the column dimension to avoid Bank Conflicts (explained below)
    __shared__ float tile[TILE_DIM][TILE_DIM+1];

     // 2. Global Input Coordinates
    int x = blockIdx.x * TILE_DIM + threadIdx.x;//column index of the matrix
    int y = blockIdx.y * TILE_DIM + threadIdx.y;//row index

    // 3. Load Data into Shared Memory (Coalesced Read)
    if(x<width && y<height){
        int inputindex = y * width + x;
        // Read from Global (row-major), write to Shared as-is
        tile[threadIdx.y][threadIdx.x] = input[inputindex];
    }

    // 4. Synchronization barrier
    // Ensure the entire tile is loaded before we start reading it back
    __syncthreads();

    // 5. Calculate Transposed Global Coordinates
    // We swap blockIdx.x and blockIdx.y because the blocks are transposed
    // We swap threadIdx.x and threadIdx.y logic for the output location
    int x = blockIdx.y * TILE_DIM + threadIdx.x;//column index of the matrix
    int y = blockIdx.x * TILE_DIM + threadIdx.y;//row index

    // 6. Write Data to Global Memory (Coalesced Write)
    if(x<height && y<width){
        int inputindex = y * height+ x;
        // Read from Shared (transposed), Write to Global (row-major)
        // Note the swapped indices: tile[threadIdx.x][threadIdx.y]
        output[inputindex] = tile[threadIdx.x][threadIdx.y];
    }
}

int main(){
    int width = 10;
    int height = 10;

    // float *h_input, *h_output;

    float *h_input = (float *)malloc(width * height * sizeof(float));
    float *h_output = (float *)malloc(width * height * sizeof(float));

    for (int i=0; i<width * height; i++){
        h_input[i] = (float)i;
    }

    float *d_input, *d_output;
    cudaMalloc(&d_input, width * height * sizeof(float));
    cudaMalloc(&d_output, width * height * sizeof(float));

    cudaMemcpy(d_input, h_input, width * height * sizeof(float), cudaMemcpyHostToDevice);

    dim3 blocksize(10, 10);
    dim3 gridsize((width + blocksize.x - 1) / blocksize.x, (height + blocksize.x - 1) / blocksize.y);
    transposeMatrixShared<<<gridsize, blocksize>>>(d_input, d_output, width, height);

    cudaMemcpy(h_output, d_output, width * height * sizeof(float), cudaMemcpyDeviceToHost);

    printf("Input Matrix: \n");
    for (int i=0; i<width * height; i++){
        std::cout<< h_input[i] <<", ";
    }

    printf("Output Matrix: \n");
    for (int i=0; i<width * height; i++){
        std::cout<< h_output[i] <<", ";
    }

    cudaFree(d_input);
    cudaFree(d_output);
    return 0;
}