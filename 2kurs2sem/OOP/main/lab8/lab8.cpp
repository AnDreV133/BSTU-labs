#include <iostream>

#include "WordCollector.h"

int main()
{
    WordCollector *wc = new WordCollector();
    wc->recordToFile("source.txt", "text.txt");
    wc->~WordCollector();

    return 0;
}