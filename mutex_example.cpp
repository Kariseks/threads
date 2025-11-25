#include "mutex_example.h"
#include "quicksort.h"
#include <barrier>
#include <mutex>
#include <random>
//=====================================================================================================================
using namespace std;
//=====================================================================================================================
void thread_info()
{

    cout << "ID of this thread is: " << this_thread::get_id() << "\t this number dosen't corespond to process ID of OS, because this"
            << "number is provided by thread library, not by the kernel" << endl;
    cout << "Kernel see " << thread::hardware_concurrency() << " logic thread" << endl;

}
//=====================================================================================================================
std::vector<int> make_int_vector(int min_val, int max_val, std::size_t count) {
    if (min_val > max_val) {
        throw std::invalid_argument("min_val must be <= max_val");
    }
    std::random_device rd;                  // źródło ziarna (może delegować do systemowego RNG)
    std::mt19937 gen(rd());                 // silnik Mersenne Twister z ziarnem z rd
    std::uniform_int_distribution<int> dist(min_val, max_val); // rozkład całkowity na [min,max]
    std::vector<int> v;
    v.reserve(count);
    for (std::size_t i = 0; i < count; ++i) {
        v.push_back(dist(gen));
    }
    return v;
}
//=====================================================================================================================
void switching_threads()
{
    cout << "This example shows how to synchronize etap of multiple threads" << endl;

    auto v = make_int_vector(-100,100, 1'000'000);  //orginal
    auto v1 = v;    // copy of v for first thread
    auto v2 = v;    //copy of v for second thread

    constexpr size_t threads_num {2};
    std::barrier barrier{threads_num};  //locks the threads beetwen etaps
    std::mutex celebration;             //block other threads from interrupting a celebration

    jthread first{ [&barrier, &v1,&celebration]() ->void
        {
            quickSort(0, v1.size()-1, v1.data());
            celebration.lock(); //first thread dosen't use RAII
                cout << "First thread ended sorting" << endl;
            celebration.unlock();
            //-----------------------------------------------------------------------------------
            barrier.arrive_and_wait();  //waiting for every one to complete their task

            long sum = 0;
            for(auto iter = v1.begin(); iter != v1.end(); ++iter)
            {
                sum += *iter;
                this_thread::yield();   //i want the first to end this stage slower
            }
            celebration.lock();
                cout << "First thread ended counting the sum, it is:= "<< sum << endl;
            celebration.unlock();
        }
    };
    jthread second{ [&barrier, &v2,&celebration]() ->void
        {
            quickSort(0, v2.size()-1, v2.data());
            this_thread::sleep_for(chrono::milliseconds(250));  //i want this thread to end this stage slower
            celebration.lock(); //first thread dosen't use RAII
            cout << "Second thread ended sorting" << endl;
            celebration.unlock();
            //-----------------------------------------------------------------------------------
            barrier.arrive_and_wait();  //waiting for every one to complete their task

            long sum = 0;
            for(auto iter = v2.begin(); iter != v2.end(); ++iter)
            {
                sum += *iter;
            }
            celebration.lock();
            cout << "Second thread ended counting the sum, it is:= "<< sum << endl;
            celebration.unlock();
        }
    };

    first.join();
    second.join();  //both of them should end before end of the program

}
//=====================================================================================================================
void producer(std::timed_mutex & mtx)
{


}
//---------------------------------------------------------------------------------------------------------------------
void consumer(std::timed_mutex & mtx)
{

}
//---------------------------------------------------------------------------------------------------------------------
void mutex_timed()
{

}






















