#include "report.h"

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int Report::stat_var = 0;


Report::Report() {}




void Report::report_bigger(std::vector<int> vec)
{
    cout << "Inside member function, using private fields. \"this\" allows to use class fileds " << endl;
    int counter = 0;
    auto result = count_if(vec.begin(), vec.end(), [&counter,this](int j)->bool
        {
            ++priv_var;
            ++counter;
            return j > priv_threshold ? true : false;
        });
    cout << "The result of counting is:= " << result << ", counted:= " << counter << " items of given vec" << endl;

}

void Report::report_smaller(std::vector<int> vec)
{
    cout << "Inside member function, using public fields. It required \"this\" for public fields too" << endl;
    int counter = 0;
    auto result = count_if(vec.begin(), vec.end(), [this, &counter /*, &stat_var*/](int j)->bool
                           {
                               ++priv_var;
                               ++counter;
                               return j > priv_threshold ? true : false;
                           });
    cout << "The result of counting is:= " << result << ", counted:= " << counter << " items of given vec" << endl;
}

void Report::report(std::vector<int> vec)
{

}
