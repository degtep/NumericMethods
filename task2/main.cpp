#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include "complex.h"
#include "interpolation.h"

const double RATIONAL_SHIFT = 1e2;
const double IM_SHIFT = 0.05;
const int PRECISION = 2;
const char* INPUT_FILENAME = "input.txt";
const char* OUTPUT_FILENAME = "output.txt";

int main()
{    
    std::vector<double> x;
    std::vector<double> f;
    std::ifstream input(INPUT_FILENAME);

    double currentX, currentF;
    while (input >> currentX >> currentF)
    {
        x.push_back(currentX);
        f.push_back(currentF);
    }
    input.close();

    std::ofstream output(OUTPUT_FILENAME);
    output << std::fixed << std::setprecision(PRECISION);
    std::cerr << std::fixed << std::setprecision(PRECISION + 1);
    for (size_t i = 0; i < x.size(); ++i)
    {
        Complex point(x[i], IM_SHIFT);
        double error;
        output << computePolynomialValue(x, f, point, error) << std::endl;
        std::cerr << "Polynomial interpolation point " << i + 1
                  << " error = " << error << std::endl;
    }
    for (size_t i = 0; i < x.size(); ++i)
    {
        Complex point(x[i], IM_SHIFT);
        double error;
        output << computeRationalValue(x, f, point, RATIONAL_SHIFT, error) << std::endl;
        std::cerr << "Rational interpolation point " << i + 1
                  << " error = " << error << std::endl;
    }
    output.close();

    return 0;
}

