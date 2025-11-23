#ifndef MODERN_CPP_PART_TWO_H
#define MODERN_CPP_PART_TWO_H
// Algorithm call with predicate
// Using lambda expression
#include <iostream>
#include <algorithm>
#include <vector>

void part_two()
{
    int divisor = 0;
    while(divisor == 0)
    {
        std::cout << "Enter divisor:= ";
        std::cin >> divisor;
        std::cout << std::endl;
    }
    std::vector<int> vec{4, 2, 3, 5, 1};

    std::cout << "Elements of vector: ";

    for (auto i : vec)
        std::cout << i << ", ";
    std::cout << '\n';

    // Call std::count_if() algorithm using a lambda expression as predicate
    auto n_even = std::count_if(vec.begin(), vec.end(),

                                // The lambda definition goes inside the call!
                                [&divisor] (int n)
                                {
                                    return (n % divisor == 0);
                                }
                                );

    std::cout << "The vector has " << n_even << " element(s) with even values\n";
}
#endif // MODERN_CPP_PART_TWO_H
