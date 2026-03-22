#include<bits/stdc++.h>
#include<cuda_runtime.h>
using namespace std;

__global__ void matrix_transpose(const float *input, float *output, int width, int height){
    int x=blockIdx.x*blockDim.x+threadIdx.x;//column index of the matrix
    int y=blockIdx.y*blockDim.y+threadIdx.y;//row index

    if(x < width && y < height){
        //conver in row major order
        int inputindex = y * width + x;
        int outputindex = x * height + y;

        output[outputindex]=input[inputindex]; //simialr to input[y][x] -> output[x][y]
    }
}

int main(){
    int width=10;
    int height=10;

    float *h_input=(float*)malloc(width * height * sizeof(float));
    float *h_output=(float*)malloc(width * height * sizeof(float));
    
    for(int i=0; i<width*height; i++){
        h_input[i]=(float)i;
    }

    float *d_input, *d_output;
    cudaMalloc((void**)&d_input, width*height*sizeof(float));
    cudaMalloc((void**)&d_output, width*height*sizeof(float));

    cudaMemcpy(d_input, h_input, width*height*sizeof(float), cudaMemcpyHostToDevice);

    dim3 threadsperblock(32, 32);
    dim3 numblocks(
        (width + threadsperblock.x - 1)/threadsperblock.x,
        (height + threadsperblock.y - 1)/threadsperblock.y
    );

    matrix_transpose<<<numblocks, threadsperblock>>>(d_input, d_output, width, height);

    cudaDeviceSynchronize();
    cudaMemcpy(h_output, d_output, width*height*sizeof(float), cudaMemcpyDeviceToHost);

    printf("Input Matrix: \n");
    for(int i=0; i<width*height; i++){
        cout<<h_input[i]<<", ";
    }

    printf("\n");
    printf("Output matrix: \n");
    for(int i=0; i<width*height; i++){
        cout<<h_output[i]<<", ";
    }

    cudaFree(d_input);
    cudaFree(d_output);

    return 0;

}