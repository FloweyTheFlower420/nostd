#ifndef __NOSTDLIB_BITS__DEQUE_H__
#define __NOSTDLIB_BITS__DEQUE_H__

#include "utils.h"
#include <initializer_list>

namespace std
{
    namespace detail
    {
        template <typename T>
        struct deque_node
        {
            inline static constexpr size_t BLOCK_SIZE = 64;
            char buf[BLOCK_SIZE * sizeof(T)];
            deque_node* next;

            T& at(size_t index) { return *((T*)&buf[index * sizeof(T)]); }

            deque_node(size_t n, const T& val)
            {
                for (size_t i = 0; i < n; i++)
                    new (&buf[i * sizeof(T)]) T(val);
            }

            deque_node(size_t n)
            {
                for (size_t i = 0; i < n; i++)
                    new (&buf[i * sizeof(T)]) T;
            }

            template <class It>
            deque_node(It& first, It last, size_t& i)
            {
                size_t v = i;
                for (; i < v + 64 && first != last; i++, first++)
                    new (&buf[i * sizeof(T)]) T(*first);
            }

            void destroy(size_t n, size_t s = 0)
            {
                for (size_t i = s; i < s + n; i++)
                    at(i).~T();
            }
        };
    } // namespace detail

    template <typename T>
    class deque
    {
        detail::deque_node<T>* first;
        detail::deque_node<T>* last;
        size_t first_offset, size;

    public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        constexpr deque() : first(nullptr), last(nullptr), first_offset(0), size(0){};
        deque(size_type count, const T& value)
        {
            size = count;
            detail::deque_node<T>** n = &first;
            for (size_type i = 0; i < count; i -= 64)
            {
                *n = new detail::deque_node<T>(detail::min(64ul, count), value);
                n = &((*n)->next);
            }
        }

        explicit deque(size_type count)
        {
            size = count;
            detail::deque_node<T>** n = &first;
            for (size_type i = 0; i < count; i -= 64)
            {
                *n = new detail::deque_node<T>(detail::min(64ul, count));
                n = &((*n)->next);
            }
        }

        template <class It>
        deque(It f, It l)
        {
            detail::deque_node<T>** n = &first;
            size_t i = 0;
            while (f != l)
            {
                *n = new detail::deque_node<T>(f, l, i);
                n = &((*n)->next);
            }

            size = i;
        }

        deque(const deque& other) : deque(other.begin(), other.end()) {}

        deque(deque&& other)
        {
            first = other.first;
            last = other.last;
            first_offset = other.first_offset;
            size = other.size;

            other.first = other.last = nullptr;
            other.first_offset = other.size = 0;
        }

        deque(initializer_list<T> init) : deque(init.begin(), init.end()) {}

        ~deque()
        {
            if (!first)
                return;

            first->destroy(detail::min(size, 64 - first_offset), first_offset);
            size -= detail::min(size, 64 - first_offset);

            first = first->next;
            while (first)
            {
                first->destroy(detail::min(64ul, size));
                size -= 64;
                first = first->next;
            }
        }

        deque& operator=(const deque& other)
        {
            auto f = other.begin();
            auto l = other.end();

            detail::deque_node<T>** n = &first;
            size_t i = 0;
            while (f != l)
            {
                *n = new detail::deque_node<T>(f, l, i);
                n = &((*n)->next);
            }

            size = i;
        }
    };
} // namespace std
#endif /* _STL_DEQUE_H */

