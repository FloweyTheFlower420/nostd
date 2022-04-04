#ifndef __NOSTDLIB_BITS_TYPE_TRAITS_SIMPLE_CATAGORIES_H__
#define __NOSTDLIB_BITS_TYPE_TRAITS_SIMPLE_CATAGORIES_H__
#include "type_traits_utils.h"
#include <cstddef>

namespace std
{
    template <typename T>
    struct is_array : false_type
    {
    };
    template <typename T>
    struct is_array<T[]> : true_type
    {
    };
    template <typename T, size_t N>
    struct is_array<T[N]> : true_type
    {
    };
    template <typename T>
    inline constexpr bool is_array_v = is_array<T>::value;

    template <typename T>
    using is_enum = bool_constant<__is_enum(T)>;
    template <typename T>
    inline constexpr bool is_enum_v = is_enum<T>::value;

    template <typename T>
    using is_union = bool_constant<__is_union(T)>;
    template <typename T>
    inline constexpr bool is_union_v = is_union<T>::value;

    namespace detail
    {
        template <typename T>
        integral_constant<bool, !is_union<T>::value> test(int T::*);
        template <typename T>
        false_type test(...);
    } // namespace detail

    template <typename T>
    using is_class = decltype(detail::test<T>(nullptr));
    template <typename T>
    inline constexpr bool is_class_v = is_class<T>::value;

    template <typename T>
    struct is_const;
    template <typename T>
    struct is_function : public negation<is_const<const T>>
    {
    };
    template <typename T>
    struct is_function<T&> : public false_type
    {
    };
    template <typename T>
    struct is_function<T&&> : public false_type
    {
    };
    template <typename T>
    inline constexpr bool is_function_v = is_function<T>::value;

    template <typename T>
    struct is_pointer : false_type
    {
    };
    template <typename T>
    struct is_pointer<T*> : true_type
    {
    };
    template <typename T>
    struct is_pointer<T* const> : true_type
    {
    };
    template <typename T>
    struct is_pointer<T* volatile> : true_type
    {
    };
    template <typename T>
    struct is_pointer<T* const volatile> : true_type
    {
    };
    template <typename T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;

    template <typename T>
    struct is_lvalue_reference : false_type
    {
    };
    template <typename T>
    struct is_lvalue_reference<T&> : true_type
    {
    };
    template <typename T>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    template <typename T>
    struct is_rvalue_reference : false_type
    {
    };
    template <typename T>
    struct is_rvalue_reference<T&&> : true_type
    {
    };
    template <typename T>
    inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

} // namespace std

#endif
