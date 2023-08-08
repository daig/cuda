#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wcast-qual"
#define __NV_CUBIN_HANDLE_STORAGE__ static
#if !defined(__CUDA_INCLUDE_COMPILER_INTERNAL_HEADERS__)
#define __CUDA_INCLUDE_COMPILER_INTERNAL_HEADERS__
#endif
#include "crt/host_runtime.h"
#include "test.fatbin.c"
extern void __device_stub__Z3addPiS_S_(int *, int *, int *);
static void __nv_cudaEntityRegisterCallback(void **);
static void __sti____cudaRegisterAll(void) __attribute__((__constructor__));
void __device_stub__Z3addPiS_S_(int *__par0, int *__par1, int *__par2){__cudaLaunchPrologue(3);__cudaSetupArgSimple(__par0, 0UL);__cudaSetupArgSimple(__par1, 8UL);__cudaSetupArgSimple(__par2, 16UL);__cudaLaunch(((char *)((void ( *)(int *, int *, int *))add)));}
# 6 "test.cu"
void add( int *__cuda_0,int *__cuda_1,int *__cuda_2)
# 6 "test.cu"
{__device_stub__Z3addPiS_S_( __cuda_0,__cuda_1,__cuda_2);


}
# 1 "test_output/test.cudafe1.stub.c"
static void __nv_cudaEntityRegisterCallback( void **__T2) {  __nv_dummy_param_ref(__T2); __nv_save_fatbinhandle_for_managed_rt(__T2); __cudaRegisterEntry(__T2, ((void ( *)(int *, int *, int *))add), _Z3addPiS_S_, (-1)); }
static void __sti____cudaRegisterAll(void) {  __cudaRegisterBinary(__nv_cudaEntityRegisterCallback);  }

#pragma GCC diagnostic pop
