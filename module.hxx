#pragma once
#include <vector>
#include <cassert>
using std::vector;

namespace cuda {
    struct modul {
        CUmodule raw;
        modul(const char* fn){ cuModuleLoad(&raw, fn); }
        modul(const void* image){ cuModuleLoadData(&raw, image); }

        enum class loading_mode { eager = 1, lazy = 2, };

        enum class jit_option {
            max_registers = 0,
            threads_per_block = 1,
            wall_time = 2,
            info_log_buffer = 3,
            info_log_buffer_size_bytes = 4,
            error_log_buffer = 5,
            error_log_buffer_size_bytes = 6,
            optimization_level = 7,
            target_from_current_context = 8,
            jit_target = 9,
            fallback_strategy = 10,
            generate_debug_info = 11,
            log_verbose = 12,
            generate_line_info = 13,
            cache_mode = 14,
            fast_compile = 16,
            global_symbol_names = 17,
            global_symbol_addresses = 18,
            global_symbol_count = 19,
            pic = 30,
        };
        struct option_list {
            vector<jit_option> options;
            vector<void*> values;
            option_list& add(jit_option o, void* v) {
                options.push_back(o);
                values.push_back(v);
                return *this; }
            size_t size() const { return options.size(); }

            inline option_list& max_registers(unsigned int& v) { return add(jit_option::max_registers, (void*)&v); }
            inline option_list& threads_per_block(unsigned int& v) { return add(jit_option::threads_per_block, (void*)&v); }
            inline option_list& wall_time(float& v) { return add(jit_option::wall_time, (void*)&v); }
            inline option_list& info_log_buffer(char(&v)[]) { return add(jit_option::info_log_buffer, (void*)&v); }
            inline option_list& info_log_buffer_size_bytes(size_t& v) { return add(jit_option::info_log_buffer_size_bytes, (void*)&v); }

        };
        modul(const void* image, option_list& o) {
            result_t r = static_cast<result_t>(cuModuleLoadDataEx(&raw, image, o.size(), reinterpret_cast<CUjit_option*>(o.options.data()), o.values.data())); 
            std::cout << "cuModuleLoadDataEx: " << cuda::result::string(r) << std::endl;
        }

        auto get_function(const char* name) {
            CUfunction f;
            result_t r = static_cast<result_t>(cuModuleGetFunction(&f, raw, name));
            std::cout << "cuModuleGetFunction: " << cuda::result::string(r) << std::endl;
            return f; }
    };
}
