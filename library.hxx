#pragma once
#include <vector>
#include <cassert>
#include "module.hxx"
#include "dev_ptr.hxx"
using std::vector;


namespace cuda::lib {
    
    enum class option {
        host_universal_function_and_data_table = 0,
        binary_is_preserved = 1,
        num_options,
    };

    struct option_list : ::option_list<lib::option,void*, option_list> {
//        host_universal_fuection_and_data_table(v) { add(option::host_universal_function_and_data_table, nullptr); }
    };
}

namespace cuda {

    struct library {
        CUlibrary data;
        ~library() { cuLibraryUnload(data); }
        library(const void* code, jit::option_list& jo, lib::option_list& lo) {
            result_t r = static_cast<result_t::e>(cuLibraryLoadData(&data, code, reinterpret_cast<CUjit_option*>(jo.options.data()), jo.values.data(), jo.size(), reinterpret_cast<CUlibraryOption*>(lo.options.data()), lo.values.data(), lo.size()));
            std::cout << "cuLibraryLoadData: " << r << std::endl;
        }
        library(const std::string fn, jit::option_list& jo, lib::option_list& lo) {
            result_t r = static_cast<result_t::e>(cuLibraryLoadFromFile(&data, fn.c_str(), reinterpret_cast<CUjit_option*>(jo.options.data()), jo.values.data(), jo.size(), reinterpret_cast<CUlibraryOption*>(lo.options.data()), lo.values.data(), lo.size()));
            std::cout << "cuLibraryLoadFromFile: " << r << std::endl;
        }

        void* get_unified_function(const std::string symbol) {
            void* f;
            result_t r = static_cast<result_t::e>(cuLibraryGetUnifiedFunction(&f, data, symbol.c_str()));
            std::cout << "cuLibraryGetUnifiedFunction: " << r << std::endl;
            return f; }
        modul get_module() {
            CUmodule m;
            result_t r = static_cast<result_t::e>(cuLibraryGetModule(&m, data));
            std::cout << "cuLibraryGetModule: " << r << std::endl;
            return m; }
        std::pair<dev_ptr,size_t> get_global(const std::string name) {
            CUdeviceptr p;
            size_t s;
            result_t r = static_cast<result_t::e>(cuLibraryGetGlobal(&p, &s, data, name.c_str()));
            std::cout << "cuLibraryGetGlobal: " << r << std::endl;
            return std::make_pair(p, s); }
        CUkernel get_kernel(const std::string name) {
            CUkernel k;
            result_t r = static_cast<result_t::e>(cuLibraryGetKernel(&k, data, name.c_str()));
            std::cout << "cuLibraryGetKernel: " << r << std::endl;
            return k; }
        std::pair<dev_ptr,size_t> get_managed(const std::string name) {
            CUdeviceptr p;
            size_t s;
            result_t r = static_cast<result_t::e>(cuLibraryGetManaged(&p, &s, data, name.c_str()));
            std::cout << "cuLibraryGetManagedSymbol: " << r << std::endl;
            return std::make_pair(p, s); }
        void* get_unified_function(const std::string symbol) const {
            void* f;
            result_t r = static_cast<result_t::e>(cuLibraryGetUnifiedFunction(&f, data, symbol.c_str()));
            std::cout << "cuLibraryGetUnifiedFunction: " << r << std::endl;
            return f; }

    };
}
