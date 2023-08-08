#pragma once
#include <vector>
#include <cassert>
#include "option_list.hxx"
using std::vector;

namespace cuda::jit {

        enum class option {
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
        struct option_list : ::option_list<jit::option,void*,option_list> {
            inline option_list& max_registers(unsigned int& v) { return add(jit::option::max_registers, (void*)&v); }
            inline option_list& threads_per_block(unsigned int& v) { return add(jit::option::threads_per_block, (void*)&v); }
            inline option_list& wall_time(float& v) { return add(jit::option::wall_time, (void*)&v); }
            inline option_list& info_log_buffer(char(&v)[]) { return add(jit::option::info_log_buffer, (void*)&v); }
            inline option_list& info_log_buffer_size_bytes(size_t& v) { return add(jit::option::info_log_buffer_size_bytes, (void*)&v); }

        };

}

