#include "mutex_example.h"
using namespace std;

int main()
{
    increment_raw_example<int>();
    increment_mutex_example<int>();

    thread_info();
    switching_threads();

    return 0;
}
