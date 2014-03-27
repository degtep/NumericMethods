#include <iostream>
#include <fstream>

#include "taskfunction.h"
#include "integrator.h"

int main() {
    const int precision = 8;
    const double maxRelativeError = 1.0 / std::pow(10, precision);
    std::cerr << "Calculating integral with max relative error = "
              << maxRelativeError
              << " ..." << std::endl;
    double result = createIntegrator(
                createIntegrator(
                    createIntegrator(TaskFunction(), BoundFunctionZ(), maxRelativeError),
                    BoundFunctionY(), maxRelativeError),
                BoundFunctionX(), maxRelativeError).integrate();

    std::cerr.precision(precision);
    std::cerr << "Result = " << result << std::endl;

    std::ofstream os("outfile.txt");
    os.precision(precision);
    os << result << std::endl;
    os.close();

    return 0;
}
