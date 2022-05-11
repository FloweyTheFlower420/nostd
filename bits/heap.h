#ifndef __NOSTDLIB_BITS_HEAP_H__
#define __NOSTDLIB_BITS_HEAP_H__

#include "iterator_simple_types.h"
#include <utility>
#include <functional>

namespace std
{
    namespace detail
    {
        template <typename It, typename D, typename C>
        D is_heap_until(It first, D n, C& comp)
        {
            D parent = 0;
            for (D child = 1; child < n; ++child)
            {
                if (comp(first + parent, first + child))
                    return child;
                if ((child & 1) == 0)
                    ++parent;
            }
            return n;
        }

        template <typename It, typename D>
        bool is_heap(It first, D n)
        {
            return is_heap_until(first, n, [](It a, It b) { return *a < *b; }) == n;
        }

        template <typename It, typename C, typename D>
        bool is_heap(It first, C comp, D n)
        {
            return is_heap_until(first, n, [comp](It a, It b) { return comp(*a, *b); }) == n;
        }

        template <typename It>
        bool is_heap(It first, It last)
        {
            return is_heap(first, distance(first, last));
        }

        template <typename It, typename C>
        bool is_heap(It first, It last, C comp)
        {
            return is_heap(first, std::move(comp), distance(first, last));
        }

        template <typename It, typename D, typename T, typename C>
        void push_heap(It first, D i, D top, T v, C& comp)
        {
            D parent = (i - 1) / 2;
            while (i > top && comp(first + parent, v))
            {
                *(first + i) = std::move(*(first + parent));
                i = parent;
                parent = (i - 1) / 2;
            }
            *(first + i) = std::move(v);
        }
        template <typename It, typename D, typename T, typename C>
        void adjust_heap(It first, D i, D len, T val, C comp)
        {
            const D top = i;
            D second = i;

            while (second < (len - 1) / 2)
            {
                second = 2 * (second + 1);
                if (comp(first + second, first + (second - 1)))
                    second--;
                *(first + i) = std::move(*(first + second));
                i = second;
            }

            if ((len & 1) == 0 && second == (len - 2) / 2)
            {
                second = 2 * (second + 1);
                *(first + i) = std::move(*(first + (second - 1)));
                i = second - 1;
            }

            push_heap(first, i, top, std::move(val), [comp](It a, It b) { return comp(*a < *b); });
        }

        template <typename It, typename C>
        void pop_heap(It first, It last, It ret, C& comp)
        {
            using value_t = typename iterator_traits<It>::value_type;
            using diff_t = typename iterator_traits<It>::difference_type;

            value_t value = std::move(*ret);
            *ret = std::move(*first);
            detail::adjust_heap(first, (0), diff_t(last - first), std::move(value), comp);
        }

        template <typename It, typename C>
        void make_heap(It first, It last, C& comp)
        {
            using value_t = typename iterator_traits<It>::value_type;
            using diff_t = typename iterator_traits<It>::difference_type;

            if (last - first < 2)
                return;

            const diff_t len = last - first;
            diff_t parent = (len - 2) / 2;
            while (true)
            {
                value_t value = std::move(*(first + parent));
                adjust_heap(first, parent, len, std::move(value), comp);
                if (parent == 0)
                    return;
                parent--;
            }
        }
    } // namespace detail
    template <typename It>
    void push_heap(It first, It last)
    {
        using value_t = typename iterator_traits<It>::value_type;
        using diff_t = typename iterator_traits<It>::difference_type;

        value_t v = std::move(*(last - 1));
        detail::push_heap(first, diff_t((last - first) - 1), diff_t(0), std::move(v), [](It a, It b) { return *a < *b; });
    }

    template <typename It, typename C>
    void push_heap(It first, It last, C comp)
    {
        using value_t = typename iterator_traits<It>::value_type;
        using diff_t = typename iterator_traits<It>::difference_type;

        value_t value = std::move(*(last - 1));
        detail::push_heap(first, diff_t((last - first) - 1), diff_t(0), std::move(value),
                          [comp](It a, It b) { return comp(*a < *b); });
    }

    template <typename It>
    void pop_heap(It first, It last)
    {
        if (last - first > 1)
        {
            --last;
            detail::pop_heap(first, last, last, [](It a, It b) { *a < *b; });
        }
    }
    template <typename It, typename C>
    void pop_heap(It first, It last, C comp)
    {
        if (last - first > 1)
        {
            --last;
            detail::pop_heap(first, last, last, [comp](It a, It b) { comp(*a, *b); });
        }
    }

    template <typename It>
    void make_heap(It first, It last)
    {
        detail::make_heap(first, last, [](It a, It b) { *a < *b; });
    }

    template <typename It, typename C = less<typename iterator_traits<It>::value_type>>
    void make_heap(It first, It last, C comp)
    {
        using value_t = typename iterator_traits<It>::value_type;
            using diff_t = typename iterator_traits<It>::difference_type;

            if (last - first < 2)
                return;

            const diff_t len = last - first;
            diff_t parent = (len - 2) / 2;
            while (true)
            {
                value_t value = std::move(*(first + parent));
                adjust_heap(first, parent, len, std::move(value), [comp](It a, It b) { comp(*a, *b); });
                if (parent == 0)
                    return;
                parent--;
            }
    }


    template <typename It, typename C = less<typename iterator_traits<It>::value_type>>
    void sort_heap(It first, It last, C comp)
    {
        sort_heap(first, last, last, [comp](It a, It b) { return comp(*a, *b); });
    }
} // namespace std

#endif
