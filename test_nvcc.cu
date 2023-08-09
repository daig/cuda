#include <iostream>
// nvcc -keep -keep-dir test_output test0.cu
#define N 10000000

using namespace std;

__global__ void add(int *a, int *b, int *c) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < N) { c[tid] = a[tid] + b[tid]; }
}


int main() {
    int *a, *b, *c;
    int *d_a, *d_b, *d_c;
    int size = N * sizeof(int);

    cudaMalloc((int **) &d_a, size);
    cudaMalloc((int **) &d_b, size);
    cudaMalloc((int **) &d_c, size);

    a = (int *) malloc(size);
    b = (int *) malloc(size);
    c = (int *) malloc(size);

    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i;
    }

    cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

    add<<<(N + 255) / 256, 256>>>(d_a, d_b, d_c);

    cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

    for (int i = 0; i < N; i++) {
        cout << c[i] << endl;
    }

    free(a);
    free(b);
    free(c);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return 0;

}

