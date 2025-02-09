//
// Created by admin on 24.12.2022.
//

#include <stdio.h>
#include <stdbool.h>

#include "Circle.h"

#define EPS 0.000001

// ввод параметров окружности с
void inputCircle(Circle *c) {
    inputPoint(&c->center);
    scanf("%lf", &c->r);
}

// ввод массива с размера n параметров окружностей
void inputCircles(Circle *c, const size_t n) {
    for (size_t i = 0; i < n; ++i, ++c)
        inputCircle(c);
}

// вывод параметров окружности с
void outputCircle(Circle c) {
    outputPoint(c.center);
    printf("%.4f\n", c.r);
}

// вывод массива с размера n параметров окружностей
void outputCircles(const Circle *const c, const size_t n) {
    for (size_t i = 0; i < n; ++i)
        outputCircle(c[i]);
}

// возвращает true если окружности с1 и с2 соприкасаются
// в одной точке, иначе - false
bool hasOneOuterIntersection(Circle c1, Circle c2) {
    return getDistance(c1.center, c2.center) - (c1.r + c2.r) < EPS;
}
