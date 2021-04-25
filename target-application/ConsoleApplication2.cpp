// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
int sum(int a, int b)
{
    int result = 0;
    result = a + b;
    std::cout << " the result is: " << result << std::endl;

    return result;
}
int main()
{
    while (true)
    {
        std::cout << " calling sum: " << std::endl;
        int result = sum(5, 5);
        std::cout << "sum is located at: " << &sum << std::endl;
        Sleep(100);
        system("CLS");
    }
    return 0;
}