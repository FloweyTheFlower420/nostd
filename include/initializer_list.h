#ifndef __NOSTDLIB_INITIALIZER_LIST_H__
#define __NOSTDLIB_INITIALIZER_LIST_H__
#include <cstddef.h>

namespace std
{
    /// initializer_list
    template <class E>
    class initializer_list
    {
    public:
        using value_type = E;
        using reference = const E&;
        using const_reference = const E&;
        using size_type = size_t;
        using iterator = const E*;
        using const_iterator = const E*;

    private:
        iterator data;
        size_type len;

        constexpr initializer_list(const_iterator a, size_type l) : data(a), len(l) {}

    public:
        constexpr initializer_list() noexcept : data(nullptr), len(0) {}
        constexpr size_type size() const noexcept { return len; }
        constexpr const_iterator begin() const noexcept { return data; }
        constexpr const_iterator end() const noexcept { return begin() + size(); }
    };

    template <class T>
    constexpr const T* begin(initializer_list<T> l) noexcept
    {
        return l.begin();
    }

    template <class T>
    constexpr const T* end(initializer_list<T> l) noexcept
    {
        return l.end();
    }
} // namespace std

#endif
