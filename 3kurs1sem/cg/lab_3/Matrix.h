//
// Created by dmitr on 13.10.2024.
//

#ifndef LAB_3_MATRIX_H
#define LAB_3_MATRIX_H

#include <string>
#include <vector>
#include <cmath>


class Vector;

class Matrix {
public:
    float data[9]{};
    float *matrix[3]{};

    Matrix(std::initializer_list<float> v) {
        memcpy(data, v.begin(), sizeof(float) * 9);
        matrix[0] = data;
        matrix[1] = data + 3;
        matrix[2] = data + 6;
    }

    explicit Matrix(std::vector<float> v) {
        memcpy(data, &v[0], sizeof(float) * 9);
        matrix[0] = data;
        matrix[1] = data + 3;
        matrix[2] = data + 6;
    }

    static Matrix rotation(float angle) {
        const float PI = 3.14159274;
        angle = angle / 180 * PI;
        float cosA = cos(angle);
        float sinA = sin(angle);
        return Matrix{cosA, sinA, 0,
                      -sinA, cosA, 0,
                      0, 0, 1};
    }

    static Matrix scale(float scale) {
        return {scale, 0, 0,
                0, scale, 0,
                0, 0, 1};
    }

    static Matrix transform(float x, float y) {
        return {1, 0, x,
                0, 1, y,
                0, 0, 1};
    }

    static Matrix mirror_horizontal() {
        return {-1, 0, 0,
                0, 1, 0,
                0, 0, 1};
    }

    static Matrix mirror_vertical() {
        return {1, 0, 0,
                0, -1, 0,
                0, 0, 1};
    }

    static Matrix WorldToScreen(float X1, float Y1, float X2, float Y2, float x1, float
    y1, float x2, float y2) {
        float px = (X2 - X1) / (x2 - x1), py = (Y2 - Y1) / (y2 - y1);
        return {
                px, 0, 0,
                0, -py, 0,
                X1 - x1 * px, Y2 + y1 * py, 1};
    }


    Matrix operator*(Matrix another) {
        float dataNew[9] = {};
        float *matrixNew[3]; // todo simplify
        matrixNew[0] = dataNew;
        matrixNew[1] = dataNew + 3;
        matrixNew[2] = dataNew + 6;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                matrixNew[i][j] = 0;

                for (int k = 0; k < 3; k++) {
                    matrixNew[i][j] += this->matrix[i][k] * another.matrix[k][j];
                }
            }
        }

        return Matrix(std::vector<float>(dataNew, dataNew + 9));
    }

//    Vector operator*(Vector &vec) {
//        return Vector({
//                              vec.vector[0] * this->matrix[0][0] + vec.vector[1] * this->matrix[0][1] +
//                              vec.vector[2] * this->matrix[0][2],
//                              vec.vector[0] * this->matrix[1][0] + vec.vector[1] * this->matrix[1][1] +
//                              vec.vector[2] * this->matrix[1][2],
//                              vec.vector[0] * this->matrix[2][0] + vec.vector[1] * this->matrix[2][1] +
//                              vec.vector[2] * this->matrix[2][2]});
//    }

//    friend class Vector;
};

class Vector {
public:
    float vector[3];

    Vector(std::initializer_list<float> v) {
        memcpy(vector, v.begin(), sizeof(float) * 3);
    }

    Vector(std::vector<float> v) {
        memcpy(vector, &v[0], sizeof(float) * 3);
    }

    Vector(float x, float y) {
        this->vector[0] = x;
        this->vector[1] = y;
        this->vector[2] = 1;
    }

    Vector() {
        this->vector[0] = 0;
        this->vector[1] = 0;
        this->vector[2] = 1;
    }

public:
    float x() {
        return this->vector[0];
    }

    float y() {
        return this->vector[1];
    }

//    Vector operator*(const Matrix &A) {
//        Vector E;
//        vector[0] = x() * A.matrix[0][0]
//                    + y() * A.matrix[1][0]
//                    + A.matrix[2][0];
//        vector[1] = x() * A.matrix[0][1]
//                    + y() * A.matrix[1][1]
//                    + A.matrix[2][1];
//
//        float h = x() * A.matrix[0][2] + y() * A.matrix[1][2] + A.matrix[2][2];
//        vector[0] /= h;
//        vector[1] /= h;
//
//        return E;
//    }

    Vector operator * (const Matrix A) {
        Vector result;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                result.vector[i] += A.matrix[i][j] * vector[j];
            }
        }
        return result;
    }
//    friend class Matrix;
};


#endif //LAB_3_MATRIX_H
