#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include "Vector.h"
#include "Matrix.h"
#include "Shape.h"

// Время от начала запуска программы
float time = 0;
int n_angles = 10;
float x_rot = 0;
float y_rot = 0;
float z_rot = 0;
float x_trsl = 0;
float y_trsl = 0;
float z_trsl = 0;


// Тип проекции (перспективная или ортографическая)
int projection_type = 0;
int draw_type = 0;

class Painter {
public:

    void Draw(Frame &frame) {
        int n = n_angles;

        Matrix projection_matrix = Matrix::Ortho(-2.0 * frame.width / frame.height, 2.0 * frame.width / frame.height,
                                                 -2.0,
                                                 2.0, 1.0f, 100.0f);

        Matrix front = Matrix::RotationX(-90 * M_PI / 180) *
                       Matrix::Translation(0, 0, -4) *
                       projection_matrix *
                       Matrix::Viewport(0, frame.height / 2, frame.width / 2, frame.height / 2);
        Shape front_shape(n, &frame);
        front_shape.draw_polygon(front);

        Matrix side = Matrix::RotationY(-90 * M_PI / 180) *
                      Matrix::Translation(0, 0, -4) *
                      projection_matrix *
                      Matrix::Viewport(frame.width / 2, frame.height / 2, frame.width / 2, frame.height / 2);
        Shape side_shape(n, &frame);
        side_shape.draw_polygon(side);

        Matrix top = Matrix::Translation(0, 0, -4) *
                     projection_matrix *
                     Matrix::Viewport(0, 0, frame.width / 2, frame.height / 2);
        Shape top_shape(n, &frame);
        top_shape.draw_polygon(top);

        Matrix transform = Matrix::RotationZ(z_rot * M_PI / 180) *
                           Matrix::RotationX(((-90 + x_rot) * M_PI / 180)) *
                           Matrix::RotationY(y_rot * M_PI / 180) *
                           Matrix::Translation(0, 0, -4) *
                           Matrix::Translation(x_trsl, y_trsl, z_trsl);

        // Выбор матрицы проектирования
        Matrix main_matrix;
        switch (projection_type % 2) {
            case 0:
                projection_matrix = Matrix::Frustum(-0.5 * frame.width / frame.height, 0.5 * frame.width / frame.height,
                                                    -0.5, 0.5, 1.0, 20);
                main_matrix =
                        transform *
                        projection_matrix *
                        Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
                break;
            case 1:
                projection_matrix = Matrix::Ortho(-2.0 * frame.width / frame.height, 2.0 * frame.width / frame.height,
                                                  -2.0,
                                                  2.0, 1.0f, 100.0f);
                main_matrix =
                        transform *
                        projection_matrix *
                        Matrix::Viewport(frame.width / 2, 0, frame.width / 2, frame.height / 2);
                break;
        }

        Shape main_shape(n, &frame);
        switch (draw_type % 3) {
            case 0:
                main_shape.draw_polygon(main_matrix);
                break;
            case 1:
                main_shape.draw_edges(main_matrix);
                break;
            case 2:
                main_shape.draw_full(main_matrix);
                break;
        }

    }
};

#endif // PAINTER_H