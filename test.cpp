#include <iostream>

#include "include/smvector.hpp"


void var1()
{
    smv::smvector<int> arr1;
    for (int i = 1; i <= 20; ++i) 
    {
        if (i % 2 == 0)
        {
            arr1.push_back(i, 1);
        }
        else
        {
            arr1.push_front(i, 1);
        }
        std::cout << arr1.size() << '/' << arr1.capacity() << ' ';
    }
    std::cout << std::endl;

    for (const auto& value : arr1) 
    {
        std::cout << value << ' ';
    }
    std::cout << '\n' << std::endl;
}


void var2()
{
    smv::smvector<int> arr2;
    arr2.set_growth_factor(1.5);
    for (int i = 1; i <= 20; ++i) 
    {
        if (i % 2 == 0)
        {
            arr2.push_back(i);
        }
        else
        {
            arr2.push_front(i);
        }
        std::cout << arr2.size() << '/' << arr2.capacity() << ' ';
    }
    std::cout << std::endl;

    for (const auto& value : arr2) 
    {
        std::cout << value << ' ';
    }
    std::cout << std::endl;
}


int main() 
{
    var1();
    var2();

    return 0;
}