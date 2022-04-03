#ifndef __NOSTDLIB_SOURCE_LOCATION_H__
#define __NOSTDLIB_SOURCE_LOCATION_H__
#include <cstddef>

namespace std
{
    struct source_location
    {
        static constexpr source_location current(size_t l = __builtin_LINE(), size_t c = __builtin_COLUMN(),
                                                 const char* filename = __builtin_FILE(),
                                                 const char* funcname = __builtin_FUNCTION()) noexcept
        {
            return source_location(l, c, filename, funcname);
        };

        constexpr source_location(size_t l = __builtin_LINE(), size_t c = __builtin_COLUMN(),
                                  const char* filename = __builtin_FILE(), const char* funcname = __builtin_FUNCTION())
            : l(l), c(c), filename(filename), funcname(funcname)
        {
        }

        constexpr size_t line() const noexcept { return l; }
        constexpr size_t column() const noexcept { return c; }
        constexpr const char* file_name() const noexcept { return filename; }
        constexpr const char* function_name() const noexcept { return funcname; }

    private:
        size_t l;
        size_t c;
        const char* filename;
        const char* funcname;
    };
} // namespace std

#endif
