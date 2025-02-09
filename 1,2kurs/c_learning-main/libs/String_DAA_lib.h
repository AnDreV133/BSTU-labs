//
// Created by dmitr on 08.03.2023.
//

#ifndef MAIN_C_STRING_DAA_LIB_H
#define MAIN_C_STRING_DAA_LIB_H

#endif //MAIN_C_STRING_DAA_LIB_H

#define MAX_STRING_SIZE 60
#define MAX_N_WORDS_IN_STRING 20

typedef struct WordDescriptor {
    char *begin; // позиция начала слова
    char *end; // позиция первого символа, после последнего символа слова
} WordDescriptor;

typedef struct BagOfWords {
    WordDescriptor words[MAX_N_WORDS_IN_STRING];
    size_t size;
} BagOfWords;

unsigned long long strlen1(const char *s);

unsigned long long strlen2(const char *s);

unsigned long long strlen_(const char *s);

char *find(char *begin, const char *end, int ch);

char *findNonSpace(char *begin);

char *findSpace(char *begin);

char *findNonSpaceReverse(char *rbegin, const char *rend);

char *findSpaceReverse(char *rbegin, const char *rend);

int strcmp_(const char *lhs, const char *rhs);

char *copy(const char *beginSource, const char *endSource,
           char *beginDestination);

char *copyIf(char *beginSource, const char *endSource,
             char *beginDestination, int (*f)(int));

char *copyIfReverse(char *rbeginSource, const char *rendSource,
                    char *beginDestination, int (*f)(int));

char *getEndOfString(char *s);

void removeNonLetters(char *s);

int getWord(char *beginSearch, WordDescriptor *word);

int strcmpWords_(WordDescriptor w1, WordDescriptor w2);

void removeExtraSpaces(char *s);

void digitToEnd(WordDescriptor word);

void outputWords(BagOfWords bag);

void getBagOfWords(BagOfWords *bag, char *s);

int isLexicographicalOrder(char *s);

void replace(char *source, char *w1, char *w2);

void replaceDigitsBySpaces(char *s);

void getWordAlternation(char *s1, char *s2, char *destination);