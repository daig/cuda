#pragma once
#include <vector>
#include <cassert>
#include "jit_options.hxx"
#include "dev_ptr.hxx"
using std::vector;


namespace cuda {
    struct modul {
        CUmodule data;
        modul(CUmodule m) : data(m) {}
        modul(const std::string fn){ cuModuleLoad(&data, fn.c_str() ); }
        modul(const vector<unsigned char> image){ cuModuleLoadData(&data, image.data()); }
        ~modul(){ cuModuleUnload(data); }

        enum class loading_mode { eager = 1, lazy = 2, };

        modul(const void* image, jit::option_list& o) {
            result_t r = static_cast<result_t::e>(cuModuleLoadDataEx(&data, image, o.size(), reinterpret_cast<CUjit_option*>(o.options.data()), o.values.data())); 
            std::cout << "cuModuleLoadDataEx: " << r << std::endl;
        }

        CUfunction get_function(const std::string name) {
            CUfunction f;
            result_t r = static_cast<result_t::e>(cuModuleGetFunction(&f, data, name.c_str()));
            std::cout << "cuModuleGetFunction: " << r << std::endl;
            return f; }
        std::pair<dev_ptr,size_t> get_global(const std::string name) {
            CUdeviceptr p; size_t s;
            result_t r = static_cast<result_t::e>(cuModuleGetGlobal(&p, &s, data, name.c_str()));
            std::cout << "cuModuleGetGlobal: " << r << std::endl;
            return std::make_pair(p, s); }
        auto get_loading_mode() {
            CUmoduleLoadingMode m;
            result_t r = static_cast<result_t::e>(cuModuleGetLoadingMode(&m));
            std::cout << "cuModuleGetGlobal: " << r << std::endl;
            return static_cast<loading_mode>(m); }

    enum class jit_input { cubin = 0, ptx = 1, fatbinary = 2, object = 3, library = 4, nvvm = 5, };

    struct link_state {
        CUlinkState data;
        link_state() { cuLinkCreate(0, nullptr, nullptr, &data); }
        link_state(jit::option_list& o) {
            result_t r = static_cast<result_t::e>(cuLinkCreate(o.size(), reinterpret_cast<CUjit_option*>(o.options.data()), o.values.data(), &data));
            std::cout << "cuLinkCreate: " << r << std::endl;
        }
        ~link_state() { cuLinkDestroy(data); }
        void add_ptx(const std::string ptx, jit::option_list& o, const char* name = nullptr) {
            result_t r = static_cast<result_t::e>
                (cuLinkAddData(data
                              ,CUjitInputType::CU_JIT_INPUT_PTX
                              ,const_cast<void*>(static_cast<const void*>(ptx.c_str()))
                              ,ptx.size()
                              ,name
                              ,o.size()
                              ,reinterpret_cast<CUjit_option*>(o.options.data())
                              ,o.values.data()));
            std::cout << "cuLinkAddData: " << r << std::endl;
        }
        void add_file(const char* fn, jit_input t, jit::option_list& o) {
            result_t r = static_cast<result_t::e>
                (cuLinkAddFile(data
                              ,static_cast<CUjitInputType>(t)
                              ,fn
                              ,o.size()
                              ,reinterpret_cast<CUjit_option*>(o.options.data())
                              ,o.values.data())
                );
            std::cout << "cuLinkAddFile: " << r << std::endl;
        }
        vector<unsigned char> complete() {
            size_t s;
            void* cubin;
            result_t r = static_cast<result_t::e>(cuLinkComplete(data, &cubin, &s));
            std::cout << "cuLinkComplete: " << r << std::endl;
            return vector<unsigned char>(static_cast<unsigned char*>(cubin), static_cast<unsigned char*>(cubin) + s); }
        };

    };
}
