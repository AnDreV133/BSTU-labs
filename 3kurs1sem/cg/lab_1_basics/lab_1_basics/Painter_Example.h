#ifndef PAINTER_H
#define PAINTER_H

#include "Frame.h"
#include "Assets.h"

class Painter_Example {
public:
    void draw(Frame &frame) {
        Assets::drawChessBg(frame);

        int W = frame.width, H = frame.height;
        // ������ ������� ������ ������ (7 / 8), ����� �� �� ������� ������ ������
        float a = 7.0f / 8 * ((W < H) ? W - 1 : H - 1) / sqrt(2);
        if (a < 1) return; // ���� ���� ����� ���������, �� ������ �� ������
        float angle = globalAngle; // ���� ��������
        a = a / 2;

        // �������������� �������� ���������� ������ � ������ ��������
        struct {
            float x;
            float y;
        } C = {W / 2.f, H / 2.f}, A[4] = {{C.x + a, C.y + a},
                                          {C.x + a, C.y - a},
                                          {C.x - a, C.y - a},
                                          {C.x - a, C.y + a}};


        // ������������ ��� ������� �������� ������ ����� C �� ���� angle
        for (auto &ai: A) {
            float xi = ai.x, yi = ai.y;
            ai.x = (xi - C.x) * cos(angle) - (yi - C.y) * sin(angle) + C.x;
            ai.y = (xi - C.x) * sin(angle) + (yi - C.y) * cos(angle) + C.y;
        }

        // ������ ������� ��������
        for (int i = 0; i < 4; i++) {
            int i2 = (i + 1) % 4;
            frame.drawLine( // ��������� ����� 0.5f, ����� ������������ ����� ��������� ����������� ��� �������������� � ������ ����
                    {int(A[i].x + 0.5f), int(A[i].y + 0.5f)},
                    {int(A[i2].x + 0.5f), int(A[i2].y + 0.5f)},
                    Color(200, 30, 45)
            );
        }

        // ������ ��������� ����������
        frame.drawCircle(
                {(int) C.x, (int) C.y},
                int(a * sqrt(2) + 0.5f),
                Color(100, 100, 250)
        );

        // ������ �������, �� ������� ������� ������������
        if (globalClickedPixel.x >= 0 && globalClickedPixel.x < W &&
            globalClickedPixel.y >= 0 && globalClickedPixel.y < H)
            frame.setPixel(
                    {globalClickedPixel.x, globalClickedPixel.y},
                    {34, 175, 60}
            ); // ������� ������� �����
    }
};

#endif // PAINTER_H