#ifndef __NOSTDLIB_UTILITY_H__
#define __NOSTDLIB_UTILITY_H__

#include "../bits/utils.h"
#include <type_traits.h>

namespace std {
namespace detail {
#if __has_builtin(__type_pack_element)
template <size_t N, typename... Ts> struct type_pack_element {
  using type = __type_pack_element<N, Ts...>;
};
#else
template <size_t N, typename... Ts>
requires(N < sizeof...(Ts)) struct type_pack_element;

template <size_t N, typename T, typename... Ts>
struct type_pack_element<N, T, Ts...> : type_pack_element<N - 1, Ts...> {};

template <typename T, typename... Ts> struct type_pack_element<0, T, Ts...> {
  using type = T;
};
#endif

} // namespace detail
template <typename T> constexpr add_rvalue_reference_t<T> declval() noexcept {}

template <typename T> void swap(T &a, T &b) noexcept(/* see below */);

template <typename T2, size_t N>
void swap(T2 (&a)[N], T2 (&b)[N]) noexcept(/* see below */);

template <typename T> constexpr T &&forward(remove_reference_t<T> &t) noexcept {
  return static_cast<T &&>(t);
}

template <typename T>
constexpr T &&forward(remove_reference_t<T> &&t) noexcept {
  static_assert(!is_lvalue_reference_v<T>,
                "Can not forward an rvalue as an lvalue.");
  return static_cast<T &&>(t);
}

template <typename T> remove_reference_t<T> &&move(T &&arg) {
  return static_cast<remove_reference_t<T> &&>(arg);
}

template <typename T, typename U = T>
constexpr T exchange(T &obj, U &&new_value) {
  T old_value = move(obj);
  obj = forward<U>(new_value);
  return old_value;
}
template <typename T>
constexpr conditional_t<!is_nothrow_move_constructible<T>::value &&
                            is_copy_constructible<T>::value,
                        const T &, T &&>
move_if_noexcept(T &x) noexcept {
  return static_cast<conditional_t<!is_nothrow_move_constructible<T>::value &&
                                       is_copy_constructible<T>::value,
                                   const T &, T &&>>(x);
};

struct in_place_t {
  explicit in_place_t() = default;
};
inline constexpr in_place_t in_place{};
template <class T> struct in_place_type_t {
  explicit in_place_type_t() = default;
};
template <class T> inline constexpr in_place_type_t<T> in_place_type{};
template <size_t I> struct in_place_index_t {
  explicit in_place_index_t() = default;
};
template <size_t I> inline constexpr in_place_index_t<I> in_place_index{};

} // namespace std

#endif
