#ifndef __NOSTDLIB_BITS_TYPE_TRAITS_CV_H__
#define __NOSTDLIB_BITS_TYPE_TRAITS_CV_H__
#include "type_traits_utils.h"
#include <cstddef.h>

namespace std
{
    template <typename T>
    struct remove_cv
    {
        using type = T;
    };
    template <typename T>
    struct remove_cv<const T>
    {
        using type = T;
    };
    template <typename T>
    struct remove_cv<volatile T>
    {
        using type = T;
    };
    template <typename T>
    struct remove_cv<const volatile T>
    {
        using type = T;
    };
    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    template <typename T>
    struct remove_const
    {
        using type = T;
    };
    template <typename T>
    struct remove_const<const T>
    {
        using type = T;
    };
    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    template <typename T>
    struct remove_volatile
    {
        using type = T;
    };
    template <typename T>
    struct remove_volatile<volatile T>
    {
        using type = T;
    };
    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template <typename T>
    struct add_cv
    {
        using type = const volatile T;
    };
    template <typename T>
    struct add_const
    {
        using type = const T;
    };
    template <typename T>
    struct add_volatile
    {
        using type = volatile T;
    };
    template <typename T>
    using add_cv_t = typename add_cv<T>::type;
    template <typename T>
    using add_const_t = typename add_const<T>::type;
    template <typename T>
    using add_volatile_t = typename add_volatile<T>::type;

    template <typename, typename>
    struct is_same;
    template <typename T>
    using is_void = is_same<void, remove_cv_t<T>>;
    template <typename T>
    inline constexpr bool is_void_v = is_void<T>::value;

    template <typename T>
    struct is_null_pointer : is_same<nullptr_t, remove_cv_t<T>>
    {
    };
    template <typename T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    template <typename T>
    struct is_integral
        : matches_any<is_same, remove_cv_t<T>, bool, char, char32_t, wchar_t, short, int, long, long long, __int128>
    {
    };
    template <typename T>
    inline constexpr bool is_integral_v = is_integral<T>::value;

    template <typename T>
    struct is_floating_point : matches_any<is_same, remove_cv_t<T>, float, double, long double>
    {
    };
    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
} // namespace std

#endif
