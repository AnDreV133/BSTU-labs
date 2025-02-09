#include "../../../headers/function/specific/Line.h"

Line::Line(std::vector<double> constants) : Function::Function(constants)
{
    if (constants.size() != 2)
        throw std::invalid_argument("num of constants not equal 2");

    a = constants[0];
    b = constants[1];
}

Line::~Line() {
    delete this;
}

double Line::calc(double x)
{
    return a * x + b;
}

std::string Line::toString() {
    return std::to_string(a) + " * x + " + std::to_string(b);
}