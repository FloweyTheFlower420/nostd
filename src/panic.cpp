#include <panic.h>

namespace std
{
    [[noreturn]] void panic(const char* msg)
    {
        printf("panic: %s", msg);
        printf("stacktrace:");
        static void* stack_buffer[0x400];
        size_t n = backtrace(0, 0x400, stack_buffer);
        for(int i = 0; i < n; i++)
        {
            printf("#%d: %p\n", i, stack_buffer[i]);
        }

        detail::errors::__halt();
    }

}
