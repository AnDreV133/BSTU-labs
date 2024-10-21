//
// Created by dmitr on 25.09.2024.
//

#ifndef LAB_2_COLORED_SQUARE_RADIALMULTICOLORINTERPOLATOR_H
#define LAB_2_COLORED_SQUARE_RADIALMULTICOLORINTERPOLATOR_H


#include <vector>
#include <iostream>
#include "BaseInterpolator.h"
#include "global_values.h"

class RadialMultiColorInterpolator : public BaseInterpolator {
    std::vector<COLOR> &colors;
    float cx, cy;
    float angle;
public:
    RadialMultiColorInterpolator(float _cx, float _cy, float _angle, std::vector<COLOR> &_colors)
            : colors(_colors), cx(_cx), cy(_cy), angle(_angle) {}

    COLOR color(float x, float y) override {
        double dx = (double) x - cx, dy = (double) y - cy;
        double radius = sqrt(dx * dx + dy * dy);

        float h = (radius / 20 + angle) / colors.size();
        h = ((h) - (int) (h)) * colors.size();

        int index0 = (int) ((h)) % colors.size();
        int index1 = ((int) (h) + 1) % colors.size();

        float h1 = ((h) - (int) (h));
        float h0 = (1 - h1);

//        std::cout << h << ' ' << h0 << " " << h1 << std::endl;

        float r = h0 * colors[index0].RED + h1 * colors[index1].RED;
        float g = h0 * colors[index0].GREEN + h1 * colors[index1].GREEN;
        float b = h0 * colors[index0].BLUE + h1 * colors[index1].BLUE;

        return COLOR(r, g, b, global_alpha);
    }
};


#endif //LAB_2_COLORED_SQUARE_RADIALMULTICOLORINTERPOLATOR_H
