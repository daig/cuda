#include <iostream>
#include <cuda.h>
#define N 10000000

__global__ void vector_add(float *out, float *a, float *b, int n) {
    for (int i = 0; i < n; i++){ out[i] = a[i] + b[i]; }
}

int main() {
    CUcontext* ctx;
    cuCxtCreate(ctx, 0, 0);

}
