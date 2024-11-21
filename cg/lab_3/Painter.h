#ifndef PAINTER_H
#define PAINTER_H

#include <exception>
#include <iostream>
#include <ntdef.h>
#include <cmath>
#include "Frame.h"
#include "Assets.h"
#include "global_values.h"
#include "Matrix.h"
#include "interpolator/BarycentricInterpolator.h"
#include "SawShader.h"

class Painter {
    COLOR sawColor = {150, 150, 150};
    COLOR sawCenterColor = {100, 100, 100};

    void drawSaw_1(
            Frame &frame,
            Matrix &WS,
            float xC, float yC,
            float globalAngle
    ) {
        globalAngle = globalAngle * 180.0f / M_PI * 2;

        float maxAngle = 360.0f;
        float rotateAngle = 10.0f;
        float angle = 0.0f;
        while (angle < maxAngle) {

            // точки для рисования зубьев
            Vector p[] = {
                    Vector(-0.5f, 0), Vector(0.5f, 0),
                    Vector(0.5f, 1), Vector(-0.5f, 1)};

            for (int i = 0; i < _countof(p); i++) {
                p[i] = p[i] * Matrix::rotation(angle + globalAngle) * WS;
            }

            // применяем шейдер
            TriangleShader shader1(
                    p[0].x() + xC, p[0].y() + yC,
                    p[1].x() + xC, p[1].y() + yC,
                    p[2].x() + xC, p[2].y() + yC,
                    -14, -14, 14, -14, 14, 14,
                    sawColor
            );
            frame.Triangle(
                    p[0].x() + xC, p[0].y() + yC,
                    p[1].x() + xC, p[1].y() + yC,
                    p[2].x() + xC, p[2].y() + yC,
                    shader1
            );
            TriangleShader shader2(
                    p[2].x() + xC, p[2].y() + yC,
                    p[3].x() + xC, p[3].y() + yC,
                    p[0].x() + xC, p[0].y() + yC,
                    14, 14, -14, 14, -14, -14,
                    sawColor
            );
            frame.Triangle(
                    p[2].x() + xC, p[2].y() + yC,
                    p[3].x() + xC, p[3].y() + yC,
                    p[0].x() + xC, p[0].y() + yC,
                    shader2
            );

            // отклоняем угол для рисования следующего зуба
            angle += rotateAngle;
        }
    }

    void drawSaw_2(
            Frame &frame,
            Matrix &WS,
            float xC, float yC,
            float radius,
            float maxCoord,
            float globalAngle
    ) {
//        globalAngle = globalAngle * 180.0f / M_PI * 2;
        globalAngle = 0;
        maxCoord = maxCoord * 2;

        float maxAngle = 360.0f;
        float rotateAngle = 12.0f;
        float angle = 0.0f;
        while (angle < maxAngle) {

            // точки для рисования зубьев
            Vector p4 = {maxCoord / 2, maxCoord / 2, 1.0f};
            p4 = p4 * Matrix::rotation(globalAngle /*+ angle*/) * WS;

            Vector p3 = {-maxCoord / 2, maxCoord / 2, 1.0f};
            p3 = p3 * Matrix::rotation(globalAngle /*+ angle*/) * WS;

            Vector p2 = {maxCoord / 2, -maxCoord / 2, 1.0f};
            p2 = p2 * Matrix::rotation(globalAngle /*+ angle*/) * WS;

            Vector p1 = {-maxCoord / 2, -maxCoord / 2, 1.0f};
            p1 = p1 * Matrix::rotation(globalAngle /*+ angle*/) * WS;

            TriangleShader shader1(
                    p1.x() + xC, p1.y() + yC,
                    p2.x() + xC, p2.y() + yC,
                    p4.x() + xC, p4.y() + yC,
                    -14, -14, 14, -14, 14, 14,
                    sawColor
            );
            frame.Triangle(
                    p1.x() + xC, p1.y() + yC,
                    p2.x() + xC, p2.y() + yC,
                    p4.x() + xC, p4.y() + yC,
                    shader1
            );
            TriangleShader shader2(
                    p1.x() + xC, p1.y() + yC,


                    p3.x() + xC, p3.y() + yC,
                    p4.x() + xC, p4.y() + yC,
                    -14, -14, 14, -14, 14, 14,

                    sawColor
            );
            frame.Triangle(
                    p3.x() + xC, p3.y() + yC,

                    p4.x() + xC, p4.y() + yC,
                    p1.x() + xC, p1.y() + yC,

                    shader2
            );

            // отклоняем угол для рисования следующего зуба
            angle += rotateAngle;
        }
    }

    void drawSaw(
            Frame &frame,
            Matrix &WS,
            float xC, float yC,
            float radius,
            float maxCoord,
            float globalAngle
    ) {
        globalAngle = globalAngle * 180.0f / M_PI * 2;

        float maxAngle = 360.0f;
        float rotateAngle = 12.0f;
        float angle = 0.0f;
        while (angle < maxAngle) {
            // точки для рисования зубьев
            Vector p5 = {0.0f, maxCoord, 1.0f};
            p5 = p5 * Matrix::rotation(10 + globalAngle + angle) * WS;

            Vector p4 = {0.0f, maxCoord - 1, 1.0f};
            p4 = p4 * Matrix::rotation(358 + globalAngle + angle) * WS;

            Vector p3 = {0.0f, maxCoord - 2, 1.0f};
            p3 = p3 * Matrix::rotation(3 + globalAngle + angle) * WS;

            Vector p2 = {0.0f, maxCoord - 3, 1.0f};
            p2 = p2 * Matrix::rotation(10 + globalAngle + angle) * WS;

            Vector p1 = {0.0f, maxCoord - 3, 1.0f};
            p1 = p1 * Matrix::rotation(350 + globalAngle + angle) * WS;

            // рисуем зубья
            frame.Triangle(
                    p5.x() + xC, p5.y() + yC,
                    p4.x() + xC, p4.y() + yC,
                    p3.x() + xC, p3.y() + yC,
                    sawColor
            );
            frame.Triangle(
                    p4.x() + xC, p4.y() + yC,
                    p2.x() + xC, p2.y() + yC,
                    p1.x() + xC, p1.y() + yC,
                    sawColor
            );

            // отклоняем угол для рисования следующего зуба
            angle += rotateAngle;
        }
        // рисуем внешний круг
        frame.Circle(
                xC, yC,
                radius * 0.84,
                sawColor
        );
        // рисуем внутренний круг
        frame.Circle(
                xC, yC,
                radius * 0.3,
                sawCenterColor
        );
    }

    int currH = 0;
    float currAngle = 0;

    void drawStars(
            Frame &frame,
            Matrix WS,
            float xC, float yC,
            float globalAngle
    ) {
        globalAngle = globalAngle * 180.0f / M_PI;

        // получаем переменную для просчёта удаления искр
        int h = (int) globalAngle % 40;
        // условие для повторного воспроизведения
        if (currH > h) currAngle = rand() % 40 - 10;
        currH = h;


        // изменяем положение координатной плоскости
        Matrix toss =
                Matrix::rotation(currAngle)
                * Matrix::transform(0, h * 0.075f)
                * Matrix::rotation(globalAngle)
                * Matrix::scale(h * 0.02f);

        // применяем матрицу на точки для построения звезды
        Vector p1 = {0.0f, 0.75, 1.0f};
        p1 = p1 * toss * WS;
        Vector p2 = {0.0f, 0.25, 1.0f};
        p2 = p2 * toss * WS;
        Vector p3 = {0.0f, -0.3, 1.0f};
        p3 = p3 * toss * WS;
        Vector p4 = {0.5, -0.75, 1.0f};
        Vector p4_ = {-0.5, -0.75, 1.0f};
        p4 = p4 * toss * WS;
        p4_ = p4_ * toss * WS;
        Vector p5 = {0.75, 0.25, 1.0f};
        Vector p5_ = {-0.75, 0.25, 1.0f};
        p5 = p5 * toss * WS;
        p5_ = p5_ * toss * WS;

        // интерополируем цвета
        BarycentricInterpolator interpolator(
                xC, yC + 10,
                0, 0,
                frame.width - 1, 0,
                {255, 255, 0},
                {180, 180, 180},
                {180, 180, 180}
        );
        // получаем цвет из интерполятора
        COLOR starColor = interpolator.color(p3.x() + xC, p3.y() + yC);

        // рисуем звезду
        frame.Triangle(
                p5.x() + xC, p5.y() + yC,
                p2.x() + xC, p2.y() + yC,
                p3.x() + xC, p3.y() + yC,
                starColor
        );
        frame.Triangle(
                p4.x() + xC, p4.y() + yC,
                p3.x() + xC, p3.y() + yC,
                p1.x() + xC, p1.y() + yC,
                starColor
        );
        frame.Triangle(
                p5_.x() + xC, p5_.y() + yC,
                p2.x() + xC, p2.y() + yC,
                p3.x() + xC, p3.y() + yC,
                starColor
        );
        frame.Triangle(
                p4_.x() + xC, p4_.y() + yC,
                p3.x() + xC, p3.y() + yC,
                p1.x() + xC, p1.y() + yC,
                starColor
        );
    }

public:
    void Draw(Frame &frame) {
        int W = frame.width, H = frame.height;

        // Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана
        float a = 7.0 / 8.0 * ((W < H) ? W - 1 : H - 1) / 2;

        Assets::drawChessBg(frame);

        Matrix WS = Matrix::WorldToScreen(
                W / 2 - a, H / 2 - a,
                W / 2 + a, H / 2 + a,
                -1, -1, 1, 1
        );


        float globalAngle = global_angle;

//        TriangleShader shader(
//                W, H,
//                0, 0,
//                W,-H,
//                sawColor
//        );
//        frame.Triangle(
//                W, H,
//                0, 0,
//                W,-H,
//                shader
//        );
//        frame.Triangle(
//                W, H,
//                0, 0,
//                -W,H,
//                shader
//        );

        drawSaw_1(
                frame,
                WS,
                W / 2, H / 2,
                globalAngle
//                0
        );

        drawStars(
                frame,
                WS,
                W * 5 / 8.2, H / 2,
                globalAngle
        );
    }
//
//    void Draw(Frame &frame) {
//        float W = frame.width, H = frame.height;
//
//        // Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана
//        float a = 7.0 / 8.0 * ((W < H) ? W : H);
//
//        float beta = global_angle; // Угол поворота фигуры
//
//        // Матрица для преобразования мировых координат в экранные
//        // Область вывода определяем как квадрат со стороной a в центре экрана
//        Matrix WS = Matrix::WorldToScreen(W / 2 - a / 2, H / 2 - a / 2, W / 2 + a / 2, H / 2 + a / 2, -1, -1, 1, 1);
//
//        // Шейдерный класс для рисования треугольника, который будет половиной квадрата
//        class TriangleShader {
//            float x0, y0, x1, y1, x2, y2;   // Координаты трёх вершин квадрата в экранной системе координат
//            float wx0, wy0, wx1, wy1, wx2, wy2;  // Координаты трёх вершин квадрата в мировой системе координат
//            float S;  // Площадь треугольника
//            COLOR C0, C1; // Цвет ободка и внутренней части
//
//        public:
//            TriangleShader(float _x0, float _y0, float _x1, float _y1, float _x2, float
//            _y2,
//                           float _wx0, float _wy0, float _wx1, float _wy1,
//                           float _wx2, float _wy2, COLOR _C0, COLOR _C1) :
//                    x0(_x0), y0(_y0), x1(_x1), y1(_y1), x2(_x2), y2(_y2),
//                    wx0(_wx0), wy0(_wy0), wx1(_wx1), wy1(_wy1), wx2(_wx2), wy2(_wy2),
//                    C0(_C0), C1(_C1),
//                    S((_y1 - _y2) * (_x0 - _x2) + (_x2 - _x1) * (_y0 - _y2)) {
//            }
//
//            // Основная функция main рассчитывает цвет точки с координатами (x, y) для квадрата, который расположен в начале координат, и
//            // закругляет ему края
//            // Центр квадрата имеет координаты (0, 0). Сторона квадрата равна 2
//            COLOR main(float x, float y) {
//                if (x < 0) x = -x; // Отражаем точку в первую четверть, чтобы было легче считать
//                if (y < 0) y = -y;
//
//                float r = 0.5; // Радиус сопряжения
//                // Координаты центра сопряжения в первой четверти
//                float x0 = 1 - r, y0 = 1 - r;
//                // Если точка выходит за пределы области, возвращаем полностью прозрачный пиксель
//                if (x + y > 2 - r && (x - x0) * (x - x0) + (y - y0) * (y - y0) > r * r) {
//                    return COLOR(0, 0, 0, 0);
//                }
//
////                float d;
////                // Сделаем так, чтобы цвет зависел от расстояния до границы фигуры
////                // Если точка находится вблизи угла квадрата, то рассчитываем расстояние от точки (x, y) до круга и делим его на радиус
////                if (x > 1 - r && y > 1 - r)
////                {
////                    d = (r - sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0))) / r;
////                }
////                else // В противном случае находим ближайшую сторону квадрата
////                {
////                    if (x > 1 - r || y > 1 - r)
////                        d = std::min(1 - y, 1 - x) / r;
////                    else d = 1;
////                }
//                return C0;
//            }
//
//            COLOR color(float x, float y) {
//                // Барицентрическая интерполяция
//                float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
//                float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
//                float h2 = ((y0 - y1) * (x - x1) + (x1 - x0) * (y - y1)) / S;
//                //float h2 = 1 - h0 - h1;
//
//                // Если точка (x, y) находится вне треугольника
//                if (h0 < -1E-6 || h1 < -1E-6 || h2 < -1E-6) {
//                    return COLOR(0, 0, 0); // Ошибка алгоритма растеризации, еслирисуется чёрный пиксель
//                }
//
//                // Интерполируем мировые координаты вершин
//                float wx = h0 * wx0 + h1 * wx1 + h2 * wx2;
//                float wy = h0 * wy0 + h1 * wy1 + h2 * wy2;
//
//                return main(wx, wy);
//            }
//        };
//
//
//        Vector A[] = {Vector(-0.75f, -0.75f), Vector(0.75f, -0.75f), Vector(0.75f,
//                                                                            0.75f), Vector(-0.75f, 0.75f)};
//
//        // Поворот квадрата
//        for (int i = 0; i < _countof(A); i++) {
//            A[i] = A[i] * (Matrix::rotation(beta * 5) * WS);
//        }
//
//
//        TriangleShader shader1(A[0].x(), A[0].y(), A[1].x(), A[1].y(), A[2].x(), A[2].y(), -1, -1, 1, -1, 1, 1,
//                               COLOR(50, 255, 50), COLOR(255, 255, 255));
//        frame.Triangle(A[0].x(), A[0].y(), A[1].x(), A[1].y(), A[2].x(), A[2].y(), shader1);
//
////        TriangleShader shader2(A[2].x(), A[2].y(), A[3].x(), A[3].y(), A[0].x(), A[0].y(), 1, 1, -1,
////                               1, -1, -1, COLOR(50, 255, 50), COLOR(255, 255, 255));
////        frame.Triangle(A[2].x(), A[2].y(), A[3].x(), A[3].y(), A[0].x(), A[0].y(), shader2);
//
//        drawSaw_1(
//                frame,
//                WS,
//                W / 2, H / 2,
//                a / 2,
//                14,
//                beta
////                0
//        );
//    }
};

#endif // PAINTER_H


//        std::cout << modes[0] << ' ' << modes[1] << ' ' << modes[2] << ' ' << modes[3] << ' ' << modes[4] << ' '
//                  << std::endl;

//        SectorInterpolator interpolator(C.x, C.y);
//        frame.Triangle(0, 0, W - 1, 0, 0, H - 1, interpolator);
//        frame.Triangle(W - 1, 0, 0, H - 1, W - 1, H - 1, interpolator);

//        if(interpolator != nullptr) delete interpolator;