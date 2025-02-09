#include "../../headers/function/FunctionManager.h"

FunctionManager::FunctionManager()
{
}

FunctionManager::FunctionManager(std::vector<Function *> functions)
{
    this->functions = functions;
}

FunctionManager::~FunctionManager()
{
    delete this;
}

void FunctionManager::addFunction(Function *function)
{
    functions.push_back(function);
}

std::vector<Function *> FunctionManager::getFunctions()
{
    return functions;
}

Function *FunctionManager::getFunctionById(int id)
{
    return functions[id];
}

size_t FunctionManager::size()
{
    return functions.size();
}