//
// Created by dmitr on 21.09.2024.
//

#ifndef LAB_2_COLORED_SQUARE_SECTORINTERPOLATOR_H
#define LAB_2_COLORED_SQUARE_SECTORINTERPOLATOR_H


#include "BaseInterpolator.h"

// Класс для расчёта радиальной интерполяции
class SectorInterpolator : public BaseInterpolator {
    COLOR Red = {255, 0, 0},
            Green = {0, 255, 0},
            Blue = {0, 0, 255};
    float cx, cy; // Центр
    float angle = 0;  // Начальный угол красного цвета

public:
    SectorInterpolator(float _x0, float _y0) :
            cx(_x0), cy(_y0) {
    }

    COLOR color(float x, float y) override {
        int r = 0;
        int g = 0;
        int b = 0;

        float angle = 180.f + atan2(y - cy, x - cx) * 180 / M_PI;
        if (angle >= 0 && angle < 60) {
            r = 255;
            g = (angle / 60.0) * 255;
        }
        else if (angle >= 60 && angle < 120) {
            angle -= 60;
            r = (1 - angle / 60.0) * 255;
            g = 255;
        }
        else if (angle >= 120 && angle < 180) {
            angle -= 120;
            g = 255;
            b = (angle / 60.0) * 255;
        }
        else if (angle >= 180 && angle < 240) {
            angle -= 180;
            g = (1 - angle / 60.0) * 255;
            b = 255;
        }
        else if (angle >= 240 && angle < 300) {
            angle -= 240;
            r = (angle / 60.0) * 255;
            b = 255;
        }
        else if (angle >= 300 && angle <= 360) {
            angle -= 300;
            r = 255;
            b = (1 - angle / 60.0) * 255;
        }

        return COLOR(r, g, b, global_alpha);
    }
};


#endif //LAB_2_COLORED_SQUARE_SECTORINTERPOLATOR_H
