#include "../Function.h"

#ifndef Line_h
#define Line_h

class Line : public Function
{
private:
    std::vector<double> constants;

public:
    Line(std::vector<double> constants);
    ~Line();
    double calc(double x) override;
    std::string toString() override;

protected:
    double a;
    double b;
};

#endif