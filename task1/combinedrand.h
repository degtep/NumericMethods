#ifndef COMBINEDRAND_H
#define COMBINEDRAND_H

#include <algorithm>
#include <memory>
#include "simplerand.h"

const size_t S_SIZE = 500;

const int FACTOR_A1 = 40014;
const int FACTOR_B1 = 12345;
const int FACTOR_M1 = 2147483563;
const int INIT_FACTOR1 = 12345;

const int FACTOR_A2 = 40692;
const int FACTOR_B2 = 54321;
const int FACTOR_M2 = 2147483399;
const int INIT_FACTOR2 = 54321;

class CombinedRand
{
private:
    int32_t m_s[S_SIZE];
    SimpleRand<int32_t> *m_rand1;
    SimpleRand<int32_t> *m_rand2;

public:
    CombinedRand()
    {
        m_rand1 = new SimpleRand<int32_t>(FACTOR_A1, FACTOR_B1, FACTOR_M1, INIT_FACTOR1);
        m_rand2 = new SimpleRand<int32_t>(FACTOR_A2, FACTOR_B2, FACTOR_M2, INIT_FACTOR2);
        m_s[0] = std::abs(m_rand1->Current() - m_rand2->Current());
        for(size_t i = 1; i < S_SIZE; ++i)
        {
            m_s[i] = std::abs(m_rand1->Next() - m_rand2->Next());
        }
    }

    int32_t Next()
    {
        int32_t r1 = m_rand1->Next();
        size_t currentIndex = r1 % S_SIZE;
        int32_t currentNumber = m_s[currentIndex];
        m_s[currentIndex] = std::abs(r1 - m_rand2->Next());
        return currentNumber;
    }

    ~CombinedRand()
    {
        delete m_rand1;
        delete m_rand2;
    }
};

#endif // COMBINEDRAND_H
