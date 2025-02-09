#include <vector>
#include <stdexcept>
#include <string>

#ifndef Function_h
#define Function_h

class Function
{
private:
public:
    Function(const std::vector<double> constants);
    ~Function();
    virtual std::string toString();
    virtual double calc(double x) = 0;

protected:
    std::vector<double> constants;
    
};

#endif