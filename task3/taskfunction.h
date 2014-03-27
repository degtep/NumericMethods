#ifndef TASKFUNCTION_H
#define TASKFUNCTION_H

#include <cmath>
#include <vector>

inline double pow2(const double& arg)
{
    return arg * arg;
}

class TaskFunction
{
public:
    double operator()(const std::vector<double> &data) const
    {
         return (std::exp((data[0] * data[1] + data[1] * data[2] + data[2] * data[0]) / 2.0) - 1.0)
                * (std::exp((pow2(data[0]) * data[1] + pow2(data[1]) * data[2] + pow2(data[2]) * data[0]
                    + data[0] * data[1] * data[2]) / 2.0) - 1.0)
                * std::log(1.0 + data[0] + data[1] + data[2]
                    + std::log(1.0 + pow2(data[0]) + pow2(data[1]) + pow2(data[2])));
    }
};

class BoundFunctionX
{
public:
    double low(const std::vector<double> &data) const
    {
        (void)data;
        return 0.0;
    }
    double high(const std::vector<double> &data) const
    {
        (void)data;
        return 2.0;
    }
};

class BoundFunctionY
{
private:
    inline double sqrtArg(const double &arg1) const
    {
        double result = 1.0 - pow2(arg1 - 1.0);
        if (result < 0)
        {
            return 0.0;
        }
        return result;
    }

public:
    double low(const std::vector<double> &data) const
    {
        return 1.0 - std::sqrt(sqrtArg(data[0]));
    }
    double high(const std::vector<double> &data) const
    {
        return 1.0 + std::sqrt(sqrtArg(data[0]));
    }
};

class BoundFunctionZ
{
private:
    inline double sqrtArg(const double &arg1, const double &arg2) const
    {
        double result = 1.0 - pow2(arg1 - 1.0) - pow2(arg2 - 1.0);
        if (result < 0)
        {
            return 0.0;
        }
        return result;
    }
public:
    double low(const std::vector<double> &data) const
    {
        return 1.0 - std::sqrt(sqrtArg(data[0], data[1]));
    }
    double high(const std::vector<double> &data) const
    {
        return 1.0 + std::sqrt(sqrtArg(data[0], data[1]));
    }
};

#endif // TASKFUNCTION_H
