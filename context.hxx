#pragma once
#include "result.hxx"
#include "device.hxx"
namespace cuda {
    struct context_t {

        CUcontext raw;
        context_t() noexcept : raw(nullptr) {}
        context_t(CUcontext raw) noexcept : raw(raw) {}
        ~context_t() noexcept { if (raw) cuCtxDestroy(raw); }

        union flags {
            struct bits {
                enum class sched : unsigned int {
                    aut           = 0b000,
                    spin          = 0b001,
                    yield         = 0b010,
                    blocking_sync = 0b100, };
                sched sched : 3;
                bool lmem_resize_to_max : 1;
                bool coredump_enable : 1;
                bool user_coredump_enable : 1;
                bool sync_memops : 1; };
            unsigned int raw;
            bits bits; };
        explicit context_t(device_t d, flags f = {0}) noexcept {
            cuCtxCreate(&raw, f.raw, d.raw); }
        context_t(const context_t&) = delete;
        context_t(context_t&& other) = delete;
        context_t& operator=(const context_t&) = delete;
        context_t& operator=(context_t&& other) = delete;

        // create_v3

        operator bool() const noexcept { return raw; }

        struct api_version_t { unsigned int raw; api_version_t(unsigned int raw) : raw(raw) {} };
        api_version_t api_version() const noexcept { unsigned int r; cuCtxGetApiVersion(raw, &r); return r; }
        struct id_t { unsigned long long raw; id_t(unsigned long long raw) : raw(raw) {} };
        id_t id() const noexcept { unsigned long long r; cuCtxGetId(raw,&r); return r; }
};

} // namespace cuda
