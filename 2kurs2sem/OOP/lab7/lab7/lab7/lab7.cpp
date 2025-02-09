#include "solve.h"
int main() {
    setlocale(LC_ALL, "Russian");

    QuadraticEquationSolver solver;
    solver.startInput();

    QuadraticEquationSolver solver1(1, -3, 2);
    solver1.solveEquation(); 

    try {
        QuadraticEquationSolver solver2(0, 2, 3);
        solver2.solveEquation();
    }
    catch (...) {
        std::cerr << "error\n";
    }

    try {
    QuadraticEquationSolver solver3(1, 0, 2);
    solver3.solveEquation();  
    }
    catch (...) {
        std::cerr << "error\n";
    }

    QuadraticEquationSolver solver4(1, -3, 0);
    solver4.solveEquation();  

    QuadraticEquationSolver solver5(1, 0, 0);
    solver5.solveEquation();

    try {
        QuadraticEquationSolver solver6(1, 2, 3);
        solver6.solveEquation();
    }
    catch (...) {
        std::cerr << "error\n";
    }

    return 0;
}
