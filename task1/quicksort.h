#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stddef.h>
#include <stack>
#include <vector>

template <typename T>
size_t partition(std::vector<T> &data, size_t startIndex, size_t endIndex)
{
    T pivotValue = data[endIndex];
    size_t i = startIndex - 1;
    for(size_t j = startIndex; j < endIndex; j++)
    {
        if(data[j] <= pivotValue)
        {
            i = i + 1;
            T t = data[j];
            data[j] = data[i];
            data[i] = t;
        }
    }

    data[endIndex] = data[i+1];
    data[i+1] = pivotValue;

    return i+1;
}

template <typename T>
void quickSort(std::vector<T> &data)
{
    std::stack<std::pair<size_t,size_t> > callStack;
    std::pair<size_t,size_t> init(0, data.size() - 1);
    callStack.push(init);
    while(!callStack.empty())
    {
        std::pair<size_t,size_t> p = callStack.top();
        callStack.pop();
        if(p.first < p.second)
        {
            size_t pivot = partition(data, p.first, p.second);
            if (pivot > 0)
            {
                std::pair<size_t,size_t> p1(p.first, pivot - 1);
                callStack.push(p1);
            }
            std::pair<size_t,size_t> p2(pivot + 1, p.second);
            callStack.push(p2);
        }
    }
}

#endif // QUICKSORT_H
