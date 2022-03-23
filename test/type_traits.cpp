#include "../type_traits.h"

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ ":" S2(__LINE__)

template <bool t, bool... Bs>
using all_t = std::conjunction<std::bool_constant<t == Bs>...>;

#define ctest(expr) static_assert(expr, LOCATION);
#define ctestf(...) static_assert(all_t<false, __VA_ARGS__>::value, LOCATION);
#define ctestt(...) static_assert(all_t<true, __VA_ARGS__>::value, LOCATION);
#define test(expr)                                                                                                          \
    {                                                                                                                       \
        if (!expr)                                                                                                          \
        {                                                                                                                   \
            std::cerr << "Failed: " << LOCATION << '\n';                                                                    \
            exit(-1);                                                                                                       \
        }                                                                                                                   \
    }
#define testf() test(all_t<false, __VA_ARGS__>::value)
#define testt() test(all_t<true, __VA_ARGS__>::value)
int main()
{
    using namespace std;
    ctestt(is_void<void>::value, is_void_v<void>);
    ctestf(is_void<int>::value, is_void_v<int>);

    ctestt(is_null_pointer<decltype(nullptr)>::value, is_null_pointer_v<decltype(nullptr)>);
    ctestf(is_null_pointer<int*>::value, is_null_pointer_v<int*>);
}
