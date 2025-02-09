//
// Created by dmitr on 08.03.2023.
//

#include <memory.h>
#include "String_DAA_lib.h"

#include <ctype.h>
#include <stdio.h>

char stringBuffer_[MAX_STRING_SIZE + 1];

unsigned long long strlen1(const char *s) {
    int i = 0;
    while (s[i])
        i++;

    return i;
}

unsigned long long strlen2(const char *s) {
    int i = 0;
    while (*s) {
        i++;
        s++;
    }

    return i;
}

unsigned long long strlen_(const char *begin) {
    const char *end = begin;
    while (*end)
        end++;

    return end - begin;
}

char *find(char *begin, const char *end, int ch) {
    while (begin != end && *begin != ch)
        begin++;

    return begin;
}

char *findNonSpace(char *begin) {
    while (*begin && isspace(*begin))
        begin++;

    return begin;
}

char *findSpace(char *begin) {
    while (*begin && !isspace(*begin))
        begin++;

    return begin;
}

char *findNonSpaceReverse(char *rbegin, const char *rend) {
    while (rbegin != rend && isspace(*rbegin))
        rbegin--;

    return rbegin;
}

char *findSpaceReverse(char *rbegin, const char *rend) {
    while (rbegin != rend && !isspace(*rbegin))
        rbegin--;

    return rbegin;
}

int strcmp_(const char *lhs, const char *rhs) {
    while (*lhs && *lhs == *rhs) {
        lhs++;
        rhs++;
    }

    return *lhs - *rhs;
}

char *copy(const char *beginSource, const char *endSource,
           char *beginDestination) {
    unsigned long long size = endSource - beginSource;
    memcpy(beginDestination, beginSource, size);

    return beginDestination + size;
}

char *copyIf(char *beginSource, const char *endSource,
             char *beginDestination, int (*f)(int)) {
    while (beginSource != endSource) {
        if (f(*beginSource)) {
            *beginDestination = *beginSource;
            beginDestination++;
        }

        beginSource++;
    }

    return beginDestination;
}

char *copyIfReverse(char *rbeginSource, const char *rendSource,
                    char *beginDestination, int (*f)(int)) {
    while (rbeginSource != rendSource) {
        if (f(*rbeginSource)) {
            *beginDestination = *rbeginSource;
            beginDestination++;
        }

        rbeginSource--;
    }

    return beginDestination;
}

void removeNonLetters(char *s) {
    char *endSource = getEndOfString(s);
    char *destination = copyIf(s, endSource, s, isgraph);
    *destination = '\0';
}

void removeExtraSpaces(char *s) {
    char *destination = s;
    char prevSymbol = *s;
    while (*s) {
        if (*s != prevSymbol || prevSymbol != ' ') {
            *destination = *s;
            destination++;
        }

        prevSymbol = *s++;
    }

    if (prevSymbol == ' ')
        destination--;

    *destination = '\0';
}

int getWord(char *beginSearch, WordDescriptor *word) {
    word->begin = findNonSpace(beginSearch);

    if (!*word->begin)
        return 0;

    word->end = findSpace(word->begin);

    return 1;
}

void digitToEnd(WordDescriptor word) {
    char *endStringBuffer = copy(word.begin,
                                 word.end,
                                 stringBuffer_);
    char *recPos = copyIf(stringBuffer_,
                          endStringBuffer,
                          word.begin,
                          isalpha);
    copyIf(stringBuffer_,
           endStringBuffer,
           recPos,
           isdigit);
}

void replaceDigitsBySpaces(char *s) {
    char *recPtr = stringBuffer_;
    char *readPtr = s;
    while (*readPtr) {
        if (isdigit(*readPtr)) {
            int nSpaces = *readPtr - '0';
            for (int i = 0; i < nSpaces; i++) {
                *recPtr = ' ';
                recPtr++;
            }
        } else {
            *recPtr = *readPtr;
            recPtr++;
        }
        readPtr++;
    }

    long long nSym = recPtr - stringBuffer_;
    memcpy(s, stringBuffer_, nSym);
    s[nSym] = '\0';
}

void replace(char *source, char *w1, char *w2) {
    if (!*w1) return;

    size_t w1Size = strlen_(w1);
    size_t w2Size = strlen_(w2);
    WordDescriptor word1 = {w1, w1 + w1Size};
    WordDescriptor word2 = {w2, w2 + w2Size};

    char *readPtr, *recPtr;
    if (w1Size >= w2Size) {
        readPtr = source;
        recPtr = source;
    } else {
        copy(source,
             source + strlen_(source) + 1,
             stringBuffer_);
        readPtr = stringBuffer_;
        recPtr = source;
    }

    while (*readPtr) {
        WordDescriptor word = {readPtr, readPtr + w1Size};
        if (strcmpWords_(word, word1) == 0) {
            recPtr = copy(word2.begin,
                          word2.end,
                          recPtr);
            readPtr += w1Size;
        } else
            *recPtr++ = *readPtr++;
    }

    *recPtr = '\0';
}

int isLexicographicalOrder(char *s) {
    WordDescriptor lastWord;
    if (!getWord(s, &lastWord))
        return 1;

    WordDescriptor currentWord;
    while (1) {
        if (getWord(lastWord.end, &currentWord) == 0)
            return 1;

        if (strcmpWords_(lastWord, currentWord) > 0)
            return 0;

        lastWord = currentWord;
    }
}

char *getEndOfString(char *s) {
    while (*s) s++;

    return s;
}

void getBagOfWords(BagOfWords *bag, char *s) {
    WordDescriptor temp;
    *bag = (BagOfWords) {{s, s}, 0};
    while (*s) {
        if (!getWord(s, &temp))return;
        bag->words[bag->size].begin = temp.begin;
        bag->words[bag->size].end = temp.end;
        s = temp.end;
        bag->size++;
    }
}

void outputWords(BagOfWords bag) {
    for (int i = 0; i < bag.size; ++i) {
        for (int j = 0; j < bag.words[i].end - bag.words[i].begin; ++j)
            printf("%c", bag.words[i].begin[j]);

        printf("\n");
    }
}

void getWordAlternation(char *s1, char *s2, char *destination) {
    WordDescriptor word1, word2;
    int isW1Found, isW2Found;
    char *beginSearch1 = s1, *beginSearch2 = s2;
    while ((isW1Found = getWord(beginSearch1, &word1)),
            (isW2Found = getWord(beginSearch2, &word2)),
            isW1Found || isW2Found) {
        if (isW1Found) {
            destination = copy(word1.begin, word1.end, destination);
            *destination = ' ';
            destination++;
            beginSearch1 = word1.end;
        }
        if (isW2Found) {
            destination = copy(word2.begin, word2.end, destination);
            *destination = ' ';
            destination++;
            beginSearch2 = word2.end;
        }
    }
    *(destination - 1) = '\0';
}

int strcmpWords_(WordDescriptor w1, WordDescriptor w2) {
    while (w1.begin != w1.end && w2.begin != w2.end && *w1.begin == *w2.begin) {
        w1.begin++;
        w2.begin++;
    }

    if (w1.begin == w1.end && w2.begin == w2.end)
        return 0;

    return (int) (w1.begin - w2.begin);
}