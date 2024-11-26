#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include "Vector.h"
#include "Matrix.h"
#include "shape.h"

// Время от начала запуска программы
float time = 0;
float Xrot = 0;
float Yrot = 0;
float Zrot = 0;

// Тип проекции (перспективная или ортографическая)
int projection_type = 0;
int draw_type = 0;
float t_z_value = -4;
int N = 10;

class Painter {
public:

    void Draw(Frame &frame) {

        float angle = time; // Угол поворота объекта

        Matrix projection_matrix = Matrix::Frustum(-0.5 * frame.width / frame.height, 0.5 * frame.width / frame.height,
                                                   -0.5, 0.5, 1.0, 20); // Матрица проектирования

        Matrix front = Matrix::RotationX(-90 * M_PI / 180) *
                       Matrix::Translation(0, 0, -4) *
                       projection_matrix *
                       Matrix::Viewport(0, frame.height / 2, frame.width / 2, frame.height / 2);

        Shape front_shape(10, &frame);
        front_shape.draw_polygon(front);

        Matrix side = Matrix::RotationY(-90 * M_PI / 180) *
                      Matrix::Translation(0, 0, -4) *
                      projection_matrix *
                      Matrix::Viewport(frame.width / 2, frame.height / 2, frame.width / 2, frame.height / 2);

        Shape side_shape(10, &frame);
        side_shape.draw_polygon(side);

        Matrix top = Matrix::Translation(0, 0, -4) *
                     projection_matrix *
                     Matrix::Viewport(0, 0, frame.width / 2, frame.height / 2);

        Shape top_shape(10, &frame);
        top_shape.draw_polygon(top);


        Matrix main_matrix;

        if (projection_type % 5 == 0) {
            main_matrix = Matrix::RotationX(0 / 180) *
                          Matrix::Translation(0, 0, -4) *
                          projection_matrix *
                          Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
        }

        // Выбор матрицы проектирования
        if (projection_type % 5 == 1) {
            projection_matrix = Matrix::Ortho(-2.0 * frame.width / frame.height, 2.0 * frame.width / frame.height, -2.0,
                                              2.0, 1.0f, 100.0f);
            main_matrix = Matrix::RotationX(-90 * M_PI / 180) *
                          Matrix::Translation(0, 0, -4) *
                          projection_matrix *
                          Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
        }

        if (projection_type % 5 == 2) {
            main_matrix =
                    Matrix::RotationZ(-22.5 * M_PI / 180) *
                    Matrix::RotationX(-67.5 * M_PI / 180) *
                    Matrix::Translation(0, 0, -4) *
                    projection_matrix *
                    Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
        }

        if (projection_type % 5 == 3) {
            main_matrix =
                    Matrix::RotationZ(-45 * M_PI / 180) *
                    Matrix::RotationX(-45 * M_PI / 180) *
                    Matrix::Translation(0, 0, -4) *
                    projection_matrix *
                    Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
        }

        if (projection_type % 5 == 4) {
            main_matrix =
                    Matrix::RotationZ(Zrot) *
                    Matrix::RotationX(Xrot + (-90 * M_PI / 180)) *
                    Matrix::RotationY(Yrot) *
                    Matrix::Translation(0, 0, t_z_value) *
                    projection_matrix *
                    Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
        }

        Shape main_shape(N, &frame);
        if(draw_type % 3 == 0){
            main_shape.draw_polygon(main_matrix);
        }
        if(draw_type % 3 == 1){
            main_shape.draw_edges(main_matrix);
        }
        if(draw_type % 3 == 2){
            main_shape.draw_full(main_matrix);
        }

//        main_matrix =
//                Matrix::RotationZ(Zrot) *
//                Matrix::RotationX(Xrot + (0 / 180)) *
//                Matrix::RotationY(Yrot) *
//                Matrix::Translation(0, 0, t_z_value) *
//                projection_matrix *
//                Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
//
//        Vector v1 = {1, 1, 1};
//        Vector v2 = {1, -1, 1};
//        Vector v3 = {-1, 1, 1};
//        Vector v4 = {-1, -1, 1};
//
//        TriangleShader TrShader1 (v1, v2, v3);
//        TriangleShader TrShader2 (v2, v3, v4);
//
//        v1.hom_to_st(main_matrix);
//        v2.hom_to_st(main_matrix);
//        v3.hom_to_st(main_matrix);
//        v4.hom_to_st(main_matrix);

//        frame.Triangle(v1, v2, v3, TrShader1);
//        frame.Triangle(v2, v3, v4, TrShader2);

    }
};

#endif // PAINTER_H