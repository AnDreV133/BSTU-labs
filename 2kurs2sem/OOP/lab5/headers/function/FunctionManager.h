#include "Function.h"

#ifndef FunctionManager_h
#define FunctionManager_h

class FunctionManager
{
private:
    std::vector<Function *> functions;

public:
    FunctionManager();
    FunctionManager(std::vector<Function *> functions);
    ~FunctionManager();

    Function *getFunctionById(int id);
    std::vector<Function *> getFunctions();
    void addFunction(Function *function);
    size_t size();
};

#endif