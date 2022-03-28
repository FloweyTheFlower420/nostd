#ifndef __NOSTDLIB_VARIANT_H__
#define __NOSTDLIB_VARIANT_H__

#include "../bits/utils.h"
#include <cstddef.h>
#include <initializer_list.h>
#include <type_traits.h>
#include <utility.h>

namespace std
{
    namespace detail
    {
        template <size_t index, bool b, bool... Bs>
        struct first_index_true
        {
            static inline constexpr size_t value =
                sizeof...(Bs) == 0 ? (b ? index : -1) : (b ? index : first_index_true<index + 1, Bs...>::value);
        };

        template <typename T, typename U>
        using can_construct = is_constructible<U, T>;

    } // namespace detail
    template <typename... Ts>
    class variant
    {
        static_assert(sizeof...(Ts) != 0, "Variant cannot be empty");

        using first_t = detail::type_pack_element<0, Ts...>;

        inline constexpr static auto align = detail::max_comptime<size_t, alignof(Ts)...>::value;
        inline constexpr static auto size = detail::max_comptime<size_t, sizeof(Ts)...>::value;

        using deleter = void (*)(char*);
        using constructor = void (*)(const char*, char*);

        inline constexpr static deleter destructor[] = {+[](char* p) { reinterpret_cast<Ts*>(p)->~Ts(); }...};

        inline constexpr static constructor copy_constructor[size] = {
            +[](const char* from, char* to) { new (to) Ts(*(Ts*)from); }...};

        inline constexpr static constructor move_constructor[size] = {
            +[](const char* from, char* to) { new (to) Ts(*(Ts*)from); }...};

        alignas(align) char data[size];
        size_t idx;

        constexpr void safe_delete()
        {
            if(idx != npos)
                destructor[idx](data);
        }

    public:
        inline static constexpr size_t npos = -1;

        // Constructors {{{

        constexpr variant() noexcept(noexcept(first_t())) : idx(0) { new (data) first_t(); }
        constexpr variant(const variant& other) : idx(other.index)
        {
            copy_constructor[other.index](other.data, data);
        }

        constexpr variant(variant&& other) noexcept((... && is_nothrow_move_constructible_v<Ts>)) : idx(other.index)
        {
            move_constructor[other.index](other.data, data);
        }

        template <typename T>
        constexpr variant(T&& t) noexcept(noexcept(T(forward(t))))
        {
            idx = detail::first_index_true<0, is_constructible_v<Ts, T>...>::value;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_constructible_v<Ts, T>...>::value, Ts...>(
                forward(t));
        }

        template <typename T, typename... Args>
        constexpr explicit variant(in_place_type_t<T>, Args&&... args)
        {
            idx = detail::first_index_true<0, is_same_v<T, Ts>...>::value;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_same_v<T, Ts>...>::value, Ts...>(
                forward(args...));
        }

        template <typename T, typename U, typename... Args>
        constexpr explicit variant(in_place_type_t<T>, initializer_list<U> il, Args&&... args)
        {
            idx = detail::first_index_true<0, is_same_v<T, Ts>...>::value;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_same_v<T, Ts>...>::value, Ts...>(il,
                forward(args...));
        }

        template <size_t I, typename... Args>
        constexpr explicit variant(in_place_index_t<I>, Args&&... args)
        {
            idx = I;
            new (data) detail::type_pack_element<I, Ts...>(forward(args)...);
        }

        template <size_t I, typename U, typename... Args>
        constexpr explicit variant(in_place_index_t<I>, initializer_list<U> il, Args&&... args)
        {
            idx = I;
            new (data) detail::type_pack_element<I, Ts...>(il, forward(args)...);
        }

        // }}}

        ~variant() { destructor[idx](data); }

        constexpr variant& operator=(const variant& rhs)
        {
            safe_delete();
            idx = npos;
            copy_constructor[rhs.index](rhs.data, data);
            idx = rhs.index;
            return *this;
        }

        constexpr variant& operator=(variant&& rhs) noexcept((... && is_nothrow_move_constructible_v<Ts>))
        {
            idx = npos;
            safe_delete();
            move_constructor[rhs.index](rhs.data, data);
            idx = rhs.index;
            rhs.index = npos;
            return *this;
        }

        template <typename T>
        constexpr variant& operator=(T&& t) noexcept(noexcept(T(forward(t))))
        {
            safe_delete();
            idx = npos;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_constructible_v<Ts, T>...>::value, Ts...>(
                forward(t));
            idx = detail::first_index_true<0, is_constructible_v<Ts, T>...>::value;
            return *this;
        }

        constexpr std::size_t index() const noexcept { return idx; }

        constexpr bool valueless_by_exception() const noexcept { return idx == -1; }

        template <typename T, typename... Args>
        constexpr T& emplace(Args&&... args)
        {
            safe_delete();
            idx = npos;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_same_v<Ts, T>...>::value, Ts...>(
                forward(args)...);
            idx = detail::first_index_true<0, is_same_v<Ts, T>...>::value;
            return *((T*) data);
        }

        template <typename T, typename U, typename... Args>
        constexpr T& emplace( std::initializer_list<U> il, Args&&... args )
        {
            safe_delete();
            idx = npos;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_same_v<Ts, T>...>::value, Ts...>(
                il, forward(args)...);
            idx = detail::first_index_true<0, is_same_v<Ts, T>...>::value;
            return *((T*) data);
        }

        template <typename::size_t I, typename... Args>
    constexpr std::variant_alternative_t<I, variant>& emplace( Args&&... args );
        };

    template <std::size_t I, class T>
    struct variant_alternative;
    
    template <std::size_t I, class... Ts>
    struct variant_alternative<I, variant<Ts...>>
    {
        using type = detail::type_pack_element<I, Ts...>;
    }
} // namespace std

#endif
