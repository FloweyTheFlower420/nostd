#ifndef __NOSTDLIB_BACKTRACE_H__
#define __NOSTDLIB_BACKTRACE_H__
#include <cstddef>

namespace std
{
    void backtrace(size_t skip, size_t count, void** buf, size_t* base_ptr = nullptr)
    {
        __asm__ __volatile__("movq %%rbp, %0" : "=g"(base_ptr) : : "memory" );

        while(true) 
        {
            size_t old_bp = base_ptr[0];
            size_t ret_addr = base_ptr[1];
            if (!ret_addr)
                break;

            if(skip != 0)
                skip--;
            else if(count != 0)
            {
                *buf++ = (void*)ret_addr;
                count--;
            }
            
            if (!old_bp)
                break;

            base_ptr = (size_t*) old_bp;
        }
    }
};

#endif
