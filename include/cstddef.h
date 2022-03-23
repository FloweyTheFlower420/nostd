#ifndef __NOSTDLIB_INCLUDE_CSTDDEF_H__
#define __NOSTDLIB_INCLUDE_CSTDDEF_H__

namespace std
{
    using size_t = decltype(sizeof(int));
    using ptrdiff_t = decltype(((char*)1) - ((char*)0));
    using nullptr_t = decltype(nullptr);

    namespace detail
    {
        template <typename T, typename... Ts>
        auto max_align_of()
        {
            if constexpr (sizeof...(Ts) == 0)
                return T{};
            else
            {
                using t = decltype(max_align_of<Ts...>());
                if constexpr (alignof(T) > alignof(t))
                    return T{};
                else
                    return t{};
            }
        }
    } // namespace detail
    struct alignas(decltype(std::detail::max_align_of<char, short, int, long, long long, double, float, long double>()))
        max_align_t
    {
    };
} // namespace std

#define NULL ((void*)0)

#if __has_builtin(__builtin_offsetof)
#define offsetof(type, member) __builtin_offsetof(type, member)
#else
#define offsetof(type, member) ((size_t) & (((type*)0)->member))
#endif

#endif
