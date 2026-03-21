#include<bits/stdc++.h>
#include<cuda_runtime.h>
using namespace std;

__global__ void mat_vec_mul(const float *A, const float *X, float *B, int N){
    int index=blockIdx.x * blockDim.x + threadIdx.x;//row index

    if(index<N){
        float sum=0.0f;
        for(int j=0; j<N; j++){
            sum+=A[index* N + j] * X[j]; //A[0 * 10 + 0]=A[0], A[0+1]=A[1]
        }
        B[index]=sum;
    }
}

int main(){
    float *A, *X, *B;
    const int N=10;

    A=(float*)malloc(N * N * sizeof(float));
    X=(float*)malloc(N * sizeof(float));
    B=(float*)malloc(N * sizeof(float));

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i*N+j]=1.0f;
        }
        X[i]=2.0f;
        B[i]=0.0f;
    }

    float *d_a, *d_b, *d_x;
    cudaMalloc(&d_a, N*N*sizeof(float));
    cudaMalloc(&d_x, N*sizeof(float));
    cudaMalloc(&d_b, N*sizeof(float));

    cudaMemcpy(d_a, A, N * N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, X, N * sizeof(float), cudaMemcpyHostToDevice);

    int threadsperblock=256;
    int numblocks=((int)ceil((float)N/threadsperblock));
    mat_vec_mul<<<numblocks, threadsperblock>>>(d_a, d_x, d_b, N);

    cudaDeviceSynchronize();
    cudaMemcpy(B, d_b, N * sizeof(float), cudaMemcpyDeviceToHost);

    for(int i=0; i<N*N; i++){
        cout<<A[i]<<", ";
    }

    printf("\n");
    for(int i=0; i<N; i++){
        cout<<X[i]<<", ";
    }

    printf("\n");
    for(int i=0; i<N; i++){
        cout<<B[i]<<", ";
    }

    cudaFree(d_x);
    cudaFree(d_a);
    cudaFree(d_b);
}
using namespace std;

__global__ void mat_vec_mul(const flaot *A, const float *X, float *B, int N){
    int index=blockIdx.x * blockDim.x + threadIdx.x;//row index

    if(index<N){
        float sum=0.0f;
        for(int j=0; j<N; j++){
            sum+=A[index* N + j] * X[j]; //A[0 * 10 + 0]=A[0], A[0+1]=A[1]
        }
        B[index]=sum;
    }
}

int main(){
    float *A, *X, *B;
    const int N=10;

    A=(float*)malloc(N * N * sizeof(float));
    X=(float*)malloc(N * sizeof(float));
    B=(float*)malloc(N * sizeof(float));

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i*N+j]=1.0f;
        }
        X[i]=2.0f;
        B[i]=0.0f;
    }

    float *d_a, *d_b, *d_x;
    cudaMalloc(&d_a, N*N*sizeof(float));
    cudaMalloc(&d_x, N*N*sizeof(float));
    cudaMalloc(&d_b, N*N*sizeof(float));

    cudaMemcpy(d_a, A, N * N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, X, N * N * sizeof(float), cudaMemcpyHostToDevice);

    int threadsperblock=256;
    int numblocks=((int)ceil((float)N/threadsperblock));
    mat_vec_mul<<<numblocks, threadsperblock>>>(d_a, d_x, d_b, N);

    cudaDeviceSynchronize();
    cudaMemcpy(B, d_b, N * sizeof(float), cudaMemcpyDeviceToHost);

    for(int i=0; i<N*N; i++){
        cout<<A[i]<<", ";
    }

    printf("/n");
    for(int i=0; i<N*N; i++){
        cout<<X[i]<<", ";
    }

    printf("/n");
    for(int i=0; i<N; i++){
        cout<<B[i]<<", ";
    }

    cudaFree(d_x);
    cudaFree(d_a);
    cudaFree(d_b);
}
using namespace std;

__global__ void mat_vec_mul(const flaot *A, const float *X, float *B, int N){
    int index=blockIdx.x * blockDim.x + threadIdx.x;//row index

    if(index<N){
        float sum=0.0f;
        for(int j=0; j<N; j++){
            sum+=A[index* N + j] * X[j]; //A[0 * 10 + 0]=A[0], A[0+1]=A[1]
        }
        B[index]=sum;
    }
}

int main(){
    float *A, *X, *B;
    const int N=10;

    A=(float*)malloc(N * N * sizeof(float));
    X=(float*)malloc(N * sizeof(float));
    B=(float*)malloc(N * sizeof(float));

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            A[i*N+j]=1.0f;
        }
        X[i]=2.0f;
        B[i]=0.0f;
    }

    float *d_a, *d_b, *d_x;
    cudaMalloc(&d_a, N*N*sizeof(float));
    cudaMalloc(&d_x, N*N*sizeof(float));
    cudaMalloc(&d_b, N*N*sizeof(float));

    cudaMemcpy(d_a, A, N * N * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, X, N * N * sizeof(float), cudaMemcpyHostToDevice);

    int threadsperblock=256;
    int numblocks=((int)ceil((float)N/threadsperblock));
    mat_vec_mul<<<numblocks, threadsperblock>>>(d_a, d_x, d_b, N);

    cudaDeviceSynchronize();
    cudaMemcpy(B, d_b, N * sizeof(float), cudaMemcpyDeviceToHost);

    for(int i=0; i<N*N; i++){
        cout<<A[i]<<", ";
    }

    printf("/n");
    for(int i=0; i<N*N; i++){
        cout<<X[i]<<", ";
    }

    printf("/n");
    for(int i=0; i<N; i++){
        cout<<B[i]<<", ";
    }

    cudaFree(d_x);
    cudaFree(d_a);
    cudaFree(d_b);
}