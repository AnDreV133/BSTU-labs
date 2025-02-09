#include "../../headers/function/IOHandler.h"
#include "../../headers/function/Solver.h"
#include "../../headers/function/specific/Line.h"
#include "../../headers/function/specific/Hyperbola.h"
#include "../../headers/function/specific/Parabola.h"

#include <iostream>

IOHandler::IOHandler()
{
    this->functionManager = new FunctionManager();
    this->solver = new Solver(functionManager);
}

IOHandler::~IOHandler()
{
    delete this;
}

void IOHandler::start()
{
    int command_index;
    while (true)
    {
        std::cout << "1- add function\n2- calc functions\n";
        std::cin >> command_index;
        switch (command_index)
        {
        case 1:
            std::cout << "\nChange type of function\n1- Line\n2- Hyperbola\n3- Parabola\n";
            std::cin >> command_index;

            double a;
            std::cout << "input a: ";
            std::cin >> a;

            double b;
            std::cout << "input b: ";
            std::cin >> b;

            switch (command_index)
            {
            case 1:
                functionManager->addFunction(new Line({a, b}));
                break;
            case 2:
                functionManager->addFunction(new Hyperbola({a, b}));
                break;
            case 3:
                double c;
                std::cout << "input c: ";
                std::cin >> c;
                functionManager->addFunction(new Parabola({a, b, c}));
                break;

            default:
                std::cerr << "Incorrect command index\n";
                break;
            }

            break;

        case 2:
            std::cout << "\nHow much function wou want calculate\n1- One\n2- Many\n";
            std::cin >> command_index;
            switch (command_index)
            {
            case 1:
                for (size_t i = 0; i < functionManager->size(); i++)
                {
                    std::cout << i + 1 << ' ' << functionManager->getFunctionById(i)->toString() << '\n';
                }
                int index;
                std::cout << "Change function:\n";
                std::cin >> index;

                std::cout << "1- By X\n2- By range\n";
                std::cin >> command_index;
                switch (command_index)
                {
                case 1:
                    double x;
                    std::cout << "Write X\n";
                    std::cin >> x;

                    std::cout << "Result: "
                              << "f(" << x << ") = " << solver->getY(x, index-1) << '\n';

                    break;
                case 2:
                    double a;
                    std::cout << "Write A\n";
                    std::cin >> a;

                    double b;
                    std::cout << "Write B\n";
                    std::cin >> b;

                    double step;
                    std::cout << "Write step\n";
                    std::cin >> step;

                    for (auto result : solver->getXYByRegion(a, b, step, index-1))
                        std::cout << result.first << "\t| " << result.second << '\n';

                    break;

                default:
                    std::cerr << "Incorrect command index\n";

                    break;
                }

                break;
            case 2:
                std::cout << "1- By X\n2- By range\n";
                std::cin >> command_index;
                switch (command_index)
                {
                case 1:
                    double x;
                    std::cout << "Write X\n";
                    std::cin >> x;

                    std::cout << "Result:\n";
                    for (size_t i = 0; i < functionManager->size(); i++)
                        std::cout << "f(" << x << ") = " << solver->getY(x, i) << '\n';

                    break;
                case 2:
                    double a;
                    std::cout << "Write A\n";
                    std::cin >> a;

                    double b;
                    std::cout << "Write B\n";
                    std::cin >> b;

                    double step;
                    std::cout << "Write step\n";
                    std::cin >> step;

                    for (auto table : solver->getXYByRegion(a, b, step))
                    {
                        for (auto result : table)
                            std::cout << result.first << " | " << result.second << '\n';

                        std::cout << "------\n";
                    }

                    break;

                default:
                    std::cerr << "Incorrect command index\n";
                    break;
                }

                break;
            default:
                std::cerr << "Incorrect command index\n";
                break;
            }
        }
    }
}

// std::string getInfoAllFunctionsByX(double x)
// {
//     std::string res;
//     for (Function *function : functions)
//         res += "f(" + std::to_string(x) + ") = " + function->toString() + " = " + std::to_string(function->calc(x)) + "\n";
//     res += "\b";

//     return res;
// }