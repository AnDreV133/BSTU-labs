#include <iostream>
#include <vector>

// #include "headers/function/specific/Line.h"
// #include "headers/function/specific/Parabola.h"
// #include "headers/function/specific/Hyperbola.h"
// #include "headers/function/Function.h"
// #include "headers/function/FunctionManager.h"
#include "headers/function/IOHandler.h"

int main()
{
    // Line l1({2, 2});
    // Line l2({2, 2});
    // Parabola p1({3, 2, 1});
    // Line l3({2, 2});
    // Hyperbola h1({2, 2});

    // FunctionManager fm({&l1, &l2, &l3});
    // fm.addFunction(&p1);
    // fm.addFunction(&h1);

    // std::cout << fm.getInfoAllFunctionsByX(4);
    
    IOHandler ioh = IOHandler();
    ioh.start();

    return 0;
}

