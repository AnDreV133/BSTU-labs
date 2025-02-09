//
// Created by dmitr on 22.09.2024.
//

#ifndef LAB_2_COLORED_SQUARE_ONECOLORINTERPOLATOR_H
#define LAB_2_COLORED_SQUARE_ONECOLORINTERPOLATOR_H


#include "BaseInterpolator.h"

class OneColorInterpolator : public BaseInterpolator {
    COLOR c = {0, 0, 0, 255};
public:
    OneColorInterpolator(COLOR _color) : c(_color) {}

    COLOR color(float x, float y) override {
        return c;
    }
};


#endif //LAB_2_COLORED_SQUARE_ONECOLORINTERPOLATOR_H
