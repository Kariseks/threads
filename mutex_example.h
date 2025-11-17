#ifndef MUTEX_EXAMPLE_H
#define MUTEX_EXAMPLE_H
//=====================================================================================================================
#include <iostream>
#include <thread>
#include<vector>
//=====================================================================================================================
//=====================================================================================================================
//=====================================================================================================================
template<typename T>
void mergeSort(std::vector<T> vec)
{

}
//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void increment(T & obj, std::size_t n)
{
    while(n > 0)
    {
        ++obj;
        --n;
    }
}
//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void increment_raw_example()
{
    constexpr std::size_t n = 1'000'000;    //how many times the variable should be increased
    T obj{0};

    std::jthread first{[&obj, n] ()->void{increment(obj,n);}};
    std::jthread second{[&obj, n] ()->void{increment(obj,n);}};

    first.join();
    second.join();

    std::cout << "This function should increment the variable " << (n<<1) << " times, but only increased it " << obj << " times." << std::endl;

}
//=====================================================================================================================
template<typename T>
void increment_mutex(T & obj, std::size_t n, std::mutex & mutex)
{
    while(n > 0)
    {
        mutex.lock();   //it is important to lock the smallest possible region, that is critical due to race conditions
        ++obj;
        mutex.unlock();

        --n;
    }
}
//---------------------------------------------------------------------------------------------------------------------
template<typename T>
void increment_mutex_example()
{
    constexpr std::size_t n = 1'000'000;    //how many times the variable should be increased
    T obj{0};
    std::mutex mutex;
    std::jthread first{increment_mutex<T>, std::ref(obj), n, std::ref(mutex)};
    std::jthread second{increment_mutex<T>, std::ref(obj), n, std::ref(mutex)};

    first.join();
    second.join();

    std::cout << "This function with mutex should increment the variable " << (n<<1) << " times, var increded " << obj << " times." << std::endl;

}
//=====================================================================================================================
#endif // MUTEX_EXAMPLE_H
