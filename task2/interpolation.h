#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>
#include <complex>
#include <stddef.h>
#include "complex.h"

double computeRationalValue(const std::vector<double> &x,
                            const std::vector<double> &f,
                            const Complex &point,
                            const double shift,
                            double &error)
{
    size_t count = x.size();
    std::vector<Complex> r1(count);
    std::vector<Complex> r2(count);
    std::vector<Complex> nextR(count);
    for (size_t i = 0; i < count; ++i)
    {
        r1[i] = Complex(0.0);
        r2[i] = Complex(f[i] + shift);
    }

    for (size_t i = 1; i < count; ++i)
    {
        for (size_t j = 0; j < count - i; ++j)
        {
            Complex denominator = (point - x[j]) / (point - x[j + i])
                    * (1.0 - (r2[j + 1] - r2[j]) / (r2[j + 1] - r1[j + 1])) - 1.0;

            nextR[j] = r2[j + 1] + (r2[j + 1] - r2[j]) / denominator;
        }

        std::swap(r1, r2);
        std::swap(r2, nextR);
    }
    error = (std::abs(r2[0].re() - nextR[0].re()) +
            std::abs(r2[0].re() - nextR[1].re())) / 2;

    return r2[0].re() - shift;
}

double computePolynomialValue(const std::vector<double> &x,
                              const std::vector<double> &f,
                              const Complex &point,
                              double &error)
{
    size_t count = x.size();
    std::vector<Complex> polynomial(count);
    std::vector<Complex> polynomialNext(count);
    for (size_t i = 0; i < count; ++i)
    {
        polynomial[i] = Complex(f[i]);
    }
    for (size_t i = 1; i < count; ++i)
    {
        for (size_t j = 0; j < count - i; ++j)
        {
            polynomialNext[j] = (polynomial[j] * (x[i + j] - point)
                    + polynomial[j + 1] * (point - x[j]))
                    / (x[i + j] - x[j]);
        }
        std::swap(polynomial, polynomialNext);
    }
    error = (std::abs(polynomial[0].re() - polynomialNext[0].re()) +
            std::abs(polynomial[0].re() - polynomialNext[1].re())) / 2;
    return polynomial[0].re();
}

#endif // INTERPOLATION_H
