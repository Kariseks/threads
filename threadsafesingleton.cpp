#include "threadsafesingleton.h"
#include <mutex>

using namespace std;

ThreadSafeSingleton *ThreadSafeSingleton::getInstance()
{
    static ThreadSafeSingleton * instance{nullptr};
    static mutex mtx;



    if(lock_guard{mtx}; instance == nullptr)
    {
        instance = new ThreadSafeSingleton{};
    }
    return instance;
}

ThreadSafeSingleton::~ThreadSafeSingleton()
{
    //this should live as long as the program, so don't need to release allocated memeor for this instance
}
