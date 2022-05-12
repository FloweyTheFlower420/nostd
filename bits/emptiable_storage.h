#ifndef __NOSTDLIB_BITS_EMPTIABLE_STORAGE_H__
#define __NOSTDLIB_BITS_EMPTIABLE_STORAGE_H__

#include <type_traits>

namespace std::detail
{
    template <typename T>
    class emptiable_storage
    {
        alignas(T) char buffer[sizeof(T)];

    public:
        constexpr emptiable_storage() {}

        template <typename... Args>
        emptiable_storage(Args&&... v)
        {
            new (buffer) T(std::forward<Args>(v)...);
        }

        T& as() { return *((T*)buffer); }
        void destroy() { as().~T(); }
    };

    template <typename T, size_t N>
    class emptiable_array
    {
        alignas(T) char buffer[sizeof(T) * N];

    public:
        constexpr emptiable_array() {}

        emptiable_array(size_t n)
        {
            for(size_t i = 0; i < n; i++)
                new (buffer + sizeof(T) * i) T;
        }

        emptiable_array(const T& v, size_t n)
        {
            for(size_t i = 0; i < n; i++)
                new (buffer + sizeof(T) * i) T(v);
        }

        emptiable_array(const T& v, size_t from, size_t to)
        {
            for(size_t i = from; i < to; i++)
                new (buffer + sizeof(T) * i) T(v);
        }

        T& at(size_t i) { return *((T*)(buffer + sizeof(T) * i)); }
        void destroy(size_t start, size_t len = 0)
        {
           for(size_t i = 0; i < len; i++) 
                return at(i + start).~T(); 
        }

        void init(const T& v, size_t i)
        {
            new (&at(i)) T(v);
        }
    };

} // namespace std::detail

#endif
