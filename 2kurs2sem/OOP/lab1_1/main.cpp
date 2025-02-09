// #include "mainwindow.h"

// #include <QApplication>
// #include <QLocale>
// #include <QTranslator>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;

int strToInt(string s)
{
    char c;
    int shift = 1;
    int sign = 1;
    int res = 0;
    for (int i = s.length() - 1; i >= 0; i--)
    {
        if (s[i] == '-')
            sign = -1;
        else if (s[i] >= '0' && s[i] <= '9')
            res += (s[i] - '0') * shift;
        else
        {
            cerr << "символ не цифра\n";
            throw;
        }

        shift *= 10;
    }

    return sign * res;
}

vector<float> getFloatNumFromFile(string path)
{
    ifstream fin(path);
    if (!fin.is_open())
    {
        cerr << "проблемы с открытием файла\n";
        throw;
    }

    vector<float> res;
    string numFromFile;
    while (fin >> numFromFile)
    {
        int divPointPosition = numFromFile.find('.');
        int integerPart = strToInt(numFromFile.substr(0, divPointPosition));
        int floatPart = strToInt(numFromFile.substr(divPointPosition + 1, 2));

        int sign = 1;
        if (integerPart < 0)
        {
            sign = -1;
            integerPart = -integerPart;
        }

        res.push_back((float)(sign * (integerPart + (floatPart / 100.0))));
    }

    fin.close();

    return res;
}

enum passGrade
{
    PASS_TRUE,
    PASS_FALSE
};

struct ExemResult
{
    string name;
    string surname;
    string patronymic;
    int physicGrade;
    int mathGrade;
    passGrade russianLangGrade;
};

bool isGradeInBound(int grade, int maxGrade)
{
    return grade >= 0 && grade <= maxGrade;
}

void delBadResults(string path, int maxGradeForDel)
{
    ifstream sourceFile(path);
    if (!sourceFile.is_open())
    {
        cerr << "проблемы с открытием файла\n";
        throw;
    }

    ofstream tempFileForRecord("temp_" + path); // correct the name
    if (!tempFileForRecord.is_open())
    {
        cerr << "проблемы с созданием файла\n";
        throw;
    }

    string abiturientResultString;
    while (getline(sourceFile, abiturientResultString))
    {
        istringstream ist(abiturientResultString);
        vector<string> bufOfWords;
        string iterWord;
        while (ist >> iterWord)
        {
            bufOfWords.push_back(iterWord);
        }

        if (bufOfWords.size() != 6)
        {
            cerr << "не все данные введены или введено слишком много";
            throw;
        }

        ExemResult abiturientResult;
        abiturientResult.name = bufOfWords[0];
        abiturientResult.surname = bufOfWords[1];
        abiturientResult.patronymic = bufOfWords[2];
        int physicGrade = stoi(bufOfWords[3]);
        int mathGrade = stoi(bufOfWords[4]);
        if (!isGradeInBound(physicGrade, 10) || !isGradeInBound(mathGrade, 10))
        {
            cerr << "оценка за пределами от 0 до 10";
            throw;
        }
        abiturientResult.physicGrade = physicGrade;
        abiturientResult.mathGrade = mathGrade;
        if (bufOfWords[5] == "зачёт")
            abiturientResult.russianLangGrade = PASS_TRUE;
        else if (bufOfWords[5] == "незачёт")
            abiturientResult.russianLangGrade = PASS_FALSE;
        else
        {
            cerr << "поле может содержать только слова \"зачёт\" и \"незачёт\"";
            throw;
        }

        if (isGradeInBound(abiturientResult.physicGrade, 3) || isGradeInBound(abiturientResult.mathGrade, 3) || abiturientResult.russianLangGrade == PASS_FALSE || isGradeInBound(abiturientResult.physicGrade + abiturientResult.mathGrade, maxGradeForDel - 1))
        {
        }
        else
        {
            tempFileForRecord << abiturientResult.name << ' ' << abiturientResult.surname << ' ' << abiturientResult.patronymic << ' ' << abiturientResult.physicGrade << ' ' << abiturientResult.mathGrade << ' ' << (abiturientResult.russianLangGrade == PASS_TRUE ? "зачёт" : "незачёт");
        }
    }

    sourceFile.close();
    tempFileForRecord.close();

    if (remove(path.data()))
    {
        cerr << "файл не удалён";
        throw;
    }

    if (rename(("temp_" + path).data(), path.data()))
    {
        cerr << "временный файл не переименнован";
        throw;
    }
}

int main()
{
    setlocale(LC_ALL, "rus");
    // a)
    for (auto num : getFloatNumFromFile("test.txt"))
    {
        cout << num << ' ';
    }

    // b)
    delBadResults("abitura.txt", 0);

    return 0;
}
