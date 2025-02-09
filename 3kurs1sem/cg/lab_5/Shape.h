#ifndef LAB_4_AFFINE_3D_SIMPLE_CUBE_SHAPE_H
#define LAB_4_AFFINE_3D_SIMPLE_CUBE_SHAPE_H

#include <vector>
#include "Vector.h"
#include "Matrix.h"
#include "Frame.h"
#include "Shader.h"
#include <math.h>

class Shape {
    std::vector<Vector> l_6;
    std::vector<Vector> l_6_hom;
    std::vector<Vector> u_6;
    std::vector<Vector> u_6_hom;
    std::vector<Vector> l_N;
    std::vector<Vector> l_N_hom;
    std::vector<Vector> u_N;
    std::vector<Vector> u_N_hom;
    Vector cu_6;
    Vector cu_6_hom;
    Vector cl_6;
    Vector cl_6_hom;
    Vector c_N;
    Vector c_N_hom;
    int N;
    Frame *frame;

    Shape *draw_polygon_with_shader_non_normalise() {
        for (int i = 0; i < 6; i++) {
            int pigment = 100 + 150 / 6 * i;
            int next_i = (i + 1) % 6;
            Shader shader1(u_6_hom[i], u_6_hom[next_i], cu_6_hom);
            frame->Triangle(u_6[i], u_6[next_i], cu_6, shader1);
            frame->Triangle(u_6[i], u_6[next_i], l_6[i], COLOR{0, pigment, 0, 255});
            frame->Triangle(l_6[i], l_6[next_i], u_6[next_i],
                            COLOR{0, 0, pigment, 255});
            frame->Triangle(l_6[i], l_6[next_i], cl_6, COLOR{0, pigment, pigment, 255});
        }

        for (int i = 0; i < N; i++) {
            int pigment = 100 + 150 / N * i;
            int next_i = (i + 1) % N;
            frame->Triangle(u_N[i], u_N[next_i], l_N[i],
                            COLOR{pigment, pigment, 0, 255});
            frame->Triangle(l_N[i], l_N[next_i], u_N[next_i],
                            COLOR{pigment, pigment, 0, 255});
            frame->Triangle(l_N[i], l_N[next_i], c_N,
                            COLOR{pigment, 0, pigment, 255});
        }

        return this;
    }

    Shape *draw_polygon_non_normalise() {
        for (int i = 0; i < 6; i++) {
            int pigment = 100 + 150 / 6 * i;
            int next_i = (i + 1) % 6;
            frame->Triangle(u_6[i], u_6[next_i], cu_6, COLOR{pigment, 0, 0, 255});
            frame->Triangle(u_6[i], u_6[next_i], l_6[i], COLOR{0, pigment, 0, 255});
            frame->Triangle(l_6[i], l_6[next_i], u_6[next_i],
                            COLOR{0, 0, pigment, 255});
            frame->Triangle(l_6[i], l_6[next_i], cl_6, COLOR{0, pigment, pigment, 255});
        }

        for (int i = 0; i < N; i++) {
            int pigment = 100 + 150 / N * i;
            int next_i = (i + 1) % N;
            frame->Triangle(u_N[i], u_N[next_i], l_N[i],
                            COLOR{pigment, pigment, 0, 255});
            frame->Triangle(l_N[i], l_N[next_i], u_N[next_i],
                            COLOR{pigment, pigment, 0, 255});
            frame->Triangle(l_N[i], l_N[next_i], c_N,
                            COLOR{pigment, 0, pigment, 255});
        }

        return this;
    }

    Shape *draw_edges_non_normalise() {
        COLOR color = {255, 255, 255, 255};

        for (int i = 0; i < 6; i++) {
            int next_i = (i + 1) % 6;
            frame->DrawLine(u_6[i], u_6[next_i], color);
            frame->DrawLine(u_6[i], l_6[i], color);
            frame->DrawLine(l_6[i], u_6[next_i], color);
            frame->DrawLine(l_6[i], l_6[next_i], color);
            frame->DrawLine(u_6[i], cu_6, color);
            frame->DrawLine(l_6[i], cl_6, color);
        }

        for (int i = 0; i < N; i++) {
            int next_i = (i + 1) % N;
            frame->DrawLine(u_N[i], u_N[next_i], color);
            frame->DrawLine(u_N[i], l_N[i], color);
            frame->DrawLine(l_N[i], l_N[next_i], color);
            frame->DrawLine(l_N[i], c_N, color);
        }

        return this;
    }

    Shape *draw_square_with_normalise(Matrix &projection_matrix) {
        projection_matrix = Matrix::RotationX(90 * M_PI / 180) * projection_matrix;

        Vector v1_hom = {1, 1, 0, 1};
        Vector v2_hom = {1, -1, 0, 1};
        Vector v3_hom = {-1, 1, 0, 1};
        Vector v4_hom = {-1, -1, 0, 1};

        Vector v1 = {1, 1, 0, 1};
        v1.hom_to_st(projection_matrix);
        Vector v2 = {1, -1, 0, 1};
        v2.hom_to_st(projection_matrix);
        Vector v3 = {-1, 1, 0, 1};
        v3.hom_to_st(projection_matrix);
        Vector v4 = {-1, -1, 0, 1};
        v4.hom_to_st(projection_matrix);

        Shader shader1(v1_hom, v2_hom, v3_hom);
        frame->Triangle(v1, v2, v3, shader1);

        Shader shader2(v2_hom, v3_hom, v4_hom);
        frame->Triangle(v2, v3, v4, shader2);
    }

public:
    Shape(int n, Frame *f) {
        N = n;
        frame = f;

        u_6.reserve(6);
        l_6.reserve(6);
        u_N.reserve(N);
        l_N.reserve(N);

        Matrix rot_z_matrix = Matrix::RotationZ(60 * M_PI / 180);
        u_6.emplace_back(0.7, 0, 0.7);
        u_6_hom.emplace_back(0.7, 0, 0.7);
        for (int i = 1; i < 6; i++) {
            Vector temp = u_6[i - 1] * rot_z_matrix;
            u_6.emplace_back(temp.x, temp.y, temp.z);
            u_6_hom.emplace_back(temp.x, temp.y, temp.z);
        }

        for (int i = 0; i < 6; i++) {
            l_6.emplace_back((u_6[i].x + u_6[(i + 1) % 6].x) / 2,
                             (u_6[i].y + u_6[(i + 1) % 6].y) / 2,
                             (u_6[i].z + u_6[(i + 1) % 6].z) / 2 - 0.2);
            l_6_hom.emplace_back((u_6[i].x + u_6[(i + 1) % 6].x) / 2,
                                 (u_6[i].y + u_6[(i + 1) % 6].y) / 2,
                                 (u_6[i].z + u_6[(i + 1) % 6].z) / 2 - 0.2);
        }

        rot_z_matrix = Matrix::RotationZ(360 / N * M_PI / 180);
        u_N.emplace_back(0.4, 0, 0.5);
        u_N_hom.emplace_back(0.4, 0, 0.5);
        l_N.emplace_back(0.4, 0, -0.5);
        l_N_hom.emplace_back(0.4, 0, -0.5);
        for (int i = 1; i < N; i++) {
            Vector temp = u_N[i - 1] * rot_z_matrix;
            u_N.emplace_back(temp.x, temp.y, temp.z);
            u_N_hom.emplace_back(temp.x, temp.y, temp.z);
            l_N.emplace_back(temp.x, temp.y, temp.z - 1);
            l_N_hom.emplace_back(temp.x, temp.y, temp.z - 1);
        }

        cu_6 = Vector(0, 0, 0.7);
        cu_6_hom = Vector(0, 0, 0.7);
        cl_6 = Vector(0, 0, 0.5);
        cl_6_hom = Vector(0, 0, 0.5);
        c_N = Vector(0, 0, -0.9);
        c_N_hom = Vector(0, 0, -0.9);
    }

    Shape *normalize_coordinates(Matrix &projection_matrix) {
        c_N.hom_to_st(projection_matrix);
        cu_6.hom_to_st(projection_matrix);
        cl_6.hom_to_st(projection_matrix);

        for (int i = 0; i < 6; i++) {
            u_6[i].hom_to_st(projection_matrix);
            l_6[i].hom_to_st(projection_matrix);
        }

        for (int i = 0; i < N; i++) {
            u_N[i].hom_to_st(projection_matrix);
            l_N[i].hom_to_st(projection_matrix);
        }

        return this;
    }

    Shape *draw_polygon(Matrix &projection_matrix) {
        return normalize_coordinates(projection_matrix)
                ->draw_polygon_non_normalise();

    }

    Shape *draw_edges(Matrix &projection_matrix) {
        return normalize_coordinates(projection_matrix)
                ->draw_edges_non_normalise();
    }

    Shape *draw_full(Matrix &projection_matrix) {
        return normalize_coordinates(projection_matrix)
                ->draw_polygon_non_normalise()
                ->draw_edges_non_normalise();
    }

    Shape *draw_with_shader(Matrix &projection_matrix) {
        return normalize_coordinates(projection_matrix)
                ->draw_polygon_with_shader_non_normalise();
    }

    Shape *draw_square(Matrix &projection_matrix) {
        return draw_square_with_normalise(projection_matrix);
    }
};

//Shape *rotate(Matrix &rotation_matrix) {
//    c_N = c_N * rotation_matrix;
//    cu_6 = cu_6 * rotation_matrix;
//    cl_6 = cl_6 * rotation_matrix;
//
//    for (int i = 0; i < 6; i++) {
//        u_6[i] = u_6[i] * rotation_matrix;
//        l_6[i] = l_6[i] * rotation_matrix;
//    }
//
//    for (int i = 0; i < N; i++) {
//        u_N[i] = u_N[i] * rotation_matrix;
//        l_N[i] = l_N[i] * rotation_matrix;
//    }
//
//    return this;
//}

#endif
