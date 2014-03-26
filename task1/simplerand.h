#ifndef SIMPLERAND_H
#define SIMPLERAND_H

#include <stddef.h>
#include <vector>

template <typename T>
class SimpleRand
{
private:
    const T m_factorA;
    const T m_factorB;
    const T m_factorM;
    const T m_factorR;
    const T m_factorQ;
    T m_current;

public:
    SimpleRand(const T factorA,
               const T factorB,
               const T factorM,
               const T initFactor):
        m_factorA(factorA),
        m_factorB(factorB),
        m_factorM(factorM),
        m_factorR(m_factorM % m_factorA),
        m_factorQ(m_factorM / m_factorA),
        m_current(initFactor)
    {
    }

    T Current()
    {
        return m_current;
    }

    T Next()
    {
        T y = m_current / m_factorQ;
        m_current = m_factorA * (m_current - y * m_factorQ) + m_factorB - y * m_factorR;
        if (m_current < 0)
        {
            m_current += m_factorM;
        }
        if (m_current >= m_factorM)
        {
            m_current -= m_factorM;
        }
        return m_current;
    }
};


#endif // SIMPLERAND_H
