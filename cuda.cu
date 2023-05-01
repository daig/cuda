#include <cuda.h>
#include <iostream>
#include <string>
#include <vector>
#include "result.hxx"
// #include "context.hxx"
#include "version.hxx"
#include "device.hxx"
#include "context.hxx"
#include "module.hxx"

namespace cuda {
inline auto init() noexcept { return static_cast<result_t>(cuInit(0)); }
} // namespace cuda

using std::cout, std::endl;

int main(){

    cuda::init();
    cout << "CUDA Driver Version: " << cuda::driver_version() << endl;
    auto dev = cuda::device_t::get(0);
    cuda::context_t ctx(dev);

  const char* ptxCode =
    ".version 7.0\n"
    ".target sm_30\n"
    "\n"
    ".visible .func _Z6addIntii(\n"
    "  .param .s32 addend1,\n"
    "  .param .s32 addend2)\n"
    "{\n"
    "  .reg .s32 sum;\n"
    "  add.s32 sum, addend1, addend2;\n"
    "  ret.s32 sum;\n"
    "}\n";
  cuda::modul::option_list opts;
  using cujit = cuda::modul::jit_option;
  size_t buffer_size = 1024; char buffer[buffer_size];
  opts.info_log_buffer_size_bytes(buffer_size)
      .info_log_buffer(buffer);
  cuda::modul m(ptxCode, opts);
  auto f = m.get_function("addInt");

}

