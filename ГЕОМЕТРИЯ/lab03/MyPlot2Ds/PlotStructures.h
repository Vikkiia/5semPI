#pragma once
#include <windows.h>

// Прямоугольник с координатами double
struct CRectD
{
    double left, top, right, bottom;

    CRectD() : left(0), top(0), right(0), bottom(0) {}
    CRectD(double l, double t, double r, double b) : left(l), top(t), right(r), bottom(b) {}

    double Width() const { return right - left; }
    double Height() const { return bottom - top; }
};

// Структура для пера
struct CMyPen
{
    int PenStyle;       // Стиль пера
    int PenWidth;       // Толщина пера 
    COLORREF PenColor;  // Цвет пера 

    CMyPen() {
        PenStyle = PS_SOLID;
        PenWidth = 1;
        PenColor = RGB(0, 0, 0);
    }

    void Set(int PS, int PW, COLORREF PC) {
        PenStyle = PS;
        PenWidth = PW;
        PenColor = PC;
    }
};