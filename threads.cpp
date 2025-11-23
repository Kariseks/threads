#include "threads.h"

#include <iostream>
#include <stop_token>
#include <thread>

using namespace std;

void uninterruptable()
{
    auto counter = 0;

    while(counter < 10)
    {
        cout << "Uninterruptable thread iteration:= " << counter << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        ++counter;
    }
}
void interruptable(std::stop_token stoken)
{
    auto counter = 0;

    while(counter < 10)
    {
        if(stoken.stop_requested()) //check if somone has setted the token
            return;
        cout << "Interruptable thread iteration:= " << counter << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        ++counter;

    }
}

void interrupting_example()
{
    cout << R"(This is an example to show how to stop a thread. \nYou can only set this flag once, so when it is set, you should return from the thread)" <<endl;

    jthread uninter{uninterruptable};
    jthread inter{interruptable};

    this_thread::sleep_for(chrono::milliseconds(1000));
    inter.request_stop();
    uninter.request_stop();
    cout << "---------------------------------" << endl;
    cout << "stop requested" << endl;
    cout << "---------------------------------" << endl;

}
