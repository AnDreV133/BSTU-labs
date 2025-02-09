//
// Created by dmitr on 03.12.2024.
//



#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include "Vector.h"
#include "Shader.h"
#include <optional>
#include <tuple>
#include <vector>

// Время от начала запуска программы
int n_angles = 10;
float x_rot = 0;
float y_rot = 0;
float z_rot = 0;
float x_trsl = 0;
float y_trsl = 0;
float z_trsl = 0;
int global_alpha = 255;

// Тип проекции (перспективная или ортографическая)
int projection_type = 0;
int draw_type = 0;

const std::vector<Vector> get_points() {
    std::vector<Vector> result;

    result.emplace_back(0, 0, -0.9); // 0 - нижняя точка ножки
    result.emplace_back(0, 0, 0.5); // 1 - верхняя точка ножки
    result.emplace_back(0, 0, 0.7); // 2 - верхняя точка шлипки

    result.emplace_back(0.4, 0, -0.5); // 3..7 - нижний многоугольник
    Matrix rot_z_matrix = Matrix::RotationZ(360 / 5 * M_PI / 180);
    for (int i = 1; i < 5; i++) {
        Vector temp = result.back() * rot_z_matrix;
        result.emplace_back(temp.x, temp.y, temp.z);
    }

    result.emplace_back(0.4, 0, 0.499); // 8..12 - верхний многоугольник
    for (int i = 1; i < 5; i++) {
        Vector temp = result.back() * rot_z_matrix;
        result.emplace_back(temp.x, temp.y, temp.z);
    }

    rot_z_matrix = Matrix::RotationZ(60 * M_PI / 180);
    std::vector<Vector> sub; // 13..18 - верхний многоугольник
    sub.emplace_back(0.7, 0, 0.7);
    for (int i = 1; i < 6; i++) {
        Vector temp = sub[i - 1] * rot_z_matrix;
        sub.emplace_back(temp.x, temp.y, temp.z);
    }

    result.insert(result.end(), sub.begin(), sub.end());

    for (int i = 0; i < 6; i++) { // 19..24 - нижний многоугольник
        result.emplace_back((sub[i].x + sub[(i + 1) % 6].x) / 2,
                            (sub[i].y + sub[(i + 1) % 6].y) / 2,
                            (sub[i].z + sub[(i + 1) % 6].z) / 2 - 0.2);
    }

    // координаты куба
    result.emplace_back(-0.99, -0.99, -0.99); // 25
    result.emplace_back(-0.99, -0.99, 0.99); // 26
    result.emplace_back(-0.99, 0.99, -0.99); // 27
    result.emplace_back(-0.99, 0.99, 0.99); // 28
    result.emplace_back(0.99, -0.99, -0.99); // 29
    result.emplace_back(0.99, -0.99, 0.99); // 30
    result.emplace_back(0.99, 0.99, -0.99); // 31
    result.emplace_back(0.99, 0.99, 0.99); // 32

    return result;
}

std::vector<std::tuple<int, int, int>> get_polygons() {
    std::vector<std::tuple<int, int, int>> result;

    // наконечник снизу
    result.emplace_back(0, 3, 4);
    result.emplace_back(0, 4, 5);
    result.emplace_back(0, 5, 6);
    result.emplace_back(0, 6, 7);
    result.emplace_back(0, 7, 3);

    // бок с низу - стенка
    result.emplace_back(9, 3, 4);
    result.emplace_back(10, 4, 5);
    result.emplace_back(11, 5, 6);
    result.emplace_back(12, 6, 7);
    result.emplace_back(8, 7, 3);

    // бок с верху - тенка
    result.emplace_back(3, 8, 9);
    result.emplace_back(4, 9, 10);
    result.emplace_back(5, 10, 11);
    result.emplace_back(6, 11, 12);
    result.emplace_back(7, 12, 8);

    // основание нижнего многоугольника
    result.emplace_back(1, 19, 20);
    result.emplace_back(1, 20, 21);
    result.emplace_back(1, 21, 22);
    result.emplace_back(1, 22, 23);
    result.emplace_back(1, 23, 24);
    result.emplace_back(1, 24, 19);

    // бока нижнего многоугольника
    result.emplace_back(14, 19, 20);
    result.emplace_back(15, 20, 21);
    result.emplace_back(16, 21, 22);
    result.emplace_back(17, 22, 23);
    result.emplace_back(18, 23, 24);
    result.emplace_back(13, 24, 19);

    // бока верхнего многоугольника
    result.emplace_back(14, 15, 20);
    result.emplace_back(15, 16, 21);
    result.emplace_back(16, 17, 22);
    result.emplace_back(17, 18, 23);
    result.emplace_back(18, 13, 24);
    result.emplace_back(13, 14, 19);

    // верхний многоугольник
    result.emplace_back(13, 14, 2);
    result.emplace_back(14, 15, 2);
    result.emplace_back(15, 16, 2);
    result.emplace_back(16, 17, 2);
    result.emplace_back(17, 18, 2);
    result.emplace_back(18, 13, 2);

    // куб
    result.emplace_back(25, 26, 27);
    result.emplace_back(26, 27, 28);
    result.emplace_back(27, 28, 31);
    result.emplace_back(28, 32, 31);
    result.emplace_back(29, 32, 31);
    result.emplace_back(30, 29, 32);
    result.emplace_back(25, 30, 29);
    result.emplace_back(26, 25, 30);

    result.emplace_back(28, 26, 32);
    result.emplace_back(26, 32, 30);

    result.emplace_back(25, 27, 31);
    result.emplace_back(31, 29, 25);

    return result;
}

std::vector<BaseShader *> get_materials(int alpha) {
    std::vector<BaseShader *> result;

    for (int i = 0; i < 33; i++) {
        result.emplace_back(new ColorShader({100 + i * 10, 0, 60, alpha}));
    }

    Matrix rot_z_matrix = Matrix::RotationZ(60 * M_PI / 180);
    std::vector<Vector> sub; // 13..18 - верхний многоугольник
    sub.emplace_back(0.7, 0, 0.7);
    for (int i = 1; i < 6; i++) {
        Vector temp = sub[i - 1] * rot_z_matrix;
        sub.emplace_back(temp.x, temp.y, temp.z);
    }

    std::vector<Vector> v3;
    v3.emplace_back(sub.at(0));
    v3.emplace_back(sub.at(1));
    v3.emplace_back(0, 0, 0.7);
    result.emplace_back(new DottedShader(v3.at(0), v3.at(1), v3.at(2), alpha));
    v3[0] = sub.at(1);
    v3[1] = sub.at(2);
    result.emplace_back(new DottedShader(v3.at(0), v3.at(1), v3.at(2), alpha));
    v3[0] = sub.at(2);
    v3[1] = sub.at(3);
    result.emplace_back(new DottedShader(v3.at(0), v3.at(1), v3.at(2), alpha));
    v3[0] = sub.at(3);
    v3[1] = sub.at(4);
    result.emplace_back(new DottedShader(v3.at(0), v3.at(1), v3.at(2), alpha));
    v3[0] = sub.at(4);
    v3[1] = sub.at(5);
    result.emplace_back(new DottedShader(v3.at(0), v3.at(1), v3.at(2), alpha));
    v3[0] = sub.at(5);
    v3[1] = sub.at(0);
    result.emplace_back(new DottedShader(v3.at(0), v3.at(1), v3.at(2), alpha));
    Vector v25 = {-1, -1, -1};
    Vector v26 = {-1, -1, 1};
    Vector v27 = {-1, 1, -1};
    Vector v28 = {-1, 1, 1};
    Vector v29 = {1, -1, -1};
    Vector v30 = {1, -1, 1};
    Vector v31 = {1, 1, -1};
    Vector v32 = {1, 1, 1};
    result.emplace_back(new ChessShader(v25, v26, v27, alpha));
    result.emplace_back(new ChessShader(v26, v27, v28, alpha));
    result.emplace_back(new ChessShader(v27, v28, v31, alpha));
    result.emplace_back(new ChessShader(v28, v32, v31, alpha));
    result.emplace_back(new ChessShader(v29, v32, v31, alpha));
    result.emplace_back(new ChessShader(v30, v29, v32, alpha));
    result.emplace_back(new ChessShader(v25, v30, v29, alpha));
    result.emplace_back(new ChessShader(v26, v25, v30, alpha));
    result.emplace_back(new ChessShader(v28, v26, v32, alpha));
    result.emplace_back(new ChessShader(v26, v32, v30, alpha));
    result.emplace_back(new ChessShader(v25, v27, v31, alpha));
    result.emplace_back(new ChessShader(v31, v29, v25, alpha));

    return result;
}

#define EPS 0.0000000001

std::tuple<bool, Vector> intersect_points_2d(Vector A, Vector B, Vector C, Vector D) {
    float dABx = std::abs(B.x - A.x);
    float dCDx = std::abs(C.x - D.x);
    float dABy = std::abs(B.y - A.y);
    float dCDy = std::abs(C.y - D.y);
    Vector O = {};

    if (dABx < EPS && dCDy < EPS) {
        O.x = A.x;
        O.y = C.y;

        return {true, O};
    } else if (dABy < EPS && dCDx < EPS) {
        O.x = C.x;
        O.y = A.y;

        return {true, O};
    }
    if (dABx < EPS && dCDx < EPS || dABy < EPS && dCDy < EPS) {
        // Прямые параллельны
        return {false, O};
    } else if (dABx < EPS && dABy < EPS || dCDx < EPS && dCDy < EPS) {
        // Передана точка
        return {false, O};
    } else if (dABx < EPS) {
        // AB параллельна абсциссе
        O.x = A.x;
        O.y = (O.x - C.x) * (D.y - C.y) / (D.x - C.x) + C.y;

        return {
                O.x < fmax(C.x, D.x) && O.x > fmin(C.x, D.x) &&
                O.y < fmax(A.y, B.y) && O.y > fmin(A.y, B.y) &&
                O.y < fmax(C.y, D.y) && O.y > fmin(C.y, D.y), O};
    } else if (dABy < EPS) {
        // AB параллельна ординате
        O.y = A.y;
        O.x = (O.y - C.y) * (D.x - C.x) / (D.y - C.y) + C.x;

        return {O.x < fmax(A.x, B.x) && O.x > fmin(A.x, B.x) &&
                O.x < fmax(C.x, D.x) && O.x > fmin(C.x, D.x) &&
                O.y < fmax(C.y, D.y) && O.y > fmin(C.y, D.y), O};
    } else if (dCDy < EPS) {
        O.y = C.y;
        O.x = (O.y - A.y) * (B.x - A.x) / (B.y - A.y) + A.x;

        return {O.x < fmax(A.x, B.x) && O.x > fmin(A.x, B.x) &&
                O.x < fmax(C.x, D.x) && O.x > fmin(C.x, D.x) &&
                O.y < fmax(A.y, B.y) && O.y > fmin(A.y, B.y), O};
    } else if (dCDx < EPS) {
        O.x = C.x;
        O.y = (O.x - A.x) * (B.y - A.y) / (B.x - A.x) + A.y;

        return {O.x < fmax(A.x, B.x) && O.x > fmin(A.x, B.x) &&
                O.y < fmax(A.y, B.y) && O.y > fmin(A.y, B.y) &&
                O.y < fmax(C.y, D.y) && O.y > fmin(C.y, D.y), O};
    } else {
        O.y = ((D.y - C.y) * (A.x - A.y * (B.x - A.x) / (B.y - A.y) - C.x) / (D.x - C.x) + C.y) /
              (1 - (D.y - C.y) * (B.x - A.x) / ((D.x - C.x) * (B.y - A.y)));
        O.x = (O.y - C.y) * (D.x - C.x) / (D.y - C.y) + C.x;

        return {O.x < fmax(A.x, B.x) && O.x > fmin(A.x, B.x) &&
                O.x < fmax(C.x, D.x) && O.x > fmin(C.x, D.x) &&
                O.y < fmax(A.y, B.y) && O.y > fmin(A.y, B.y) &&
                O.y < fmax(C.y, D.y) && O.y > fmin(C.y, D.y), O};
    }
}

// Возвращает 1, если AB ближе CD.
// Возвращает -1, еслир AB дальше CD.
// Возвращает 0, если отрезки не пересекаются.
int cmp_sides(Vector A, Vector B, Vector C, Vector D) {
    auto inters_res = intersect_points_2d(
            {A.x, A.y, 0, 0},
            {B.x, B.y, 0, 0},
            {C.x, C.y, 0, 0},
            {D.x, D.y, 0, 0}
    );

    // Если точки совпадают, нужно вернуть 0, иначе всё ломается (очень плохо ломается)
    if (std::abs(A.x - C.x) < EPS && std::abs(A.y - C.y) < EPS ||
        std::abs(A.x - D.x) < EPS && std::abs(A.y - D.y) < EPS ||
        std::abs(B.x - C.x) < EPS && std::abs(B.y - C.y) < EPS ||
        std::abs(B.x - D.x) < EPS && std::abs(B.y - D.y) < EPS)
        return 0;

    if (!std::get<0>(inters_res)) return 0;

    auto O = std::get<1>(inters_res);
    float zAB, zCD;

    if (std::abs(B.x - A.x) > std::abs(B.y - A.y)) {
        zAB = (O.x - A.x) * (B.z - A.z) / (B.x - A.x) + A.z;
    } else {
        zAB = (O.y - A.y) * (B.z - A.z) / (B.y - A.y) + A.z;
    }

    if (std::abs(D.x - C.x) > std::abs(D.y - C.y)) {
        zCD = (O.x - C.x) * (D.z - C.z) / (D.x - C.x) + C.z;
    } else {
        zCD = (O.y - C.y) * (D.z - C.z) / (D.y - C.y) + C.z;
    }

    if (zAB < zCD) {
        return 1;
    } else {
        return -1;
    }
}

bool is_point_inside_polygon(Vector O, std::vector<Vector> polygon) {
    if (std::abs(O.x - polygon[0].x) < EPS && std::abs(O.x - polygon[0].x) < EPS ||
        std::abs(O.x - polygon[1].x) < EPS && std::abs(O.x - polygon[1].x) < EPS ||
        std::abs(O.x - polygon[2].x) < EPS && std::abs(O.x - polygon[2].x) < EPS)
        return true;

    int neg_count = 0, pos_count = 0;
    std::vector<Vector> vectors;
    for (int i = 0; i < polygon.size(); i++) {
        vectors.push_back(
                {
                        polygon[i].x - O.x,
                        polygon[i].y - O.y,
                        polygon[i].z - O.z
                });
    }

    for (int i = 0; i < polygon.size(); i++) {
        Vector a = vectors[i], b = vectors[(i + 1) % polygon.size()];
        float cz = a.x * b.y - b.x * a.y;
        if (cz < 0) {
            neg_count++;
        } else {
            pos_count++;
        }
    }

    return neg_count == polygon.size() || pos_count == polygon.size();
}

// Возвращает 1, если A ближе B
// Возвращает -1, если B ближе A
// Возвращает 0, если A и B не пересекаются.
int cmp_triangles(std::vector<Vector> A, std::vector<Vector> B) {
    std::vector<Vector> polygonA;
    for (auto & a : A) {
        polygonA.emplace_back(a.x / a.w, a.y / a.w, a.z / a.w);
    }
    std::vector<Vector> polygonB;
    for (auto & b : B) {
        polygonB.emplace_back(b.x / b.w, b.y / b.w, b.z / b.w);
    }

    Vector Ea;
    Vector Eb;
    bool Eb_first = true;
    bool a_in_b = true;
    bool b_in_a = true;

    for (int i = 0; i < polygonA.size(); i++) {
        Ea.x += polygonA[i].x / polygonA.size();
        Ea.y += polygonA[i].y / polygonA.size();
        Ea.z += polygonA[i].z / polygonA.size();
        a_in_b &= is_point_inside_polygon(polygonA[i], polygonB);

        for (int j = 0; j < polygonB.size(); j++) {
            auto compare_sides = cmp_sides(
                    polygonA[i], polygonA[(i + 1) % polygonA.size()],
                    polygonB[j], polygonB[(j + 1) % polygonB.size()]
            );

            if (compare_sides) return compare_sides;

            if (!Eb_first) continue;

            Eb.x += polygonB[j].x / polygonB.size();
            Eb.y += polygonB[j].y / polygonB.size();
            Eb.z += polygonB[j].z / polygonB.size();

            b_in_a &= is_point_inside_polygon(polygonB[j], polygonA);
        }

        Eb_first = false;
    }

    bool swapped = false;

    if (!a_in_b && !b_in_a) return 0;

    // Делаем так, чтобы b был внутри a
    if (a_in_b) {
        std::swap(Ea, Eb);
        std::swap(polygonA, polygonB);
        swapped = true;
    }

    auto P0 = polygonA[0];
    auto P1 = polygonA[1];
    auto P2 = polygonA[2];

    float z = ((Eb.y - P0.y) * ((P1.x - P0.x) * (P2.z - P0.z) - (P2.x - P0.x) * (P1.z - P0.z)) -
               (Eb.x - P0.x) * ((P1.y - P0.y) * (P2.z - P0.z) - (P2.y - P0.y) * (P1.z - P0.z))) /
              ((P1.x - P0.x) * (P2.y - P0.y) - (P2.x - P0.x) * (P1.y - P0.y)) + P0.z;

    if (Eb.z < z) {
        if (swapped) {
            return 1;
        } else {
            return -1;
        }
    } else {
        if (swapped) {
            return -1;
        } else {
            return 1;
        }
    }
}

class Painter {
public:

    void Draw(Frame &frame) {

        auto A = get_points();
        auto polygons = get_polygons();
        auto materials = get_materials(global_alpha);

        Matrix projection_matrix; // Матрица проектирования

        // Выбор матрицы проектирования
        if (projection_type % 2) {
            projection_matrix = Matrix::Ortho(-2.0 * frame.width / frame.height, 2.0 * frame.width / frame.height, -2.0,
                                              2.0, 1, 140.0f);

        } else {
            projection_matrix = Matrix::Frustum(-0.5 * frame.width / frame.height, 0.5 * frame.width / frame.height,
                                                -0.5,
                                                0.5, 1, 140);
        }

        Matrix proj_viewport = projection_matrix *                 // Проектирование
                               Matrix::Viewport(0, 0, frame.width, frame.height);

        Matrix transform = Matrix::RotationZ(z_rot * M_PI / 180) *
                           Matrix::RotationX(((-90 + x_rot) * M_PI / 180)) *
                           Matrix::RotationY(y_rot * M_PI / 180) *
                           Matrix::Translation(0, 0, -4) *
                           Matrix::Translation(x_trsl, y_trsl, z_trsl);

        // Преобразование нормализованных координат в оконные
        Matrix general_matrix = transform * proj_viewport;

        std::vector<Vector> B(A.size());
        for (int i = 0; i < A.size(); i++) {
            B[i] = A[i] * general_matrix;
        }

        std::vector<std::vector<Vector>> polygons_vals;
        std::vector<int> polygons_indices;
        for (int i = 0; i < polygons.size(); i++) {
            // Формируем список полигонов
            polygons_vals.push_back(
                    {
                            B[std::get<0>(polygons[i])],
                            B[std::get<1>(polygons[i])],
                            B[std::get<2>(polygons[i])]
                    });
            polygons_indices.push_back(i);
        }

        auto H = std::vector<std::vector<int>>(polygons.size(), std::vector<int>(polygons.size(), 0));
        for (int i = 0; i < polygons.size(); i++) {
            for (int j = 0; j < i; j++) {
                auto res = cmp_triangles(polygons_vals[i], polygons_vals[j]);
                H[i][j] = res;
                H[j][i] = -res;
            }
        }

        while (H.size() != 0) {
            int index_to_del = 0;
            int min_ones = H.size();
            for (int i = 0; i < H.size(); i++) {
                int ones = 0;

                for (int j = 0; j < H[i].size(); j++) {
                    if (H[i][j] == 1) {
                        ones++;
                    }
                }

                if (ones < min_ones) {
                    min_ones = ones;
                    index_to_del = i;
                }
            }

            frame.Triangle(
                    polygons_vals[index_to_del][0],
                    polygons_vals[index_to_del][1],
                    polygons_vals[index_to_del][2],
                    materials[polygons_indices[index_to_del]]);

            H.erase(H.begin() + index_to_del);
            for (int i = 0; i < H.size(); i++) {
                H[i].erase(H[i].begin() + index_to_del);
            }

            polygons_vals.erase(polygons_vals.begin() + index_to_del);
            polygons_indices.erase(polygons_indices.begin() + index_to_del);
        }

        for (int i = 0; i < materials.size(); i++) {
            free(materials[i]);
        }
    }
};

#endif // PAINTER_H