#include "FunctionManager.h"
#ifndef Solver_h
#define Solver_h

class Solver
{
private:
    FunctionManager *functionManager;

public:
    Solver();
    Solver(FunctionManager *functionManager);
    ~Solver();

    std::vector<std::pair<double, double>> getXYByRegion(double a, double b, double step, double index);
    std::vector<std::vector<std::pair<double, double>>> getXYByRegion(double a, double b, double step);
    double getY(double x, double index);
    std::vector<double> getY(double x);
};

#endif