#include "../Function.h"

#ifndef Hyperbola_h
#define Hyperbola_h

class Hyperbola : public Function
{
private:
    std::vector<double> constants;
    double a;
    double b;
public:
    Hyperbola(std::vector<double> constants);
    ~Hyperbola();
    double calc(double x) override;
    std::string toString() override;
};

#endif