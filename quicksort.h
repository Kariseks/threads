#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <utility>

namespace{
using namespace std;
template <typename T>
unsigned int partition(size_t b, size_t e, size_t pivIdx, T  tab[])
{
    //check elems, if smaller than pivot, swap with pivot;
    //move the pivot to the the end of local table range
    swap(tab[e], tab[pivIdx]);

    auto pivot = tab[e];
    int next = b;
    for(unsigned int j=b; j < e; ++j)  //idx - idx of next elem after the highest idx of low side part of tab -> is also the idx of Pivot
    {
        if(tab[j] <= pivot)
        {
            swap(tab[next], tab[j]);
            ++next;
        }
    }
    //put pivot just after the left side so we have [left pivot right]
    swap(tab[next], tab[e]);

    return next;
}
}   //end of unnamed namespace
//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void quickSort(std::size_t b, std::size_t e, T tab[])
{
    //validation of the args and table
    if (b >= e)
        return;
    /*
    //basic problem condition
    if(e-b+1 <= thresholdAlgorithm) //basic problem
    {
        mergeSort(b,e,tab);
        return;
    }
    */
    //divide
    int pivIdx = e;    //here pivIdx should be calculated
    pivIdx = partition(b, e, pivIdx, tab);
    //conquer
    quickSort(b, pivIdx-1, tab);
    quickSort(pivIdx+1, e, tab);
}
#endif // QUICKSORT_H
