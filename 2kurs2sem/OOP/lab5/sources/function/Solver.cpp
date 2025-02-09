#include "../../headers/function/Function.h"
#include "../../headers/function/Solver.h"

Solver::Solver()
{
}

Solver::Solver(FunctionManager *functionManager)
{
    this->functionManager = functionManager;
}

Solver::~Solver()
{
    delete this;
}

std::vector<std::pair<double, double>> Solver::getXYByRegion(double a, double b, double step, double index)
{
    auto res = std::vector<std::pair<double, double>>();
    while (a < b)
    {
        res.push_back({a, getY(a, index)});

        a += step;
    }

    return res;
}
std::vector<std::vector<std::pair<double, double>>> Solver::getXYByRegion(double a, double b, double step)
{
    auto res = std::vector<std::vector<std::pair<double, double>>>();
    for (size_t i = 0; i < functionManager->size(); i++)
    {
        res.push_back(getXYByRegion(a, b, step, i));
    }

    return res;
}
double Solver::getY(double x, double index)
{
    return functionManager->getFunctionById(index)->calc(x);
}

std::vector<double> Solver::getY(double x)
{
    auto res = std::vector<double>();
    for (size_t i = 0; i < functionManager->size(); i++)
    {
        res.push_back(getY(x, i));
    }

    return res;
}