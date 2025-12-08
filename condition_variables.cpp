#include "condition_variables.h"
//=====================================================================================================================
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
//=====================================================================================================================
using namespace std;
//=====================================================================================================================
void do_work();
void wait_for_work(mutex & mtx, bool & is_data_ready, condition_variable & c_var);
void set_data_ready(mutex & mtx, bool & is_data_ready, condition_variable & c_var);
//---------------------------------------------------------------------------------------------------------------------
void simple_condition_variable_example()
{
    mutex mtx;
    condition_variable c_var;
    bool is_data_ready{false};


    jthread t1 {wait_for_work, std::ref(mtx), std::ref(is_data_ready), std::ref(c_var)};

    this_thread::sleep_for(chrono::seconds(5));

    jthread t2 {set_data_ready, std::ref(mtx), std::ref(is_data_ready), std::ref(c_var)};
}
//---------------------------------------------------------------------------------------------------------------------
/*!
 * \brief do_work this function is responsible for processing shared data
 */
void do_work()
{
    cout << "Here shared data would be processed" << endl;
}
//---------------------------------------------------------------------------------------------------------------------
/*!
 * \brief wait_for_work this function ise responsible for
 *  - locking the mutex
 */
void wait_for_work(mutex & mtx, bool & is_data_ready, condition_variable & c_var)
{
    cout << "Waiting for work" << endl;
    unique_lock lock(mtx);
    c_var.wait(lock, [&]{ return is_data_ready; });

    do_work();
    cout << "Work done" << endl;
}
//---------------------------------------------------------------------------------------------------------------------
void set_data_ready(mutex & mtx, bool & is_data_ready, condition_variable & c_var)
{
    {
        lock_guard lock{mtx};
        is_data_ready = true;   //has to be changed under mutex, because otherwise the Consumer could be after checking predicate, and before waiting. this lead to lost wakeup
    }
    cout << "Data is prepared and set as ready, sending notification" << endl;
    c_var.notify_one();
}
//---------------------------------------------------------------------------------------------------------------------
