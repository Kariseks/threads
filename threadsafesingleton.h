#ifndef THREADSAFESINGLETON_H
#define THREADSAFESINGLETON_H

class ThreadSafeSingleton
{
public:
    ThreadSafeSingleton() = default;
    ThreadSafeSingleton(const ThreadSafeSingleton &) = delete;
    void operator=(const ThreadSafeSingleton &) = delete;
    ThreadSafeSingleton * getInstance();
    ~ThreadSafeSingleton();
private:


};

#endif // THREADSAFESINGLETON_H
