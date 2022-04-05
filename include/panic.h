#ifndef __NOSTDLIB_PANIC_H__
#define __NOSTDLIB_PANIC_H__
#include <backtrace.h>
#include <printf.h>
#include "../bits/user_implement.h"

namespace std
{
    [[noreturn]] void panic(const char* msg);
}

#endif
