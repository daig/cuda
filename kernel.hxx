#pragma once
#include <vector>
#include <cassert>
using std::vector;

namespace cuda {

    struct kernel {
        CUkernel data;
        kernel(CUmodule m, const std::string name) { cuModuleGetFunction(&data, m, name.c_str()); }
    };
}
