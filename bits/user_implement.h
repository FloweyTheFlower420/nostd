#ifndef __NOSTDLIB_USER_IMPLEMENT_H__
#define __NOSTDLIB_USER_IMPLEMENT_H__
#include <cstddef>
namespace std::detail
{
    void* malloc(size_t size);
    void* aligned_malloc(size_t size, max_align_t align);
    void free();
    void putc(char ch);

    namespace errors
    {
        [[noreturn]] void __stdexcept_out_of_range();
        [[noreturn]] void __stdexcept_bad_alloc();
        [[noreturn]] void __stdexcept_bad_variant_access();
        [[noreturn]] void __printf_argument_notfound();
        [[noreturn]] void __printf_undefined_specifier_for_length();
    } // namespace errors
} // namespace std::detail
#endif
