#ifndef COMPLEX_H
#define COMPLEX_H

const double epsilon = 1e-9;

class Complex
{
private:
    double m_re;
    double m_im;

public:
    Complex(): m_re(0.0), m_im(0.0) {}
    Complex(const Complex &rhs): m_re(rhs.m_re), m_im(rhs.m_im) {}

    explicit Complex(const double &re, const double &im): m_re(re), m_im(im) {}
    explicit Complex(const double &re): m_re(re), m_im(0.0) {}

    inline double re() const
    {
        return m_re;
    }

    inline double im() const
    {
        return m_im;
    }

    double mod() const
    {
        return re() * re() + im() * im();
    }

    Complex operator + (const Complex &rhs) const
    {
        return Complex(re() + rhs.re(), im() + rhs.im());
    }

    Complex operator - (const Complex &rhs) const
    {
        return Complex(re() - rhs.re(), im() - rhs.im());
    }

    Complex operator * (const Complex &rhs) const
    {
        return Complex(re() * rhs.re() - im() * rhs.im(),
                       re() * rhs.im() + im() * rhs.re());
    }

    Complex operator / (const double rhs) const
    {
        return Complex(re() / rhs, im() / rhs);
    }

    Complex operator / (const Complex &rhs) const
    {
        if (rhs.mod() < epsilon)
        {
            return Complex(0.0, 0.0);
        }
        return (Complex(*this) * Complex(rhs.re(), -rhs.im()) / rhs.mod());
    }

    Complex operator - (const double rhs) const
    {
        return Complex(*this) - Complex(rhs);
    }
};

Complex operator - (const double lhs, const Complex &rhs)
{
    return Complex(lhs - rhs.re(), -rhs.im());
}

#endif // COMPLEX_H
