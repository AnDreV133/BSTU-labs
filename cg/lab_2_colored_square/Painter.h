#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include "Assets.h"

float global_angle = 0;

class Painter {
public:

    void Draw(Frame &frame) {

        // Класс для расчёта барицентрической интерполяции
        class BarycentricInterpolator {
            float x0, y0, x1, y1, x2, y2, S;
            COLOR C0, C1, C2;

        public:
            BarycentricInterpolator(float _x0, float _y0, float _x1, float _y1, float _x2, float _y2, COLOR A0,
                                    COLOR A1, COLOR A2) :
                    x0(_x0), y0(_y0), x1(_x1), y1(_y1), x2(_x2), y2(_y2),
                    S((_y1 - _y2) * (_x0 - _x2) + (_x2 - _x1) * (_y0 - _y2)), C0(A0), C1(A1), C2(A2) {
            }


            COLOR color(float x, float y) {
                // Барицентрическая интерполяция
                float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
                float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
                float h2 = 1 - h0 - h1;
                float r = h0 * C0.RED + h1 * C1.RED + h2 * C2.RED;
                float g = h0 * C0.GREEN + h1 * C1.GREEN + h2 * C2.GREEN;
                float b = h0 * C0.BLUE + h1 * C1.BLUE + h2 * C2.BLUE;
                float a = h0 * C0.ALPHA + h1 * C1.ALPHA + h2 * C2.ALPHA;
                // Из-за погрешности аппроксимации треугольника учитываем, что центр закрашиваемого пикселя может находится вне треугольника.
                // По этой причине значения r, g, b могут выйти за пределы диапазона [0, 255].
                return COLOR(r, g, b, a);
            }
        };


        // Класс для расчёта радиальной интерполяции
        class RadialInterpolator {
            float cx, cy; // Центр прямоугольника
            COLOR C0, C1; // Цвета радиальной заливки
            float angle;  // Начальный угол заливки

        public:
            RadialInterpolator(float _x0, float _y0, float _x1, float _y1, COLOR A0, COLOR A1, float _angle) :
                    cx((_x0 + _x1) / 2.0f), cy((_y0 + _y1) / 2.0f),
                    C0(A0), C1(A1), angle(_angle) {
            }


            COLOR color(float x, float y) {
                double dx = (double) x - cx, dy = (double) y - cy;
                double radius = sqrt(dx * dx + dy * dy);

                float h0 = (sin(radius / 10 + angle) + 1.0f) / 2;
                float h1 = 1 - h0;

                float r = h0 * C0.RED + h1 * C1.RED;
                float g = h0 * C0.GREEN + h1 * C1.GREEN;
                float b = h0 * C0.BLUE + h1 * C1.BLUE;

                return COLOR(r, g, b);

            }
        };

        // Класс для расчёта радиальной интерполяции
        class SectorInterpolator {
            COLOR Red = {255, 0, 0},
                    Green = {0, 255, 0},
                    Blue = {0, 0, 255};
            float cx, cy; // Центр
            float angle = 0;  // Начальный угол красного цвета

//            float getangle(float x, float y, float cx, float cy) {
////                Vector2 relPoint = point - center;
//                return
////                return atan2(y-cy, x-cx);
//            }

        public:
            SectorInterpolator(float _x0, float _y0, float _r, float _angle = 0) :
                    cx(_x0), cy(_y0), angle(_angle) {
            }

            COLOR color(float x, float y) {
//                double dx = (double) x - cx, dy = (double) y - cy;
//                double radius = sqrt(dx * dx + dy * dy);
//
//                float h0 = (sin(radius / 10 + angle) + 1.0f) / 2;
//                float h1 = 1 - h0;

                float a = static_cast<int>((atan2(y - cy, x - cx) * 180 / M_PI) + 450) % 360 / 360.0;
//
                float r = 0;
                float g = 0;
                float b = 0;

                const float third_part = 0.333333;

                if (a >= 0 && a < third_part) {
                    r = 255 - 255 * a;
                    g = 255 - 255 * (1 - a / third_part);
                } else if (a >= third_part && a < 2 * third_part) {
                    a -= third_part;
                    g = 255 - 255 * a;
                    b = 255 - 255 * (1 - a / third_part);
                } else if (a >= 2 * third_part && a <= 1){
                    a -= 2 * third_part;
                    b = 255 - 255 * a;
                    r = 255 - 255 * (1 - a / third_part);
                }

                return COLOR(r, g, b);
            }
        };


        int W = frame.width, H = frame.height;

        // Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана
        float a = 7.0 / 8.0 * ((W < H) ? W - 1 : H - 1) / sqrt(2);

        float angle = global_angle; // Угол поворота
        a = a / 2;

        // Инициализируем исходные координаты центра и вершин квадрата
        struct {
            float x;
            float y;
        }
                C = {W / 2 + 0.5f, H / 2 + 0.5f},

                A[3] = {
                {C.x, C.y - a},
                {static_cast<float>(+a * sin(2. * M_PI / 3) + C.x),
                      static_cast<float>( -a * cos(2. * M_PI / 3) + C.y)},
                {static_cast<float>(+a * sin(-2. * M_PI / 3) + C.x),
                      static_cast<float>(-a * cos(-2. * M_PI / 3) + C.y)}},
                B[3] = {
                {A[0].x, A[0].y},
                {A[1].x, A[1].y},
                {A[2].x, A[2].y}
        };

        // Поворачиваем все вершины квадрата вокруг точки C на угол angle
        for (
                int i = 0;
                i < 3; i++) {
            double xi = A[i].x, yi = A[i].y;
            A[i].x = (xi - C.x) * cos(angle) - (yi - C.y) * sin(angle) + C.x;
            A[i].y = (xi - C.x) * sin(angle) + (yi - C.y) * cos(angle) + C.y;
        }

        float x0 = 0, y0 = 0, x1 = W, y1 = H;

        Assets::drawChessBg(frame);
//
        frame.
                Circle(
                C
                        .x, C.y,
                a,
                {255, 0, 0}
        );
        frame.
                Triangle(
                B[0]
                        .x, B[0].y,
                B[1].x, B[1].y,
                B[2].x, B[2].y,
                {0, 255, 0}
        );
        frame.
                Circle(
                C
                        .x, C.y,
                a / 2,
                {255, 0, 0}
        );

        SectorInterpolator interpolator1(C

                                                 .x, C.y, a);

        interpolator1.
                color(W
                      / 2 + 2, 4);
        interpolator1.
                color(700, 439);
        interpolator1.
                color(226, 492);
        interpolator1.
                color(534, 17);

        frame.Triangle(
                (A[0].x + C.x) / 2, (A[0].y + C.y) / 2,
                (A[1].x + C.x) / 2, (A[1].y + C.y) / 2,
                (A[2].x + C.x) / 2, (A[2].y + C.y) / 2,
//                {0, 255, 0}
                interpolator1
        );
        frame.Circle(
                C.x, C.y,
                a / 4,
                {255, 0, 0}
        );

        SectorInterpolator interpolator(C.x, C.y, a);
        frame.Triangle(0, 0, W - 1, 0, 0, H - 1, interpolator);
        frame.Triangle(W - 1, 0, 0, H - 1, W - 1, H - 1, interpolator);

    }
};

#endif // PAINTER_H


/*

  RadialInterpolator radialInterpolator(
                x0, y0,
                x1, y1,
                COLOR(255, 0, 0),
                COLOR(100, 20, 0),
                global_angle
        );
        frame.Triangle(
                x0, y0,
                x0, y1,
                x1, y0,
                radialInterpolator
        );
        frame.Triangle(x0, y1,
                       x1, y0,
                       x1, y1,
                       radialInterpolator
        );

        // Рисование полупрозрачного квадрата
        BarycentricInterpolator interpolator1(
                A[3].x, A[3].y,
                A[2].x, A[2].y,
                A[0].x, A[0].y,
                COLOR(0, 255, 0, 200),
                COLOR(255, 0, 0, 200),
                COLOR(0, 0, 255, 200)
        );
        frame.Triangle(
                A[3].x, A[3].y,
                A[2].x, A[2].y,
                A[0].x, A[0].y,
                interpolator1
        );
//
        BarycentricInterpolator interpolator2(
                A[0].x, A[0].y,
                A[2].x, A[2].y,
                A[1].x, A[1].y,
                COLOR(0, 0, 255, 200),
                COLOR(255, 0, 0, 200),
                COLOR(0, 255, 0, 200)
        );
        frame.Triangle(
                A[0].x, A[0].y,
                A[2].x, A[2].y,
                A[1].x, A[1].y,
                interpolator2
        );

 */