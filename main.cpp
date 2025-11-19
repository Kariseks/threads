#include "mutex_example.h"
#include "report.h"

using namespace std;

int main()
{
    //increment_raw_example<int>();
    //increment_mutex_example<int>();

    //thread_info();
    //switching_threads();

    Report reporter;

    auto vec = make_int_vector(-100,100, 10'000'000);
    reporter.live_reporter(vec);

    return 0;
}
