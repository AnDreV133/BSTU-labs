#include "../Function.h"

#ifndef Parabola_h
#define Parabola_h

class Parabola : public Function
{
private:
    std::vector<double> constants;
public:
    Parabola(std::vector<double> constants);
    ~Parabola();
    double calc(double x) override;
    std::string toString() override;

protected:
    double a;
    double b;
    double c;
};

#endif