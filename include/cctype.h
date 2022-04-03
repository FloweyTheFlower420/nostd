#ifndef __NOSTDLIB_CTYPE_H__
#define __NOSTDLIB_CTYPE_H__

namespace std
{
    namespace detail
    {
        template <typename T, T from, T to>
        constexpr bool range(T t)
        {
            return t >= from && t <= to;
        }
    } // namespace detail

    constexpr bool isblank(char c) { return c == ' ' || c == '\t'; }
    constexpr bool iscntrl(char c) { return detail::range<char, 0x00, 0x1f>(c) || c == 0x7F; }
    constexpr bool isdigit(char c) { return detail::range<char, '0', '9'>(c); }
    constexpr bool islower(char c) { return detail::range<char, 'a', 'z'>(c); }
    constexpr bool ispunct(char c)
    {
        return detail::range<char, '!', '/'>(c) || detail::range<char, ';', '@'>(c) || detail::range<char, '[', '`'>(c) ||
               detail::range<char, '{', '~'>(c);
    }

    constexpr bool isspace(char c) { return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; }

    constexpr bool isupper(char c) { return detail::range<char, 'A', 'Z'>(c); }
    constexpr bool isxdigit(char c)
    {
        return isdigit(c) || detail::range<char, 'a', 'f'>(c) || detail::range<char, 'A', 'F'>(c);
    }

    constexpr bool isalpha(char c) { return isupper(c) || islower(c); }
    constexpr bool isalnum(char c) { return isalpha(c) || isdigit(c); }
    constexpr bool isgraph(char c) { return isalnum(c) || ispunct(c); }
    constexpr bool isprint(char c) { return isgraph(c) || isspace(c); }

} // namespace std

#endif
