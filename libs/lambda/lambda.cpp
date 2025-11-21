#include "lambda.h"


using namespace std;

Lambda::Lambda() {}

int Lambda::math_operation(int nothing, std::function<int (double)> operation)
{
    if (operation == nullptr)
            return 0;

    int result = nothing;
    for(int i=0; i<nothing; ++i)
    {
        result = operation(result);
    }
    return result;
}
