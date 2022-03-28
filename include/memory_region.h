#ifndef __NOSTDLIB_MEMORY_REGION_H__
#define __NOSTDLIB_MEMORY_REGION_H__
#include "../bits/utils.h"
#include <cstddef.h>

namespace std
{
    // An non-owning memory region
    // Will not free the memory region after the object is destoryed
    class memory_region
    {
        size_t size;
        void* buffer;

    public:
        static constexpr size_t MAX_OP_SIZE = (size_t)-1;
        // construct with buffer
        constexpr memory_region(size_t n, void* buffer) : size(n), buffer(buffer)
        {
        }
        // default initalizer
        constexpr memory_region() : size(0), buffer(nullptr)
        {
        }
        // copy-constructing of memory-region is not allowed
        memory_region(const memory_region&) = delete;
        constexpr memory_region(memory_region&& rhs) : size(rhs.size), buffer(rhs.buffer)
        {
            rhs.size = 0;
        }

        // default delete
        ~memory_region() = default;

        // copies min(size, rhs.size) bytes from *this to rhs
        size_t copy(memory_region& rhs, size_t n = MAX_OP_SIZE)
        {
            memcpy(rhs.buffer, buffer, std::mix(rhs.size, size, n));
        }
        size_t set(uint8_t val, size_t n = MAX_OP_SIZE)
        { memset(buffer, val, std::min(size, n);
        }
        size_t clear()
        {
            return set(0);
        }
    };
} // namespace std

#endif
