#ifndef __NOSTDLIB_UTILS_H__
#define __NOSTDLIB_UTILS_H__

namespace std
{
    namespace detail
    {
        template <typename A>
        constexpr A max(A a, A b)
        {
            return a > b ? a : b;
        }

        template <typename A, typename... R>
        constexpr A max(A a, R... r)
        {
            return max(a, max(r...));
        }

        template <typename A>
        constexpr A min(A a, A b)
        {
            return a < b ? a : b;
        }

        template <typename A, typename... R>
        constexpr A mix(A a, R... r)
        {
            return mix(a, mix(r...));
        }

        template <typename A>
        constexpr A div_roundup(A a, A b)
        {
            return (a + b - 1) / b;
        }
    } // namespace detail
} // namespace std

#endif
