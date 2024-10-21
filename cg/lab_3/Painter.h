#ifndef PAINTER_H
#define PAINTER_H

#include <exception>
#include <iostream>
#include "Frame.h"
#include "Assets.h"
#include "global_values.h"
#include "Matrix.h"
#include "interpolator/BarycentricInterpolator.h"
#include "SawShader.h"

class Painter {
    COLOR sawColor = {150, 150, 150};
    COLOR sawCenterColor = {100, 100, 100};

    void drawSaw_(
            Frame &frame,
            Matrix &WS,
            float xC, float yC,
            float radius,
            float maxCoord,
            float globalAngle
    ) {
        globalAngle = globalAngle * 180.0f / M_PI * 2;

        float maxAngle = 360.0f;
        float rotateAngle = 12.0f;
        float angle = 0.0f;
        while (angle < maxAngle) {
            // точки дл€ рисовани€ зубьев
            Vector p3 = {0, maxCoord, 1.0f};
            p3 = p3 * Matrix::rotation(10 + globalAngle + angle) * WS;

            Vector p2 = {0, maxCoord, 1.0f};
            p2 = p2 * Matrix::rotation(10 + globalAngle + angle) * WS;

            Vector p1 = {0, 0, 1.0f};
            p1 = p1 * WS;

//            // рисуем зубь€
//            frame.Triangle(
//                    p5.x() + xC, p5.y() + yC,
//                    p4.x() + xC, p4.y() + yC,
//                    p3.x() + xC, p3.y() + yC,
//                    sawColor
//            );

            TriangleShader shader(
                    p1.x() + xC, p1.y() + yC,
                    p2.x() + xC, p2.y() + yC,
                    p3.x() + xC, p3.y() + yC,
                    sawColor
            );
            frame.Triangle(
                    p3.x() + xC, p3.y() + yC,
                    p2.x() + xC, p2.y() + yC,
                    p1.x() + xC, p1.y() + yC,
                    shader
            );

            // отклон€ем угол дл€ рисовани€ следующего зуба
            angle += rotateAngle;
        }
    }

    void drawSaw(
            Frame &frame,
            Matrix &WS,
            float xC, float yC,
            float radius,
            float maxCoord,
            float globalAngle
    ) {
        globalAngle = globalAngle * 180.0f / M_PI * 2;

        float maxAngle = 360.0f;
        float rotateAngle = 12.0f;
        float angle = 0.0f;
        while (angle < maxAngle) {
            // точки дл€ рисовани€ зубьев
            Vector p5 = {0.0f, maxCoord, 1.0f};
            p5 = p5 * Matrix::rotation(10 + globalAngle + angle) * WS;

            Vector p4 = {0.0f, maxCoord - 1, 1.0f};
            p4 = p4 * Matrix::rotation(358 + globalAngle + angle) * WS;

            Vector p3 = {0.0f, maxCoord - 2, 1.0f};
            p3 = p3 * Matrix::rotation(3 + globalAngle + angle) * WS;

            Vector p2 = {0.0f, maxCoord - 3, 1.0f};
            p2 = p2 * Matrix::rotation(10 + globalAngle + angle) * WS;

            Vector p1 = {0.0f, maxCoord - 3, 1.0f};
            p1 = p1 * Matrix::rotation(350 + globalAngle + angle) * WS;

            // рисуем зубь€
            frame.Triangle(
                    p5.x() + xC, p5.y() + yC,
                    p4.x() + xC, p4.y() + yC,
                    p3.x() + xC, p3.y() + yC,
                    sawColor
            );
            frame.Triangle(
                    p4.x() + xC, p4.y() + yC,
                    p2.x() + xC, p2.y() + yC,
                    p1.x() + xC, p1.y() + yC,
                    sawColor
            );

            // отклон€ем угол дл€ рисовани€ следующего зуба
            angle += rotateAngle;
        }
        // рисуем внешний круг
        frame.Circle(
                xC, yC,
                radius * 0.84,
                sawColor
        );
        // рисуем внутренний круг
        frame.Circle(
                xC, yC,
                radius * 0.3,
                sawCenterColor
        );
    }

    int currH = 0;
    float currAngle = 0;

    void drawStars(
            Frame &frame,
            Matrix WS,
            float xC, float yC,
            float maxCoord,
            float globalAngle
    ) {
        globalAngle = globalAngle * 180.0f / M_PI;

        // получаем переменную дл€ просчЄта удалени€ искр
        int h = (int) globalAngle % (int) (maxCoord * 3);
        // условие дл€ повторного воспроизведени€
        if (currH > h) currAngle = rand() % 40 - 10;
        currH = h;

        // измен€ем положение координатной плоскости
        Matrix toss =
                Matrix::rotation(currAngle)
                * Matrix::transform(0, h)
                * Matrix::rotation(globalAngle)
                * Matrix::scale(h * 0.025f);

        // примен€ем матрицу на точки дл€ построени€ звезды
        Vector p1 = {0.0f, maxCoord * 3 / 4, 1.0f};
        p1 = p1 * toss * WS;
        Vector p2 = {0.0f, maxCoord * 1 / 4 - 1.0f, 1.0f};
        p2 = p2 * toss * WS;
        Vector p3 = {0.0f, -maxCoord * 1 / 4 - 1.4f, 1.0f};
        p3 = p3 * toss * WS;
        Vector p4 = {maxCoord / 2, -maxCoord * 3 / 4, 1.0f};
        Vector p4_ = {-maxCoord / 2, -maxCoord * 3 / 4, 1.0f};
        p4 = p4 * toss * WS;
        p4_ = p4_ * toss * WS;
        Vector p5 = {maxCoord * 3 / 4, maxCoord * 1 / 4 - 1.0f, 1.0f};
        Vector p5_ = {-maxCoord * 3 / 4, maxCoord * 1 / 4 - 1.0f, 1.0f};
        p5 = p5 * toss * WS;
        p5_ = p5_ * toss * WS;

        // интерополируем цвета
        BarycentricInterpolator interpolator(
                xC, yC + 10,
                0, 0,
                frame.width - 1, 0,
                {255, 255, 0},
                {180, 180, 180},
                {180, 180, 180}
        );
        // получаем цвет из интерпол€тора
        COLOR starColor = interpolator.color(p3.x() + xC, p3.y() + yC);

        // рисуем звезду
        frame.Triangle(
                p5.x() + xC, p5.y() + yC,
                p2.x() + xC, p2.y() + yC,
                p3.x() + xC, p3.y() + yC,
                starColor
        );
        frame.Triangle(
                p4.x() + xC, p4.y() + yC,
                p3.x() + xC, p3.y() + yC,
                p1.x() + xC, p1.y() + yC,
                starColor
        );
        frame.Triangle(
                p5_.x() + xC, p5_.y() + yC,
                p2.x() + xC, p2.y() + yC,
                p3.x() + xC, p3.y() + yC,
                starColor
        );
        frame.Triangle(
                p4_.x() + xC, p4_.y() + yC,
                p3.x() + xC, p3.y() + yC,
                p1.x() + xC, p1.y() + yC,
                starColor
        );
    }

public:
    void Draw(Frame &frame) {
        int W = frame.width, H = frame.height;

        // –азмер рисунка возьмЄм меньше (7 / 8), чтобы он не касалс€ границ экрана
        float a = 7.0 / 8.0 * ((W < H) ? W - 1 : H - 1) / 2;

        Assets::drawChessBg(frame);

        Matrix WS = Matrix::WorldToScreen(
                W / 2 - a / 2, H / 2 - a / 2,
                W / 2 + a / 2, H / 2 + a / 2,
                -15, -15, 15, 15
        );

        float globalAngle = global_angle;

        TriangleShader shader(
                W, H,
                0, 0,
                W,-H,
                sawColor
        );
        frame.Triangle(
                W, H,
                0, 0,
                W,-H,
                shader
        );
        frame.Triangle(
                W, H,
                0, 0,
                -W,H,
                shader
        );

        drawSaw_(
                frame,
                WS,
                W / 2, H / 2,
                a / 2,
                15,
                -globalAngle
//                0
        );

        drawStars(
                frame,
                WS,
                W * 5 / 8.2, H / 2,
                15,
                globalAngle
        );
    }
};

#endif // PAINTER_H


//        std::cout << modes[0] << ' ' << modes[1] << ' ' << modes[2] << ' ' << modes[3] << ' ' << modes[4] << ' '
//                  << std::endl;

//        SectorInterpolator interpolator(C.x, C.y);
//        frame.Triangle(0, 0, W - 1, 0, 0, H - 1, interpolator);
//        frame.Triangle(W - 1, 0, 0, H - 1, W - 1, H - 1, interpolator);

//        if(interpolator != nullptr) delete interpolator;