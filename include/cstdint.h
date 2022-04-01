#ifndef __NOSTDLIB_INCLUDE_CSTDINT_H__
#define __NOSTDLIB_INCLUDE_CSTDINT_H__

namespace std {
namespace detail {
template <int size, typename T, typename... Ts> auto select_integral() {
  if constexpr (sizeof(T) == size)
    return T{};
  else {
    static_assert(sizeof...(Ts) != 0, "Unable to find proper type for stdint");
    return select_integral<size, Ts...>();
  }
}

template <int size>
using integral_select_t =
    decltype(select_integral<size, char, short, int, long, long long>());

template <int size>
using uintegral_select_t =
    decltype(select_integral<size, unsigned char, unsigned short, unsigned int,
                             unsigned long, unsigned long long>());

template <int size>
inline constexpr __INTMAX_TYPE__ intmax_of = (1ULL << (size * 8 - 1)) - 1;
} // namespace detail
} // namespace std

using int8_t = std::detail::integral_select_t<1>;
using uint8_t = std::detail::uintegral_select_t<1>;
using int16_t = std::detail::integral_select_t<2>;
using uint16_t = std::detail::uintegral_select_t<2>;
using int32_t = std::detail::integral_select_t<4>;
using uint32_t = std::detail::uintegral_select_t<4>;
using int64_t = std::detail::integral_select_t<8>;
using uint64_t = std::detail::uintegral_select_t<8>;

using int_least8_t = int8_t;
using uint_least8_t = uint8_t;
using int_least16_t = int16_t;
using uint_least16_t = uint16_t;
using int_least32_t = int32_t;
using uint_least32_t = uint32_t;
using int_least64_t = int64_t;
using uint_least64_t = uint64_t;
using intmax_t = __INTMAX_TYPE__;
using uintmax_t = __UINTMAX_TYPE__;
using intptr_t = std::detail::integral_select_t<sizeof(void *)>;
using uintptr_t = std::detail::uintegral_select_t<sizeof(void *)>;

#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807

#define INT8_MIN -128
#define INT16_MIN -32768
#define INT32_MIN -2147483648
#define INT64_MIN -9223372036854775808

#define INT_LEAST8_MAX 127
#define INT_LEAST16_MAX 32767
#define INT_LEAST32_MAX 2147483647
#define INT_LEAST64_MAX 9223372036854775807

#define INT_LEAST8_MIN -128
#define INT_LEAST16_MIN -32768
#define INT_LEAST32_MIN -2147483648
#define INT_LEAST64_MIN -9223372036854775808

#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 4294967295
#define UINT64_MAX 18446744073709551615

#define UINT_FAST8_MAX 4294967295
#define UINT_FAST16_MAX 4294967295
#define UINT_FAST32_MAX 4294967295
#define UINT_FAST64_MAX 18446744073709551615

#if defined(__x86_64__)
using int_fast8_t = int32_t;
using uint_fast8_t = uint32_t;
using int_fast16_t = int32_t;
using uint_fast16_t = uint32_t;
using int_fast32_t = int32_t;
using uint_fast32_t = uint32_t;
using int_fast64_t = int64_t;
using uint_fast64_t = uint64_t;

#define INT_FAST8_MAX 2147483647
#define INT_FAST16_MAX 2147483647
#define INT_FAST32_MAX 2147483647
#define INT_FAST64_MAX 9223372036854775807

#define INT_FAST8_MIN -2147483648
#define INT_FAST16_MIN -2147483648
#define INT_FAST32_MIN -2147483648
#define INT_FAST64_MIN -9223372036854775808
#endif

#define INTPTR_MAX std::detail::intmax_of<sizeof(intptr_t >)>
#define INTMAX_MAX std::detail::intmax_of(sizeof(intmax_t>)>

#endif
