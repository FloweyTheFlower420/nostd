#ifndef __NOSTDLIB_NEW_H__
#define __NOSTDLIB_NEW_H__
#include <cstddef.h>

namespace std {
enum class align_val_t : size_t {};

struct nothrow_t {
  explicit constexpr nothrow_t() = default;
};
inline constexpr nothrow_t nothrow{};

using new_handler = void (*)();

new_handler set_new_handler(new_handler) throw();
new_handler get_new_handler() noexcept;

struct destroying_delete_t {
  explicit destroying_delete_t() = default;
};
inline constexpr destroying_delete_t destroying_delete{};
} // namespace std

#if __has_builtin(__builtin_launder)
namespace std {
template <typename _Tp>
[[nodiscard]] constexpr _Tp *launder(_Tp *__p) noexcept {
  return __builtin_launder(__p);
}

template <typename _Ret, typename... _Args>
void launder(_Ret (*)(_Args...)) = delete;
template <typename _Ret, typename... _Args>
void launder(_Ret (*)(_Args......)) = delete;

void launder(void *) = delete;
void launder(const void *) = delete;
void launder(volatile void *) = delete;
void launder(const volatile void *) = delete;
} // namespace std
#endif

[[nodiscard]] void *operator new(std::size_t) throw(std::bad_alloc);
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new(std::size_t, std::align_val_t);
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new(std::size_t, const std::nothrow_t &) noexcept;
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new(std::size_t, std::align_val_t, const std::nothrow_t &);

[[nodiscard]] void *operator new[](std::size_t) throw(std::bad_alloc);
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new[](std::size_t, std::align_val_t);
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new[](std::size_t, const std::nothrow_t &) noexcept;
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new[](std::size_t, std::align_val_t, const std::nothrow_t &);

void operator delete(void *) noexcept;
void operator delete(void *, std::align_val_t) noexcept;
void operator delete(void *, const std::nothrow_t &) noexcept;
void operator delete(void *, std::align_val_t, const std::nothrow_t &) noexcept;
void operator delete(void *, std::size_t) noexcept;
void operator delete(void *, std::size_t, std::align_val_t) noexcept;

void operator delete[](void *) noexcept;
void operator delete[](void *, std::align_val_t) noexcept;
void operator delete[](void *, const std::nothrow_t &) noexcept;
void operator delete[](void *, std::align_val_t,
                       const std::nothrow_t &) noexcept;
void operator delete[](void *, std::size_t) noexcept;
void operator delete[](void *, std::size_t, std::align_val_t) noexcept;

[[nodiscard]] inline void *operator new(std::size_t, void *p) noexcept {
  return p;
}
[[nodiscard]] inline void *operator new[](std::size_t, void *p) noexcept {
  return p;
}

inline void operator delete(void *, void *) noexcept {}
inline void operator delete[](void *, void *) noexcept {}

#endif
