#ifndef __NOSTDLIB_BITS_TYPE_TRAITS_PTR_H__
#define __NOSTDLIB_BITS_TYPE_TRAITS_PTR_H__

namespace std
{
    template <typename T>
    struct remove_pointer
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T*>
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T* const>
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T* volatile>
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T* const volatile>
    {
        using type = T;
    };
    template <typename T>
    using remove_pointer_t = typename remove_pointer<T>::type;

    template <typename T>
    struct type_identity
    {
        using type = T;
    };
    template <typename T>
    using type_identity_t = typename type_identity<T>::type;

    template <typename T>
    struct remove_reference
    {
        using type = T;
    };
    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };
    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };
    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    namespace detail
    {
        template <typename T>
        auto try_add_lvalue_reference(int) -> type_identity<T&>;
        template <typename T>
        auto try_add_lvalue_reference(...) -> type_identity<T>;
        template <typename T>
        auto try_add_rvalue_reference(int) -> type_identity<T&&>;
        template <typename T>
        auto try_add_rvalue_reference(...) -> type_identity<T>;
    } // namespace detail

    template <typename T>
    struct add_lvalue_reference : decltype(detail::try_add_lvalue_reference<T>(0))
    {
    };
    template <typename T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    template <typename T>
    struct add_rvalue_reference : decltype(detail::try_add_rvalue_reference<T>(0))
    {
    };
    template <typename T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    namespace detail
    {
        template <typename T>
        auto try_add_pointer(int) -> type_identity<typename remove_reference<T>::type*>;
        template <typename T>
        auto try_add_pointer(...) -> type_identity<T>;
    } // namespace detail

    template <typename T>
    struct add_pointer : decltype(detail::try_add_pointer<T>(0))
    {
    };
    template <typename T>
    using add_pointer_t = typename add_pointer<T>::type;

    // }}}
}

#endif
