#ifndef MINHEAP_H
#define MINHEAP_H

#include <stddef.h>
#include <vector>

template <typename T>
class MinHeapNode
{
protected:
    T m_current;
public:
    MinHeapNode(): m_current(0) {}

    virtual ~MinHeapNode(){}

    T current()
    {
        return m_current;
    }

    virtual bool fillNext()
    {
        return false;
    }
};

template <typename THeapNode>
class MinHeap
{
private:
    std::vector<THeapNode> m_heap;

    void swap(THeapNode *x, THeapNode *y)
    {
        THeapNode temp = *x;
        *x = *y;
        *y = temp;
    }

    size_t left(const size_t i)
    {
        return (2 * i + 1);
    }
    size_t right(const size_t i)
    {
        return (2 * i + 2);
    }

public:
    MinHeap()
    {
    }

    void addHeapNode(THeapNode &node)
    {
        m_heap.push_back(node);
    }

    void heapify()
    {
        int i = (m_heap.size() - 1) / 2;
        while (i >= 0)
        {
            minHeapify(i);
            --i;
        }
    }

    void minHeapify(size_t i)
    {
        size_t l = left(i);
        size_t r = right(i);
        size_t smallest = i;
        if (l < m_heap.size() && m_heap[l].current() < m_heap[i].current())
        {
            smallest = l;
        }
        if (r < m_heap.size() && m_heap[r].current() < m_heap[smallest].current())
        {
            smallest = r;
        }
        if (smallest != i)
        {
            swap(&m_heap[i], &m_heap[smallest]);
            minHeapify(smallest);
        }
    }

    THeapNode getMin()
    {
        return m_heap[0];
    }

    bool replaceMin()
    {
        if (m_heap[0].fillNext())
        {
            minHeapify(0);
            return true;
        }
        else
        {
            if (m_heap.size() > 0)
            {
                m_heap.erase(m_heap.begin(), m_heap.begin() + 1);
            }
            if (m_heap.size() > 0)
            {
                return true;
            }
        }
        return false;
    }
};


#endif // MINHEAP_H
