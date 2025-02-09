#include "../../../headers/function/specific/Hyperbola.h"

Hyperbola::Hyperbola(std::vector<double> constants) : Function::Function(constants)
{
    if (constants.size() != 2)
        throw std::invalid_argument("num of constants not equal 2");

    a = constants[0];
    b = constants[1];
}

Hyperbola::~Hyperbola() { delete this; }

double Hyperbola::calc(double x)
{
    return a / x + b;
}

std::string Hyperbola::toString() {
    return std::to_string(a) + " / x + " + std::to_string(b);
}