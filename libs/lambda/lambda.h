#ifndef LAMBDA_H
#define LAMBDA_H

#include <functional>
#include <iostream>

class Lambda
{
public:
    Lambda();
    int math_operation(int nothing, std::function<int(double)> operation = [](double arg)->int{
            std::cout << "\t inside default lambda. Got " << arg << ", as an argument" << std::endl;
            return arg*arg;});
};

#endif // LAMBDA_H
