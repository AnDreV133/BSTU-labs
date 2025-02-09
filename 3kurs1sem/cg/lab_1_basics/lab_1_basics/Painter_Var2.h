//
// Created by dmitr on 04.09.2024.
//

#ifndef LAB_1_BASICS_PAINTER_VAR2_H
#define LAB_1_BASICS_PAINTER_VAR2_H

#include "Frame.h"

class Painter_Var2 {
public:
    void draw(Frame &frame) {
        Assets::drawChessBg(frame);

        int W = frame.width, H = frame.height;

        float scaledSize = 7.0f / 8 * ((W < H) ? W - 1 : H - 1) / sqrt(2) - 10;
        if (scaledSize < 1) return;

        float angle = globalAngle;
        PixelCoord center = {W / 2, H / 2};

        frame.drawSector(
                center,
                -3.14,
                -3.14/1.5,
                scaledSize,
                staticPartColor
        );

//        frame.drawCircle(
//                center,
//                scaledSize,
//                staticPartColor
//        );
//
//        drawEqualityTriangle(
//                frame,
//                {frame.width / 2, frame.height / 2},
//                scaledSize,
//                0,
//                staticPartColor
//        );
//
//        frame.drawCircle(
//                center,
//                scaledSize / 2,
//                staticPartColor
//        );
//
//        drawEqualityTriangle(
//                frame,
//                {frame.width / 2, frame.height / 2},
//                scaledSize / 2,
//                -angle,
//                movablePartColor
//        );
//
//        frame.drawCircle(
//                center,
//                scaledSize / 4,
//                staticPartColor
//        );
    }

private:
    Color movablePartColor = {20, 200, 200};
    Color staticPartColor = {0, 0, 0};

    void drawEqualityTriangle(
            Frame &frame,
            PixelCoord center,
            int heightFromCenter,
            float angle,
            Color color
    ) {
        float halfSide = heightFromCenter * cos(M_PI / 6);
        PixelCoord vertices[] = {
                {
                        center.x,
                        center.y - heightFromCenter
                },
                {
                        int(center.x - halfSide),
                        center.y + heightFromCenter / 2
                },
                {
                        int(center.x + halfSide + 1),
                        center.y + heightFromCenter / 2
                }
        };

        for (PixelCoord &p: vertices) {
            PixelCoord prevP = p;
            p.x = (prevP.x - center.x) * cos(angle)
                  - (prevP.y - center.y) * sin(angle)
                  + center.x;
            p.y = (prevP.x - center.x) * sin(angle)
                  + (prevP.y - center.y) * cos(angle)
                  + center.y;
        }

        for (int i = 0; i < 3; i++) {
            int i2 = (i + 1) % 3;
            frame.drawLine(
                    {
                            int(vertices[i].x + 0.5f),
                            int(vertices[i].y + 0.5f)
                    },
                    {
                            int(vertices[i2].x + 0.5f),
                            int(vertices[i2].y + 0.5f)
                    },
                    color
            );
        }
    }
};

#endif //LAB_1_BASICS_PAINTER_VAR2_H


//void drawEqualityTriangle(
//        Frame &frame,
//        PixelCoord center,
//        int heightFromCenter,
//        float angle,
//        Color color
//) {
//    PixelCoord vertices[] = {
//            {
//                    center.x,
//                    center.y + heightFromCenter
//            },
//            {
//                    int(center.x - heightFromCenter * cos(M_PI / 6)),
//                    center.y - heightFromCenter / 2
//            },
//            {
//                    int(center.x + heightFromCenter * cos(M_PI / 6)),
//                    center.y - heightFromCenter / 2
//            }
//    };
//
//    for (PixelCoord &p: vertices) {
//        p.x = (p.x - center.x) * cos(angle)
//              - (p.y - center.y) * sin(angle)
//              + center.x;
//        p.y = (p.x - center.x) * sin(angle)
//              - (p.y - center.y) * cos(angle)
//              + center.y;
//    }
//
//    for (int i = 0; i < 3; i++) {
//        int i2 = (i + 1) % 3;
//        frame.drawLine(
//                {
//                        int(vertices[i].x + 0.5f),
//                        int(vertices[i].y + 0.5f)
//                },
//                {
//                        int(vertices[i2].x + 0.5f),
//                        int(vertices[i2].y + 0.5f)
//                },
//                color
//        );
//    }
//}