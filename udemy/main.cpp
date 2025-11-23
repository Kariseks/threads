#include "modern_cpp_part_two.h"
#include <iostream>
#include <vector>
/*
 * Write a program which:
 * Defines a vector whose elements have type int. Use universal initialization to set its values to 4, 2, 3, 5 and 1, in that order.
 * Uses a range-for loop to display all the elements of the vector in order. Use the auto keyword for the type of the elements.
 * Uses a range-for loop to add 2 to all the elements.
 * Uses a range-for loop to display all the elements of the vector again.
 */
using namespace std;
int main(int argc, char *argv[])
{
    part_two();
    // Defines a vector whose elements have type int. Use universal initialization to set its values to 4, 2, 3, 5 and 1, in that order.
    vector<int> vec{4,2,3,5,1};
    // Uses a range-for loop to display all the elements of the vector in order. Use the auto keyword for the type of the elements
    for( auto elem : vec)
        cout << elem << endl;

    cout << endl;
    //Uses a range-for loop to add 2 to all the elements.
    for (auto & elem : vec)
        elem +=2;

    // Uses a range-for loop to display all the elements of the vector again.
    for( auto elem : vec)
        cout << elem << endl;

}
