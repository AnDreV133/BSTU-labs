//
// Created by dmitr on 21.09.2024.
//

#ifndef LAB_2_COLORED_SQUARE_RADIALINTERPOLATOR_H
#define LAB_2_COLORED_SQUARE_RADIALINTERPOLATOR_H

// Класс для расчёта радиальной интерполяции
class RadialInterpolator : public BaseInterpolator {
    float cx, cy; // Центр прямоугольника
    COLOR C0, C1; // Цвета радиальной заливки
    float angle;  // Начальный угол заливки

public:
    RadialInterpolator(
            float _x0, float _y0,
            float _x1, float _y1,
            COLOR A0, COLOR A1,
            float _angle
    ) :
            cx((_x0 + _x1) / 2.0f),
            cy((_y0 + _y1) / 2.0f),
            C0(A0), C1(A1),
            angle(_angle) {}


    COLOR color(float x, float y) override{
        double dx = (double) x - cx, dy = (double) y - cy;
        double radius = sqrt(dx * dx + dy * dy);

        float h0 = (sin(radius / 10 + angle) + 1.0f) / 2;
        float h1 = 1 - h0;

        float r = h0 * C0.RED + h1 * C1.RED;
        float g = h0 * C0.GREEN + h1 * C1.GREEN;
        float b = h0 * C0.BLUE + h1 * C1.BLUE;

        return COLOR(r, g, b, global_alpha);
    }
};


#endif //LAB_2_COLORED_SQUARE_RADIALINTERPOLATOR_H
