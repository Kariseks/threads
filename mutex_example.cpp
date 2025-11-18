#include "mutex_example.h"
#include <barrier>
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

void switching_threads()
{
    cout << "This example shows how to synchronize two threads with atomic flag" << endl;
    constexpr size_t threads_num {2};
    std::barrier barrier{threads_num};
    //jthread first{[]->void()};

}
