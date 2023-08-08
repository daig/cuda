#include <cuda.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "result.hxx"
// #include "context.hxx"
#include "version.hxx"
#include "device.hxx"
#include "context.hxx"
#include "module.hxx"
#include "library.hxx"
#include "memory.hxx"
#include "stream.hxx"

using std::cout, std::endl;
namespace cuda {
inline auto init() noexcept { return static_cast<result_t::e>(cuInit(0)); }
} // namespace cuda


void* readFile(const char* filename) {

    using std::cout, std::endl;
    std::ifstream file(filename, std::ios::ate);
    if (!file) { std::cerr << "Failed to open file: " << filename << endl; return nullptr;}
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size];
    if (!file.read(buffer, size)) { std::cerr << "Failed to read file: " << filename << endl; return nullptr;}

    file.close();
    return buffer;
}


int main(){

    cuda::init();
    std::cout << "CUDA Driver Version: " << cuda::driver_version() << std::endl;
    auto devices = cuda::device_t::all();
    auto dev = devices[0];
    cuda::context_t ctx(dev);

    const void* ptxCode = readFile("test.ptx");

  cuda::jit::option_list opts;
  using cujit = cuda::jit::option;
  size_t buffer_size = 1024; char buffer[buffer_size]{};
  opts.info_log_buffer_size_bytes(buffer_size)
      .info_log_buffer(buffer);
  cuda::modul m(ptxCode, opts);
  auto f = m.get_function("_Z3addPiS_S_");
  for (int i = 0; i < buffer_size; i++) { std::cout << buffer[i]; }
  cuda::result_t r = static_cast<cuda::result_t::e>(cuDeviceGetPCIBusId(buffer, buffer_size, dev.raw));
  cuda::device_t dev2(buffer);
  cout << "cuDeviceGetPCIBusId: " << r << endl;
  cout << buffer << endl;
  cout << (dev2 == dev) << endl;
}

