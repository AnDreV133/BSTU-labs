//
// Created by dmitr on 13.11.2024.
//

#ifndef LAB_4_SHADER_H
#define LAB_4_SHADER_H

#include "Frame.h"

//class Shader
//{
//public:
//    Vector vertices[3];
//
//    Shader(Vector D0, Vector D1, Vector D2) : vertices{ D0, D1, D2 } {};
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

// Cтруктура для задания цвета
typedef struct tagCOLOR {
    unsigned char RED;        // Компонента красного цвета
    unsigned char GREEN;    // Компонента зелёного цвета
    unsigned char BLUE;        // Компонента синего цвета
    unsigned char ALPHA;    // Прозрачность (альфа канал)

    tagCOLOR(int red, int green, int blue, int alpha = 255)
            : RED(red), GREEN(green), BLUE(blue), ALPHA(alpha) {}

} COLOR;

class BaseShader {
public:
    BaseShader() {}

    virtual COLOR main(Vector data) = 0;

    virtual Vector getPixelData(double h0, double h1, double h2) = 0;
};

class ColorShader : public BaseShader {
public:
    COLOR base;

    ColorShader(COLOR base) : base(base) {}

    COLOR main(Vector data) {
        return base;
    }

    Vector getPixelData(double h0, double h1, double h2) {
        return Vector();
    }
};

class DottedShader : public BaseShader {
    Vector A, B, C;
    int alpha;

public:
    DottedShader(Vector A, Vector B, Vector C, int alpha) : A(A), B(B), C(C), alpha(alpha) {}

    COLOR main(Vector data) {
        data.x += 1;
        data.y += 1;

        double x_factor = fmod(data.x, 0.2) * 5;
        double y_factor = fmod(data.y, 0.2) * 5;

        if (pow(x_factor - 0.5, 2) + pow(y_factor - 0.5, 2) < 0.25 * 0.25) {
            return {255, 255, 255, alpha};
        } else {
            return {255, 70, 0, alpha};
        }
    }

    Vector getPixelData(double h0, double h1, double h2) {
        Vector result = {
                static_cast<float>(A.x * h0 + B.x * h1 + C.x * h2),
                static_cast<float>(A.y * h0 + B.y * h1 + C.y * h2),
                static_cast<float>(A.z * h0 + B.z * h1 + C.z * h2)
        };

        return result;
    }
};

class ChessShader : public BaseShader {
public:
    Vector vertices[3];
    int alpha;

    ChessShader(Vector D0, Vector D1, Vector D2, int alpha) : vertices{D0, D1, D2}, alpha(alpha) {};

    COLOR main(Vector data) {
        float x = data.x;
        float y = data.y;

        float a = fmod(x + 1, 0.24f);
        float b = fmod(y + 1, 0.24f);

        if ((a > 0.12f) == (b > 0.12f))
            return COLOR(200, 200, 200, alpha);
        else
            return COLOR(20, 20, 0, alpha);
    }

    Vector getPixelData(double h0, double h1, double h2) {
        float x = h0 * vertices[0].x + h1 * vertices[1].x + h2 * vertices[2].x;
        float y = h0 * vertices[0].y + h1 * vertices[1].y + h2 * vertices[2].y;
        float z = h0 * vertices[0].z + h1 * vertices[1].z + h2 * vertices[2].z;
        float w = 1.f;

        return {x, y, z, w};
    }
};


#endif //LAB_4_SHADER_H
