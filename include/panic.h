#ifndef __NOSTDLIB_PANIC_H__
#define __NOSTDLIB_PANIC_H__
#include "../bits/user_implement.h"
#include <backtrace.h>
#include <printf.h>

namespace std
{
    [[noreturn]] void panic(const char* msg);
}

#endif
