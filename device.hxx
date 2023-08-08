#pragma once
#include <cassert>
//#include "device_attribute.hxx"

namespace cuda {

struct device_t {
    CUdevice raw;
    device_t(size_t ordinal) { cuDeviceGet(&raw, ordinal); }
    device_t(std::string pciBusId) { cuDeviceGetByPCIBusId(&raw, pciBusId.c_str()); }
    static inline size_t count() { int count; cuDeviceGetCount(&count); return count; }
    static inline vector<device_t> all() {
        size_t n = count();
        vector<device_t> result; result.reserve(n);
        for (size_t i = 0; i < n; ++i) { result.emplace_back(i); }
        return result;
    }
    bool operator==(const device_t& other) const { return raw == other.raw; }
                                                                                     

    std::string pci_bus_id() const {
        char r[16];
        cuDeviceGetPCIBusId(r, 16, raw);
        return std::string(r); }

    using uuid_t = CUuuid;
    enum class attribute {
        max_threads_per_block = 1,
        max_block_dim_x = 2,
        max_block_dim_y = 3,
        max_block_dim_z = 4,
        max_grid_dim_x = 5,
        max_grid_dim_y = 6,
        max_grid_dim_z = 7,
        max_shared_memory_per_block = 8,
        total_constant_memory = 9,
        warp_size = 10,
        max_pitch = 11,
        max_registers_per_block = 12,
        clock_rate = 13,
        texture_alignment = 14,
        gpu_overlap = 15,
        multi_processor_count = 16,
        kernel_exec_timeout = 17,
        integrated = 18,
        can_map_host_memory = 19,
        compute_mode = 20,
        max_texture1d_width = 21,
        max_texture2d_width = 22,
        max_texture2d_height = 23,
        max_texture3d_width = 24,
        max_texture3d_height = 25,
        max_texture3d_depth = 26,
        max_texture2d_layered_width = 27,
        max_texture2d_layered_height = 28,
        max_texture2d_layered_layers = 29,
        surface_alignment = 30,
        concurrent_kernels = 31,
        ecc_enabled = 32,
        pci_bus_id = 33,
        pci_device_id = 34,
        tcc_driver = 35,
        memory_clock_rate = 36,
        global_memory_bus_width = 37,
        l2_cache_size = 38,
        max_threads_per_multiprocessor = 39,
        async_engine_count = 40,
        unified_addressing = 41,
        max_texture1d_layered_width = 42,
        max_texture1d_layered_layers = 43,
        max_texture2d_gather_width = 45,
        max_texture2d_gather_height = 46,
        max_texture3d_width_alternate = 47,
        max_texture3d_height_alternate = 48,
        max_texture3d_depth_alternate = 49,
        pci_domain_id = 50,
        texture_pitch_alignment = 51,
        max_texturecubemap_width = 52,
        max_texturecubemap_layered_width = 53,
        max_texturecubemap_layered_layers = 54,
        max_surface1d_width = 55,
        max_surface2d_width = 56,
        max_surface2d_height = 57,
        max_surface3d_width = 58,
        max_surface3d_height = 59,
        max_surface3d_depth = 60,
        max_surface1d_layered_width = 61,
        max_surface1d_layered_layers = 62,
        max_surface2d_layered_width = 63,
        max_surface2d_layered_height = 64,
        max_surface2d_layered_layers = 65,
        max_surfacecubemap_width = 66,
        max_surfacecubemap_layered_width = 67,
        max_surfacecubemap_layered_layers = 68,
        max_texture2d_linear_width = 70,
        max_texture2d_linear_height = 71,
        max_texture2d_linear_pitch = 72,
        max_texture2d_mipmapped_width = 73,
        max_texture2d_mipmapped_height = 74,
        compute_capability_major = 75,
        compute_capability_minor = 76,
        max_texture1d_mipmapped_width = 77,
        stream_priorities_supported = 78,
        global_l1_cache_supported = 79,
        local_l1_cache_supported = 80,
        max_shared_memory_per_multiprocessor = 81,
        max_registers_per_multiprocessor = 82,
        managed_memory = 83,
        is_multi_gpu_board = 84,
        multi_gpu_board_group_id = 85,
        host_native_atomic_supported = 86,
        single_to_double_precision_perf_ratio = 87,
        pageable_memory_access = 88,
        concurrent_managed_access = 89,
        compute_preemption_supported = 90,
        can_use_host_pointer_for_registered_mem = 91,
        max_shared_memory_per_block_optin = 97,
        can_flush_remote_writes = 98,
        host_register_supported = 99,
        pageable_memory_access_uses_host_page_tables = 100,
        direct_managed_mem_access_from_host = 101,
        virtual_address_management_supported = 102,
        handle_type_posix_file_descriptor_supported = 103,
        handle_type_win32_handle_supported = 104,
        handle_type_win32_kmt_handle_supported = 105,
        max_blocks_per_multiprocessor = 106,
        generic_compression_supported = 107,
        max_persisting_l2_cache_size = 108,
        max_access_policy_window_size = 109,
        gpu_direct_rdma_with_cuda_vmm_supported = 110,
        reserved_shared_memory_per_block = 111,
        sparse_cudarray_supported = 112,
        host_register_read_only_supported = 113,
        timeline_semaphore_interop_supported = 114,
        memory_pools_supported = 115,
        gpudirect_rdma_supported = 116,
        gpudirect_rdma_flush_writes_options = 117,
        gpudirect_rdma_writes_ordering = 118,
        mempool_supported_handle_types = 119,
        cluster_launch = 120,
        deferred_mapping_cudarray_supported = 121,
        can_use_64bitstream_memops = 122,
        can_use_stream_wait_value_nor = 123,
        dma_buf_supported = 124,
        icp_event_supported = 125,
        mem_sync_domain_count = 126,
        tensor_map_access_supported = 127,
        unified_function_pointers = 129,
        multicast_supported = 132 };

        inline int get(attribute a) {
            int v;
            cuDeviceGetAttribute(&v, static_cast<CUdevice_attribute>(a), this->raw);
            return v; } // TODO: errors
        
        inline bool execAffinitySupport() {
            int r;
            cuDeviceGetExecAffinitySupport(&r, CU_EXEC_AFFINITY_TYPE_SM_COUNT, this->raw);
            return r; } // TODO: errors
        // bytes
        inline size_t totalMem() { size_t r; cuDeviceTotalMem(&r, this->raw); return r; } // TODO: errors
};
}// namespace cuda
