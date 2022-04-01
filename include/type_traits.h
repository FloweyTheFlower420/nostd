// vim:foldmethod=marker
#ifndef __NOTSTDLIB_TYPE_TRAITS_H__
#define __NOTSTDLIB_TYPE_TRAITS_H__
#include "../bits/type_traits_cv.h"
#include "../bits/type_traits_ptr.h"
#include "../bits/type_traits_simple_catagories.h"
#include "../bits/type_traits_supported_operations.h"
#include "../bits/type_traits_utils.h"

#include <cstddef.h>

#include "utility.h"

namespace std {
// Type-Type relation {{{

template <typename T, typename U> struct is_same : false_type {};
template <typename T> struct is_same<T, T> : true_type {};
template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

namespace detail {
template <typename B> true_type test_pre_ptr_convertible(const volatile B *);
template <typename> false_type test_pre_ptr_convertible(const volatile void *);

template <typename, typename> auto test_pre_is_base_of(...) -> true_type;
template <typename B, typename D>
auto test_pre_is_base_of(int)
    -> decltype(test_pre_ptr_convertible<B>(static_cast<D *>(nullptr)));
} // namespace detail

template <typename B, typename D>
struct is_base_of
    : bool_constant<is_class<B>::value &&
                    is_class<D>::value &&decltype(detail::test_pre_is_base_of<
                                                  B, D>(0))::value> {};

namespace detail {
template <typename T>
auto test_returnable(int)
    -> decltype(void(static_cast<T (*)()>(nullptr)), true_type{});
template <typename> auto test_returnable(...) -> false_type;

template <typename F, typename T>
auto test_implicitly_convertible(int)
    -> decltype(void(declval<void (&)(T)>()(declval<F>())), true_type{});

template <typename, typename>
auto test_implicitly_convertible(...) -> false_type;
} // namespace detail

template <typename F, typename T>
struct is_convertible
    : bool_constant<(decltype(detail::test_returnable<T>(0))::value &&
                     decltype(detail::test_implicitly_convertible<F, T>(
                         0))::value) ||
                    (is_void<F>::value && is_void<T>::value)> {};

template <typename F, typename T>
inline constexpr bool is_convertible_v = is_convertible<F, T>::value;
// }}}

// Composite type catagories {{{

namespace detail {
template <typename T> struct is_member_pointer_helper : false_type {};
template <typename T, class U>
struct is_member_pointer_helper<T U::*> : true_type {};
} // namespace detail
template <typename T>
struct is_member_pointer : detail::is_member_pointer_helper<remove_cv_t<T>> {};
template <typename T>
inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template <typename T>
struct is_arithmetic : disjunction<is_integral<T>, is_floating_point<T>> {};
template <typename T>
inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

template <typename T>
struct is_fundamental
    : disjunction<is_arithmetic<T>, is_void<T>, is_null_pointer<T>> {};
template <typename T>
inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

template <typename T>
struct is_scalar : disjunction<is_arithmetic<T>, is_enum<T>,
                               is_member_pointer<T>, is_null_pointer<T>> {};
template <typename T> inline constexpr bool is_scalar_v = is_scalar<T>::value;

template <typename T>
struct is_object
    : disjunction<is_scalar<T>, is_array<T>, is_union<T>, is_class<T>> {};
template <typename T> inline constexpr bool is_object_v = is_object<T>::value;

template <typename T> struct is_compound : disjunction<is_fundamental<T>> {};
template <typename T>
inline constexpr bool is_compound_v = is_compound<T>::value;

template <typename T>
struct is_reference
    : disjunction<is_lvalue_reference<T>, is_rvalue_reference<T>> {};
template <typename T>
inline constexpr bool is_reference_v = is_reference<T>::value;

// }}}

// Supported operations {{{

// }}}

// Signs {{{

namespace detail {
template <typename T> struct make_signed { using type = T; };
template <> struct make_signed<char> { using type = signed char; };
template <> struct make_signed<unsigned char> { using type = signed char; };
template <> struct make_signed<unsigned short> { using type = signed short; };
template <> struct make_signed<unsigned int> { using type = signed int; };
template <> struct make_signed<unsigned long> { using type = signed long; };
template <> struct make_signed<unsigned long long> {
  using type = signed long long;
};

template <typename T, bool _IsInt = is_integral<T>::value,
          bool _IsEnum = is_enum<T>::value>
class __make_signed_selector;

template <typename T> class __make_signed_selector<T, true, false> {
  using __signed_type = typename make_signed<remove_cv_t<T>>::__type;

public:
  using __type = typename __match_cv_qualifiers<_Tp, __signed_type>::__type;
};

template <typename T> class __make_signed_selector<T, false, true> {
  typedef typename __make_unsigned_selector<T>::__type __unsigned_type;

public:
  typedef typename __make_signed_selector<__unsigned_type>::__type __type;
};
} // namespace detail

// }}}

// Remove extent {{{

template <typename T> struct remove_extent { using type = T; };
template <typename T> struct remove_extent<T[]> { using type = T; };
template <typename T, size_t N> struct remove_extent<T[N]> { using type = T; };
template <typename T> using remove_extent_t = typename remove_extent<T>::type;

template <typename T> struct remove_all_extents { using type = T; };
template <typename T> struct remove_all_extents<T[]> {
  using type = typename remove_all_extents<T>::type;
};
template <typename T, size_t N> struct remove_all_extents<T[N]> {
  using type = typename remove_all_extents<T>::type;
};
template <typename T>
using remove_all_extents_t = typename remove_all_extents<T>::type;

// }}}

// Misc transforms {{{

template <bool B, typename T, typename F> struct conditional {
  using type = T;
};
template <typename T, typename F> struct conditional<false, T, F> {
  using type = F;
};
template <bool B, typename T, typename F>
using conditional_t = typename conditional<B, T, F>::type;

template <typename T> struct decay {
private:
  using U = remove_reference_t<T>;

public:
  using type = conditional_t<
      is_array_v<U>, remove_extent_t<U> *,
      conditional_t<is_function_v<T>, add_pointer_t<U>, remove_cv_t<U>>>;
};
template <typename T> using decay_t = typename decay<T>::type;

template <typename T> struct remove_cvref {
  using type = remove_cv_t<remove_reference_t<T>>;
};
template <typename T> using remove_cvref_t = typename remove_cvref<T>::type;

template <bool B, typename T = void> struct enable_if {};
template <class T> struct enable_if<true, T> { using type = T; };
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <typename...> using void_t = void;

// }}}

// Type properties {{{

// is_const<T>
template <typename T> struct is_const : false_type {};
template <typename T> struct is_const<const T> : true_type {};

// is_volatile<T>
template <typename T> struct is_volatile : false_type {};
template <typename T> struct is_volatile<volatile T> : true_type {};

// is_trivially_copyable<T>
template <typename T>
struct is_trivially_copyable : bool_constant<__is_trivially_copyable(T)> {};
template <typename T>
inline constexpr bool is_trivially_copyable_v = is_trivially_copyable<T>::value;

// is_standard_layout<T>
template <typename T>
struct is_standard_layout : bool_constant<__is_standard_layout(T)> {};
template <typename T>
inline constexpr bool is_standard_layout_v = is_standard_layout<T>::value;

// is_trivial<T>
template <typename T>
struct is_trivial : conjunction<is_trivially_copyable<T> &&
                                is_trivially_default_constructible<T>> {};
template <typename T> inline constexpr bool is_trivial_v = is_trivial<T>::value;

// has_unique_object_representations<T>
template <typename _Tp>
struct has_unique_object_representations
    : bool_constant<__has_unique_object_representations(
          remove_cv_t<remove_all_extents_t<_Tp>>)> {};
template <typename T>
inline constexpr bool has_unique_object_representations_t =
    has_unique_object_representations<T>::value;

// is_empty<T>
template <typename T> struct is_empty : bool_constant<__is_empty(T)> {};
template <typename T> inline constexpr bool is_empty_v = is_empty<T>::value;

// is_polymorphic<T>
namespace detail {
template <typename T>
true_type detect_is_polymorphic(
    decltype(dynamic_cast<const volatile void *>(static_cast<T *>(nullptr))));
template <typename T> false_type detect_is_polymorphic(...);

} // namespace detail
template <typename T>
struct is_polymorphic : decltype(detail::detect_is_polymorphic<T>(nullptr)) {};
template <typename T>
inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

// is_abstract<T>
template <typename T> struct is_abstract : bool_constant<__is_abstract(T)> {};
template <typename T>
inline constexpr bool is_abstract_v = is_abstract<T>::value;

// is_final<T>
template <typename T> struct is_final : bool_constant<__is_final(T)> {};
template <typename T> inline constexpr bool is_final_v = is_final<T>::value;

// is_abstract<T>
template <typename T>
struct is_aggregate : public bool_constant<__is_aggregate(T)> {};

// is_signed<T>
namespace detail {
template <typename T, bool = is_arithmetic<T>::value>
struct is_signed : bool_constant<T(-1) < T(0)> {};
template <typename T> struct is_signed<T, false> : false_type {};
} // namespace detail

template <typename T> struct is_signed : detail::is_signed<T>::type {};

// is_unsigned<T>
namespace detail {
template <typename T, bool = is_arithmetic<T>::value>
struct is_unsigned : bool_constant<T(0) < T(-1)> {};
template <typename T> struct is_unsigned<T, false> : false_type {};
} // namespace detail

template <typename T> struct is_unsigned : detail::is_unsigned<T>::type {};

// is_bounded_array<T>
template <typename T> struct is_bounded_array : false_type {};
template <typename T, size_t N> struct is_bounded_array<T[N]> : true_type {};
template <typename T>
inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

// is_unbounded_array<T>
template <typename T> struct is_unbounded_array : false_type {};
template <typename T> struct is_unbounded_array<T[]> : true_type {};
template <typename T>
inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

// }}}

template <size_t length, size_t align = 4> struct aligned_storage {
  struct type {
    alignas(align) unsigned char data[length];
  };
};
} // namespace std
#endif
