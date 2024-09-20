#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include "Assets.h"

class Painter_Example {
public:
    void draw(Frame &frame) {
        Assets::drawChessBg(frame);

        int W = frame.width, H = frame.height;
        // Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана
        float a = 7.0f / 8 * ((W < H) ? W - 1 : H - 1) / sqrt(2);
        if (a < 1) return; // Если окно очень маленькое, то ничего не рисуем
        float angle = globalAngle; // Угол поворота
        a = a / 2;

        // Инициализируем исходные координаты центра и вершин квадрата
        struct {
            float x;
            float y;
        } C = {W / 2.f, H / 2.f}, A[4] = {{C.x + a, C.y + a},
                                          {C.x + a, C.y - a},
                                          {C.x - a, C.y - a},
                                          {C.x - a, C.y + a}};


        // Поворачиваем все вершины квадрата вокруг точки C на угол angle
        for (auto &ai: A) {
            float xi = ai.x, yi = ai.y;
            ai.x = (xi - C.x) * cos(angle) - (yi - C.y) * sin(angle) + C.x;
            ai.y = (xi - C.x) * sin(angle) + (yi - C.y) * cos(angle) + C.y;
        }

        // Рисуем стороны квадрата
        for (int i = 0; i < 4; i++) {
            int i2 = (i + 1) % 4;
            frame.drawLine( // Добавляем везде 0.5f, чтобы вещественные числа правильно округлялись при преобразовании к целому типу
                    {int(A[i].x + 0.5f), int(A[i].y + 0.5f)},
                    {int(A[i2].x + 0.5f), int(A[i2].y + 0.5f)},
                    Color(200, 30, 45)
            );
        }

        // Рисуем описанную окружность
        frame.drawCircle(
                {(int) C.x, (int) C.y},
                int(a * sqrt(2) + 0.5f),
                Color(100, 100, 250)
        );

        // Рисуем пиксель, на который кликнул пользователь
        if (globalClickedPixel.x >= 0 && globalClickedPixel.x < W &&
            globalClickedPixel.y >= 0 && globalClickedPixel.y < H)
            frame.setPixel(
                    {globalClickedPixel.x, globalClickedPixel.y},
                    {34, 175, 60}
            ); // Пиксель зелёного цвета
    }
};

#endif // PAINTER_H