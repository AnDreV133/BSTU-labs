#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <exception>

class EquationSolver {
public:
    EquationSolver() {}
    virtual void solveEquation() = 0;
    void startInput() {
        while (true) {
            std::cout << "Введите параметры.\n";
            std::string args;
            std::getline(std::cin, args);
            std::stringstream ss(args);
            
            std::string arg;
            while (ss >> arg)
                params.push_back(stod(arg));

            try {
                solveEquation();
            }
            catch (NotSolveException& e) {
                std::cerr << "NotSolveException " << e.what();
                break;
            }
            catch (BadInputException& e) {
                std::cerr << "BadInputException " << e.what();
                break;
            }
        }
    }
    
    class NotSolveException : public std::exception
    {
    public:
        NotSolveException(const char* msg) : std::exception(msg) {}
    };

    class BadInputException : public std::exception
    {
    public:
        BadInputException(const char* msg) : std::exception(msg) {}
    };

protected:
    std::vector<double> params;
};

class QuadraticEquationSolver : public EquationSolver {
public:
    QuadraticEquationSolver() {}

    QuadraticEquationSolver(double a, double b, double c) {
        params.clear();
        params.push_back(a);
        params.push_back(b);
        params.push_back(c);
    }

    void solveEquation() override {
        double a, b, c;
        if (params.size() == 3) {
            a = params[0];
            b = params[1];
            c = params[2];
        }
        else {
            throw BadInputException("Превышено количество аргументов\n");
        }

        if (a == 0) throw BadInputException("a = 0");

        double discriminant = b * b - 4 * a * c;
        if (discriminant > 0) {
            double x1 = (-b + sqrt(discriminant)) / (2 * a);
            double x2 = (-b - sqrt(discriminant)) / (2 * a);
            std::cout << "Уравнение имеет два корня: x1 = " << x1 << ", x2 = " << x2 << std::endl;
        }
        else if (discriminant == 0) {
            double x = -b / (2 * a);
            std::cout << "Уравнение имеет один корень: x = " << x << std::endl;
        }
        else {
            throw NotSolveException("Уравнение не имеет действительных корней\n");
        }
    }
};
