#ifndef VIEWER_H
#define VIEWER_H

#include <string>
#include "ISaveable.h"

class Viewer : public ISaveable {
private:
    std::string filePath;
    std::string getFileExtension(const std::string& filePath) {
        auto pos = filePath.rfind('.');
        if (pos <= 0) return "";
        return filePath.substr(pos+1, std::string::npos);
    }

public:
    Viewer(const std::string &filePath) {
        std::string fileExt = getFileExtension(filePath);
        if (fileExt == "jpeg" || fileExt == "bmp" || fileExt == "png")
            this->filePath = filePath;
        else
            std::__throw_runtime_error("uncorrect file extension.");
    }

    std::string onSave() override {
        return filePath;
    }

};


#endif // VIEWER_H
