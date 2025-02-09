//
// Created by admin on 24.12.2022.
//

#ifndef MAIN_C_POINT_H
#define MAIN_C_POINT_H

#endif //MAIN_C_POINT_H

typedef struct Point {
    double x;
    double y;
} Point;

// ввод координат точки p
void inputPoint(Point *p);

// вывод координат точки p
void outputPoint(Point p);

// ввод массива p размера n координат точек
void inputPoints(Point *p, const size_t n);

// вывод массива p размера n координат точек
void outputPoints(const Point *const p, const size_t n);

// возвращает среднюю точку между p1 и p2
Point getMiddlePoint(Point p1, Point p2);

// возвращает true если координаты p1 равны координатам p2, иначе false
bool arePointsEqual(Point p1, Point p2);

// возвращает true если p3 ровно посеридине между p1 и p2, иначе false
bool isPointBetween(Point p1, Point p2, Point p3);

// обменивает координаты p между собой
void swapCoordinates(Point *p);

// обменивает течки p1 и p2 между собой
void swapPoints(Point *p1, Point *p2);

// возвращает длинну от течки p1 до точки p2
double getDistance(Point p1, Point p2);

// добавляет точку по координатам x и y в массив точек pN
// по индексу i
void addPoint(Point *const pN, size_t i, double x, double y);

// компаратор для qsort, сортирует по неубыванию
// координат точек, если x соыпадает, то сравнивается по y
int comparatorPoint(const Point *a, const Point *b);