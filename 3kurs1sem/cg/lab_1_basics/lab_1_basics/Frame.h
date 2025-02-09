#ifndef FRAME_H
#define FRAME_H

#include <math.h>

// Угол поворота фигуры
float globalAngle = 0;

// Координаты последнего пикселя, который выбрал пользователь
struct {
    int x, y;
} globalClickedPixel = {-1, -1};

struct PixelCoord {
    int x, y;
};

// Cтруктура для задания цвета
typedef struct tagCOLOR {
    unsigned char RED;        // Компонента красного цвета
    unsigned char GREEN;    // Компонента зелёного цвета
    unsigned char BLUE;        // Компонента синего цвета
    unsigned char ALPHA;    // Прозрачность (альфа канал)

    tagCOLOR() : RED(0), GREEN(0), BLUE(0), ALPHA(255) {}

    tagCOLOR(
            unsigned char red,
            unsigned char green,
            unsigned char blue,
            unsigned char alpha = 255
    ) : RED(red), GREEN(green), BLUE(blue), ALPHA(alpha) {}

} Color;

template<typename T>
void swap(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}

// Буфер кадра
class Frame {
    // Указатель на массив пикселей
    // Буфер кадра будет представлять собой матрицу, которая располагается в памяти в виде непрерывного блока
    Color *pixels;

    // Указатели на строки пикселей буфера кадра
    Color **matrix;

public:

    // Размеры буфера кадра
    int width, height;

    Frame(int _width, int _height) : width(_width), height(_height) {
        int size = width * height;

        // Создание буфера кадра в виде непрерывной матрицы пикселей
        pixels = new Color[size];

        // Указатели на строки пикселей запишем в отдельный массив
        matrix = new Color *[height];

        // Инициализация массива указателей
        for (int i = 0; i < height; i++) {
            matrix[i] = pixels + i * width;
        }
    }


    // Задаёт цвет color пикселю с координатами (x, y)
    void setPixel(PixelCoord p, Color color) {
        matrix[p.y][p.x] = color;
    }

    // Возвращает цвет пикселя с координатами (x, y)
    Color getPixel(PixelCoord p) {
        return matrix[p.y][p.x];
    }


    // Рисование окружности
    void drawCircle(PixelCoord p0, int radius, Color color) {
        int x = 0, y = radius;

        int D = y * y - (y - 1) * (y - 1);
        while (x < y) {
            // Если ближе точка (x, y - 1), то смещаемся к ней
            if (D > 0) {
                y--;
                D -= 4 * y;
            }
            // Перенос и отражение вычисленных координат на все октанты окружности

            setPixel({p0.x + x, p0.y + y}, color);
            setPixel({p0.x + x, p0.y - y}, color);
            setPixel({p0.x + y, p0.y + x}, color);
            setPixel({p0.x + y, p0.y - x}, color);
            setPixel({p0.x - x, p0.y + y}, color);
            setPixel({p0.x - x, p0.y - y}, color);
            setPixel({p0.x - y, p0.y + x}, color);
            setPixel({p0.x - y, p0.y - x}, color);
            x++;
            D += 4 * x - 2;
        }
    }


    // Рисование отрезка
    void drawLine(PixelCoord p1, PixelCoord p2, Color color) {
        int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;

        int dy = y2 - y1, dx = x2 - x1;
        if (dx == 0 && dy == 0) {
            matrix[y1][x1] = color;
            return;
        }

        int D = dy * x1 - dy * x2 - dx * (y1 - y2);

        if (abs(dx) > abs(dy)) {
            if (x2 < x1) {
                // Обмен местами точек (x1, y1) и (x2, y2)
                swap(x1, x2);
                swap(y1, y2);
                dx = -dx;
                dy = -dy;
            }

            int D = dy * (x1 - x2) - dx * (y1 - y2);

            int y, p = 0;
            for (int x = x1; x <= x2; x++) {
                y = (p + D) / dx + y1;
                p += dy;
                matrix[y][x] = color;
            }
        } else {
            if (y2 < y1) {
                // Обмен местами точек (x1, y1) и (x2, y2)
                swap(x1, x2);
                swap(y1, y2);
                dx = -dx;
                dy = -dy;
            }

            int x, p = 0;
            for (int y = y1; y <= y2; y++) {
                x = (p + D) / dy + x1;
                p += dx;
                matrix[y][x] = color;
            }
        }
    }

    float get_angle(PixelCoord p) {
        int x1 = 0, y1 = 1, x2 = p.x, y = p.y;

        return atan2(x1 * p.y - y1 * x2, x1 * x2 + y1 * p.y);
    }

    void drawSector(
            PixelCoord p0,
            float angleStart,
            float angleEnd,
            int radius,
            Color color
    ) {
        int x = 0, y = radius;

        int D = y * y - (y - 1) * (y - 1);
        while (x < y) {
            // Если ближе точка (x, y - 1), то смещаемся к ней
            if (D > 0) {
                y--;
                D -= 4 * y;
            }

            // Перенос и отражение вычисленных координат на все октанты окружности
            float a = get_angle({x, y});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x + x, p0.y + y}, color);
            a = get_angle({x, -y});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x + x, p0.y - y}, color);
            a = get_angle({y, x});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x + y, p0.y + x}, color);
            a = get_angle({y, -x});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x + y, p0.y - x}, color);
            a = get_angle({-x, y});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x - x, p0.y + y}, color);
            a = get_angle({-x, -y});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x - x, p0.y - y}, color);
            a = get_angle({-y, x});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x - y, p0.y + x}, color);
            a = get_angle({-y, -x});
            if (a >= angleStart && a <= angleEnd) setPixel({p0.x - y, p0.y - x}, color);

            x++;
            D += 4 * x - 2;
        }

    }


    ~Frame() {
        delete[]pixels;
        delete[]matrix;
    }

};


#endif // FRAME_H
