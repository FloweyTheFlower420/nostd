#ifndef __NOSTDLIB_BITS_DECLVAL_H__
#define __NOSTDLIB_BITS_DECLVAL_H__

namespace std
{
    template <typename T>
    T&& declval() noexcept { __builtin_unreachable(); }
} // namespace std

#endif
