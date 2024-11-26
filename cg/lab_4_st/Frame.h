#ifndef FRAME_H
#define FRAME_H

#include <math.h>
#include <Vector.h>

// Cтруктура для задания цвета
typedef struct tagCOLOR
{
	unsigned char RED;		// Компонента красного цвета
	unsigned char GREEN;	// Компонента зелёного цвета
	unsigned char BLUE;		// Компонента синего цвета
	unsigned char ALPHA;	// Прозрачность (альфа канал)

	tagCOLOR(int red, int green, int blue, int alpha = 255)
		: RED(red), GREEN(green), BLUE(blue), ALPHA(alpha) { }

} COLOR;


// Пиксель в буфере кадра
typedef struct tagPIXEL
{
	unsigned char RED;		// Компонента красного цвета
	unsigned char GREEN;	// Компонента зелёного цвета
	unsigned char BLUE;		// Компонента синего цвета
	float Z;				// Глубина пикселя
	tagPIXEL() : RED(0), GREEN(0), BLUE(0), Z(INFINITY) { }
} PIXEL;


template<typename TYPE> void swap(TYPE& a, TYPE& b)
{
	TYPE t = a;
	a = b;
	b = t;
}

class interpolator{
public:
//    COLOR get_color(){
//        if(color.GREEN != 0 || color.BLUE != 0 || color.RED != 0 || color.ALPHA != 0){
//            return color;
//        }
//
//        if(x % 20 < 10 && y % 20 < 10 || x % 20 >= 10 && y % 20 >= 10){
//            return {50, 50, 50, 255};
//        }
//        return {150, 150, 150, 255};
//    }
};

COLOR interpolator(int x, int y, COLOR color){
    if(color.GREEN != 0 || color.BLUE != 0 || color.RED != 0 || color.ALPHA != 0){
        return color;
    }

    if(x % 20 < 10 && y % 20 < 10 || x % 20 >= 10 && y % 20 >= 10){
        return {50, 50, 50, 255};
    }
    return {150, 150, 150, 255};
}

// Буфер кадра
class Frame 
{
	// Указатель на массив пикселей
	// Буфер кадра будет представлять собой матрицу, которая располагается в памяти в виде непрерывного блока
	PIXEL* pixels;

public:

	// Размеры буфера кадра
	int width, height;

	Frame(int _width, int _height) : width(_width), height(_height)
	{
		int size = width * height;

		// Создание буфера кадра в виде непрерывной матрицы пикселей
		pixels = new PIXEL[size];

	}

	// Задаёт цвет color пикселю с координатами (x, y)
	void SetPixel(int x, int y, COLOR color)
	{
		PIXEL* pixel = pixels + (size_t)y * width + x; // Находим нужный пиксель в матрице пикселей
		pixel->RED = color.RED;
		pixel->GREEN = color.GREEN;
		pixel->BLUE = color.BLUE;
	}

	// Возвращает цвет пикселя с координатами (x, y)
	COLOR GetPixel(int x, int y) 
	{
		PIXEL* pixel = pixels + (size_t)y * width + x; // Находим нужный пиксель в матрице пикселей
		return COLOR(pixel->RED, pixel->GREEN, pixel->BLUE);
	}

	
	template <class ShaderClass>void Triangle(Vector p0, Vector p1, Vector p2, ShaderClass &shader)
	{

        COLOR c = {0, 0, 0, 0};

        float x0 = p0.x;
        float y0 = p0.y;
        float z0 = p0.z;
        float x1 = p1.x;
        float y1 = p1.y;
        float z1 = p1.z;
        float x2 = p2.x;
        float y2 = p2.y;
        float z2 = p2.z;

        float w0 = p0.w;
        float w1 = p1.w;
        float w2 = p2.w;

        x0 /= w0;
        y0 /= w0;
        z0 /= w0;

        x1 /= w1;
        y1 /= w1;
        z1 /= w1;

        x2 /= w2;
        y2 /= w2;
        z2 /= w2;

		// Отсортируем точки таким образом, чтобы выполнилось условие: y0 < y1 < y2
		if (y1 < y0)
		{
			swap(x1, x0);
			swap(y1, y0);
			swap(z1, z0);
            swap(w1, w0);
            swap(shader.vertices[1], shader.vertices[0]);
		}
		if (y2 < y1)
		{
			swap(x2, x1);
			swap(y2, y1);
			swap(z2, z1);
            swap(w2, w1);
            swap(shader.vertices[2], shader.vertices[1]);
		}
		if (y1 < y0)
		{
			swap(x1, x0);
			swap(y1, y0);
			swap(z1, z0);
            swap(w1, w0);
            swap(shader.vertices[1], shader.vertices[0]);
		}

		// Определяем номера строк пикселей, в которых располагаются точки треугольника
		int Y0 = (int) (y0 + 0.5f);
		int Y1 = (int) (y1 + 0.5f);
		int Y2 = (int) (y2 + 0.5f);

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
			float y = Y + 0.5; // Координата y середины пикселя

			// Вычисление координат граничных пикселей
			int X0 = (int) ((y - y0) / (y1 - y0) * (x1 - x0) + x0 + 0.5f);
			int X1 = (int) ((y - y0) / (y2 - y0) * (x2 - x0) + x0 + 0.5f);

			if (X0 > X1) swap(X0, X1); // Сортировка пикселей
			if (X0 < 0) X0 = 0; // Отсечение невидимых пикселей в строке y
			if (X1 > width) X1 = width;

			for (int X = X0; X < X1; X++)
			{
				double x = X + 0.5; // Координата x середины пикселя

				// Середина пикселя имеет координаты (x, y)
				// Вычислим барицентрические координаты этого пикселя
				double h0 = ((y1 - y2) * (x - x2) + (x2 - x1) * (y - y2)) / S;
				double h1 = ((y2 - y0) * (x - x2) + (x0 - x2) * (y - y2)) / S;
				double h2 = ((y0 - y1) * (x - x1) + (x1 - x0) * (y - y1)) / S;

                double ih0 = h0 / w0;
                double ih1 = h1 / w1;
                double ih2 = h2 / w2;

                double NZ = 1.0 / (ih0 + ih1 + ih2);

                h0 = ih0 * NZ;
                h1 = ih1 * NZ;
                h2 = ih2 * NZ;

                double Z = h0 * z0 + h1 * z1 + h2 * z2; // Глубина пикселя

				// Определение глубины точки в экранной системе координат				
				PIXEL* pixel = pixels + (size_t)Y * width + X; // Вычислим адрес пикселя (Y, X) в матрице пикселей pixels
				// pixel->Z - глубина пикселя, которая уже записана в буфер кадра
				// Если текущий пиксель находится ближе того пикселя, который уже записан в буфере кадра
				if (Z > -1 && Z < 1 && Z < pixel->Z)
				{ // то обновляем пиксель в буфере кадра
					//float zmax = 0.8, zmin = 0.1;
					//color = COLOR(255 - (Z - zmin) / (zmax - zmin) * 255, 100, 100);
				    //color = COLOR((2.5 - Z) / 4 * 255, 0, 0);

                    c = shader.main(shader.getPixelData(h0, h1, h2));
					pixel->RED = c.RED;
					pixel->GREEN = c.GREEN;
					pixel->BLUE = c.BLUE;
					pixel->Z = Z;
				}
			}
		}

		// Рисование нижней части треугольника
		for (int Y = Y1; Y < Y2; Y++)
		{
			float y = Y + 0.5; // Координата y середины пикселя

			// Вычисление координат граничных пикселей
			int X0 = (int)((y - y1) / (y2 - y1) * (x2 - x1) + x1 + 0.5f);
			int X1 = (int)((y - y0) / (y2 - y0) * (x2 - x0) + x0 + 0.5f);

			if (X0 > X1) swap(X0, X1);  // Сортировка пикселей
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

                double ih0 = h0 / w0;
                double ih1 = h1 / w1;
                double ih2 = h2 / w2;

                double NZ = 1.0 / (ih0 + ih1 + ih2);

                h0 = ih0 * NZ;
                h1 = ih1 * NZ;
                h2 = ih2 * NZ;

				float Z = h0 * z0 + h1 * z1 + h2 * z2; // Глубина пикселя

				// Определение глубины точки в экранной системе координат
				// Если текущий пиксель находится ближе того пикселя, который уже записан в буфере кадра
				PIXEL* pixel = pixels + (size_t)Y * width + X;
				if (Z > -1 && Z < 1 && Z < pixel->Z)
				{
					//float zmax = 0.8, zmin = 0.1;
					//color = COLOR(255 - (Z - zmin) / (zmax - zmin) * 255, 100, 100);
					//color = COLOR((2.5 - Z) / 4 * 255, 0, 0);
                    c = shader.main(shader.getPixelData(h0, h1, h2));
					pixel->RED = c.RED;
					pixel->GREEN = c.GREEN;
					pixel->BLUE = c.BLUE;
					pixel->Z = Z;
				}
			}
		}
	}

//	void Quad(Vector p0,  Vector p1, Vector p2, Vector p3, COLOR color)
//	{
//		Triangle(p0, p1, p2, color);
//		Triangle(p2, p3, p0, color);
//	}

    void DrawLine(Vector p1, Vector p2, COLOR color) {
        float x1 = p1.x / p1.w;
        float y1 = p1.y / p1.w;
        float x2 = p2.x / p2.w;
        float y2 = p2.y / p2.w;

        int dy = y2 - y1, dx = x2 - x1;
        if (dx == 0 && dy == 0) {
            SetPixel(x1, y1, color);
            return;
        }

        if (abs(dx) > abs(dy)) {
            if (x2 < x1) {
                // Обмен местами точек (x1, y1) и (x2, y2)
                swap(x1, x2);
                swap(y1, y2);
                dx = -dx;
                dy = -dy;
            }

            int y, dx2 = dx / 2, p = 0;
            if (dy < 0) dx2 = -dx2;
            for (int x = x1; x <= x2; x++) {
                // y = (dy * (x - x1) + dx2) / dx + y1;
                y = (p + dx2) / dx + y1;
                p += dy;

                SetPixel(x, y, color);
            }
        } else {
            if (y2 < y1) {
                // Обмен местами точек (x1, y1) и (x2, y2)
                swap(x1, x2);
                swap(y1, y2);
                dx = -dx;
                dy = -dy;
            }

            int x, dy2 = dy / 2, p = 0;
            if (dx < 0) dy2 = -dy2;
            for (int y = y1; y <= y2; y++) {
                x = (p + dy2) / dy + x1;
                p += dx;
                SetPixel(x, y, color);
            }
        }
    }


	~Frame(void)
	{
		delete []pixels;
	}

};


#endif // FRAME_H
