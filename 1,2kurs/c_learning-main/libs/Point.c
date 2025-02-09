//
// Created by admin on 24.12.2022.
//


#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "Point.h"

#define EPS 0.000001

// ввод координат точки p
void inputPoint(Point *p) {
    scanf("%lf %lf", &p->x, &p->y);
}

// вывод координат точки p
void outputPoint(Point p) {
    printf("(%.4f; %.4f)\n", p.x, p.y);
}

// ввод массива p размера n координат точек
void inputPoints(Point *p, const size_t n) {
    for (size_t i = 0; i < n; ++i, ++p)
        inputPoint(p);
}

// вывод массива p размера n координат точек
void outputPoints(const Point *const p, const size_t n) {
    for (size_t i = 0; i < n; ++i)
        outputPoint(p[i]);
}

// возвращает среднюю точку между p1 и p2
Point getMiddlePoint(Point p1, Point p2) {
    p1.x = (p1.x + p2.x) / 2;
    p1.y = (p1.y + p2.y) / 2;

    return p1;
}

// возвращает true если координаты p1 равны координатам p2, иначе false
bool arePointsEqual(Point p1, Point p2) {
    return fabs(p1.x - p2.x) < EPS && fabs(p1.y - p2.y) < EPS;
}

// возвращает true если p3 ровно посеридине между p1 и p2, иначе false
bool isPointBetween(Point p1, Point p2, Point p3) {
    return arePointsEqual(getMiddlePoint(p1, p2), p3);
}

// обменивает координаты p между собой
void swapCoordinates(Point *p) {
    double t = p->x;
    p->x = p->y;
    p->y = t;
}

// обменивает течки p1 и p2 между собой
void swapPoints(Point *p1, Point *p2) {
    Point t = *p1;
    *p1 = *p2;
    *p2 = t;
}

// возвращает длинну от течки p1 до точки p2
double getDistance(Point p1, Point p2) {
    double delX2 = pow(p1.x - p2.x, 2);
    double delY2 = pow(p1.y - p2.y, 2);

    return sqrt(delX2 + delY2);
}

// добавляет точку по координатам x и y в массив точек pN
// по индексу i
void addPoint(Point *const pN, size_t i, double x, double y) {
    Point *p = pN;
    p += i;
    p->x = x;
    p->y = y;
}

// компаратор для qsort, сортирует по неубыванию
// координат точек, если x соыпадает, то сравнивается по y
int comparatorPoint(const Point *a, const Point *b) {
    if (a->x > b->x)
        return 1;
    else if (fabs(a->x - b->x) < EPS)
        if (a->y > b->y)
            return 1;
        else
            return -1;
    else
        return -1;
}