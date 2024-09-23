#ifndef PAINTER_H
#define PAINTER_H

#include <exception>
#include <iostream>
#include "Frame.h"
#include "Assets.h"
#include "global_values.h"
#include "interpolator/SectorInterpolator.h"
#include "interpolator/BarycentricInterpolator.h"
#include "interpolator/RadialInterpolator.h"
#include "interpolator/OneColorInterpolator.h"

class Painter {
    enum DrawMode {
        SECTOR = 0,
        RADIAL = 1,
        BARYCENTRIC = 2,
//        FILL = 3,
    };

    DrawMode spinDrawMode(DrawMode mode) {
        switch (mode) {
            case SECTOR:
                return RADIAL;
            case RADIAL:
                return BARYCENTRIC;
            case BARYCENTRIC:
//                return FILL;
//            case FILL:
                return SECTOR;
        }
        return SECTOR;
    }

    BaseInterpolator &getInterpolator(
            int indexMode,
            int action,
            OneColorInterpolator &oneColorInterpolator,
            SectorInterpolator &sectorInterpolator,
            RadialInterpolator &radialInterpolator,
            BarycentricInterpolator &barycentricInterpolator
    ) {
        if (action == CHANGE) {
            modes[indexMode] = spinDrawMode(modes[indexMode]);
            highlighted[indexMode] = false;
        } else if (action == HIGHLIGHT) {
            for (auto &h: highlighted) h = false;
            highlighted[indexMode] = true;
        }

        if (highlighted[indexMode]) {
            return oneColorInterpolator;
        }

        if (modes[indexMode] == SECTOR) {
            return sectorInterpolator;
        } else if (modes[indexMode] == RADIAL) {
            return radialInterpolator;
        } else if (modes[indexMode] == BARYCENTRIC) {
            return barycentricInterpolator;
        } else {
            std::cerr << "Unknown draw mode\n";
            throw std::exception();
        }
    }

    OneColorInterpolator highlightColor{{160, 160, 0, 255}};

    DrawMode modes[5] = {RADIAL, BARYCENTRIC, SECTOR, BARYCENTRIC, SECTOR};
    bool highlighted[5] = {};
    int actionOnFigure[5] = {NOTHING, NOTHING, NOTHING, NOTHING, NOTHING};

public:
    enum ACTION {
        NOTHING = 0,
        HIGHLIGHT = 1,
        CHANGE = 2
    };

    void Draw(Frame &frame) {
        int W = frame.width, H = frame.height;

        // Размер рисунка возьмём меньше (7 / 8), чтобы он не касался границ экрана
        float a = 7.0 / 8.0 * ((W < H) ? W - 1 : H - 1) / 2;

        // Инициализируем исходные координаты центра и вершин квадрата
        struct {
            float x;
            float y;
        }
                C = {W / 2 + 0.5f, H / 2 + 0.5f},
                A_st[3] = {
                {C.x, C.y - a},
                {static_cast<float>(+a * sin(2. * M_PI / 3) + C.x),
                      static_cast<float>( -a * cos(2. * M_PI / 3) + C.y)},
                {static_cast<float>(+a * sin(-2. * M_PI / 3) + C.x),
                      static_cast<float>(-a * cos(-2. * M_PI / 3) + C.y)}},
                B_mv[3] = {
                {A_st[0].x, A_st[0].y},
                {A_st[1].x, A_st[1].y},
                {A_st[2].x, A_st[2].y}
        };

        // Поворачиваем все вершины треугольника вокруг точки C на угол global_angle
        for (int i = 0; i < 3; i++) {
            double xi = B_mv[i].x, yi = B_mv[i].y;
            B_mv[i].x = (xi - C.x) * cos(global_angle) - (yi - C.y) * sin(global_angle) + C.x;
            B_mv[i].y = (xi - C.x) * sin(global_angle) + (yi - C.y) * cos(global_angle) + C.y;
        }


        Assets::drawChessBg(frame);

        SectorInterpolator sectorInterpolator(C.x, C.y);
        RadialInterpolator radialInterpolator1(
                C.x, C.y,
                C.x, C.y,
                {255, 20, 20, global_alpha},
                {50, 255, 50, global_alpha},
                0
        );
        BarycentricInterpolator barycentricInterpolator1(
                A_st[0].x, A_st[0].y,
                A_st[1].x, A_st[1].y,
                A_st[2].x, A_st[2].y,
                {200, 40, 40, global_alpha},
                {50, 200, 50, global_alpha},
                {50, 50, 200, global_alpha}
        );
        RadialInterpolator radialInterpolator2(
                C.x, C.y,
                C.x, C.y,
                {255, 255, 20, global_alpha},
                {50, 255, 255, global_alpha},
                0
        );
        BarycentricInterpolator barycentricInterpolator2(
                A_st[0].x, A_st[0].y,
                A_st[1].x, A_st[1].y,
                A_st[2].x, A_st[2].y,
                {200, 200, 200, global_alpha},
                {50, 50, 50, global_alpha},
                {0, 0, 200, global_alpha}
        );
        RadialInterpolator radialInterpolator3(
                C.x, C.y,
                C.x, C.y,
                {255, 255, 255, global_alpha},
                {50, 50, 50, global_alpha},
                global_angle
        );
        BarycentricInterpolator barycentricInterpolator3(
                A_st[0].x, A_st[0].y,
                A_st[1].x, A_st[1].y,
                A_st[2].x, A_st[2].y,
                {200, 0, 0, global_alpha},
                {0, 200, 0, global_alpha},
                {0, 0, 200, global_alpha}
        );
        RadialInterpolator radialInterpolator4(
                C.x, C.y,
                C.x, C.y,
                {255, 20, 20, global_alpha},
                {50, 255, 50, global_alpha},
                0
        );
        BarycentricInterpolator barycentricInterpolator4(
                (B_mv[0].x + C.x) / 2, (B_mv[0].y + C.y) / 2,
                (B_mv[1].x + C.x) / 2, (B_mv[1].y + C.y) / 2,
                (B_mv[2].x + C.x) / 2, (B_mv[2].y + C.y) / 2,
                {200, 40, 40, global_alpha},
                {50, 200, 50, global_alpha},
                {50, 50, 200, global_alpha}
        );

        if (global_action != NOTHING
            && global_clicked_pixel.x != -1
            && global_clicked_pixel.y != -1
                ) {
            actionOnFigure[0] = frame.IsPointInCircle(
                    C.x, C.y, a,
                    global_clicked_pixel.x,
                    global_clicked_pixel.y
            ) ? global_action : NOTHING;
            actionOnFigure[1] = frame.IsPointInTriangle(
                    A_st[0].x, A_st[0].y,
                    A_st[1].x, A_st[1].y,
                    A_st[2].x, A_st[2].y,
                    global_clicked_pixel.x, global_clicked_pixel.y
            ) ? global_action : NOTHING;
            actionOnFigure[2] = frame.IsPointInCircle(
                    C.x, C.y, a / 2,
                    global_clicked_pixel.x,
                    global_clicked_pixel.y
            ) ? global_action : NOTHING;
            actionOnFigure[3] = frame.IsPointInTriangle(
                    (B_mv[0].x + C.x) / 2, (B_mv[0].y + C.y) / 2,
                    (B_mv[1].x + C.x) / 2, (B_mv[1].y + C.y) / 2,
                    (B_mv[2].x + C.x) / 2, (B_mv[2].y + C.y) / 2,
                    global_clicked_pixel.x, global_clicked_pixel.y
            ) ? global_action : NOTHING;
            actionOnFigure[4] = frame.IsPointInCircle(
                    C.x, C.y, a / 4,
                    global_clicked_pixel.x,
                    global_clicked_pixel.y
            ) ? global_action : NOTHING;
        }

        bool isFirstFigureChange = false;
        for (int i = 4; i >= 0; i--) {
            if (!isFirstFigureChange && actionOnFigure[i] != NOTHING) {
                isFirstFigureChange = true;
            } else {
                actionOnFigure[i] = NOTHING;
            }
        }

        // Рисуем внешний круг
        frame.Circle(
                C.x,
                C.y,
                a,
                getInterpolator(
                        0,
                        actionOnFigure[0],
                        highlightColor,
                        sectorInterpolator,
                        radialInterpolator1,
                        barycentricInterpolator1
                )
        );

        // Рисуем статичный треугольник
        frame.Triangle(
                A_st[0].x, A_st[0].y,
                A_st[1].x, A_st[1].y,
                A_st[2].x, A_st[2].y,
                getInterpolator(
                        1,
                        actionOnFigure[1],
                        highlightColor,
                        sectorInterpolator,
                        radialInterpolator2,
                        barycentricInterpolator2
                )
        );

        // Рисуем средний круг
        frame.Circle(
                C.x, C.y,
                a / 2,
                getInterpolator(
                        2,
                        actionOnFigure[2],
                        highlightColor,
                        sectorInterpolator,
                        radialInterpolator3,
                        barycentricInterpolator3
                )
        );

        // Рисуем вращающийся внутренний треугольник
        frame.Triangle(
                (B_mv[0].x + C.x) / 2, (B_mv[0].y + C.y) / 2,
                (B_mv[1].x + C.x) / 2, (B_mv[1].y + C.y) / 2,
                (B_mv[2].x + C.x) / 2, (B_mv[2].y + C.y) / 2,
                getInterpolator(
                        3,
                        actionOnFigure[3],
                        highlightColor,
                        sectorInterpolator,
                        radialInterpolator4,
                        barycentricInterpolator4
                )
        );
        frame.Circle(
                C.x, C.y,
                a / 4,
                {255, 0, 0, global_alpha}
        );

        global_clicked_pixel = {-1, -1};
        for (auto &action: actionOnFigure) {
            action = NOTHING;
        }
    }
};

#endif // PAINTER_H


//        std::cout << modes[0] << ' ' << modes[1] << ' ' << modes[2] << ' ' << modes[3] << ' ' << modes[4] << ' '
//                  << std::endl;

//        SectorInterpolator interpolator(C.x, C.y);
//        frame.Triangle(0, 0, W - 1, 0, 0, H - 1, interpolator);
//        frame.Triangle(W - 1, 0, 0, H - 1, W - 1, H - 1, interpolator);

//        if(interpolator != nullptr) delete interpolator;