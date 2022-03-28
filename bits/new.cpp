#include "user_implement.h"
#include <cstddef.h>
#include <new.h>

void* operator new(std::size_t count)
{
    return std::detail::malloc(count);
}
void* operator new[](std::size_t count)
{
    return std::detail::malloc(count);
}
void* operator new(std::size_t count, std::align_val_t al)
{
    return std::detail::aligned_alloc(count, al);
}
void* operator new[](std::size_t count, std::align_val_t al);
