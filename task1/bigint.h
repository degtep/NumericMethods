#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <iomanip>

template <int Length, int Base, int BasePower>
class BigInt {
public:
    typedef int32_t TDigit;
private:
    TDigit m_data[Length];
public:

    enum {base = Base};
    enum {length = Length};
    enum {basePower = BasePower};

    BigInt()
    {
        memset(m_data, 0, sizeof(TDigit) * Length);
    }

    BigInt(int data)
    {
        memset(m_data, 0, sizeof(TDigit) * Length);
        for (size_t i = 0; i < Length; ++i)
        {
            m_data[i] = data % base;
            data /= base;
        }
    }

    const BigInt& operator += (const BigInt &rhs)
    {
        for (size_t i = 0; i < Length; ++i)
        {
            m_data[i] += rhs.m_data[i];
            if (i + 1 < Length)
            {
                m_data[i+1] += m_data[i] / base;
                m_data[i] %= base;
            }
        }
        return *this;
    }

    const BigInt& multiply(const TDigit &rhs, int &power)
    {
        TDigit buffer[Length * 2];
        memset(buffer, 0, 2 * Length * sizeof(TDigit));
        for (size_t i = 0; i < Length; ++i)
        {
            buffer[i] = m_data[i] * rhs;
        }
        for (size_t i = 0; i + 1 < 2 * Length; ++i)
        {
            buffer[i+1] += buffer[i] / base;
            buffer[i] %= base;
        }
        int index = 2 * Length - 1;
        for (; index >= Length && buffer[index] == 0; --index)
        {}
        memcpy(m_data, &buffer[index + 1 - Length], Length * sizeof(TDigit));
        power = index + 1 - Length;
        return *this;
    }

    const TDigit* getData() const
    {
        return m_data;
    }

    TDigit* getData()
    {
        return m_data;
    }
};

template <typename TBigInt>
int multiply(const TBigInt &lhs, const TBigInt &rhs, TBigInt &target) {
    typename TBigInt::TDigit targetData[TBigInt::length * 2];
    memset(targetData, 0, sizeof(typename TBigInt::TDigit) * 2 * TBigInt::length);

    for (size_t i = 0; i < TBigInt::length; ++i)
    {
        for (size_t j = 0; j < TBigInt::length; ++j)
        {
            targetData[i + j] += lhs.getData()[i] * rhs.getData()[j];
        }
    }
    for (size_t i = 0; i < 2 * TBigInt::length - 1; ++i)
    {
        targetData[i + 1] += targetData[i] / TBigInt::base;
        targetData[i] %= TBigInt::base;
    }
    int index = 2 * TBigInt::length - 1;
    for (; index >= TBigInt::length && targetData[index] == 0; --index)
    {
    }
    if (index >= TBigInt::length)
    {
        memcpy(target.getData(), &targetData[index + 1 - TBigInt::length], sizeof(typename TBigInt::TDigit) * TBigInt::length);
        return index + 1 - TBigInt::length;
    } else
    {
        memcpy(target.getData(), targetData, sizeof(typename TBigInt::TDigit) * TBigInt::length);
        return 0;
    }
}

template <typename TBigInt>
void printBigInt(const TBigInt& number, std::ostream& os, int dec_power, int round = 0) {
    int i = TBigInt::length - 1;
    for (; i > round && number.getData()[i] == 0; --i)
    {
    }
    int power = dec_power + i;
    if (power < 0)
    {
        os << "0." << std::string(std::abs(power), '0');
        round -= power;
    }

    for (; i >= round; --i) {
        if (dec_power + i == -1)
        {
            os << ".";
        }
        os << std::setfill('0') << std::setw(TBigInt::basePower) << number.getData()[i];
    }
}

#endif // BIGINT_H
