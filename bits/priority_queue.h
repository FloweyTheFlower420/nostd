#ifndef __NOSTDLIB_BITS_PRIORITY_QUEUE_H__
#define __NOSTDLIB_BITS_PRIORITY_QUEUE_H__
#include "heap.h"
#include <functional>
#include <vector>

namespace std
{
    template <typename T, typename S = vector<T>, typename C = less<typename S::value_type>>
    class priority_queue
    {
        static_assert(is_same_v<T, typename S::value_type>, "value_type must be the same as the underlying container");

    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using size_type = typename S::size_type;
        using container_type = S;
        using value_compare = C;

    protected:
        S c;
        C comp;

    public:
        template <typename = enable_if_t<is_default_constructible_v<C> && is_default_constructible_v<S>>>
        priority_queue() : c(), comp()
        {
        }
        explicit priority_queue(const C& c, const S& s) : c(s), comp(c) { make_heap(c.begin(), c.end(), comp); }

        explicit priority_queue(const C& c, S&& s = S()) : c(move(s)), comp(c) { make_heap(c.begin(), c.end(), comp); }

        template <typename It>
        priority_queue(It first, It last, const C& c = C()) : c(first, last), comp(c)
        {
            make_heap(c.begin(), c.end(), comp);
        }

        template <typename It>
        priority_queue(It first, It last, const C& c, const S& s) : c(s), comp(c)
        {
            c.insert(c.end(), first, last);
            make_heap(c.begin(), c.end(), comp);
        }

        template <typename It>
        priority_queue(It first, It last, const C& c, S&& s) : c(move(s)), comp(c)
        {
            c.insert(c.end(), first, last);
            make_heap(c.begin(), c.end(), comp);
        }

        [[nodiscard]] bool empty() const { return c.empty(); }
        [[nodiscard]] size_type size() const { return c.size(); }
        [[nodiscard]] const_reference top() const { return c.front(); }

        void push(const value_type& val)
        {
            c.push_back(val);
            push_heap(c.begin(), c.end(), comp);
        }

        void push(value_type&& val)
        {
            c.push_back(move(val));
            push_heap(c.begin(), c.end(), comp);
        }

        template <typename... Args>
        void emplace(Args&&... args)
        {
            c.emplace_back(forward<Args>(args)...);
            push_heap(c.begin(), c.end(), comp);
        }

        void pop()
        {
            pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }

        void swap(priority_queue& rhs) noexcept(is_nothrow_swappable_v<S>&& is_nothrow_swappable_v<C>)
        {
            swap(c, rhs.c);
            swap(comp, rhs.comp);
        }
    };
} // namespace std

#endif
