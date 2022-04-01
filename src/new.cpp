#include "../bits/user_implement.h"
#include <cstddef.h>
#include <new.h>

using namespace std;

[[nodiscard]] void *operator new(size_t size) {
  void *ptr = detail::malloc(size);
  if (ptr == nullptr)
    detail::errors::__stdexcept_bad_alloc();
  return ptr;
}

[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new(size_t size, align_val_t align) {
  void *ptr = detail::aligned_malloc(size, align);
  if (ptr == nullptr)
    detail::errors::__stdexcept_bad_alloc();
  return ptr;
}

[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new(size_t size, const nothrow_t &) noexcept {
  void *ptr = detail::malloc(size);
  if (ptr == nullptr)
    detail::errors::__stdexcept_bad_alloc();
  return ptr;
}

[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new(size_t size, align_val_t align, const nothrow_t &) {
  void *ptr = detail::aligned_malloc(size, align);
  if (ptr == nullptr)
    detail::errors::__stdexcept_bad_alloc();
  return ptr;
}

[[nodiscard]] void *operator new[](size_t) {}
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new[](size_t, align_val_t);
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new[](size_t, const nothrow_t &) noexcept;
[[nodiscard, gnu::malloc, gnu::alloc_size(1)]] void *
operator new[](size_t, align_val_t, const nothrow_t &);

void operator delete(void *) noexcept;
void operator delete(void *, align_val_t) noexcept;
void operator delete(void *, const nothrow_t &) noexcept;
void operator delete(void *, align_val_t, const nothrow_t &) noexcept;
void operator delete(void *, size_t) noexcept;
void operator delete(void *, size_t, align_val_t) noexcept;

void operator delete[](void *) noexcept;
void operator delete[](void *, align_val_t) noexcept;
void operator delete[](void *, const nothrow_t &) noexcept;
void operator delete[](void *, align_val_t, const nothrow_t &) noexcept;
void operator delete[](void *, size_t) noexcept;
void operator delete[](void *, size_t, align_val_t) noexcept;
