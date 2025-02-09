#include "FunctionManager.h"
#include "Solver.h"

#ifndef IOHandler_h
#define IOHandler_h

class IOHandler
{
private:
    FunctionManager *functionManager;
    Solver *solver;
public:
    IOHandler();
    ~IOHandler();
    
    void start();
};

#endif