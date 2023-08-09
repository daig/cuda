#include <iostream>
#include <fstream>
#include <cuda.h>
// clang++-16 mytest.cpp -I/usr/local/cuda/include -L/usr/local/cuda/lib64 -lcuda

const size_t N = 100000;


// /usr/local/cuda/include

using namespace std;

int main() {

    CUresult cuResult = cuInit(0);
    if (cuResult != CUDA_SUCCESS) { cout << "cuInit failed" << endl; return 1; }

    CUdevice cuDevice;
    cuResult = cuDeviceGet(&cuDevice, 0);
    if (cuResult != CUDA_SUCCESS) { cout << "cuDeviceGet failed" << endl; return 1; }

    CUcontext cuContext;
    cuResult = cuCtxCreate(&cuContext, 0, cuDevice);
    if (cuResult != CUDA_SUCCESS) { cout << "cuCtxCreate failed" << endl; return 1; }

    int *a, *b, *c;
    CUdeviceptr d_a, d_b, d_c;
    int size = N * sizeof(int);

    cuResult = cuMemAlloc(&d_a, size);
    if (cuResult != CUDA_SUCCESS) { cout << "cuMemAlloc d_a failed" << endl; return 1; }

    cuResult = cuMemAlloc(&d_b, size);
    if (cuResult != CUDA_SUCCESS) { cout << "cuMemAlloc d_b failed" << endl; return 1; }

    cuResult = cuMemAlloc(&d_c, size);
    if (cuResult != CUDA_SUCCESS) { cout << "cuMemAlloc d_c failed" << endl; return 1; }

    a = (int *) malloc(size);
    b = (int *) malloc(size);
    c = (int *) malloc(size);

    for (int i = 0; i < N; i++) {
        a[i] = i;
        b[i] = i;
    }

    cuResult = cuMemcpyHtoD(d_a, a, size);
    if (cuResult != CUDA_SUCCESS) { cout << "cuMemcpyHtoD d_a failed" << endl; return 1; }
    cuResult = cuMemcpyHtoD(d_b, b, size);
    if (cuResult != CUDA_SUCCESS) { cout << "cuMemcpyHtoD d_b failed" << endl; return 1; }

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

    cuResult = cuMemcpyDtoH(c, d_c, size);
    if (cuResult != CUDA_SUCCESS) { cout << "cuMemcpyDtoH failed" << endl; return 1; }

    for (int i = 0; i < N; i++) {
        cout << c[i] << endl;
    }

    free(a);
    free(b);
    free(c);

    cuMemFree(d_a);
    cuMemFree(d_b);
    cuMemFree(d_c);
    cuCtxDestroy(cuContext);

    return 0;

}
