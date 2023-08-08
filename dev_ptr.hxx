#pragma once
namespace cuda { 
    struct dev_ptr {
        CUdeviceptr data;
        dev_ptr(CUdeviceptr d) : data(d) {}
        operator CUdeviceptr() const { return data; }
    }; 

    struct dev_array_base {
        CUdeviceptr data;
        size_t size;
        operator CUdeviceptr() const { return data; }
        operator bool() const { return size; }
        protected:
            dev_array_base(CUdeviceptr d, size_t s) : data(d), size(s) {}
            dev_array_base() : data(0), size(0) {}
            ~dev_array_base() { if (size)
                { result_t r = static_cast<result_t::e>(cuMemFree(data));
                  std::cout << "cuMemFree: " << r << std::endl; } }
    };
    struct dev_array_linear : dev_array_base {
        dev_array_linear(size_t s) : dev_array_base(0, s) {
            result_t r = static_cast<result_t::e>(cuMemAlloc(&data, size));
            std::cout << "cuMemAlloc: " << r << std::endl;
        }
    };
    struct dev_array_pitched : dev_array_base {
        enum class elt_size : size_t {_4 = 4, _8 = 8, _16 = 16};
        size_t pitch;
        dev_array_pitched(size_t width_bytes, size_t height_rows, elt_size elt_size_bytes) {
            result_t r = static_cast<result_t::e>(cuMemAllocPitch(&data, &pitch, width_bytes, height_rows, static_cast<unsigned int>(elt_size_bytes)));
            std::cout << "cuMemAllocPitch: " << r << std::endl;
            size = pitch * height_rows;
        }

    };
    struct dev_array_managed : dev_array_base {
        struct supported_t {
            bool engaged;
            bool is_supported;
            bool operator()() {
                if (!engaged) {
                    int v;
                    result_t r = static_cast<result_t::e>(cuDeviceGetAttribute(&v, CU_DEVICE_ATTRIBUTE_MANAGED_MEMORY, 0));
                    std::cout << "cuDeviceGetAttribute: " << r << std::endl;
                    is_supported = v;
                    engaged = true;}
                return is_supported; }
        };
        static supported_t supported;
        enum class attach_t : unsigned int {global = CU_MEM_ATTACH_GLOBAL, host = CU_MEM_ATTACH_HOST, single = CU_MEM_ATTACH_SINGLE};
        attach_t attach_type;
        dev_array_managed(size_t s, attach_t flags = attach_t::global) : dev_array_base(0, s), attach_type(flags) {
            assert(supported());
            result_t r = static_cast<result_t::e>(cuMemAllocManaged(&data, size, static_cast<unsigned int>(flags)));
            std::cout << "cuMemAllocManaged: " << r << std::endl;
        }
    };
}
