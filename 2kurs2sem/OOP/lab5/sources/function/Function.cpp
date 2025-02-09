#include "../../headers/function/Function.h"

Function::Function(std::vector<double> constants) {
    this->constants = constants;
}

Function::~Function() {
    delete this;
}

std::string Function::toString() {
    std::string res; 
    for (double constant : constants)
        res += std::to_string(constant) + " ";

    return res;
}


