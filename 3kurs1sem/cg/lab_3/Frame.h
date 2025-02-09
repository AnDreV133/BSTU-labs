#ifndef FRAME_H
#define FRAME_H

#include <math.h>
#include <cmath>

// C�������� ��� ������� �����
typedef struct tagCOLOR {
    unsigned char RED;        // ���������� �������� �����
    unsigned char GREEN;    // ���������� ������� �����
    unsigned char BLUE;        // ���������� ������ �����
    unsigned char ALPHA;    // ������������ (����� �����)

    tagCOLOR() : RED(0), GREEN(0), BLUE(0), ALPHA(255) {}

    tagCOLOR(int red, int green, int blue, int alpha = 255)
            : RED(red), GREEN(green), BLUE(blue), ALPHA(alpha) {
        if (red < 0) RED = 0;
        else if (red > 255) RED = 255;
        if (green < 0) GREEN = 0;
        else if (green > 255) GREEN = 255;
        if (blue < 0) BLUE = 0;
        else if (blue > 255) BLUE = 255;
        if (alpha < 0) ALPHA = 0;
        else if (alpha > 255) ALPHA = 255;
    }

} COLOR;


template<typename TYPE>
void swap(TYPE &a, TYPE &b) {
    TYPE t = a;
    a = b;
    b = t;
}


// ����� �����
class Frame {
    COLOR getColorByAlpha(COLOR color, int x, int y) {
        // ��� ��������� �������������� ����� ����� ������������ �����-����������
        if (color.ALPHA < 255) {
            COLOR written = matrix[y][x]; // ��� ���������� � ������ ����� �������� �����, �.�. ���� ����
            float a = color.ALPHA / 255.0f, b = 1 - a;
            color.RED = color.RED * a + written.RED * b;
            color.GREEN = color.GREEN * a + written.GREEN * b;
            color.BLUE = color.BLUE * a + written.BLUE * b;
        }

        return color;
    }

    // ��������� �� ������ ��������
    // ����� ����� ����� ������������ ����� �������, ������� ������������� � ������ � ���� ������������ �����
    COLOR *pixels;

    // ��������� �� ������ �������� ������ �����
    COLOR **matrix;

public:

    // ������� ������ �����
    int width, height;

    Frame(int _width, int _height) : width(_width), height(_height) {
        int size = width * height;

        // �������� ������ ����� � ���� ����������� ������� ��������
        pixels = new COLOR[size];

        // ��������� �� ������ �������� ������� � ��������� ������
        matrix = new COLOR *[height];

        // ������������� ������� ����������
        for (int i = 0; i < height; i++) {
            matrix[i] = pixels + (size_t) i * width;
        }
    }

    // ����� ���� color ������� � ������������ (x, y)
    void SetPixel(int x, int y, COLOR color) {
        matrix[y][x] = color;
    }

    // ���������� ���� ������� � ������������ (x, y)
    COLOR GetPixel(int x, int y) {
        return matrix[y][x];
    }


    void Triangle(float x0, float y0, float x1, float y1, float x2, float y2, COLOR color) {
        // ����������� ����� ����� �������, ����� ����������� �������: y0 < y1 < y2
        if (y1 < y0) {
            swap(y1, y0);
            swap(x1, x0);
        }
        if (y2 < y1) {
            swap(y2, y1);
            swap(x2, x1);
        }
        if (y1 < y0) {
            swap(y1, y0);
            swap(x1, x0);
        }

        // ���������� ������ ����� ��������, � ������� ������������� ����� ������������
        int Y0 = (int) (y0 + 0.5f);
        int Y1 = (int) (y1 + 0.5f);
        int Y2 = (int) (y2 + 0.5f);

        // ��������� ��������� ����� ������������
        if (Y0 < 0) Y0 = 0;
        else if (Y0 >= height) Y0 = height;

        if (Y1 < 0) Y1 = 0;
        else if (Y1 >= height) Y1 = height;

        if (Y2 < 0) Y2 = 0;
        else if (Y2 >= height) Y2 = height;

        // ��������� ������� ����� ������������
        for (float y = Y0 + 0.5f; y < Y1; y++) {
            int X0 = (int) ((y - y0) / (y1 - y0) * (x1 - x0) + x0);
            int X1 = (int) ((y - y0) / (y2 - y0) * (x2 - x0) + x0);

            if (X0 > X1) swap(X0, X1);
            if (X0 < 0) X0 = 0;
            if (X1 > width) X1 = width;

            for (int x = X0; x < X1; x++) {
                // f(x + 0.5, y)
                SetPixel(x, y, getColorByAlpha(color, x, y));
            }
        }

        // ��������� ������ ����� ������������
        for (float y = Y1 + 0.5f; y < Y2; y++) {
            int X0 = (int) ((y - y1) / (y2 - y1) * (x2 - x1) + x1);
            int X1 = (int) ((y - y0) / (y2 - y0) * (x2 - x0) + x0);

            if (X0 > X1) swap(X0, X1);
            if (X0 < 0) X0 = 0;
            if (X1 > width) X1 = width;

            for (int x = X0; x < X1; x++) {
                // f(x + 0.5, y)
                SetPixel(x, y, getColorByAlpha(color, x, y));
            }
        }

    }

    template<class InterpolatorClass>
    void Triangle(float x0, float y0, float x1, float y1, float x2, float y2, InterpolatorClass &Interpolator) {
        // ����������� ����� ����� �������, ����� ����������� �������: y0 < y1 < y2
        if (y1 < y0) {
            swap(y1, y0);
            swap(x1, x0);
        }
        if (y2 < y1) {
            swap(y2, y1);
            swap(x2, x1);
        }
        if (y1 < y0) {
            swap(y1, y0);
            swap(x1, x0);
        }

        // ���������� ������ ����� ��������, � ������� ������������� ����� ������������
        int Y0 = (int) (y0 + 0.5f);
        int Y1 = (int) (y1 + 0.5f);
        int Y2 = (int) (y2 + 0.5f);

        // ��������� ��������� ����� ������������
        if (Y0 < 0) Y0 = 0;
        else if (Y0 >= height) Y0 = height;

        if (Y1 < 0) Y1 = 0;
        else if (Y1 >= height) Y1 = height;

        if (Y2 < 0) Y2 = 0;
        else if (Y2 >= height) Y2 = height;

        double rawX0 = (Y0 + 0.5f - y0) / (y1 - y0) * (x1 - x0) + x0;
        double rawX1 = (Y0 + 0.5f - y0) / (y2 - y0) * (x2 - x0) + x0;
        int X0, X1;

        // ��������� ������� ����� ������������
        for (float y = Y0 + 0.5f; y < Y1; y++) {
            X0 = rawX0;
            X1 = rawX1;

            if (X0 > X1) swap(X0, X1);
            if (X0 < 0) X0 = 0;
            if (X1 > width) X1 = width;

            for (int x = X0; x < X1; x++) {
                // f(x + 0.5, y)
                COLOR color = Interpolator.color(x + 0.5f, y);

                SetPixel(x, y, getColorByAlpha(color, x, y));
            }

            rawX0 += (x1 - x0) / (y1 - y0);
            rawX1 += (x2 - x0) / (y2 - y0);
        }

        rawX0 = (Y1 + 0.5f - y1) / (y2 - y1) * (x2 - x1) + x1;
        rawX1 = (Y1 + 0.5f - y0) / (y2 - y0) * (x2 - x0) + x0;

        // ��������� ������ ����� ������������
        for (float y = Y1 + 0.5f; y < Y2; y++) {
            X0 = rawX0;
            X1 = rawX1;

            if (X0 > X1) swap(X0, X1);
            if (X0 < 0) X0 = 0;
            if (X1 > width) X1 = width;

            for (int x = X0; x < X1; x++) {
                // f(x + 0.5, y)
                COLOR color = Interpolator.color(x + 0.5f, y);

                SetPixel(x, y, getColorByAlpha(color, x, y));
            }

            rawX0 += (x2 - x1) / (y2 - y1);
            rawX1 += (x2 - x0) / (y2 - y0);
        }
    }

    void Circle(int x0, int y0, int r, COLOR color) {
        int x = 0, y = r;
        int D = x * x + y * y - r * r +
                x * x + (y - 1) * (y - 1) - r * r;
        int xp = -1, yp = -1;
        while (x < y) {
            if (D > 0) {
                y--;
                D -= 4 * y - 2;
            }

            if (x != xp) {
                xp = x;

                if (x != 0) {
                    for (int i = x0 - y; i < x0 + y; i++) {
                        SetPixel(i, y0 + x, getColorByAlpha(color, i, y0 + x));
                        SetPixel(i, y0 - x, getColorByAlpha(color, i, y0 - x));
                    }
                } else {
                    for (int i = x0 - y; i < x0 + y; i++) {
                        SetPixel(i, y0 + x, getColorByAlpha(color, i, y0 + x));
                    }
                }
            }
            if (y != yp) {
                yp = y;

                for (int i = x0 - x; i < x0 + x; i++) {
                    SetPixel(i, y0 + y, getColorByAlpha(color, i, y0 + y));
                    SetPixel(i, y0 - y, getColorByAlpha(color, i, y0 - y));
                }
            }

            D += 4 * x;
            x++;
        }
    }

    template<class InterpolatorClass>
    void Circle(int x0, int y0, int r, InterpolatorClass &interpolator) {
        int x = 0, y = r;
        int D = 2 * x * x + 2 * y * y - 2 * r * r - 2 * y + 1;

        int xp = -1, yp = -1;
        while (x < y) {
            // ���� ����� ����� (x, y - 1), �� ��������� � ���
            if (D > 0) {
                D -= 4 * y - 4;
                y--;
            }

            if (x != xp) {
                xp = x;

                if (x != 0) {
                    for (int i = x0 - y; i <= x0 + y; i++) {
                        SetPixel(i, y0 + x, getColorByAlpha(interpolator.color(i, y0 + x), i, y0 + x));
                        SetPixel(i, y0 - x, getColorByAlpha(interpolator.color(i, y0 - x), i, y0 - x));
                    }
                } else {
                    for (int i = x0 - y; i <= x0 + y; i++) {
                        SetPixel(i, y0 + x, getColorByAlpha(interpolator.color(i, y0 + x), i, y0 + x));
                    }
                }
            }
            if (y != yp) {
                yp = y;

                for (int i = x0 - x; i <= x0 + x; i++) {
                    SetPixel(i, y0 + y, getColorByAlpha(interpolator.color(i, y0 + y), i, y0 + y));
                    SetPixel(i, y0 - y, getColorByAlpha(interpolator.color(i, y0 - y), i, y0 - y));
                }
            }

            x++;
            D += 4 * x;
        }
    }

    bool IsPointInCircle(
            int x0, int y0,
            int radius,
            int point_x, int point_y
    ) {
        return (x0 - point_x) * (x0 - point_x) + (y0 - point_y) * (y0 - point_y) < radius * radius;
    }

    bool IsPointInTriangle(
            float x0, float y0,
            float x1, float y1,
            float x2, float y2,
            float point_x, float point_y
    ) {
        float S = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
        float h0 = ((y1 - y2) * (point_x - x2) + (x2 - x1) * (point_y - y2)) / S;
        float h1 = ((y2 - y0) * (point_x - x2) + (x0 - x2) * (point_y - y2)) / S;
        float h2 = 1 - h0 - h1;

        return h0 >= 0 && h1 >= 0 && h2 >= 0;
    }



    // ��������� ����� ��� ��������� ������������, ������� ����� ��������� ��������
    class TriangleShader_ {
        float x0, y0, x1, y1, x2, y2; // ���������� ��� ������ �������� � �������� ������� ���������
        float wx0, wy0, wx1, wy1, wx2, wy2; // ���������� ��� ������ �������� � ������� ������� ���������
        float S; // ������� ������������
        COLOR C0, C1; // ���� ������ � ���������� �����
    public:
        TriangleShader_(
                float _x0, float _y0, float _x1, float _y1, float _x2, float _y2,
                float _wx0, float _wy0, float _wx1, float _wy1,
                float _wx2, float _wy2, COLOR _C0, COLOR _C1) :
                x0(_x0), y0(_y0), x1(_x1), y1(_y1), x2(_x2), y2(_y2),
                wx0(_wx0), wy0(_wy0), wx1(_wx1), wy1(_wy1), wx2(_wx2), wy2(_wy2),
                C0(_C0), C1(_C1),
                S((_y1 - _y2) * (_x0 - _x2) + (_x2 - _x1) * (_y0 - _y2)) {
        }

// �������� ������� main ������������ ���� ����� � ������������ (x, y) ��� ��������, ������� ���������� � ������ ���������, �
// ���������� ��� ����
// ����� �������� ����� ���������� (0, 0). ������� �������� ����� 2
        COLOR main(float x, float y) {
            if (x < 0) x = -x; // �������� ����� � ������ ��������, ����� ���� ����� �������
            if (y < 0) y = -y;
            float r = 0.5; // ������ ����������
// ���������� ������ ���������� � ������ ��������
            float x0 = 1 - r, y0 = 1 - r;
// ���� ����� ������� �� ������� �������, ���������� ��������� ���������� �������
            if (x + y > 2 - r && (x - x0) * (x - x0) + (y - y0) * (y - y0) > r * r) {
                return COLOR(0, 0, 0, 0);
            }
            float d;
// ������� ���, ����� ���� ������� �� ���������� �� ������� ������
// ���� ����� ��������� ������ ���� ��������, �� ������������
            if (x > 1 - r && y > 1 - r) {
                d = (r - sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0))) / r;
            } else // � ��������� ������ ������� ��������� ������� ��������
            {
//                if (x > 1 - r || y > 1 - r)
//                    d = min(1 - y, 1 - x) / r;
//                else d = 1;
            }
            return COLOR(C0.RED * d + C1.RED * (1 - d),
                         C0.GREEN * d + C1.GREEN * (1 - d),
                         C0.BLUE * d + C1.BLUE * (1 - d), 255 - (1
                                                                 - d) * 100);
        }

        COLOR color(float x, float y) {
// ���������������� ������������
            float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
            float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
            float h2 = ((y0 - y1) * (x - x1) + (x1 - x0) * (y - y1)) / S;
//float h2 = 1 - h0 - h1;
// ���� ����� (x, y) ��������� ��� ������������
            if (h0 < -1E-6 || h1 < -1E-6 || h2 < -1E-6) {
                return COLOR(0, 0, 0); // ������ ��������� ������������, ���� �������� ������ �������
            }
// ������������� ������� ���������� ������
            float wx = h0 * wx0 + h1 * wx1 + h2 * wx2;
            float wy = h0 * wy0 + h1 * wy1 + h2 * wy2;
            return main(wx, wy);
        }
    };

    ~Frame() {
        delete[]pixels;
        delete[]matrix;
    }

};

#endif // FRAME_H
