#include <cuda.h>
#include <iostream>
#include <string>
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
    auto d = cuda::device_t::get(0);
    int x = d.get(cuda::device_t::attribute::max_threads_per_block);
    cout << "max threads per block: " << x << endl;

    cout << "affinity support: " << d.execAffinitySupport() << endl;
    cout << "total memory: " << d.totalMem() << endl;

    auto s = cuda::result::string(cuda::result_t::captured_event);
    cout << s << endl;
}

