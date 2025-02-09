//
// Created by dmitr on 16.10.2024.
//

#ifndef LAB_3_SAWSHADER_H
#define LAB_3_SAWSHADER_H

#include "Frame.h"

class TriangleShader {
    float x0, y0, x1, y1, x2, y2; // Координаты трёх вершин квадрата в экранной системе координат
    float wx0 = -14, wy0 = -14, wx1 = 14, wy1 = -14, wx2 = 14, wy2 = 14; // Координаты трёх вершин квадрата в мировой системе координат, wx1, wy1, wx2, wy2; // Координаты трёх вершин квадрата в мировой системе координат
    float S; // Площадь треугольника
    COLOR C0; // Цвет ободка и внутренней части
public:
    TriangleShader(
            float _x0, float _y0, float _x1, float _y1, float _x2, float _y2,
            float _wx0, float _wy0, float _wx1, float _wy1,
            float _wx2, float _wy2,
            COLOR _C0
    ) :
            x0(_x0), y0(_y0), x1(_x1), y1(_y1), x2(_x2), y2(_y2),
            wx0(_wx0), wy0(_wy0), wx1(_wx1), wy1(_wy1), wx2(_wx2), wy2(_wy2),
            C0(_C0),
            S((_y1 - _y2) * (_x0 - _x2) + (_x2 - _x1) * (_y0 - _y2)) {
    }

    COLOR main(float wx, float wy) {
        if (
                wy <= 2
                && wy >= 8 * wx - 14
                && wy >= -8 * wx - 14
                && ((wx - 0) * (wx - 0) + (wy - 2) * (wy - 2) >= 2 * 2
                    || (wx - 2) * (wx - 2) + (wy - 0) * (wy - 0) <= 2 * 2)
                ) {
            return C0;
        }
        return {0, 0, 0, 0};
    }

    COLOR color(float x, float y) {

        float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
        float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
        float h2 = ((y0 - y1) * (x - x1) + (x1 - x0) * (y - y1)) / S;
        if (h0 < -1E-6 || h1 < -1E-6 || h2 < -1E-6) {
            return COLOR(0, 0, 0, 0); // Ошибка алгоритма растеризации, если рисуется чёрный пиксель
        }
        float wx = h0 * wx0 + h1 * wx1 + h2 * wx2;
        float wy = h0 * wy0 + h1 * wy1 + h2 * wy2;
        return main(wx, wy);
    }
};

#endif //LAB_3_SAWSHADER_H
