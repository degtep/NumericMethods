#ifndef ROMBERG_H
#define ROMBERG_H

#include <stddef.h>
#include <vector>

template <typename TFunc>
double Romberg(const TFunc &func, const double &maxRelativeError)
{
    double lowBound = func.lowBound();
    double highBound = func.highBound();
    if (lowBound >= highBound)
    {
        return 0.0;
    }
    double interval = highBound - lowBound;
    std::vector<double> currentStep, nextStep;
    nextStep.clear();
    nextStep.push_back((func(highBound) + func(lowBound)) * interval / 2);
    size_t iteration = 0;
    while (true)
    {
        ++iteration;
        currentStep = nextStep;
        nextStep.clear();
        interval = interval / 2;
        double partialSum = 0;
        double x = lowBound + interval;
        for (size_t i = 0; i < (1 << (iteration - 1)); ++i)
        {
            partialSum += func(x) * interval;
            x += 2 * interval;
        }
        partialSum += currentStep[0] / 2;
        nextStep.push_back(partialSum);
        for (size_t k = 1; k <= iteration; k++)
        {
            nextStep.push_back((pow(4.0, k) * nextStep[k-1] - currentStep[k-1])
                    / (pow(4.0, k) - 1));
        }
        if (iteration >= 4)
        {
            double currentError = fabs(nextStep[iteration] - currentStep[iteration-1])
                    / nextStep[iteration];
            if (currentError < maxRelativeError)
            {
                break;
            }
        }
    }
    return nextStep[iteration];
}

#endif // ROMBERG_H
