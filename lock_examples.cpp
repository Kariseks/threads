#include "lock_examples.h"
//=====================================================================================================================
#include <mutex>
#include <thread>
#include <iostream>
//=====================================================================================================================
using namespace std;
//=====================================================================================================================
struct CriticalData
{
    string name;
    mutex mtx;
};

//=====================================================================================================================
//=====================================================================================================================
void dead_lock(CriticalData & data_1, CriticalData & data_2)
{
    lock_guard lock_1{data_1.mtx};
    cout << "Thread: " << this_thread::get_id() << ", has acquired resource: " << data_1.name << endl;

    this_thread::sleep_for(1s);

    lock_guard lock_2{data_2.mtx};
    cout << "Thread: " << this_thread::get_id() << ", has acquired resource: " << data_2.name << endl;

}
void simpleClock(stop_token stoken)
{
    while(! stoken.stop_requested())
    {
        static auto time_start = chrono::steady_clock::now();
        auto time_now = chrono::steady_clock::now();
        auto sec = duration_cast<std::chrono::seconds>(time_now-time_start);
        cout << "Time since start: " << sec.count() << endl;
        this_thread::sleep_for(1s);
    }
}
//---------------------------------------------------------------------------------------------------------------------

void dead_lock_example()
{
    CriticalData  data_1{"data 1"};
    CriticalData  data_2{"data 2"};

    cout << "dead lock example with 2 mutex" << endl;
    jthread clock(simpleClock);
    jthread t1(dead_lock, std::ref(data_1), std::ref(data_2));
    jthread t2(dead_lock, std::ref(data_2), std::ref(data_1));
    this_thread::sleep_for(15s);
    clock.request_stop();
}
//=====================================================================================================================
void solution_lock(CriticalData & data_1, CriticalData & data_2)
{

    std::lock(data_1.mtx, data_2.mtx);

    cout << "Thread: " << this_thread::get_id() << ", has acquired resource: " << data_1.name << endl;
    cout << "Thread: " << this_thread::get_id() << ", has acquired resource: " << data_2.name << endl;
    this_thread::sleep_for(5s);

    data_1.mtx.unlock();
    data_2.mtx.unlock();
    cout << "Thread: " << this_thread::get_id() << ", has released: " << data_1.name << endl;
    cout << "Thread: " << this_thread::get_id() << ", has released: " << data_2.name << endl;

}
//---------------------------------------------------------------------------------------------------------------------
void dead_lock_solution_lock()
{
    CriticalData  data_1{"data 1"};
    CriticalData  data_2{"data 2"};

    cout << "dead lock solution, example with 2 mutex" << endl;
    jthread t1(solution_lock, std::ref(data_1), std::ref(data_2));
    jthread t2(solution_lock, std::ref(data_2), std::ref(data_1));

    jthread clock(simpleClock);
    this_thread::sleep_for(15s);
    clock.request_stop();

}
//=====================================================================================================================
void solution_scoped_lock(CriticalData & data_1, CriticalData & data_2)
{


    cout << "Thread: " << this_thread::get_id() << ", will try to lock: " << data_1.name << endl;
    cout << "Thread: " << this_thread::get_id() << ", will try to lock: " << data_2.name << endl;
    this_thread::sleep_for(5s);

    scoped_lock lock{data_1.mtx, data_2.mtx};
    cout << "Thread: " << this_thread::get_id() << ", acquired: " << data_1.name << endl;
    cout << "Thread: " << this_thread::get_id() << ", acquired: " << data_2.name << endl;

    this_thread::sleep_for(5s);

    cout << "Thread: " << this_thread::get_id() << ", has released: " << data_1.name << endl;
    cout << "Thread: " << this_thread::get_id() << ", has released: " << data_2.name << endl;
}
//---------------------------------------------------------------------------------------------------------------------
void dead_lock_solution_scoped_lock()
{
    CriticalData  data_1{"data 1"};
    CriticalData  data_2{"data 2"};

    cout << "dead lock solution, example with 2 mutex" << endl;
    jthread t1(solution_scoped_lock, std::ref(data_1), std::ref(data_2));
    jthread t2(solution_scoped_lock, std::ref(data_2), std::ref(data_1));

    //jthread clock(simpleClock);
    this_thread::sleep_for(15s);
    //clock.request_stop();
}
//=====================================================================================================================
int counter()
{
    thread_local int cnt = 0;
    return     ++cnt;
}

//---------------------------------------------------------------------------------------------------------------------
void non_lock_example()
{
    mutex coutMutex;

    auto worker = [&coutMutex](stop_token stoken)
        {

            while( ! stoken.stop_requested())
            {
                {
                    lock_guard lock{coutMutex};
                    cout << "Thread: " << this_thread::get_id() << "\t" << counter() << endl;
                }
                this_thread::sleep_for(1s);
            }
        };

    jthread t1(worker);
    this_thread::sleep_for(190ms);
    jthread t2(worker);
    this_thread::sleep_for(205ms);
    jthread t3(worker);

    t1.join();
}













