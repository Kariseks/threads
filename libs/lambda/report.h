#ifndef REPORT_H
#define REPORT_H

#include <vector>
class Report
{
public:
    Report();

    void report_bigger(std::vector<int> vec);
    void report_smaller(std::vector<int> vec);
    void report(std::vector<int> vec);
public:
    int pub_var = 0;
    int threshold = 22;
    int priv_counter = 0;
    static int stat_var;
private:
    int priv_var = 0;




};

#endif // REPORT_H
