#ifndef __NOSTDLIB_USER_IMPLEMENT_H__
#define __NOSTDLIB_USER_IMPLEMENT_H__
#include <cstddef.h>
namespace std
{
    namespace detail
    {
        void* malloc(size_t size);
        void* aligned_malloc(size_t size, max_align_t align);
        void free();
        void putc(char ch);

    } // namespace detail
} // namespace std

#endif
