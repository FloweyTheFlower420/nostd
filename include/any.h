#ifndef __NOSTDLIB_ANY_H__
#define __NOSTDLIB_ANY_H__

#include <initializer_list.h>
#include <new.h>
#include <type_traits.h>
#include <utility.h>
namespace std
{
    class any;

    template <typename T>
    T any_cast(const any& operand);
    template <typename T>
    T any_cast(any& operand);
    template <typename T>
    T any_cast(any&& operand);
    template <typename T>
    const T* any_cast(const any* operand) noexcept;
    template <typename T>
    T* any_cast(any* operand) noexcept;

    class any
    {
        void* buffer;
        void (*deleter)(void*&);
        void (*copier)(void* const&, void*&);
        void (*mover)(void* const&, void*&);
        bool is_inplace;

        template <typename T>
        inline static constexpr bool can_inplace = is_trivially_move_constructible_v<T> && sizeof(T) <= sizeof(void*) &&
                                                   alignof(T) <= alignof(void*);

        template <typename T>
        friend T any_cast(const any& operand);
        template <typename T>
        friend T any_cast(any& operand);
        template <typename T>
        friend T any_cast(any&& operand);
        template <typename T>
        friend const T* any_cast(const any* operand) noexcept;
        template <typename T>
        friend T* any_cast(any* operand) noexcept;

    public:
        constexpr any() noexcept : buffer(nullptr), deleter(nullptr), copier(nullptr), mover(nullptr), is_inplace(false) {}

        any(const any& other)
        {
            deleter = other.deleter;
            copier = other.copier;
            mover = other.mover;
            is_inplace = other.is_inplace;

            if (other.has_value())
                copier(other.buffer, buffer);
        }

        any(any&& other)
        {
            deleter = other.deleter;
            copier = other.copier;
            mover = other.mover;
            is_inplace = other.is_inplace;

            if (other.has_value())
                mover(other.buffer, buffer);
            other.reset();
        }

        template <typename V>
        any(V&& val) : is_inplace(can_inplace<std::decay_t<V>>)
        {
            using T = std::decay_t<V>;
            if constexpr (can_inplace<T>)
            {
                new (&buffer) T(std::forward(val));
                // the void* should be interpreted as the object, not pointer
                deleter = [](void*& data) { ((T*)(&data))->~T(); };
                copier = [](void*& from, void*& to) { new (&to) T(*((T*)&from)); };
                mover = [](void*& from, void*& to) { new (&to) T(move(*((T*)&from))); };
            }
            else
            {
                buffer = new T(std::forward(val));
                deleter = [](void*& data) { delete ((T*)data); };
                copier = [](void*& from, void*& to) { to = new T(*((T*)from)); };
                mover = [](void*& from, void*& to) { to = new T(move(*((T*)from))); };
            }
        }

        template <typename V, typename... Args>
        explicit any(std::in_place_type_t<V>, Args&&... args) : is_inplace(can_inplace<std::decay_t<V>>)
        {
            using T = std::decay_t<V>;
            if constexpr (can_inplace<T>)
            {
                new (&buffer) T(std::forward(args)...);
                // the void* should be interpreted as the object, not pointer
                deleter = [](void*& data) { ((T*)(&data))->~T(); };
                copier = [](void*& from, void*& to) { new (&to) T(*((T*)&from)); };
                mover = [](void*& from, void*& to) { new (&to) T(move(*((T*)&from))); };
            }
            else
            {
                buffer = new T(std::forward(args)...);
                deleter = [](void*& data) { delete ((T*)data); };
                copier = [](void*& from, void*& to) { to = new T(*((T*)from)); };
                mover = [](void*& from, void*& to) { to = new T(move(*((T*)from))); };
            }
        }

        template <typename V, typename U, typename... Args>
        explicit any(std::in_place_type_t<V>, std::initializer_list<U> il, Args&&... args)
            : is_inplace(can_inplace<std::decay_t<V>>)
        {
            using T = std::decay_t<V>;
            if constexpr (can_inplace<T>)
            {
                new (&buffer) T(il, std::forward(args)...);
                // the void* should be interpreted as the object, not pointer
                deleter = [](void*& data) { ((T*)(&data))->~T(); };
                copier = [](void*& from, void*& to) { new (&to) T(*((T*)&from)); };
                mover = [](void*& from, void*& to) { new (&to) T(move(*((T*)&from))); };
            }
            else
            {
                buffer = new T(il, std::forward(args)...);
                deleter = [](void*& data) { delete ((T*)data); };
                copier = [](void*& from, void*& to) { to = new T(*((T*)from)); };
                mover = [](void*& from, void*& to) { to = new T(move(*((T*)from))); };
            }
        }

        any& operator=(const any& rhs)
        {
            deleter = rhs.deleter;
            copier = rhs.copier;
            mover = rhs.mover;
            is_inplace = rhs.is_inplace;

            if (rhs.has_value())
                copier(rhs.buffer, buffer);
            return *this;
        }

        any& operator=(any&& rhs) noexcept
        {
            deleter = rhs.deleter;
            copier = rhs.copier;
            mover = rhs.mover;
            is_inplace = rhs.is_inplace;

            if (rhs.has_value())
                mover(rhs.buffer, buffer);
            rhs.reset();
            return *this;
        }

        template <typename V>
        any& operator=(V&& rhs)
        {
            using T = std::decay_t<V>;
            is_inplace = can_inplace<T>;
            if constexpr (can_inplace<T>)
            {
                new (&buffer) T(std::forward(rhs));
                // the void* should be interpreted as the object, not pointer
                deleter = [](void*& data) { ((T*)(&data))->~T(); };
                copier = [](void*& from, void*& to) { new (&to) T(*((T*)&from)); };
                mover = [](void*& from, void*& to) { new (&to) T(move(*((T*)&from))); };
            }
            else
            {
                buffer = new T(std::forward(rhs));
                deleter = [](void*& data) { delete ((T*)data); };
                copier = [](void*& from, void*& to) { to = new T(*((T*)from)); };
                mover = [](void*& from, void*& to) { to = new T(move(*((T*)from))); };
            }
            return *this;
        }

        inline ~any() { reset(); }

        template <typename V, typename... Args>
        std::decay_t<V>& emplace(Args&&... args)
        {
            using T = std::decay_t<V>;
            is_inplace = can_inplace<T>;
            reset();
            if constexpr (can_inplace<T>)
            {
                new (&buffer) T(std::forward<Args>(args)...);
                // the void* should be interpreted as the object, not pointer
                deleter = [](void*& data) { ((T*)(&data))->~T(); };
                copier = [](void*& from, void*& to) { new (&to) T(*((T*)&from)); };
                mover = [](void*& from, void*& to) { new (&to) T(move(*((T*)&from))); };
                return *((T*)&buffer);
            }
            else
            {
                buffer = new T(std::forward<Args>(args)...);
                deleter = [](void*& data) { delete ((T*)data); };
                copier = [](void*& from, void*& to) { to = new T(*((T*)from)); };
                mover = [](void*& from, void*& to) { to = new T(move(*((T*)from))); };
            }
        }

        void reset() noexcept
        {
            if (has_value())
                deleter(buffer);
            buffer = nullptr;
            is_inplace = false;
        }

        void swap(any& rhs) noexcept
        {
            int mode = is_inplace + 2 * rhs.is_inplace;
            void* tmp;
            switch (mode)
            {
            case 0:
                std::swap(buffer, rhs.buffer);
                break;
            case 1:
                tmp = rhs.buffer;
                mover(buffer, rhs.buffer);
                buffer = tmp;
                break;
            case 2:
                tmp = buffer;
                rhs.mover(rhs.buffer, buffer);
                rhs.buffer = tmp;
                break;
            case 3:
                rhs.mover(rhs.buffer, tmp);
                mover(buffer, rhs.buffer);
                rhs.mover(tmp, buffer);
                break;
            }

            std::swap(deleter, rhs.deleter);
            std::swap(copier, rhs.copier);
            std::swap(mover, rhs.mover);
            std::swap(is_inplace, rhs.is_inplace);
        }

        bool has_value() const noexcept { return !is_inplace && buffer != nullptr; }
    };

    template <typename T>
    T any_cast(const any& operand)
    {
        using U = std::remove_cv_t<std::remove_reference_t<T>>;
        if (operand.is_inplace)
            return static_cast<T>(*std::any_cast<U>(&operand));
    }

    template <typename T>
    T any_cast(any& operand)
    {
        using U = std::remove_cv_t<std::remove_reference_t<T>>;
        if (operand.is_inplace)
            return static_cast<T>(*std::any_cast<U>(&operand));
    }

    template <typename T>
    T any_cast(any&& operand)
    {
        using U = std::remove_cv_t<std::remove_reference_t<T>>;
        static_cast<T>(std::move(*std::any_cast<U>(&operand)));
    }

    template <typename T>
    const T* any_cast(const any* operand) noexcept
    {
        if (!operand)
            return nullptr;
        if (operand->buffer == nullptr)
            return nullptr;

        if (operand->is_inplace)
            return (T*)&operand->buffer;
        return (T*)operand->buffer;
    }

    template <typename T>
    T* any_cast(any* operand) noexcept
    {
        if (!operand)
            return nullptr;
        if (operand->buffer == nullptr)
            return nullptr;

        if (operand->is_inplace)
            return (T*)&operand->buffer;
        return (T*)operand->buffer;
    }

    template <typename T, typename... Args>
    std::any make_any(Args&&... args)
    {
        return std::any(std::in_place_type<T>, std::forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
    std::any make_any(std::initializer_list<U> il, Args&&... args)
    {
        return std::any(std::in_place_type<T>, il, std::forward<Args>(args)...);
    };

    inline void swap(any& lhs, any& rhs) noexcept { lhs.swap(rhs); }
} // namespace std

#endif
