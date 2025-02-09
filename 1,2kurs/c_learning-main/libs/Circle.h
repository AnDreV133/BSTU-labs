//
// Created by admin on 24.12.2022.
//

#ifndef MAIN_C_CIRCLE_H
#define MAIN_C_CIRCLE_H

#endif //MAIN_C_CIRCLE_H

#include "Point.h"

typedef struct Circle {
    Point center;
    double r;
} Circle;

// ввод параметров окружности с
void inputCircle(Circle *c);

// ввод массива с размера n параметров окружностей
void inputCircles(Circle *c, const size_t n);

// вывод параметров окружности с
void outputCircle(Circle c);

// вывод массива с размера n параметров окружностей
void outputCircles(const Circle *const c, const size_t n);

// возвращает true если окружности с1 и с2 соприкасаются
// в одной точке, иначе - false
bool hasOneOuterIntersection(Circle c1, Circle c2);
