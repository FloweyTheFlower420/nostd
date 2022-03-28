#include <vector.h>

#include "/usr/include/11.2.0/iostream"

int main()
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    for(auto i : vec)
    {
        std::cout << i;
    }
}
