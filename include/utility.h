#ifndef __NOSTDLIB_UTILITY_H__
#define __NOSTDLIB_UTILITY_H__

#include "../bits/utils.h"
#include <type_traits.h>

namespace std
{
    template <typename T>
    constexpr add_rvalue_reference_t<T> declval() noexcept
    {
    }

    template <typename T>
    void swap(T& a, T& b) noexcept(/* see below */);

    template <typename T2, size_t N>
    void swap(T2 (&a)[N], T2 (&b)[N]) noexcept(/* see below */);

    template <typename T>
    constexpr T&& forward(remove_reference_t<T>& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(remove_reference_t<T>&& t) noexcept
    {
        static_assert(!is_lvalue_reference_v<T>, "Can not forward an rvalue as an lvalue.");
        return static_cast<T&&>(t);
    }

    template <typename T>
    remove_reference_t<T>&& move(T&& arg)
    {
        return static_cast<remove_reference_t<T>&&>(arg);
    }

    template <typename T, typename U = T>
    constexpr T exchange(T& obj, U&& new_value)
    {
        T old_value = move(obj);
        obj = forward<U>(new_value);
        return old_value;
    }
    template <typename T>
    constexpr conditional_t<!is_nothrow_move_constructible<T>::value && std::is_copy_constructible<T>::value, const T&, T&&>
    move_if_noexcept(T& x) noexcept
    {
        return static_cast<
            conditional_t<!is_nothrow_move_constructible<T>::value && std::is_copy_constructible<T>::value, const T&, T &&>>(
            x);
    };

} // namespace std

#endif
