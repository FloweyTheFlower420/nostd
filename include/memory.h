#ifndef __NOSTDLIB_MEMORY_H__
#define __NOSTDLIB_MEMORY_H__
#include "../bits/unique_ptr.h"

namespace std {
template <typename T>
typename std::enable_if<std::is_object<T>::value, T *>::type
addressof(T &arg) noexcept {
  return reinterpret_cast<T *>(
      &const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}

template <typename T>
typename std::enable_if<!std::is_object<T>::value, T *>::type
addressof(T &arg) noexcept {
  return &arg;
}
} // namespace std

#endif
