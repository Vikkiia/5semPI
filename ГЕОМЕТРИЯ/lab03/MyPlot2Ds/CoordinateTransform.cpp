#include"pch.h"
#include "CoordinateTransform.h"

// Реализация функции
CMatrix SpaceToWindow(CRectD& rs, CRect& rw)
{
    CMatrix K(3, 3);

    double Sx = (double)rw.Width() / rs.Width();
    double Sy = (double)rw.Height() / rs.Height();

    // ИСПРАВЛЕННЫЕ ФОРМУЛЫ:
    double Tx = rw.left - rs.left * Sx;
    double Ty = rw.bottom - rs.bottom * Sy;  // Используем bottom вместо top

    K(0, 0) = Sx;   K(0, 1) = 0;     K(0, 2) = Tx;
    K(1, 0) = 0;     K(1, 1) = -Sy;   K(1, 2) = Ty;  // Отрицательный Sy
    K(2, 0) = 0;     K(2, 1) = 0;     K(2, 2) = 1;

    return K;
}