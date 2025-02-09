#ifndef FILESAVER_H
#define FILESAVER_H

#include <forward_list>
#include <string>
#include <fstream>

#include <QStringList>
#include <QSet>
#include <QList>

#include "ISaveable.h"
#include "SmartPointer.hpp"

class FileSaver {
private:
    std::string recordFileName;
public:
    FileSaver(std::string recordFileName) {
        this->recordFileName = recordFileName;
    }

    void save(ISaveable &s) {
        std::ofstream recordFile(recordFileName, std::ofstream::app);
        if (recordFile.is_open()) {
            recordFile << s.onSave() << '\n';
            recordFile.close();
        } else {
            throw std::ios_base::failure("make or write to file is impossible");
        }
    }

    QStringList getPathesToImages() {
        std::ifstream recordFile(recordFileName, std::ofstream::app);
        if (!recordFile.is_open()) {
            throw std::ios_base::failure("file cant be read");
        }

        SmartPointer<QSet<QString>> set = new QSet<QString>();
        std::string buf;
        while (getline(recordFile, buf))
        {
            set->insert(QString(buf.c_str()));
        }

        recordFile.close();

        return QStringList(QList<QString>(set->begin(), set->end()));
    }
};

#endif // FILESAVER_H
