#ifndef __NOSTDLIB_VARIANT_H__
#define __NOSTDLIB_VARIANT_H__

#include "../bits/user_implement.h"
#include "../bits/utils.h"
#include <cstddef.h>
#include <initializer_list.h>
#include <new.h>
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

    // forward decls
    template <typename...>
    class variant;

    template <size_t I, class T>
    struct variant_alternative;

    template <size_t I, class... Ts>
    struct variant_alternative<I, variant<Ts...>>
    {
        using type = detail::type_pack_element<I, Ts...>;
    };

    template <size_t I, typename T>
    using variant_alternative_t = variant_alternative<I, T>;

    template <typename... Ts>
    class variant
    {
        static_assert(sizeof...(Ts) != 0, "Variant cannot be empty");

        template <size_t I, class... Ts1>
        friend constexpr variant_alternative_t<I, variant<Ts1...>>& get(variant<Ts1...>& v);
        template <size_t I, class... Ts1>
        friend constexpr variant_alternative_t<I, variant<Ts1...>>&& get(variant<Ts1...>&& v);
        template <size_t I, class... Ts1>
        friend constexpr const variant_alternative_t<I, variant<Ts1...>>& get(const variant<Ts1...>& v);
        template <size_t I, class... Ts1>
        friend constexpr const variant_alternative_t<I, variant<Ts1...>>&& get(const variant<Ts1...>&& v);

        template <class T, class... Ts1>
        friend constexpr T& get(variant<Ts1...>& v);
        template <class T, class... Ts1>
        friend constexpr T&& get(variant<Ts1...>&& v);
        template <class T, class... Ts1>
        friend constexpr const T& get(const variant<Ts1...>& v);
        template <class T, class... Ts1>
        friend constexpr const T&& get(const variant<Ts1...>&& v);

        template <size_t I, class... Ts1>
        friend constexpr add_pointer_t<const variant_alternative_t<I, variant<Ts1...>>> get_if(
            const variant<Ts1...>* pv) noexcept;

        using first_t = detail::type_pack_element<0, Ts...>;

        inline constexpr static auto align = detail::max_comptime<size_t, alignof(Ts)...>::value;
        inline constexpr static auto size = detail::max_comptime<size_t, sizeof(Ts)...>::value;

        using deleter = void (*)(char*);
        using constructor = void (*)(const char*, char*);
        using swapper = void (*)(char*, char*);

        inline constexpr static deleter destructor[] = {+[](char* p) { reinterpret_cast<Ts*>(p)->~Ts(); }...};

        inline constexpr static constructor copy_constructor[size] = {
            +[](const char* from, char* to) { new (to) Ts(*(Ts*)from); }...};

        inline constexpr static constructor move_constructor[size] = {
            +[](const char* from, char* to) { new (to) Ts(*(Ts*)from); }...};

        alignas(align) char data[size];
        size_t idx;

        constexpr void safe_delete()
        {
            if (idx != npos)
                destructor[idx](data);
        }

    public:
        inline static constexpr size_t npos = -1;

        constexpr variant() noexcept(noexcept(first_t())) : idx(0) { new (data) first_t(); }
        constexpr variant(const variant& other) : idx(other.index) { copy_constructor[other.index](other.data, data); }

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
            new (data)
                detail::type_pack_element<detail::first_index_true<0, is_same_v<T, Ts>...>::value, Ts...>(forward(args...));
        }

        template <typename T, typename U, typename... Args>
        constexpr explicit variant(in_place_type_t<T>, initializer_list<U> il, Args&&... args)
        {
            idx = detail::first_index_true<0, is_same_v<T, Ts>...>::value;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_same_v<T, Ts>...>::value, Ts...>(
                il, forward(args...));
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

        ~variant() { safe_delete(); }

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

        constexpr size_t index() const noexcept { return idx; }

        constexpr bool valueless_by_exception() const noexcept { return idx == -1; }

        template <typename T, typename... Args>
        constexpr T& emplace(Args&&... args)
        {
            safe_delete();
            idx = npos;
            new (data)
                detail::type_pack_element<detail::first_index_true<0, is_same_v<Ts, T>...>::value, Ts...>(forward(args)...);
            idx = detail::first_index_true<0, is_same_v<Ts, T>...>::value;
            return *((T*)data);
        }

        template <typename T, typename U, typename... Args>
        constexpr T& emplace(initializer_list<U> il, Args&&... args)
        {
            safe_delete();
            idx = npos;
            new (data) detail::type_pack_element<detail::first_index_true<0, is_same_v<Ts, T>...>::value, Ts...>(
                il, forward(args)...);
            idx = detail::first_index_true<0, is_same_v<Ts, T>...>::value;
            return *((T*)data);
        }

        template <size_t I, typename... Args>
        constexpr variant_alternative_t<I, variant>& emplace(Args&&... args)
        {
            safe_delete();
            idx = I;
            new (data) variant_alternative_t<I, variant>(forward(args)...);
        }

        template <size_t I, typename U, typename... Args>
        constexpr variant_alternative_t<I, variant>& emplace(initializer_list<U> il, Args&&... args)
        {
            safe_delete();
            idx = I;
            new (data) variant_alternative_t<I, variant>(il, forward(args)...);
        }

        // TODO: impl
        constexpr void swap(variant& rhs) noexcept(((is_nothrow_move_constructible_v<Ts> &&
                                                     is_nothrow_swappable_v<Ts>)&&...))
        {
            /*
            if (rhs.valueless_by_exception() && valueless_by_exception())
                return;

            if (idx == rhs.idx)
            {
            1    swap(get<I>(*this), get<I>(rhs));
            }
            else
                while (1) {}*/
        }
    };

    template <class T>
    struct variant_size; /* undefined */
    template <class... Types>

    struct variant_size<variant<Types...>> : integral_constant<size_t, sizeof...(Types)>
    {
    };
    template <class T>
    class variant_size<const T>;
    template <class T>
    class variant_size<volatile T>;
    template <class T>
    class variant_size<const volatile T>;

    template <size_t I, class... Ts>
    constexpr variant_alternative_t<I, variant<Ts...>>& get(variant<Ts...>& v)
    {
        if (v.index() != I)
            detail::errors::__stdexcept_bad_variant_access();
        return *((variant_alternative_t<I, variant<Ts...>>*)v.data);
    }
    template <size_t I, class... Ts>
    constexpr variant_alternative_t<I, variant<Ts...>>&& get(variant<Ts...>&& v)
    {
        if (v.index() != I)
            detail::errors::__stdexcept_bad_variant_access();
        return *((variant_alternative_t<I, variant<Ts...>>*)v.data);
    }

    template <size_t I, class... Ts>
    constexpr const variant_alternative_t<I, variant<Ts...>>& get(const variant<Ts...>& v)
    {
        if (v.index() != I)
            detail::errors::__stdexcept_bad_variant_access();
        return *((variant_alternative_t<I, variant<Ts...>>*)v.data);
    }

    template <size_t I, class... Ts>
    constexpr const variant_alternative_t<I, variant<Ts...>>&& get(const variant<Ts...>&& v)
    {
        if (v.index() != I)
            detail::errors::__stdexcept_bad_variant_access();
        return *((variant_alternative_t<I, variant<Ts...>>*)v.data);
    }

    template <class T, class... Ts>
    constexpr T& get(variant<Ts...>& v)
    {
        if (v.index() != detail::first_index_true<0, is_same_v<T, Ts>...>::value)
            return detail::errors::__stdexcept_bad_variant_access();
        return *((T*)(v.data));
    }

    template <class T, class... Ts>
    constexpr T&& get(variant<Ts...>&& v)
    {
        if (v.index() != detail::first_index_true<0, is_same_v<T, Ts>...>::value)
            return detail::errors::__stdexcept_bad_variant_access();
        return *((T*)(v.data));
    }

    template <class T, class... Ts>
    constexpr const T& get(const variant<Ts...>& v)
    {
        if (v.index() != detail::first_index_true<0, is_same_v<T, Ts>...>::value)
            return detail::errors::__stdexcept_bad_variant_access();
        return *((T*)(v.data));
    }

    template <class T, class... Ts>
    constexpr const T&& get(const variant<Ts...>&& v)
    {
        if (v.index() != detail::first_index_true<0, is_same_v<T, Ts>...>::value)
            return detail::errors::__stdexcept_bad_variant_access();
        return *((T*)(v.data));
    }

    template <size_t I, class... Ts>
    constexpr const variant_alternative_t<I, variant<Ts...>>* get_if(const variant<Ts...>* pv) noexcept
    {
        if (pv == nullptr)
            return nullptr;
        if (pv->index() != I)
            return nullptr;

        return (const variant_alternative_t<I, variant<Ts...>>*)pv->data;
    }

    template <class T, class... Ts>
    constexpr add_pointer_t<const T> get_if(const variant<Ts...>* pv) noexcept
    {
        if (pv == nullptr)
            return nullptr;
        if (pv->index() != detail::first_index_true<0, is_same_v<T, Ts>...>::value)
            return nullptr;

        return (const T*)pv->data;
    }
} // namespace std

#endif
