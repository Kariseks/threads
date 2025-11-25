#include "mutex_example.h"
#include "quicksort.h"
#include <barrier>
#include <mutex>
#include <queue>
#include <random>

//=====================================================================================================================
using namespace std;
using namespace chrono_literals;
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
auto max_wait_time = 15ms;
int8_t * buffer = nullptr;
int rand_10_20() {
    static std::mt19937 gen(std::random_device{}()); // raz inicjalizowany
    static std::uniform_int_distribution dist(10, 20); // CTAD: int się sam wywnioskuje
    return dist(gen);
}
void producer(std::timed_mutex & mtx, mutex & mtx_speaker,  atomic<bool> & is_done, unsigned long & counter)
{
    while( ! is_done.load())
    {

        std::this_thread::sleep_for(20ms);  //simulate preparing frame for GPU
        //load frame
        if(unique_lock<timed_mutex> lock(mtx, std::defer_lock);
            lock.try_lock_for(max_wait_time))
        {
            //put prepared frame in buffer for GPU.
            if( ! buffer)   //GPU didn't read the previous frame, log the drop
                delete[] buffer;

            buffer = new int8_t[60];
            this_thread::sleep_for(2ms);    //simulate work
            ++counter;

        }
        else    //new frame arrived->drop this frame, log the dropping
        {
            this_thread::sleep_for(1ms);    //simulate logging the drop
            unique_lock lock_speaker{mtx_speaker};
            cout << "Frame := " << ++counter << ", dropped"  << endl;
        }
    }

}
//---------------------------------------------------------------------------------------------------------------------
void consumer(std::timed_mutex & mtx, mutex & mtx_speaker, atomic<bool> & is_done, unsigned long & counter)
{
    while( ! is_done.load())
    {
        //cosumer have to wait till new frame is ready, would be better with variable_condition

        if( ! buffer)   //frame not ready
        {
            unique_lock lock_speaker{mtx_speaker};
            cout << "Consumer is waiting for a frame" << endl;
        }

        auto working_time = chrono::milliseconds(rand_10_20());
        {//sending frame to gpu
            unique_lock<timed_mutex> lock{mtx};
            this_thread::sleep_for(working_time);    //simulate transfering the frame to gpu;
            cout << "Frame:= " << ++counter << ", sended to GPU"  << endl;
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
void mutex_timed_example()
{
    cout << "Example with timed mutex" << endl;
    timed_mutex mtx_timed{};
    mutex mtx_speaker{};
    atomic<bool> is_done{false};
    unsigned long counter = 0;

    jthread pr_1{producer, ref(mtx_timed), ref(mtx_speaker), ref(is_done), ref(counter)};    //camera 1
    jthread pr_2{producer, ref(mtx_timed), ref(mtx_speaker), ref(is_done), ref(counter)};    //camera 2
    jthread pr_3{producer, ref(mtx_timed), ref(mtx_speaker), ref(is_done), ref(counter)};    //camera 3
    jthread pr_4{producer, ref(mtx_timed), ref(mtx_speaker), ref(is_done), ref(counter)};    //camera 4
    jthread con{consumer, ref(mtx_timed), ref(mtx_speaker), ref(is_done), ref(counter)};

    this_thread::sleep_for(10s);

    is_done.store(true);
}






















