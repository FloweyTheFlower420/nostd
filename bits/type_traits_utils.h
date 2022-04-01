#ifndef __NOSTDLIB_BITS_TYPE_TRAITS_UTILS_H__
#define __NOSTDLIB_BITS_TYPE_TRAITS_UTILS_H__

namespace std {
template <typename T, T v> struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant<T, v>;

  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};

template <bool B> using bool_constant = integral_constant<bool, B>;

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

// effectively preforms an AND operation on all T::value in Ts
template <typename... Ts> using conjunction = bool_constant<(Ts::value && ...)>;
template <typename... Ts>
inline constexpr bool conjunction_v = conjunction<Ts...>::value;

// effectively preforms an OR operation on all T::value in all Ts
template <typename... Ts> using disjunction = bool_constant<(Ts::value && ...)>;
template <typename... Ts>
inline constexpr bool disjunction_v = disjunction<Ts...>::value;

template <typename T> using negation = bool_constant<!bool(T::value)>;
template <typename T> inline constexpr bool negation_v = negation<T>::value;

template <template <typename, typename> typename P, typename T, typename... Ts>
struct matches_any : disjunction<P<T, Ts>...> {};

template <template <typename, typename> typename P, typename T, typename... Ts>
inline constexpr bool matches_any_v = matches_any<P, T, Ts...>::value;
} // namespace std

#endif
