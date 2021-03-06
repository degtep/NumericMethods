#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <stddef.h>
#include <vector>

#include "romberg.h"

template <typename TFunc, typename TBoundFunc>
class IntegratorMapper {
private:
    mutable std::vector<double> m_data;
    const TFunc &m_func;
    const TBoundFunc &m_boundFunc;
public:
    IntegratorMapper(const TFunc &func, const TBoundFunc &boundFunc,
                     const std::vector<double> &data)
        : m_data(data), m_func(func), m_boundFunc(boundFunc)
    {
        m_data.push_back(m_boundFunc.low(m_data));
    }

    double lowBound() const
    {
        return m_boundFunc.low(m_data);
    }

    double highBound() const
    {
        return m_boundFunc.high(m_data);
    }

    double operator() (const double& num) const
    {
        m_data.back() = num;
        return m_func(m_data);
    }
};

template <typename TFunc, typename TBoundFunc>
class Integrator {
private:
    double m_maxRelativeError;
    const TFunc &m_func;
    const TBoundFunc &m_boundFunc;
public:
    Integrator(const TFunc& func, const TBoundFunc &boundFunc, const double maxRelativeError)
        : m_maxRelativeError(maxRelativeError), m_func(func), m_boundFunc(boundFunc)
    {}

    double operator()(const std::vector<double> &data) const
    {
        return Romberg(IntegratorMapper<TFunc, TBoundFunc>(m_func, m_boundFunc, data),
                       m_maxRelativeError);
    }

    double integrate() const
    {
        std::vector<double> data;
        return this->operator()(data);
    }
};

template <typename TFunc, typename TBoundFunc>
Integrator<TFunc, TBoundFunc> createIntegrator
(const TFunc& func, const TBoundFunc &bound_func, const double maxRelativeError)
{
    return Integrator<TFunc, TBoundFunc>(func, bound_func, maxRelativeError);
}

#endif // INTEGRATOR_H
