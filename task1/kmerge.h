#ifndef KMERGE_H
#define KMERGE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include "minheap.h"

class NumbersStreamNode : public MinHeapNode<int32_t>
{
private:
    std::ifstream *m_fileStream;
public:
    NumbersStreamNode(std::ifstream *fileStream): m_fileStream(fileStream)
    {
        m_fileStream->read(reinterpret_cast<char *>(&m_current), sizeof(int32_t));
    }
    virtual ~NumbersStreamNode(){}

    bool fillNext()
    {
        if (!m_fileStream->eof())
        {
            m_fileStream->read(reinterpret_cast<char *>(&m_current), sizeof(int32_t));
            if (!m_fileStream->eof())
            {
                return true;
            }
        }
        return false;
    }
};

void kmergeAndSample(std::vector<std::ifstream*> &ifstreams,
                     const size_t startSkip, const size_t skipCount,
                     std::vector<int32_t> &sortedSamples)
{
    MinHeap<NumbersStreamNode> heap;
    for (size_t i = 0; i < ifstreams.size(); i++)
    {
        NumbersStreamNode node(ifstreams[i]);
        heap.addHeapNode(node);
    }
    heap.heapify();

    size_t currentSkip = startSkip;
    size_t counter = 0;
    bool nextAvailable = true;
    while(nextAvailable)
    {

        NumbersStreamNode root = heap.getMin();
        if (currentSkip == skipCount)
        {
            sortedSamples.push_back(root.current());
            currentSkip = 0;
        }
        ++counter;
        ++currentSkip;
        nextAvailable = heap.replaceMin();
    }
}
#endif // KMERGE_H
