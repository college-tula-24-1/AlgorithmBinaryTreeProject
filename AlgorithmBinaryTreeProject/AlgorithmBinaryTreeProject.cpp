#include <iostream>
#include <iomanip>

int main()
{
    int a{}, b{};
    int* aptr{ &a };
    int* bptr{};
    std::cout << std::boolalpha << (bool)((bool)aptr ^ (bool)bptr);
}
