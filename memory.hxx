#pragma once
#include <vector>

struct f16 {};

namespace cuda {
    enum class dim {_1 = 1,_2 = 2,_3 = 3};
    enum class channels {_1 = 1, _2 = 2, _4 = 4};
    enum class memory_type {host = 1, device = 2, array = 3, unified = 4};
    template<uint32_t W, uint32_t H, uint32_t D, class F, channels Channels, unsigned int Flags>
    struct array {
        CUarray data;
        ~array() { cuArrayDestroy(data); }
        constexpr uint32_t width() const { return W; }
        constexpr uint32_t height() const { return H; }
        constexpr uint32_t depth() const { return D; }
        constexpr CUarray_format format() const { 
            if constexpr (std::is_same_v<F,uint8_t>) return CU_AD_FORMAT_UNSIGNED_INT8;
            if constexpr (std::is_same_v<F,uint16_t>) return CU_AD_FORMAT_UNSIGNED_INT16;
            if constexpr (std::is_same_v<F,uint32_t>) return CU_AD_FORMAT_UNSIGNED_INT32;
            if constexpr (std::is_same_v<F,int8_t>) return CU_AD_FORMAT_SIGNED_INT8;
            if constexpr (std::is_same_v<F,int16_t>) return CU_AD_FORMAT_SIGNED_INT16;
            if constexpr (std::is_same_v<F,int32_t>) return CU_AD_FORMAT_SIGNED_INT32;
            if constexpr (std::is_same_v<F,f16>) return CU_AD_FORMAT_HALF;
            if constexpr (std::is_same_v<F,float>) return CU_AD_FORMAT_FLOAT; }
        constexpr channels channels() const { return Channels; }
        constexpr dim dimensions() const { 
            if ((!W) && (!H)) {return dim::_1;}
            if (W && H && (!D)) {return dim::_2;}
            if (W && H && D) {return dim::_3;} }
        array(CUarray a) : data(a) {}
        array() {
            if constexpr (D) {
            CUDA_ARRAY3D_DESCRIPTOR desc = {D,Flags,format(),H,(int)Channels,W};
            cuArray3DCreate(&data, &desc); 
            } else { static_assert(!Flags);
            CUDA_ARRAY_DESCRIPTOR desc = {format(),H,(int)Channels,W};
            cuArrayCreate(&data, &desc); }
        }
    };
}
