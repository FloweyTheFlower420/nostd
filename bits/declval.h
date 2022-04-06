#ifndef __NOSTDLIB_BITS_DECLVAL_H__
#define __NOSTDLIB_BITS_DECLVAL_H__
#include <type_traits>

namespace std
{
    template<typename T>
    std::add_rvalue_reference_t<T> declval() noexcept 
    {
    }
}

#endif
