#include "report.h"

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <algorithm>
#include <thread>
#include <vector>

using namespace std;

int Report::stat_var = 0;


Report::Report() {}




void Report::report_bigger(std::vector<int> &vec)
{
    cout << "Inside member function, using private fields. \"this\" allows to use class fileds " << endl;
    int counter = 0;
    auto result = count_if(vec.begin(), vec.end(), [&counter,this](int j)->bool
        {
            ++priv_var;
            ++counter;
            return j > threshold ? true : false;
        });
    cout << "The result of counting is:= " << result << ", counted:= " << counter << " items of given vec" << endl;

}

void Report::report_smaller(std::vector<int> &vec)
{
    cout << "Inside member function, using public fields. It required \"this\" for public fields too" << endl;
    int counter = 0;
    auto result = count_if(vec.begin(), vec.end(), [this, &counter /*, &stat_var*/](int j)->bool
                           {
                               ++priv_var;
                               ++counter;
                               return j > threshold ? true : false;
                           });
    cout << "The result of counting is:= " << result << ", counted:= " << counter << " items of given vec" << endl;
}

void Report::report(std::vector<int> &vec)
{

}

void Report::live_reporter(std::vector<int> &vec)
{
    cout << "This function calculate a sum of a very large table with live report on progress" << endl;

    cout << "To be able to have an access to actuall value of a variable shared between threads, it has to be volatile or atomic" << endl
         << "The drawback is optimization, the compiler can store this variable in CPU register" << endl;

    auto counted = atomic<int>{0};
    int sum{0};
    auto end_flag = atomic<bool>{false};


    jthread reporter{[&vec, &counted, &end_flag]{
        while (! end_flag.load()) {
            cout << "Summed " << counted.load(std::memory_order::relaxed) << " of " << vec.size() << endl;
            this_thread::sleep_for(chrono::milliseconds(20));
        }
    }};

    jthread counter{[&vec, &counted, &sum,&end_flag]{
        for(auto elem : vec)
        {
            sum +=elem;
            counted.fetch_add(1);
            this_thread::yield();
        }
        end_flag.store(true);
    }};

    counter.join();
    reporter.join();
}
