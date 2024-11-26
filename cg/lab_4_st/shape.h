#ifndef LAB_4_AFFINE_3D_SIMPLE_CUBE_SHAPE_H
#define LAB_4_AFFINE_3D_SIMPLE_CUBE_SHAPE_H

#include <vector>
#include "Vector.h"
#include "Matrix.h"
#include "Frame.h"
#include <math.h>

class TriangleShader
{
public:
    Vector vertices[3];

    TriangleShader(Vector D0, Vector D1, Vector D2) : vertices{ D0, D1, D2 } {};

    COLOR main(Vector data)
    {
        float x = data.x;
        float y = data.y;
        float z = data.z;

        float a = fmod(x + 1, 0.4f);
        float b = fmod(y + 1, 0.4f);

        if ((a > 0.2f) == (b > 0.2f))
            return COLOR(200, 200, 200);
        else
            return COLOR(20, 20, 0);
    }

    Vector getPixelData(double h0, double h1, double h2)
    {
        float x = h0 * vertices[0].x + h1 * vertices[1].x + h2 * vertices[2].x;
        float y = h0 * vertices[0].y + h1 * vertices[1].y + h2 * vertices[2].y;
        float z = h0 * vertices[0].z + h1 * vertices[1].z + h2 * vertices[2].z;

        return { x, y, z };
    }
};

class TriangleShaderPlain
{
public:
    Vector vertices[3];
    COLOR color;

    TriangleShaderPlain(Vector D0, Vector D1, Vector D2, COLOR color) : vertices{ D0, D1, D2 }, color(color) {};

    COLOR main(Vector data)
    {
        return color;
    }

    Vector getPixelData(double h0, double h1, double h2)
    {
        float x = h0 * vertices[0].x + h1 * vertices[1].x + h2 * vertices[2].x;
        float y = h0 * vertices[0].y + h1 * vertices[1].y + h2 * vertices[2].y;
        float z = h0 * vertices[0].z + h1 * vertices[1].z + h2 * vertices[2].z;

        return { x, y, z };
    }
};

class Shape{
    std::vector<Vector> lower_6;
    std::vector<Vector> upper_6;
    std::vector<Vector> lower_N;
    std::vector<Vector> upper_N;
    Vector end_point;
    Vector center_u_6;
    Vector center_l_6;

    std::vector<Vector> n_lower_6;
    std::vector<Vector> n_upper_6;
    std::vector<Vector> n_lower_N;
    std::vector<Vector> n_upper_N;
    Vector n_end_point;
    Vector n_center_u_6;
    Vector n_center_l_6;

    int N;
    Frame *frame;

public:
    void normalize_coordinates(Matrix &projection_matrix){
        n_end_point.hom_to_st(projection_matrix);
        n_center_u_6.hom_to_st(projection_matrix);
        n_center_l_6.hom_to_st(projection_matrix);

        for (int i = 0; i < 6; i++){
            n_upper_6[i].hom_to_st(projection_matrix);
            n_lower_6[i].hom_to_st(projection_matrix);
        }

        for(int i = 0; i < N; i++){
            n_upper_N[i].hom_to_st(projection_matrix);
            n_lower_N[i].hom_to_st(projection_matrix);
        }
    }

    Shape(int n, Frame *f){
        N = n;
        frame = f;

        upper_6.reserve(6);
        lower_6.reserve(6);
        upper_N.reserve(N);
        lower_N.reserve(N);

        Matrix R = Matrix::RotationZ(60 * M_PI / 180);
        upper_6.emplace_back(0.8, 0, 0.7);
        for (int i = 1; i < 6; i++){
            Vector temp = upper_6[i - 1] * R;
            upper_6.emplace_back(temp.x, temp.y, temp.z);
        }

        for(int i = 0; i < 6; i++){
            lower_6.emplace_back((upper_6[i].x + upper_6[(i + 1) % 6].x) / 2,
                                 (upper_6[i].y + upper_6[(i + 1) % 6].y) / 2,
                                 (upper_6[i].z + upper_6[(i + 1) % 6].z) / 2 - 0.2);
        }

        R = Matrix::RotationZ(360 / N * M_PI / 180);
        upper_N.emplace_back(0.4, 0, 0.5);
        lower_N.emplace_back(0.4, 0, -0.5);
        for(int i = 1; i < N; i++){
            Vector temp = upper_N[i - 1] * R;
            upper_N.emplace_back(temp.x, temp.y, temp.z);
            lower_N.emplace_back(temp.x, temp.y, temp.z - 1);
        }

        center_u_6 = Vector(0, 0, 0.7);
        center_l_6 = Vector(0, 0, 0.5);
        end_point = Vector(0, 0, -0.9);

        n_lower_6 = lower_6;
        n_upper_6 = upper_6;
        n_lower_N = lower_N;
        n_upper_N = upper_N;

        n_end_point = end_point;
        n_center_u_6 = center_u_6;
        n_center_l_6 = center_l_6;
    }

    void rotate(Matrix &rotation_matrix){
        end_point = end_point * rotation_matrix;
        center_u_6 = center_u_6 * rotation_matrix;
        center_l_6 = center_l_6 * rotation_matrix;

        for(int i = 0; i < 6; i++){
            upper_6[i] = upper_6[i] * rotation_matrix;
            lower_6[i] = lower_6[i] * rotation_matrix;
        }

        for(int i = 0; i < N; i++){
            upper_N[i] = upper_N[i] * rotation_matrix;
            lower_N[i] = lower_N[i] * rotation_matrix;
        }
    }

    void draw_polygon(Matrix &projection_matrix){
        this->normalize_coordinates(projection_matrix);

        for(int i = 0; i < 6; i++){

            TriangleShader u_6_s (upper_6[i], upper_6[(i + 1) % 6], center_u_6);
            frame->Triangle<TriangleShader>(n_upper_6[i], n_upper_6[(i + 1) % 6], n_center_u_6, u_6_s);

            TriangleShaderPlain u_l_c1(upper_6[i], upper_6[(i + 1) % 6], lower_6[i], {0, 0, 100 + 155 / 6 * i, 255});
            frame->Triangle<TriangleShaderPlain>(n_upper_6[i], n_upper_6[(i + 1) % 6], n_lower_6[i],  u_l_c1);

            TriangleShaderPlain u_l_c2(lower_6[i], lower_6[(i + 1) % 6],  upper_6[(i + 1) % 6], COLOR{100 + 155 / 6 * i, 0, 100 + 155 / 6 * i, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_6[i], n_lower_6[(i + 1) % 6],  n_upper_6[(i + 1) % 6], u_l_c2);

            TriangleShaderPlain l_6_s (lower_6[i], lower_6[(i + 1) % 6], center_l_6, COLOR{0, 100 + 155 / 6 * i, 0, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_6[i], n_lower_6[(i + 1) % 6], n_center_l_6, l_6_s);
        }

        for(int i = 0; i < N; i++){
            TriangleShaderPlain cil_1(upper_N[i],  upper_N[(i + 1) % N], lower_N[i], COLOR{100 + 155 / N * i, 100 + 155 / N * i, 0, 255});
            frame->Triangle<TriangleShaderPlain>(n_upper_N[i],  n_upper_N[(i + 1) % N], n_lower_N[i], cil_1);

            TriangleShaderPlain cil_2(lower_N[i], lower_N[(i + 1) % N], upper_N[(i + 1) % N], COLOR{100 + 155 / N * i, 100 + 155 / N * i, 0, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_N[i], n_lower_N[(i + 1) % N], n_upper_N[(i + 1) % N], cil_2);

            TriangleShaderPlain end(lower_N[i], lower_N[(i + 1) % N], end_point, COLOR{0, 100 + 155 / N * i, 100 + 155 / N * i, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_N[i], n_lower_N[(i + 1) % N], n_end_point,  end);
        }
    }

    void draw_edges(Matrix &projection_matrix){
        this->normalize_coordinates(projection_matrix);

        COLOR color = {255, 255, 255, 255};

        for(int i = 0; i < 6; i++){
            frame->DrawLine(n_upper_6[i], n_upper_6[(i + 1) % 6], color);
            frame->DrawLine(n_upper_6[i], n_lower_6[i], color);
            frame->DrawLine(n_lower_6[i], n_upper_6[(i + 1) % 6], color);
            frame->DrawLine(n_lower_6[i], n_lower_6[(i + 1) % 6], color);
            frame->DrawLine(n_upper_6[i], n_center_u_6, color);
            frame->DrawLine(n_lower_6[i], n_center_l_6, color);
        }

        for(int i = 0; i < N; i++){
            frame->DrawLine(n_upper_N[i], n_upper_N[(i + 1) % N], color);
            frame->DrawLine(n_upper_N[i], n_lower_N[i], color);
            frame->DrawLine(n_lower_N[i], n_lower_N[(i + 1) % N], color);
            frame->DrawLine(n_lower_N[i], n_end_point, color);
        }
    }

    void draw_full(Matrix &projection_matrix){
        this->normalize_coordinates(projection_matrix);

        for(int i = 0; i < 6; i++){


            TriangleShader u_6_s (upper_6[i], upper_6[(i + 1) % 6], center_u_6);
            frame->Triangle<TriangleShader>(n_upper_6[i], n_upper_6[(i + 1) % 6], n_center_u_6, u_6_s);

            TriangleShaderPlain u_l_c1(upper_6[i], upper_6[(i + 1) % 6], lower_6[i], {0, 0, 100 + 155 / 6 * i, 255});
            frame->Triangle<TriangleShaderPlain>(n_upper_6[i], n_upper_6[(i + 1) % 6], n_lower_6[i],  u_l_c1);

            TriangleShaderPlain u_l_c2(lower_6[i], lower_6[(i + 1) % 6],  upper_6[(i + 1) % 6], COLOR{100 + 155 / 6 * i, 0, 100 + 155 / 6 * i, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_6[i], n_lower_6[(i + 1) % 6],  n_upper_6[(i + 1) % 6], u_l_c2);

            TriangleShaderPlain l_6_s (lower_6[i], lower_6[(i + 1) % 6], center_l_6, COLOR{0, 100 + 155 / 6 * i, 0, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_6[i], n_lower_6[(i + 1) % 6], n_center_l_6, l_6_s);
        }

        for(int i = 0; i < N; i++){
            TriangleShaderPlain cil_1(upper_N[i],  upper_N[(i + 1) % N], lower_N[i], COLOR{100 + 155 / N * i, 100 + 155 / N * i, 0, 255});
            frame->Triangle<TriangleShaderPlain>(n_upper_N[i],  n_upper_N[(i + 1) % N], n_lower_N[i], cil_1);

            TriangleShaderPlain cil_2(lower_N[i], lower_N[(i + 1) % N], upper_N[(i + 1) % N], COLOR{100 + 155 / N * i, 100 + 155 / N * i, 0, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_N[i], n_lower_N[(i + 1) % N], n_upper_N[(i + 1) % N], cil_2);

            TriangleShaderPlain end(lower_N[i], lower_N[(i + 1) % N], end_point, COLOR{0, 100 + 155 / N * i, 100 + 155 / N * i, 255});
            frame->Triangle<TriangleShaderPlain>(n_lower_N[i], n_lower_N[(i + 1) % N], n_end_point,  end);
        }

        COLOR color = {255, 255, 255, 255};

        for(int i = 0; i < 6; i++){
            frame->DrawLine(n_upper_6[i], n_upper_6[(i + 1) % 6], color);
            frame->DrawLine(n_upper_6[i], n_lower_6[i], color);
            frame->DrawLine(n_lower_6[i], n_upper_6[(i + 1) % 6], color);
            frame->DrawLine(n_lower_6[i], n_lower_6[(i + 1) % 6], color);
            frame->DrawLine(n_upper_6[i], n_center_u_6, color);
            frame->DrawLine(n_lower_6[i], n_center_l_6, color);
        }

        for(int i = 0; i < N; i++){
            frame->DrawLine(n_upper_N[i], n_upper_N[(i + 1) % N], color);
            frame->DrawLine(n_upper_N[i], n_lower_N[i], color);
            frame->DrawLine(n_lower_N[i], n_lower_N[(i + 1) % N], color);
            frame->DrawLine(n_lower_N[i], n_end_point, color);
        }
    }

};

#endif
