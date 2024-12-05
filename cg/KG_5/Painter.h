// Painter.h
#ifndef PAINTER_H
#define PAINTER_H

#include <vector>
#include <algorithm>
#include <cmath>
#include "Frame.h"
#include "Vector.h"
#include "Matrix.h"

// Время от начала запуска программы
float time = 0;

// Тип проекции
enum ProjectionType {
    CENTRAL,        // Центральная проекция
    ORTHOGRAPHIC    // Ортографическая проекция
};
ProjectionType currentProjection = CENTRAL;

// Структура для хранения полигона
struct Polygon {
    std::vector<Vector> vertices; // Вершины многоугольника
    COLOR color;                  // Цвет многоугольника
};

// Глобальная прозрачность для элементов сцены
unsigned char globalAlpha = 255; // Глобальная прозрачность

// Прозрачность для шейдера
unsigned char shaderAlpha = 128; // Прозрачность шейдера (полупрозрачный)

// Класс для растеризации квадрата с шахматным узором
class QuadShader {
    // Координаты вершин в экранной системе координат
    Vector v0, v1, v2, v3;
    // Координаты вершин в мировой системе координат
    Vector wv0, wv1, wv2, wv3;

    // Преобразованные векторы ребер для вычисления функций ребер
    float edge0_dx, edge0_dy;
    float edge1_dx, edge1_dy;
    float edge2_dx, edge2_dy;
    float edge3_dx, edge3_dy;

    // Минимальные и максимальные координаты для области растеризации
    int minX, maxX, minY, maxY;

public:
    QuadShader(const std::vector<Vector>& screenVerts, const std::vector<Vector>& worldVerts) {
        // Конструктор, принимающий вершины квадрата в экранной и мировой системе координат
        v0 = screenVerts[0];
        v1 = screenVerts[1];
        v2 = screenVerts[2];
        v3 = screenVerts[3];

        wv0 = worldVerts[0];
        wv1 = worldVerts[1];
        wv2 = worldVerts[2];
        wv3 = worldVerts[3];

        // Вычисляем векторы ребер квадрата
        edge0_dx = v1.x - v0.x;
        edge0_dy = v1.y - v0.y;
        edge1_dx = v2.x - v1.x;
        edge1_dy = v2.y - v1.y;
        edge2_dx = v3.x - v2.x;
        edge2_dy = v3.y - v2.y;
        edge3_dx = v0.x - v3.x;
        edge3_dy = v0.y - v3.y;

        // Определяем границы области растеризации квадрата
        minX = (int)std::floor(std::fmin(std::fmin(v0.x, v1.x), std::fmin(v2.x, v3.x)));
        maxX = (int)std::ceil(std::fmax(std::fmax(v0.x, v1.x), std::fmax(v2.x, v3.x)));
        minY = (int)std::floor(std::fmin(std::fmin(v0.y, v1.y), std::fmin(v2.y, v3.y)));
        maxY = (int)std::ceil(std::fmax(std::fmax(v0.y, v1.y), std::fmax(v2.y, v3.y)));
    }

    // Функция для создания шахматного узора в координатах текстуры
    COLOR main(float u, float v) {
        int checkSize = 10;

        int iu = (int)(u * checkSize);
        int iv = (int)(v * checkSize);

        if ((iu + iv) % 2 == 0)
            return COLOR(200, 200, 200, shaderAlpha); // Используем shaderAlpha
        else
            return COLOR(20, 20, 0, shaderAlpha);     // Используем shaderAlpha
    }

    // Проверка, находится ли точка внутри квадрата, используя функции ребер
    bool isInside(int x, int y) {
        float fx = (float)x + 0.5f; // Центр пикселя по x
        float fy = (float)y + 0.5f; // Центр пикселя по y

        // Вычисляем значения функций ребер
        float edge0 = (fx - v0.x) * edge0_dy - (fy - v0.y) * edge0_dx;
        float edge1 = (fx - v1.x) * edge1_dy - (fy - v1.y) * edge1_dx;
        float edge2 = (fx - v2.x) * edge2_dy - (fy - v2.y) * edge2_dx;
        float edge3 = (fx - v3.x) * edge3_dy - (fy - v3.y) * edge3_dx;

        // Проверяем принадлежность пикселя всем ребрам
        return (edge0 <= 0 && edge1 <= 0 && edge2 <= 0 && edge3 <= 0);
    }

    // Интерполяция координат текстуры для точки (x, y)
    void interpolateTextureCoordinates(int x, int y, float& u, float& v) {
        // Используем афинную интерполяцию текстурных координат
        float denom = (v1.x - v0.x) * (v3.y - v0.y) - (v3.x - v0.x) * (v1.y - v0.y);

        float fx = (float)x + 0.5f;
        float fy = (float)y + 0.5f;

        u = ((fx - v0.x) * (v3.y - v0.y) - (v3.x - v0.x) * (fy - v0.y)) / denom;
        v = ((v1.x - v0.x) * (fy - v0.y) - (fx - v0.x) * (v1.y - v0.y)) / denom;
    }

    // Основная функция для получения цвета пикселя
    COLOR color(int x, int y) {
        if (isInside(x, y)) {
            // Если точка внутри квадрата, вычисляем текстурные координаты
            float u, v;
            interpolateTextureCoordinates(x, y, u, v);
            return main(u, v); // Возвращаем цвет шахматного узора
        }
        else {
            return COLOR(0, 0, 0, 0); // Прозрачный цвет для точек вне квадрата
        }
    }

    // Возвращает границы области растеризации квадрата
    void getBounds(int& outMinX, int& outMaxX, int& outMinY, int& outMaxY) {
        outMinX = minX; // Минимальный X
        outMaxX = maxX; // Максимальный X
        outMinY = minY; // Минимальный Y
        outMaxY = maxY; // Максимальный Y
    }
};

class Painter {
private:
    // Функция для генерации ножки стола
    void GenerateLeg(std::vector<struct Polygon>& polygons, float x, float y, float z, float legWidth, COLOR color) {
        // Добавляем полигоны для всех граней ножки
        polygons.push_back({ {Vector(x, y, z), Vector(x + legWidth, y, z), Vector(x + legWidth, y + legWidth, z), Vector(x, y + legWidth, z)}, color }); // Верхняя грань
        polygons.push_back({ {Vector(x, y, z - 3), Vector(x + legWidth, y, z - 3), Vector(x + legWidth, y + legWidth, z - 3), Vector(x, y + legWidth, z - 3)}, color }); // Нижняя грань
        polygons.push_back({ {Vector(x, y, z), Vector(x, y + legWidth, z), Vector(x, y + legWidth, z - 3), Vector(x, y, z - 3)}, color }); // Боковая грань 1
        polygons.push_back({ {Vector(x + legWidth, y, z), Vector(x + legWidth, y + legWidth, z), Vector(x + legWidth, y + legWidth, z - 3), Vector(x + legWidth, y, z - 3)}, color }); // Боковая грань 2
        polygons.push_back({ {Vector(x, y, z), Vector(x + legWidth, y, z), Vector(x + legWidth, y, z - 3), Vector(x, y, z - 3)}, color }); // Передняя грань
        polygons.push_back({ {Vector(x, y + legWidth, z), Vector(x + legWidth, y + legWidth, z), Vector(x + legWidth, y + legWidth, z - 3), Vector(x, y + legWidth, z - 3)}, color }); // Задняя грань
    }

    // Функция для генерации столешницы
    void GenerateTabletop(std::vector<struct Polygon>& polygons, float height, COLOR color) {
        // Добавляем полигоны для всех граней столешницы
        polygons.push_back({ {Vector(-1, -1, 0), Vector(1, -1, 0), Vector(1, 1, 0), Vector(-1, 1, 0)}, color }); // Нижняя грань
        polygons.push_back({ {Vector(-1, -1, height), Vector(1, -1, height), Vector(1, 1, height), Vector(-1, 1, height)}, color }); // Верхняя грань
        polygons.push_back({ {Vector(-1, -1, 0), Vector(-1, 1, 0), Vector(-1, 1, height), Vector(-1, -1, height)}, color }); // Боковая грань 1
        polygons.push_back({ {Vector(1, -1, 0), Vector(1, 1, 0), Vector(1, 1, height), Vector(1, -1, height)}, color }); // Боковая грань 2
        polygons.push_back({ {Vector(-1, 1, 0), Vector(1, 1, 0), Vector(1, 1, height), Vector(-1, 1, height)}, color }); // Передняя грань
        polygons.push_back({ {Vector(-1, -1, 0), Vector(1, -1, 0), Vector(1, -1, height), Vector(-1, -1, height)}, color }); // Задняя грань
    }

    // Функция для генерации перекладины между ножками стола
    void GenerateCrossbar(std::vector<struct Polygon>& polygons, float z, float legWidth, COLOR color) {
        // Добавляем полигоны для всех граней перекладины
        polygons.push_back({ {Vector(-1, -1, z), Vector(-1 + legWidth, -1, z), Vector(-1 + legWidth, 1, z), Vector(-1, 1, z)}, color }); // Нижняя грань
        polygons.push_back({ {Vector(-1, -1, z + legWidth), Vector(-1 + legWidth, -1, z + legWidth), Vector(-1 + legWidth, 1, z + legWidth), Vector(-1, 1, z + legWidth)}, color }); // Верхняя грань
        polygons.push_back({ {Vector(-1, -1, z), Vector(-1, 1, z), Vector(-1, 1, z + legWidth), Vector(-1, -1, z + legWidth)}, color }); // Боковая грань 1
        polygons.push_back({ {Vector(-1 + legWidth, -1, z), Vector(-1 + legWidth, 1, z), Vector(-1 + legWidth, 1, z + legWidth), Vector(-1 + legWidth, -1, z + legWidth)}, color }); // Боковая грань 2
    }

    // Функция для генерации куба с разными цветами для каждой грани
    void GenerateCube(std::vector<struct Polygon>& polygons, float size) {
        // Определяем половину размера куба
        float halfSize = size / 2.0f;

        // Определяем разные цвета для каждой грани куба
        COLOR frontColor(255, 0, 0, 128);     // Передняя грань - Красный
        COLOR backColor(0, 255, 0, 128);      // Задняя грань - Зеленый
        COLOR leftColor(0, 0, 255, 128);      // Левая грань - Синий
        COLOR rightColor(255, 255, 0, 128);   // Правая грань - Желтый
        COLOR topColor(0, 255, 255, 128);     // Верхняя грань - Голубой
        COLOR bottomColor(255, 0, 255, 128);  // Нижняя грань - Пурпурный

        // Грани куба
        polygons.push_back({ {Vector(-halfSize, -halfSize, -halfSize), Vector(halfSize, -halfSize, -halfSize), Vector(halfSize, halfSize, -halfSize), Vector(-halfSize, halfSize, -halfSize)}, bottomColor }); // Нижняя грань
        polygons.push_back({ {Vector(-halfSize, -halfSize, halfSize), Vector(halfSize, -halfSize, halfSize), Vector(halfSize, halfSize, halfSize), Vector(-halfSize, halfSize, halfSize)}, topColor }); // Верхняя грань
        polygons.push_back({ {Vector(-halfSize, -halfSize, -halfSize), Vector(-halfSize, halfSize, -halfSize), Vector(-halfSize, halfSize, halfSize), Vector(-halfSize, -halfSize, halfSize)}, leftColor }); // Левая грань
        polygons.push_back({ {Vector(halfSize, -halfSize, -halfSize), Vector(halfSize, halfSize, -halfSize), Vector(halfSize, halfSize, halfSize), Vector(halfSize, -halfSize, halfSize)}, rightColor }); // Правая грань
        polygons.push_back({ {Vector(-halfSize, halfSize, -halfSize), Vector(halfSize, halfSize, -halfSize), Vector(halfSize, halfSize, halfSize), Vector(-halfSize, halfSize, halfSize)}, frontColor }); // Передняя грань
        polygons.push_back({ {Vector(-halfSize, -halfSize, -halfSize), Vector(halfSize, -halfSize, -halfSize), Vector(halfSize, -halfSize, halfSize), Vector(-halfSize, -halfSize, halfSize)}, backColor }); // Задняя грань
    }

public:
    // Функция для отрисовки стола и куба
    void Draw(Frame& frame, float legWidth, float zoomScale, float rotationX, float rotationY) {
        // Размер куба
        float cubeSize = 7.0f; // Увеличенный размер куба

        // Создание матрицы проекции
        Matrix projection_matrix;

        switch (currentProjection) {
        case CENTRAL:
            // Центральная проекция
            projection_matrix = Matrix::Frustum(-0.5f * frame.width / frame.height, 0.5f * frame.width / frame.height, -0.5f, 0.5f, 1.0f, 20.0f);
            break;
        case ORTHOGRAPHIC:
            // Ортографическая проекция
            projection_matrix = Matrix::Ortho(-4.0f * frame.width / frame.height, 4.0f * frame.width / frame.height, -4.0f, 4.0f, 4.0f, 400.0f);
            break;
        }

        // Общая матрица преобразований
        Matrix general_matrices =
            Matrix::Scale(zoomScale, zoomScale, zoomScale) * // Масштабирование
            Matrix::RotationX(rotationX) *                  // Поворот вокруг оси X
            Matrix::RotationY(rotationY) *                  // Поворот вокруг оси Y
            Matrix::Translation(0, 0, -8) *                 // Перемещение по оси Z
            projection_matrix *                             // Проекция
            Matrix::Viewport(frame.width / 4, frame.height / 4, frame.width / 2, frame.height / 2);

        Matrix current_matrix = general_matrices;
        std::vector<struct Polygon> polygons;

        // Генерация фигуры
        GenerateLeg(polygons, -1, -1, 0, legWidth, COLOR(230, 35, 35, globalAlpha)); // Ножки стола
        GenerateLeg(polygons, 1 - legWidth, -1, 0, legWidth, COLOR(230, 35, 35, globalAlpha));
        GenerateLeg(polygons, -1, 1 - legWidth, 0, legWidth, COLOR(230, 35, 35, globalAlpha));
        GenerateLeg(polygons, 1 - legWidth, 1 - legWidth, 0, legWidth, COLOR(230, 35, 35, globalAlpha));
        GenerateTabletop(polygons, 0.2f, COLOR(255, 255, 255, globalAlpha));         // Столешница
        GenerateCrossbar(polygons, -2.5f, legWidth, COLOR(0, 0, 255, globalAlpha));  // Перекладина

        // Генерация куба
        GenerateCube(polygons, cubeSize); // Полупрозрачный куб с разными цветами граней

        // Алгоритм художника для сортировки полигонов
        std::vector<std::pair<float, struct Polygon>> sortedPolygons;
        for (struct Polygon& poly : polygons) {
            // Вычисление среднего значения Z-координаты
            float averageZ = 0.0f;
            for (const Vector& vertex : poly.vertices) {
                Vector transformedVertex = vertex * current_matrix;     // Трансформированная вершина
                averageZ += transformedVertex.z / transformedVertex.w;  // Учитываем перспективу
            }
            averageZ /= poly.vertices.size();                           // Среднее значение Z
            sortedPolygons.emplace_back(averageZ, poly);                // Добавляем полигон в список с сортировочным ключом
        }

        // Сортируем полигоны по убыванию Z (от дальних к ближним)
        std::sort(sortedPolygons.begin(), sortedPolygons.end(),
                  [](const std::pair<float, struct Polygon>& a, const std::pair<float, struct Polygon>& b) {
            return a.first > b.first;
            });

        // Отрисовка отсортированных полигонов
        for (const auto& sortedPair : sortedPolygons) {
            const struct Polygon& poly = sortedPair.second;

            // Трансформируем вершины
            std::vector<Vector> transformed_vertices;
            for (const Vector& v : poly.vertices) {
                Vector tv = v * current_matrix; // Преобразование
                tv.x /= tv.w; // Учитываем перспективное деление
                tv.y /= tv.w;
                tv.z /= tv.w;
                transformed_vertices.push_back(tv);
            }

            // Проверка, является ли грань верхней границей столешницы
            bool isTabletopTopFace = false;
            if (poly.vertices.size() == 4 &&
                poly.vertices[0].z == 0.2f &&
                poly.vertices[1].z == 0.2f &&
                poly.vertices[2].z == 0.2f &&
                poly.vertices[3].z == 0.2f) {
                isTabletopTopFace = true;
            }

            // Проверка, является ли грань передней гранью куба
            bool isCubeFrontFace = false;
            float halfSize = cubeSize / 2.0f; // Половина размера куба
            const float epsilon = 1e-3f; // Допустимая погрешность для сравнения
            if (poly.vertices.size() == 4 &&
                std::abs(poly.vertices[0].z - halfSize) < epsilon &&
                std::abs(poly.vertices[1].z - halfSize) < epsilon &&
                std::abs(poly.vertices[2].z - halfSize) < epsilon &&
                std::abs(poly.vertices[3].z - halfSize) < epsilon) {
                isCubeFrontFace = true;
            }

            if (isTabletopTopFace) {
                // Растеризация верхней грани столешницы с шахматным узором
                QuadShader shader(transformed_vertices, poly.vertices);

                int minX, maxX, minY, maxY;
                shader.getBounds(minX, maxX, minY, maxY); // Границы растеризации

                for (int y = minY; y <= maxY; y++) {
                    for (int x = minX; x <= maxX; x++) {
                        COLOR color = shader.color(x, y); // Цвет пикселя
                        if (color.ALPHA > 0) {
                            frame.SetPixel(x, y, color); // Установка пикселя
                        }
                    }
                }
            }
            else if (isCubeFrontFace) {
                // Растеризация передней грани куба с шахматным узором
                QuadShader shader(transformed_vertices, poly.vertices);

                int minX, maxX, minY, maxY;
                shader.getBounds(minX, maxX, minY, maxY); // Границы растеризации

                for (int y = minY; y <= maxY; y++) {
                    for (int x = minX; x <= maxX; x++) {
                        COLOR color = shader.color(x, y); // Цвет пикселя
                        if (color.ALPHA > 0) {
                            frame.SetPixel(x, y, color); // Установка пикселя
                        }
                    }
                }
            }
            else {
                // Отрисовка остальных граней
                frame.Quad(transformed_vertices[0].x, transformed_vertices[0].y, transformed_vertices[0].z,
                    transformed_vertices[1].x, transformed_vertices[1].y, transformed_vertices[1].z,
                    transformed_vertices[2].x, transformed_vertices[2].y, transformed_vertices[2].z,
                    transformed_vertices[3].x, transformed_vertices[3].y, transformed_vertices[3].z,
                    poly.color);
            }
        }
    }
};

#endif // PAINTER_H