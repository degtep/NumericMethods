#ifndef BIGFLOAT_H
#define BIGFLOAT_H

#include <vector>
#include <stdint.h>
#include <cstddef>
#include <cmath>
#include <cstring>

#include <iostream>
#include <iomanip>

#include "bigint.h"

template <int Length>
class BigFloat {
public:
    typedef int32_t TDigit;
private:
    int m_sign;
    int m_power;
    TDigit m_data[Length];
public:
    enum {basePower = 13};
    enum {base = 1 << basePower};

public:
    BigFloat(): m_sign(1), m_power(0)
    {
        for (size_t index = 0; index < Length; ++index)
        {
            m_data[index] = 0;
        }
        m_data[Length - 1] = 1;
    }

    BigFloat(const double& value)
    {
        double mantissa = frexp(value, &m_power);
        if (mantissa < 0)
        {
            mantissa = - mantissa;
            m_sign = -1;
        } else
        {
            m_sign = 1;
        }
        mantissa *= base;
        for (size_t index = 0; index < Length; ++index, mantissa *= base)
        {
            double integerPart;
            mantissa = modf(mantissa, &integerPart);
            m_data[index] = static_cast<TDigit>(integerPart);
        }
        m_data[Length - 1] = std::max(m_data[Length - 1], TDigit(1));
    }

    static void shiftRight(TDigit* data, int deltaPower)
    {
        int indexShift = deltaPower / basePower;
        int powerShift = deltaPower % basePower;

        int newIndex = Length - 1;
        TDigit mask = ~(~TDigit(0) << powerShift);
        for (; newIndex - indexShift >= 0; --newIndex)
        {
            data[newIndex] = data[newIndex - indexShift] >> powerShift;
            if (newIndex - indexShift > 0)
            {
                data[newIndex] += (data[newIndex - indexShift - 1] & mask) << (basePower - powerShift);
            }
        }
        for (; newIndex >= 0; --newIndex)
        {
            data[newIndex] = 0;
        }
    }

    static bool compare(const TDigit* lhs, const TDigit* rhs)
    {
        for (size_t i = 0; i < Length; ++i)
        {
            if (lhs[i] > rhs[i]) return true;
            if (lhs[i] < rhs[i]) return false;
        }
        return true;
    }

    static void substract(TDigit* lhs, const TDigit* rhs)
    {
        for (int i = Length - 1; i >=0; --i)
        {
            lhs[i] -= rhs[i];
            if (lhs[i] < 0)
            {
                lhs[i - 1] -= 1;
                lhs[i] += base;
            }
        }
    }

    const BigFloat& operator /= (const BigFloat& rhs)
    {
        TDigit target[Length];
        memset(target, 0, Length * sizeof(TDigit));

        TDigit rcopy[Length];
        memcpy(rcopy, rhs.m_data, Length * sizeof(TDigit));

        for (size_t index = 0; index < Length * basePower; ++index)
        {
            size_t digitIndex = index / basePower;
            size_t bitIndex = basePower - 1 - index % basePower;

            if (compare(m_data, rcopy))
            {
                substract(m_data, rcopy);
                target[digitIndex] = target[digitIndex] | (0x01 << bitIndex);
            }

            shiftRight(rcopy, 1);
        }

        memcpy(m_data, target, Length * sizeof(TDigit));
        m_power -= rhs.m_power - 1;
        m_sign *= rhs.m_sign;
        return *this;
    }

    void print(std::ostream& os) const
    {
        static const int decBase = 10;
        static const int decLength = Length * 2;
        static const int decBasePower = 1;
        typedef BigInt<decLength, decBase, decBasePower> TLongDec;

        TLongDec buffer;
        int decPower = 0;
        TDigit multiplier = 1;
        for (int index = 0; index < Length; ++index)
        {
            if (index > 0)
            {
                int operationPower = 0;
                buffer.multiply(base, operationPower);
                decPower += operationPower;
                for (int i = 0; multiplier < base && i < operationPower; ++i)
                {
                    multiplier *= decBase;
                }
            }
            buffer += m_data[index] / multiplier;
        }

        TLongDec expBuffer(1);
        int expDecPower = 0;

        int basePowerNeeded = m_power - Length * basePower;

        TLongDec expMultiplier(2);
        int expDecMultiplierPower = 0;

        if (basePowerNeeded < 0)
        {
            basePowerNeeded *= -1;
            expMultiplier = 5;
            expDecMultiplierPower = -1;
        }
        if (basePowerNeeded > 0)
        {
            for (int currentBasePower = 1; currentBasePower <= basePowerNeeded; currentBasePower *= 2)
            {
                if (basePowerNeeded & currentBasePower)
                {
                    int addDecPower = multiply(expMultiplier, expBuffer, expBuffer);
                    expDecPower = expDecPower + expDecMultiplierPower + addDecPower;
                }

                int addDecPower = multiply(expMultiplier, expMultiplier, expMultiplier);
                expDecMultiplierPower = expDecMultiplierPower * 2 + addDecPower;
            }
            int addDecPower = multiply(buffer, expBuffer, buffer);
            decPower += addDecPower + expDecPower;
        }

        if (m_sign < 0)
        {
            os << "-";
        }
        printBigInt(buffer, os, decPower, Length);
    }
};

#endif // BIGFLOAT_H
