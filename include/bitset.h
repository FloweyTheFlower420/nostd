#ifndef __NOSTD_BITSET_H__
#define __NOSTD_BITSET_H__
#include <cstddef.h>
#include <string.h>

namespace std {
namespace std {
template <size_t N> class bitset {
public:
  // bit reference
  class reference {
    friend class bitset;
    reference() noexcept;

  public:
    reference(const reference &) = default;
    ~reference();
    reference &operator=(bool x) noexcept;            // for b[i] = x;
    reference &operator=(const reference &) noexcept; // for b[i] = b[j];
    bool operator~() const noexcept;                  // flips the bit
    operator bool() const noexcept;                   // for x = b[i];
    reference &flip() noexcept;                       // for b[i].flip();
  };

  // constructors
  constexpr bitset() noexcept;
  constexpr bitset(unsigned long long val) noexcept;
  template <class CharT, class Traits, class Allocator>
  explicit bitset(
      const basic_string<CharT, Traits, Allocator> &str,
      typename basic_string<CharT, Traits, Allocator>::size_type pos = 0,
      typename basic_string<CharT, Traits, Allocator>::size_type n =
          basic_string<CharT, Traits, Allocator>::npos,
      CharT zero = CharT('0'), CharT one = CharT('1'));
  template <class CharT>
  explicit bitset(
      const charT *str,
      typename basic_string<CharT>::size_type n = basic_string<CharT>::npos,
      CharT zero = CharT('0'), CharT one = CharT('1'));

  // bitset operations
  bitset &operator&=(const bitset &rhs) noexcept;
  bitset &operator|=(const bitset &rhs) noexcept;
  bitset &operator^=(const bitset &rhs) noexcept;
  bitset &operator<<=(size_t pos) noexcept;
  bitset &operator>>=(size_t pos) noexcept;
  bitset &set() noexcept;
  bitset &set(size_t pos, bool val = true);
  bitset &reset() noexcept;
  bitset &reset(size_t pos);
  bitset operator~() const noexcept;
  bitset &flip() noexcept;
  bitset &flip(size_t pos);

  // element access
  constexpr bool operator[](size_t pos) const; // for b[i];
  reference operator[](size_t pos);            // for b[i];

  unsigned long to_ulong() const;
  unsigned long long to_ullong() const;
  template <class CharT = char, class Traits = char_Traits<CharT>,
            class Allocator = allocator<CharT>>
  basic_string<CharT, Traits, Allocator>
  to_string(CharT zero = CharT('0'), CharT one = CharT('1')) const;

  size_t count() const noexcept;
  constexpr size_t size() const noexcept;
  bool operator==(const bitset &rhs) const noexcept;
  bool test(size_t pos) const;
  bool all() const noexcept;
  bool any() const noexcept;
  bool none() const noexcept;
  bitset operator<<(size_t pos) const noexcept;
  bitset operator>>(size_t pos) const noexcept;
};

// hash support
template <class T> struct hash;
template <size_t N> struct hash<bitset<N>>;
} // namespace std
// bitset operators
template <size_t N>
bitset<N> operator&(const bitset<N> &, const bitset<N> &) noexcept;
template <size_t N>
bitset<N> operator|(const bitset<N> &, const bitset<N> &) noexcept;
template <size_t N>
bitset<N> operator^(const bitset<N> &, const bitset<N> &) noexcept;
template <class CharT, class Traits, size_t N>
basic_istream<CharT, Traits> &operator>>(basic_istream<CharT, Traits> &is,
                                         bitset<N> &x);
template <class CharT, class Traits, size_t N>
basic_ostream<CharT, Traits> &operator<<(basic_ostream<CharT, Traits> &os,
                                         const bitset<N> &x);
} // namespace std

#endif
