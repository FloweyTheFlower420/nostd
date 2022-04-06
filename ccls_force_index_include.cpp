#include <array>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <forward_list>
#include <initializer_list>
#include <iterator>
#include <memory>
//#include <memory_region.h>
#include <new>
#include <printf.h>
#include <backtrace.h>
#include <panic.h>
#include <source_location>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

static_assert(std::is_trivially_destructible<char>::value);
