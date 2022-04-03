#ifndef __NOSTDLIB_ITERATOR_H__
#define __NOSTDLIB_ITERATOR_H__

#include "../bits/insert_iterator.h"
#include "../bits/move_iterator.h"
#include "../bits/reverse_iterator.h"

namespace std
{
    template <typename It>
    constexpr typename iterator_traits<It>::difference_type distance(It first, It end)
    {
        if constexpr (is_same_v<typename iterator_traits<It>::iterator_category, random_access_iterator_tag>)
            return end - first;
        else
        {
            typename iterator_traits<It>::difference_type n = 0;
            for (; first != end; first++)
                n++;
            return n;
        }
    }
} // namespace std

#endif
