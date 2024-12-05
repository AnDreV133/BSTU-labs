// Frame.h
#ifndef FRAME_H
#define FRAME_H

#include <math.h>

// Cтруктура для задания цвета
typedef struct tagCOLOR
{
    unsigned char RED;        // Компонента красного цвета
    unsigned char GREEN;      // Компонента зелёного цвета
    unsigned char BLUE;       // Компонента синего цвета
    unsigned char ALPHA;      // Прозрачность (альфа канал)

    tagCOLOR(int red, int green, int blue, int alpha = 255)
        : RED(red), GREEN(green), BLUE(blue), ALPHA(alpha) { }

} COLOR;


typedef struct tagPIXEL
{
    unsigned char RED;        // Компонента красного цвета
    unsigned char GREEN;      // Компонента зелёного цвета
    unsigned char BLUE;       // Компонента синего цвета
    float Z;                   // Глубина пикселя

    tagPIXEL() : RED(0), GREEN(0), BLUE(0), Z(INFINITY) { }
} PIXEL;


template<typename TYPE> void customSwap(TYPE& a, TYPE& b)
{
    TYPE t = a;
    a = b;
    b = t;
}


// Буфер кадра
class Frame
{
    // Указатель на массив пикселей
    // Буфер кадра будет представлять собой матрицу, которая располагается в памяти в виде непрерывного блока
    PIXEL* pixels;
private:
    const int INSIDE = 0; // 0000
    const int LEFT = 1;   // 0001
    const int RIGHT = 2;  // 0010
    const int BOTTOM = 4; // 0100
    const int TOP = 8;    // 1000

    // Функция для вычисления outcode точки (используется в алгоритме отсечения)
    int ComputeOutCode(int x, int y) {
        int code = INSIDE;

        if (x < 0)
            code |= LEFT;
        else if (x > width - 1)
            code |= RIGHT;
        if (y < 0)
            code |= BOTTOM;
        else if (y > height - 1)
            code |= TOP;

        return code;
    }
public:
    // Размеры буфера кадра
    int width, height;

    Frame(int _width, int _height) : width(_width), height(_height)
    {
        int size = width * height;

        // Создание буфера кадра в виде непрерывной матрицы пикселей
        pixels = new PIXEL[size];

        // Заполнение всего буфера светло-серым цветом
        for (int i = 0; i < size; i++)
        {
            pixels[i].RED = 211;
            pixels[i].GREEN = 211;
            pixels[i].BLUE = 211;
            // pixels[i].Z остается равным INFINITY
        }
    }

    // Задаёт цвет color пикселю с координатами (x, y) с учетом прозрачности
    void SetPixel(int x, int y, COLOR color) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;

        PIXEL* pixel = pixels + (size_t)y * width + x;
        float alpha = color.ALPHA / 255.0f;

        // Альфа-блендинг: новый цвет поверх существующего
        pixel->RED = static_cast<unsigned char>(color.RED * alpha + pixel->RED * (1.0f - alpha));
        pixel->GREEN = static_cast<unsigned char>(color.GREEN * alpha + pixel->GREEN * (1.0f - alpha));
        pixel->BLUE = static_cast<unsigned char>(color.BLUE * alpha + pixel->BLUE * (1.0f - alpha));

        // Обновление Z-буфера (закомментировано, так как Z-буфер не используется)
        // pixel->Z = Z;
    }

    // Возвращает цвет пикселя с координатами (x, y)
    COLOR GetPixel(int x, int y)
    {
        PIXEL* pixel = pixels + (size_t)y * width + x; // Находим нужный пиксель в матрице пикселей pixels
        return COLOR(pixel->RED, pixel->GREEN, pixel->BLUE, 255); // Возвращаем цвет с полной непрозрачностью
    }

    void Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, COLOR color)
    {
        // Отсортируем точки таким образом, чтобы выполнилось условие: y0 < y1 < y2
        if (y1 < y0)
        {
            customSwap(x1, x0);
            customSwap(y1, y0);
            customSwap(z1, z0);
        }
        if (y2 < y1)
        {
            customSwap(x2, x1);
            customSwap(y2, y1);
            customSwap(z2, z1);
        }
        if (y1 < y0)
        {
            customSwap(x1, x0);
            customSwap(y1, y0);
            customSwap(z1, z0);
        }

        // Определяем номера строк пикселей, в которых располагаются точки треугольника
        int Y0 = static_cast<int>(y0 + 0.5f);
        int Y1 = static_cast<int>(y1 + 0.5f);
        int Y2 = static_cast<int>(y2 + 0.5f);

        // Отсечение невидимой части треугольника
        if (Y0 < 0) Y0 = 0;
        else if (Y0 > height) Y0 = height;

        if (Y1 < 0) Y1 = 0;
        else if (Y1 > height) Y1 = height;

        if (Y2 < 0) Y2 = 0;
        else if (Y2 > height) Y2 = height;

        float S = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2); // Площадь треугольника

        // Рисование верхней части треугольника 
        for (int Y = Y0; Y < Y1; Y++)
        {
            float y = Y + 0.5f; // Координата y середины пикселя

            // Вычисление координат граничных пикселей
            int X0 = static_cast<int>((y - y0) / (y1 - y0) * (x1 - x0) + x0 + 0.5f);
            int X1 = static_cast<int>((y - y0) / (y2 - y0) * (x2 - x0) + x0 + 0.5f);

            if (X0 > X1) customSwap(X0, X1); // Сортировка пикселей
            if (X0 < 0) X0 = 0; // Отсечение невидимых пикселей в строке y
            if (X1 > width) X1 = width;

            for (int X = X0; X < X1; X++)
            {
                double x = X + 0.5; // Координата x середины пикселя

                // Середина пикселя имеет координаты (x, y)
                // Вычислим барицентрические координаты этого пикселя
                float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
                float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
                float h2 = ((y0 - y1) * (x - x1) + (x1 - x0) * (y - y1)) / S;

                float Z = h0 * z0 + h1 * z1 + h2 * z2; // Глубина пикселя

                // Определение глубины точки в экранной системе координат
                PIXEL* pixel = pixels + (size_t)Y * width + X; // Вычислим адрес пикселя (Y, X) в матрице пикселей pixels

                // Условие обновления пикселя без учета Z-буфера
                if (Z > -1 && Z < 1 /* && Z < pixel->Z */) // Условие с Z-буфером закомментировано
                {
                    // Обновляем цвет пикселя с учетом прозрачности
                    SetPixel(X, Y, color);

                    // Обновление Z-буфера закомментировано
                    // pixel->Z = Z;
                }
            }
        }

        // Рисование нижней части треугольника
        for (int Y = Y1; Y < Y2; Y++)
        {
            float y = Y + 0.5f; // Координата y середины пикселя

            // Вычисление координат граничных пикселей
            int X0 = static_cast<int>((y - y1) / (y2 - y1) * (x2 - x1) + x1 + 0.5f);
            int X1 = static_cast<int>((y - y0) / (y2 - y0) * (x2 - x0) + x0 + 0.5f);

            if (X0 > X1) customSwap(X0, X1);  // Сортировка пикселей
            if (X0 < 0) X0 = 0; // Отсечение невидимых пикселей в строке y
            if (X1 > width) X1 = width;

            for (int X = X0; X < X1; X++)
            {
                double x = X + 0.5; // Координата x середины пикселя

                // Середина пикселя имеет координаты (x, y)
                // Вычислим барицентрические координаты этого пикселя
                float h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
                float h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
                float h2 = ((y0 - y1) * (x - x1) + (x1 - x0) * (y - y1)) / S;

                float Z = h0 * z0 + h1 * z1 + h2 * z2; // Глубина пикселя

                // Определение глубины точки в экранной системе координат
                PIXEL* pixel = pixels + (size_t)Y * width + X;

                // Условие обновления пикселя без учета Z-буфера
                if (Z > -1 && Z < 1 /* && Z < pixel->Z */)
                {
                    // Обновляем цвет пикселя с учетом прозрачности
                    SetPixel(X, Y, color);

                    // Обновление Z-буфера закомментировано
                    // pixel->Z = Z;
                }
            }
        }
    }

    // Рисование квадрата, разделенного на два треугольника
    void Quad(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, COLOR color)
    {
        Triangle(x0, y0, z0, x1, y1, z1, x2, y2, z2, color);
        Triangle(x2, y2, z2, x3, y3, z3, x0, y0, z0, color);
    }

    // Рисование линии с использованием алгоритма Сазерленда-Коэна
    void DrawLine(int x1, int y1, int x2, int y2, COLOR color)
    {
        // Вычисляем начальный код выхода (outcode) для первой точки
        int outcode0 = ComputeOutCode(x1, y1);
        // Вычисляем начальный код выхода (outcode) для второй точки
        int outcode1 = ComputeOutCode(x2, y2);

        // Переменная для проверки, нужно ли принять линию после отсечения
        bool accept = false;

        // Запускаем цикл для отсечения линии
        while (true) {
            // Если обе точки находятся внутри видимого прямоугольника
            if (!(outcode0 | outcode1)) {
                accept = true;
                break;
            }
            // Если обе точки вне видимого прямоугольника в одной и той же области
            else if (outcode0 & outcode1) {
                break;
            }
            // Если одна точка вне прямоугольника, а другая внутри
            else {
                int x, y; // Переменные для координат точки пересечения
                // Определяем код выхода для точки, находящейся вне прямоугольника
                int outcodeOut = outcode0 ? outcode0 : outcode1;

                // Проверяем пересечение с верхней границей
                if (outcodeOut & TOP) {
                    // Вычисляем координаты пересечения с верхней границей
                    x = x1 + (x2 - x1) * (height - 1 - y1) / (y2 - y1);
                    y = height - 1;
                }
                // Проверяем пересечение с нижней границей
                else if (outcodeOut & BOTTOM) {
                    // Вычисляем координаты пересечения с нижней границей
                    x = x1 + (x2 - x1) * (0 - y1) / (y2 - y1);
                    y = 0;
                }
                // Проверяем пересечение с правой границей
                else if (outcodeOut & RIGHT) {
                    // Вычисляем координаты пересечения с правой границей
                    y = y1 + (y2 - y1) * (width - 1 - x1) / (x2 - x1);
                    x = width - 1;
                }
                // Проверяем пересечение с левой границей
                else if (outcodeOut & LEFT) {
                    // Вычисляем координаты пересечения с левой границей
                    y = y1 + (y2 - y1) * (0 - x1) / (x2 - x1);
                    x = 0;
                }

                // Обновляем координаты точки, которая была вне прямоугольника
                if (outcodeOut == outcode0) {
                    x1 = x;
                    y1 = y;
                    outcode0 = ComputeOutCode(x1, y1); // Пересчитываем код выхода для новой точки
                }
                else {
                    x2 = x;
                    y2 = y;
                    outcode1 = ComputeOutCode(x2, y2); // Пересчитываем код выхода для новой точки
                }
            }
        }

        // Если линия была принята для рисования
        if (accept) {
            // Вычисляем приращения по x и y
            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            // Определяем направление движения по x
            int sx = (x1 < x2) ? 1 : -1;
            // Определяем направление движения по y
            int sy = (y1 < y2) ? 1 : -1;
            // Устанавливаем начальную ошибку
            int err = dx - dy;

            // Запускаем цикл для рисования линии по алгоритму Брезенхэма
            while (true) {
                // Рисуем текущий пиксель с учетом прозрачности
                SetPixel(x1, y1, color);
                // Проверяем, достигли ли конечной точки
                if (x1 == x2 && y1 == y2) break;

                int e2 = 2 * err; // Удваиваем ошибку для проверки условий

                // Если ошибка позволяет переместиться по x
                if (e2 > -dy) {
                    err -= dy; // Уменьшаем ошибку на dy
                    x1 += sx;  // Перемещаемся по x
                }

                // Если ошибка позволяет переместиться по y
                if (e2 < dx) {
                    err += dx; // Увеличиваем ошибку на dx
                    y1 += sy;  // Перемещаемся по y
                }
            }
        }
    }

    ~Frame(void)
    {
        delete[] pixels;
    }
};

#endif // FRAME_H
