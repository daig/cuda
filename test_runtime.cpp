#include <iostream>
#include <fstream>
#include <cuda_runtime.h>
#include <cuda.h>
// clang++-16 mytest.cpp -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -lcudart -lcuda

#define N 10000000


// /usr/local/cuda/include

using namespace std;

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

    // load the ptx file
    ifstream ptxFile("test_pretty.ptx");
    if (!ptxFile.is_open()) { cout << "Error opening file" << endl; exit(1); }
    string ptxSource((istreambuf_iterator<char>(ptxFile)), istreambuf_iterator<char>());
    //load the module
    CUmodule modul;
    cuModuleLoadData(&modul, ptxSource.c_str());
    //get the kernel function
    CUfunction kernel;
    cuModuleGetFunction(&kernel, modul, "add");

    //launch the kernel
    void* args[] = {&d_a, &d_b, &d_c};
    cuLaunchKernel(kernel, (N + 255)/256, 1, 1, 256, 1, 1, 0, nullptr, args, nullptr);
    //add<<<(N + 255) / 256, 256>>>(d_a, d_b, d_c);

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
