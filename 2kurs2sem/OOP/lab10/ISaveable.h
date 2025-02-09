#ifndef ISAVEABLE_H
#define ISAVEABLE_H

#include <string>

class ISaveable {
public:
    virtual std::string onSave() = 0;
};

#endif // ISAVEABLE_H
