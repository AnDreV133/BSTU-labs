//
// Created by dmitr on 04.09.2024.
//

#ifndef LAB_1_BASICS_ASSETS_H
#define LAB_1_BASICS_ASSETS_H

#include "Frame.h"

class Assets {
public:
    static void drawChessBg(Frame &frame) {
        // Шахматная текстура
        for (int y = 0; y < frame.height; y++)
            for (int x = 0; x < frame.width; x++) {
                if ((x + y) % 2 == 0)
                    frame.setPixel({x, y}, {230, 255, 230}); // Золотистый цвет
                    //frame.setPixel(x, y, { 217, 168, 14 });
                else
                    frame.setPixel({x, y}, {200, 200, 200}); // Чёрный цвет
                //frame.setPixel(x, y, { 255, 255, 255 }); // Белый цвет
            }
    }
};

#endif //LAB_1_BASICS_ASSETS_H
