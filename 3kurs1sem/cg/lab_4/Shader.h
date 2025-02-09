//
// Created by dmitr on 13.11.2024.
//

#ifndef LAB_4_SHADER_H
#define LAB_4_SHADER_H

#include "Frame.h"

class Shader
{
public:
    Vector vertices[3];

    Shader(Vector D0, Vector D1, Vector D2) : vertices{ D0, D1, D2 } {};

    COLOR main(Vector data)
    {
        float x = data.x;
        float y = data.y;
        float z = data.z;

        float a = fmod(x + 1, 0.24f);
        float b = fmod(y + 1, 0.24f);

        if ((a > 0.12f) == (b > 0.12f))
            return COLOR(200, 200, 200);
        else
            return COLOR(20, 20, 0);
    }

    Vector getPixelData(double h0, double h1, double h2)
    {
        float x = h0 * vertices[0].x + h1 * vertices[1].x + h2 * vertices[2].x;
        float y = h0 * vertices[0].y + h1 * vertices[1].y + h2 * vertices[2].y;
        float z = h0 * vertices[0].z + h1 * vertices[1].z + h2 * vertices[2].z;
        float w = 1.f;

        return { x, y, z, w };
    }
};

//class ShaderSquare
//{
//public:
//    Vector vertices[3];
//
//    ShaderSquare(Vector D0, Vector D1, Vector D2) : vertices{ D0, D1, D2 } {};
//
//    COLOR main(Vector data)
//    {
//        float x = data.x;
//        float y = data.y;
//        float z = data.z;
//
//        float a = fmod(x + 1, 0.24f);
//        float b = fmod(y + 1, 0.24f);
//
//        if ((a > 0.12f) == (b > 0.12f))
//            return COLOR(200, 200, 200);
//        else
//            return COLOR(20, 20, 0);
//    }
//
//    Vector getPixelData(double h0, double h1, double h2)
//    {
//        float x = h0 * vertices[0].x + h1 * vertices[1].x + h2 * vertices[2].x;
//        float y = h0 * vertices[0].y + h1 * vertices[1].y + h2 * vertices[2].y;
//        float z = h0 * vertices[0].z + h1 * vertices[1].z + h2 * vertices[2].z;
//        float w = 1.f;
//
//        return { x, y, z, w };
//    }
//};

#endif //LAB_4_SHADER_H
