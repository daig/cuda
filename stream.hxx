#pragma once
#include "context.hxx"
namespace cuda {
    struct stream {
        CUstream data;
        stream(CUstream s) : data(s) {}
        operator CUstream() const { return data; }
        stream() : data(0) {}
        ~stream() { if (data) {
            result_t r = static_cast<result_t::e>(cuStreamDestroy(data));
            std::cout << "cuStreamDestroy: " << r << std::endl; } }
        //the static CU_STREAM_LEGACY
        static stream inline legacy() {return CU_STREAM_LEGACY;}
        static stream inline per_thread() {return CU_STREAM_PER_THREAD;}

        struct priority {
            int data; priority(int p) : data(p) {} operator int() const { return data; }
            static std::pair<priority,priority> range() {
                int min, max;
                result_t r = static_cast<result_t::e>(cuCtxGetStreamPriorityRange(&min, &max));
                std::cout << "cuStreamGetPriorityRange: " << r << std::endl;
                return std::make_pair(min, max); }
        };
        enum class blocking_type : unsigned int {default_ = CU_STREAM_DEFAULT, non_blocking = CU_STREAM_NON_BLOCKING};
        explicit stream(blocking_type f) {
            result_t r = static_cast<result_t::e>(cuStreamCreate(&data, static_cast<unsigned int>(f)));
            std::cout << "cuStreamCreate: " << r << std::endl; }
        explicit stream(priority p, blocking_type f) {
            result_t r = static_cast<result_t::e>(cuStreamCreateWithPriority(&data, static_cast<unsigned int>(f), p));
            std::cout << "cuStreamCreateWithPriority: " << r << std::endl; }

        struct capture {
            using graph = CUgraph;
            enum class mode : unsigned int {global = CU_STREAM_CAPTURE_MODE_GLOBAL, thread = CU_STREAM_CAPTURE_MODE_THREAD_LOCAL, relaxed = CU_STREAM_CAPTURE_MODE_RELAXED};
            stream& s;
            graph end() {
                graph g;
                result_t r = static_cast<result_t::e>(cuStreamEndCapture(s.data, &g));
                std::cout << "cuStreamEndCapture: " << r << std::endl;
                return g; }
            enum class status : unsigned int {none = CU_STREAM_CAPTURE_STATUS_NONE, active = CU_STREAM_CAPTURE_STATUS_ACTIVE, invalidated = CU_STREAM_CAPTURE_STATUS_INVALIDATED};
            capture::status active() {
                CUstreamCaptureStatus status;
                result_t r = static_cast<result_t::e>(cuStreamIsCapturing(s.data, &status));
                std::cout << "cuStreamGetCaptureInfo: " << r << std::endl;
                return static_cast<capture::status>(status); }
        };
        capture capture(capture::mode m) {
            result_t r = static_cast<result_t::e>(cuStreamBeginCapture(data, static_cast<CUstreamCaptureMode>(m)));
            std::cout << "cuStreamBeginCapture: " << r << std::endl;
            return {*this}; }
        context_t context() {
            CUcontext c;
            result_t r = static_cast<result_t::e>(cuStreamGetCtx(data, &c));
            std::cout << "cuStreamGetCtx: " << r << std::endl;
            return {c}; }
        blocking_type flags() {
            unsigned int f;
            result_t r = static_cast<result_t::e>(cuStreamGetFlags(data, &f));
            std::cout << "cuStreamGetFlags: " << r << std::endl;
            return static_cast<blocking_type>(f); }
        unsigned long long id() {
            unsigned long long i;
            result_t r = static_cast<result_t::e>(cuStreamGetId(data, &i));
            std::cout << "cuStreamGetId: " << r << std::endl;
            return i; }
        priority priority() {
            int p;
            result_t r = static_cast<result_t::e>(cuStreamGetPriority(data, &p));
            std::cout << "cuStreamGetPriority: " << r << std::endl;
            return p; }
        bool ready() {
            result_t r = static_cast<result_t::e>(cuStreamQuery(data));
            std::cout << "cuStreamQuery: " << r << std::endl;
            return r == result_t::e::success; }

        /*
        enum class attribute { access_policy_window = 1, sync_policy = 3, attr_priority = 8, };
        void set_attr(CUstreamAttrID attr,const CUstreamAttrValue &value) {
            result_t r = static_cast<result_t::e>(cuStreamSetAttribute(data, attr, &value));
            std::cout << "cuStreamSetAttribute: " << r << std::endl; }
            */

    };
}// namespace cuda
