#ifndef __NOSTDLIB_BITS__DEQUE_H__
#define __NOSTDLIB_BITS__DEQUE_H__

#include "emptiable_storage.h"
#include "utils.h"
#include <initializer_list>
#include <vector>

namespace std
{
    template <typename T>
    class deque
    {
    public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

        inline static constexpr size_t PREALLOCATE_COUNT = 3;
        inline static constexpr size_t BLOCK_SIZE = 64;

    private:
        using deque_node = detail::emptiable_array<T, BLOCK_SIZE>;

        vector<deque_node*> buf;
        size_t first_offset, s;

    public:
        class iterator
        {
            deque* dq;
            size_t index;
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using reference = T&;
            using pointer = T*;

            constexpr iterator() noexcept : dq(nullptr), index(0) {}
            explicit iterator(const iterator& i) noexcept : dq(i.n), index(i.index) {}
            iterator(deque* n, size_t i) noexcept : dq(n), index(i) {}

            reference operator*() const noexcept { return *dq[index]; }
            pointer operator->() const noexcept { return &(*dq[index]); }
            reference operator[](difference_type n) const noexcept { return *dq[index + n]; }
            
            iterator& operator++() noexcept
            {
                ++index;
                return *this;
            }

            iterator operator++(int) noexcept { return iterator(dq, index++); }

            iterator& operator--() noexcept
            {
                --index;
                return *this;
            }

            iterator operator--(int) noexcept { return iterator(dq, index--); }

            iterator& operator+=(difference_type n) noexcept
            {
                index += n;
                return *this;
            }

            iterator& operator-=(difference_type n) noexcept
            {
                index -= n;
                return *this;
            }

            iterator operator+(difference_type n) const noexcept { return iterator(dq, index + n); }
            iterator operator-(difference_type n) const noexcept { return iterator(dq, index - n); }
        };

        class const_iterator
        {
            deque* dq;
            size_t index;
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = const T;
            using difference_type = ptrdiff_t;
            using reference = const T&;
            using pointer = const T*;

            constexpr const_iterator() noexcept : dq(nullptr), index(0) {}
            explicit const_iterator(const const_iterator& i) noexcept : dq(i.n), index(i.index) {}
            const_iterator(deque* n, size_t i) noexcept : dq(n), index(i) {}

            reference operator*() const noexcept { return *dq[index]; }
            pointer operator->() const noexcept { return &(*dq[index]); }
            reference operator[](difference_type n) const noexcept { return *dq[index + n]; }

            const_iterator& operator++() noexcept
            {
                ++index;
                return *this;
            }

            const_iterator operator++(int) noexcept { return const_iterator(dq, index++); }

            const_iterator& operator--() noexcept
            {
                --index;
                return *this;
            }

            const_iterator operator--(int) noexcept { return const_iterator(dq, index--); }

            const_iterator& operator+=(difference_type n) noexcept
            {
                index += n;
                return *this;
            }

            const_iterator& operator-=(difference_type n) noexcept
            {
                index -= n;
                return *this;
            }

            const_iterator operator+(difference_type n) const noexcept { return const_iterator(dq, index + n); }
            const_iterator operator-(difference_type n) const noexcept { return const_iterator(dq, index - n); }
        };

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr deque() : buf(), first_offset(0){};

        deque(size_type count, const T& value) { assign(count, value); }

        explicit deque(size_type count)
        {
            s = count;
            first_offset = 64 * PREALLOCATE_COUNT;

            buf.reserve(((count + 63) / 64) + PREALLOCATE_COUNT * 2);

            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
            buf.push_back(new deque_node());

            for (size_t i = 0; i < ((count + 63) / 64); i++, count -= 64)
                buf.push_back(new deque_node*(detail::min(64ul, count)));

            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
        }

        template <class It>
        deque(It f, It l) { assign(f, l); }

        deque(const deque& other) : deque(other.begin(), other.end()) {}

        deque(deque&& other) : buf(move(other.buf))
        {
            first_offset = other.first_offset;
            s = other.s;
            other.first_offset = other.s = 0;
        }

        deque(initializer_list<T> init) : deque(init.begin(), init.end()) {}

        ~deque()
        {
            for (size_t i = 0; i < s; i++)
                at(i).~T();

            for (auto i : buf)
                delete i;
            buf.clear();
        }

        deque& operator=(const deque& other) { ~deque(); }

        void assign(size_type count, const T& value)
        {
            buf.clear();
            s = count;
            first_offset = 64 * PREALLOCATE_COUNT;

            buf.reserve(((s + 63) / 64) + PREALLOCATE_COUNT * 2);

            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
            buf.push_back(new deque_node());

            for (size_t i = 0; i < ((s + 63) / 64); i++, count -= 64)
                buf.push_back(new deque_node*(detail::min(64ul, count), value));

            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
        }

        template <typename It>
        void assign(It f, It l)
        {
            buf.clear();
            first_offset = 64 * PREALLOCATE_COUNT;

            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
            buf.push_back(new deque_node());

            while (f != l)
            {
                auto val = new deque_node();
                for (size_t i = 0; i < 64 && f != l; i++, f++)
                {
                    s++;
                    val->init(i);
                }
                buf.push_back(val);
            }

            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
            buf.push_back(new deque_node());
        }

        void assign(std::initializer_list<T> ilist) { assign(ilist.begin(), ilist.end()); }

        T& at(size_t pos)
        {
            if (pos >= s)
                detail::errors::__stdexcept_out_of_range();
            size_t i = pos + first_offset;
            return buf[i / 64]->at(i % 64);
        }

        const T& at(size_t pos) const
        {
            if (pos >= s)
                detail::errors::__stdexcept_out_of_range();
            size_t i = pos + first_offset;
            return buf[i / 64]->at(i % 64);
        }

        T& operator[](size_t pos)
        {
            size_t i = pos + first_offset;
            return buf[i / 64]->at(i % 64);
        }

        const T& operator[](size_t pos) const
        {
            size_t i = pos + first_offset;
            return buf[i / 64]->at(i % 64);
        }

        reference front() { return *this[0]; }
        const_reference front() const { return *this[0]; }

        reference back() { return *this[s - 1]; }
        const_reference back() const { return *this[s - 1]; }

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, s); }
        const_iterator begin() const { return const_iterator(this, 0); }
        const_iterator end() const { return const_iterator(this, s); }
        const_iterator cbegin() const { return const_iterator(this, 0); }
        const_iterator cend() const { return const_iterator(this, s); }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }
        const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
        const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

        [[nodiscard]] bool empty() const noexcept { return s == 0; }
        size_type size() const noexcept { return s; }
        size_type max_size() const noexcept { return -1ul; }

        void shrink_to_fit() { buf.shrink_to_fit(); }

        void clear() noexcept { ~deque(); }
    };
} // namespace std
#endif
