#include <iostream>
#include <fstream>

#include "taskfunction.h"
#include "integrator.h"

int main() {
    const double maxError = 1e-8;
    double error;
    std::cerr << "Calculating integral with max error = " << maxError << " ..." << std::endl;
    double result = createIntegrator(
                createIntegrator(
                    createIntegrator(TaskFunction(), BoundFunctionZ(), maxError),
                    BoundFunctionY(), maxError),
                BoundFunctionX(), maxError).integrate(&error);

    std::cerr << "Error = " << error << std::endl;
    std::cerr.precision(12);
    std::cerr << "Result = " << result << std::endl;

    std::ofstream os("outfile.txt");
    os.precision(12);
    os << result << std::endl;
    os.close();

    return 0;
}
