#include "../../../headers/function/specific/Parabola.h"

Parabola::Parabola(std::vector<double> constants) : Function::Function(constants)
{
    if (constants.size() != 3)
        throw std::invalid_argument("num of constants not equal 3");

    a = constants[0];
    b = constants[1];
    c = constants[2];
}

Parabola::~Parabola()
{
    delete this;
}

double Parabola::calc(double x)
{
    return a * x * x + b * x + c;
}

std::string Parabola::toString() {
    return std::to_string(a) + " * x2 + " + std::to_string(b) + " x + " + std::to_string(c);
}