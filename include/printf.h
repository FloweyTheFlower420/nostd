#ifndef __NOSTDLIB_PRINTF_H__
#define __NOSTDLIB_PRINTF_H__

#include "../bits/user_implement.h"
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <variant>

namespace std
{

    namespace detail::printf
    {
        using printf_argument_wrapper =
            variant<signed char, short, int, long, long long, unsigned char, unsigned short, unsigned int, unsigned long,
                    unsigned long long, intmax_t, uintmax_t, ptrdiff_t, size_t, const char*, void*, signed char*, short*,
                    int*, long*, long long*, intmax_t*, ptrdiff_t*, size_t*>;
        class fmtargs_wrapper

        {
            size_t index;
            size_t size;
            printf_argument_wrapper* fmtargs;

        public:
            template <typename... Args>
            fmtargs_wrapper(Args&&... args)
            {
                index = 0;
                size = sizeof...(args);
                fmtargs = new printf_argument_wrapper[size];
                initializer_list<printf_argument_wrapper> il{printf_argument_wrapper((remove_cv_t<decay_t<Args>>)args)...};
                size_t i = 0;
                for (auto&& e : il)
                    fmtargs[i++] = std::move(e);
            }

            printf_argument_wrapper& next()
            {
                if (index == size)
                    detail::errors::__printf_argument_notfound();
                return fmtargs[index++];
            }

            printf_argument_wrapper& get()

            {
                if (index == size)
                    detail::errors::__printf_argument_notfound();
                return fmtargs[index];
            }

            ~fmtargs_wrapper() { delete[] fmtargs; }
        };

        enum status
        {
            ZEROPAD = 1 << 0,
            LEFT = 1 << 1,
            PLUS = 1 << 2,
            SPACE = 1 << 3,
            HASH = 1 << 4,
            UPPERCASE = 1 << 5,
        };

        enum length
        {
            NORMAL,
            CHAR,
            SHORT,
            LONG,
            LLONG,
            INTMAX,
            SIZE,
            PTRDIFF,
        };

        template <typename T>
        constexpr size_t floor_log(unsigned base, T val)
        {
            if (val < 0)
                val = -val;
            size_t ret = 0;
            while ((val /= base))
                ret++;
            return ret;
        }

        // guesses the printed length of an integer
        template <typename T>
        constexpr size_t guess_len(unsigned b, T val, unsigned flag, unsigned precision)
        {
            size_t ret = max(floor_log(b, val) + 1, (size_t)precision);

            if constexpr (is_signed_v<T>)
            {
                if (val < 0)
                    ret++;
                else if (flag & PLUS || flag & SPACE)
                    ret++;
            }

            return ret;
        }

        // obtains the amount of '0' to print
        template <typename T>
        constexpr size_t precision_pad_chars(unsigned b, T val, unsigned precision)
        {
            auto len = floor_log(b, val) + 1;
            return precision > len ? precision - len : 0;
        }

        struct printf_command
        {
            unsigned precision = 0;
            unsigned width = 0;
            int flags = 0;
            char padchar = ' ';
        };

        template <typename P>
        void handle_special(fmtargs_wrapper& fmtargs, const char*& format, printf_command& cmd, P print)
        {
            size_t width_len;
            const char* str;
            void* ptr;
            char buf[sizeof(void*) * 2] = {'0'};
            const char* int2char = (isupper(*format) || (cmd.flags & UPPERCASE)) ? "0123456789ABCDEF" : "0123456789abcdef";
            switch (*format)
            {
            case 'c':
                width_len = 1;
                break;
            case 's':
                width_len = strlen(str = get<const char*>(fmtargs.next()));
                break;
            case 'p':
                width_len = sizeof(void*) * 2;
                break;
            }

            width_len = cmd.width > width_len ? cmd.width - width_len : 0;

            if (cmd.flags & LEFT)
                for (int i = 0; i < width_len; i++)
                    print(cmd.padchar);

            switch (*format)
            {
            case 'c':
                print(get<int>(fmtargs.next()));
                return;
            case 's':
                while (*str)
                    print(*str++);
                return;
            case 'p':
                uint64_t v = (uint64_t)ptr;
                size_t index = 0;
                while (v)
                {
                    buf[index++] = int2char[v % 16];
                    v /= 16;
                }

                for (int i = sizeof(void*) * 2 - 1; i >= 0; i--)
                    print(buf[i]);
            }

            if (!(cmd.flags & LEFT))
                for (int i = 0; i < width_len; i++)
                    print(cmd.padchar);
        }

        // prints an properly formatted integer
        template <size_t idx, size_t uidx, size_t ptrindex, typename P>
        void format_int(fmtargs_wrapper& fmtargs, const printf_command& cmd, const char* format, size_t pc, P print)
        {
            char tmp[16] = {0};
            const char* int2char = (isupper(*format) || (cmd.flags & UPPERCASE)) ? "0123456789ABCDEF" : "0123456789abcdef";
            size_t width_len = 0;
            size_t precision_len = 0;
            bool sign = false;
            bool is_signed = false;

            auto itoa = [&](unsigned radix, auto val) {
                if constexpr (is_signed_v<decltype(val)>)
                    if (val < 0)
                        val = -val;
                size_t index = 0;
                while (val)
                {
                    tmp[index++] = int2char[val % radix];
                    val /= radix;
                }
            };

            switch (*format)
            {
            case 'd':
            case 'i': {
                auto val = get<idx>(fmtargs.next());
                itoa(10, val);
                if (val < 0)
                    sign = true;
                is_signed = true;
                width_len = guess_len(10, val, cmd.flags, cmd.precision);
                precision_len = precision_pad_chars(10, val, cmd.precision);
                break;
            }
            case 'u': {
                auto val = get<uidx>(fmtargs.next());
                itoa(10, val);
                width_len = guess_len(10, val, cmd.flags, cmd.precision);
                precision_len = precision_pad_chars(10, val, cmd.precision);
                break;
            }
            case 'o': {
                auto val = get<uidx>(fmtargs.next());
                itoa(8, val);
                width_len = guess_len(10, val, cmd.flags, cmd.precision);
                precision_len = precision_pad_chars(10, val, cmd.precision);
                break;
            }
            case 'x':
            case 'X': {
                auto val = get<uidx>(fmtargs.next());
                itoa(16, val);
                width_len = guess_len(10, val, cmd.flags, cmd.precision);
                precision_len = precision_pad_chars(10, val, cmd.precision);
                break;
            }
            break;
            case 'n':
                *get<ptrindex>(fmtargs.next()) = pc;
                return;
            default:
                detail::errors::__printf_undefined_specifier_for_length();
            }

            width_len = cmd.width > width_len ? cmd.width - width_len : 0;

            if (cmd.flags & LEFT)
                for (int i = 0; i < width_len; i++)
                    print(cmd.padchar);

            if (sign)
                print('-');
            else if (is_signed)
            {
                if (cmd.flags & SPACE)
                    print(' ');
                else if (cmd.flags & PLUS)
                    print('+');
            }

            for (int i = 0; i < precision_len; i++)
                print('0');

            for (int i = 15; i >= 0; i--)
            {
                if (tmp[i])
                    print(tmp[i]);
            }

            if (!(cmd.flags & LEFT))
                for (int i = 0; i < width_len; i++)
                    print(cmd.padchar);
        }

    } // namespace detail::printf

    template <typename C, typename... Args>
    int printf_callback(C printer, const char* format, Args&&... args)
    {
        using namespace detail::printf;
        fmtargs_wrapper fmtargs(args...);
        size_t printed_characters = 0;

        auto atoi_inc_str = [&]() mutable {
            unsigned int i = 0;
            while (isdigit(*format))
                i = i * 10U + (unsigned int)(*(format++) - '0');
            return i;
        };

        auto print = [&](char ch) mutable {
            printer(ch);
            printed_characters++;
        };

        while (*format)
        {
            printf_command cmd;
            length len;

            if (*format != '%')
            {
                print(*format);
                format++;
                continue;
            }
            else
                format++;

            switch (*format)
            {
            case '0':
                cmd.padchar = '0';
                break;
            case '-':
                cmd.flags |= LEFT;
                break;
            case '+':
                cmd.flags |= PLUS;
                break;
            case '#':
                cmd.flags |= HASH;
                break;
            case ' ':
                cmd.flags |= SPACE;
                break;
            }

            if (cmd.flags)
                format++;

            if (isdigit(*format))
                cmd.width = atoi_inc_str();

            else if (*format == '*')
            {
                auto w = get<int>(fmtargs.next());
                if (w < 0)
                {
                    cmd.flags |= LEFT;
                    w = -w;
                }
                else
                    cmd.width = (unsigned int)w;

                cmd.width = w;
                format++;
            }

            if (*format == '.')
            {
                format++;
                if (isdigit(*format))
                    cmd.precision = atoi_inc_str();
                else if (*format == '*')
                {
                    auto prec = get<int>(fmtargs.next());
                    cmd.precision = prec > 0 ? prec : 0;
                    format++;
                }
            }

            switch (*format)
            {
            case 'l':
                len = LONG;
                format++;
                if (*format == 'l')
                {
                    len = LLONG;
                    format++;
                }
                break;
            case 'h':
                len = SHORT;
                format++;
                if (*format == 'h')
                {
                    len = CHAR;
                    format++;
                }
                break;
            case 't':
                len = PTRDIFF;
                format++;
                break;
            case 'j':
                len = INTMAX;
                format++;
                break;
            case 'z':
                len = SIZE;
                format++;
                break;
            default:
                len = NORMAL;
                break;
            }

            if (*format == 'c' || *format == 's' || *format == 'p')
            {
                if (len != NORMAL)
                    detail::errors::__printf_undefined_specifier_for_length();
                handle_special(fmtargs, format, cmd, print);
                format++;
                continue;
            }

            switch (len)
            {
            case NORMAL:
                format_int<2, 7, 18>(fmtargs, cmd, format, printed_characters, print);
                break;
            case CHAR:
                format_int<0, 5, 16>(fmtargs, cmd, format, printed_characters, print);
                break;
            case SHORT:
                format_int<1, 6, 17>(fmtargs, cmd, format, printed_characters, print);
                break;
            case LONG:
                format_int<3, 8, 19>(fmtargs, cmd, format, printed_characters, print);
                break;
            case LLONG:
                format_int<4, 9, 20>(fmtargs, cmd, format, printed_characters, print);
                break;
            case INTMAX:
                format_int<10, 11, 21>(fmtargs, cmd, format, printed_characters, print);
                break;
            case PTRDIFF:
                format_int<12, 12, 22>(fmtargs, cmd, format, printed_characters, print);
                break;
            case SIZE:
                format_int<13, 13, 23>(fmtargs, cmd, format, printed_characters, print);
                break;
            }

            format++;
        }
        printer('\0');
        return printed_characters;
    }

    template <typename... Args>
    int printf(const char* format, Args&&... args)
    {
        return printf_callback(detail::putc, format, args...);
    }

    template <typename... Args>
    int sprintf(char* buffer, const char* format, Args&&... args)
    {
        printf_callback([&, buffer](char ch) mutable { *buffer++ = ch; }, format, args...);
    }

    template <typename... Args>
    int snprintf(char* buffer, size_t count, const char* format, Args&&... args)
    {
        printf_callback(
            [&, buffer](char ch) mutable {
                if (count)
                {
                    *buffer++ = ch;
                    count--;
                }
            },
            format, args...);
    }
} // namespace std

#endif
