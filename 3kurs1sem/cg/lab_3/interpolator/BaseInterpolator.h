//
// Created by dmitr on 21.09.2024.
//

#ifndef LAB_2_COLORED_SQUARE_BASEINTERPOLATOR_H
#define LAB_2_COLORED_SQUARE_BASEINTERPOLATOR_H


#include "Frame.h"

class BaseInterpolator {
public:
    virtual COLOR color(float x, float y) = 0;
};


#endif //LAB_2_COLORED_SQUARE_BASEINTERPOLATOR_H
