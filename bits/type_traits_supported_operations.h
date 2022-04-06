#ifndef __NOSTDLIB_BITS_TYPE_TRAITS_CONSTRUCT_H__
#define __NOSTDLIB_BITS_TYPE_TRAITS_CONSTRUCT_H__
#include "declval.h"
#include "type_traits_cv.h"
#include "type_traits_ptr.h"
#include "type_traits_utils.h"
namespace std
{
    template <typename T, typename... Args>
#if(__has_builtin(__is_constructible))
    using is_constructible = bool_constant<__is_constructible(T, Args...)>;
#else
    static_assert(false, "compiler does not have builtins required for is_constructible");
#endif

    template <typename T, typename... Args>
#if(__has_builtin(__is_trivially_constructible))
    using is_trivially_constructible = bool_constant<is_constructible<T, Args...>::value && 
        __is_trivially_constructible(T, Args...)>;
#else
    static_assert(false, "compiler does not have builtins required for is_trivially_constructible");
#endif

    namespace detail
    {
        template<typename T, typename... Args>
        bool is_nothrow_constructible_helper()
        {
            if constexpr(is_constructible<T, Args...>::value)
                return nowthrow(T(declval<Args>()...));
            else
                return false;
        }
    }

    template <typename T, typename... Args>
    using is_nothrow_constructible = bool_constant<detail::is_nothrow_constructible_helper<T, Args...>()>;
    
    template <typename T, typename... Args>
    inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;
    template <typename T, typename... Args>
    inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;
    template <typename T, typename... Args>
    inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

    template <typename T>
    using is_default_constructible = is_constructible<T>;
    template <typename T>
    using is_trivially_default_constructible = is_trivially_constructible<T>;
    template <typename T>
    using is_nothrow_default_constructible = is_nothrow_constructible<T>;

    template <typename T>
    inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;
    template <typename T>
    inline constexpr bool is_trivially_default_constructible_v = is_trivially_constructible<T>::value;
    template <typename T>
    inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_constructible<T>::value;

    namespace detail
    {
        template <typename T>
        using make_cc_param = add_lvalue_reference_t<add_const<T>>;
        template <typename T>
        using make_mc_param = add_rvalue_reference<T>;
    } // namespace detail

    template <typename T>
    using is_copy_constructible = is_constructible<T, detail::make_cc_param<T>>;
    template <typename T>
    using is_trivially_copy_constructible = is_trivially_constructible<T, detail::make_cc_param<T>>;
    template <typename T>
    using is_nothrow_copy_constructible = is_nothrow_constructible<T, detail::make_cc_param<T>>;

    template <typename T>
    inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;
    template <typename T>
    inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;
    template <typename T>
    inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

    template <typename T>
    using is_move_constructible = is_constructible<T, detail::make_mc_param<T>>;
    template <typename T>
    using is_trivially_move_constructible = is_trivially_constructible<T, detail::make_mc_param<T>>;
    template <typename T>
    using is_nothrow_move_constructible = is_nothrow_constructible<T, detail::make_mc_param<T>>;

    template <typename T>
    inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;
    template <typename T>
    inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;
    template <typename T>
    inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

    template <typename T, typename U>
#if(__has_builtin(__is_assignable))
    using is_assignable = bool_constant<__is_assignable(T, U)>;
#else
    static_assert(false, "compiler does not have builtins required for is_assignable");
#endif

    template <typename T, typename U>
#if(__has_builtin(__is_trivially_assignable))
    using is_trivially_assignable = bool_constant<is_assignable<T, U>::value && __is_trivially_assignable(T, U)>;
#else
    static_assert(false, "compiler does not have builtins required for is_trivially_assignable");
#endif

    namespace detail
    {
        template<typename T, typename U>
        bool is_nothrow_assignable_helper()
        {
            if constexpr(is_assignable<T, U>::value)
                return nowthrow(declval<T>() = declval<U>());
            else
                return false;
        }
    }

    template <typename T, typename U>
    using is_nothrow_assignable = bool_constant<detail::is_nothrow_assignable_helper<T, U>()>;

    template <typename T, typename U>
    inline constexpr bool is_assignable_v = is_assignable<T, U>::value;
    template <typename T, typename U>
    inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;
    template <typename T, typename U>
    inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

    template <typename T>
    using is_copy_assignable = is_assignable<add_lvalue_reference_t<T>, detail::make_cc_param<T>>;
    template <typename T>
    using is_trivially_copy_assignable = is_trivially_assignable<add_lvalue_reference_t<T>, detail::make_cc_param<T>>;
    template <typename T>
    using is_nothrow_copy_assignable = is_nothrow_assignable<add_lvalue_reference_t<T>, detail::make_cc_param<T>>;

    template <typename T>
    inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;
    template <typename T>
    inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;
    template <typename T>
    inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

    template <typename T>
    using is_move_assignable = is_assignable<add_lvalue_reference_t<T>, detail::make_mc_param<T>>;
    template <typename T>
    using is_trivially_move_assignable = is_trivially_assignable<add_lvalue_reference_t<T>, detail::make_mc_param<T>>;
    template <typename T>
    using is_nothrow_move_assignable = is_nothrow_assignable<add_lvalue_reference_t<T>, detail::make_mc_param<T>>;

    template <typename T>
    inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;
    template <typename T>
    inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;
    template <typename T>
    inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

    namespace detail
    {
        template <typename T>
        struct is_destructible_helper
        {
            template <typename T1, typename = decltype(declval<T1&>().~T1())>
            static true_type test(int);

            template <typename>
            static false_type test(...);

            using type = decltype(test<T>(0));
        };

        template <typename T>
        constexpr bool is_nothrow_destructible_helper()
        {
            if constexpr (is_destructible_helper<T>::type::value)
                return noexcept(declval<T&>().~T1());
            else
                return false;
        }
    } // namespace detail

    template <typename T>
    using is_destructible = typename detail::is_destructible_helper<T>::type;

    template <typename T>
#if(__has_builtin(__is_trivially_destructible))
    using is_trivially_destructible = bool_constant<is_destructible<T>::value && __is_trivially_destructible(T)>;
#elif(__has_builtin(__has_trivial_destructor))
    using is_trivially_destructible = bool_constant<is_destructible<T>::value && __has_trivial_destructor(T)>;
#else
    static_assert(false, "compiler does not have builtins required for is_trivially_destructible");
#endif

    template <typename T>
    using is_nothrow_destructible = bool_constant<detail::is_nothrow_destructible_helper<T>()>;

    template <typename T>
    inline constexpr bool is_destructible_v = is_move_assignable<T>::value;
    template <typename T>
    inline constexpr bool is_trivially_destructible_v = is_trivially_move_assignable<T>::value;
    template <typename T>
    inline constexpr bool is_nothrow_destructible_v = is_nothrow_move_assignable<T>::value;

    template <typename T>
#if(__has_builtin(__has_trivial_destructor))
    using has_virtual_destructor = bool_constant<__has_virtual_destructor(T)>;
#else
    static_assert(false, "compiler does not have builtins required for has_virtual_destructor");
#endif
    template <typename T>
    inline constexpr bool has_virtual_destructor_v = has_virtual_destructor<T>::value;
} // namespace std
#endif
