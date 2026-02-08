#pragma once
#include "Matrix.h"
#include "PlotStructures.h"

class CPlot2D
{
private:
    CMatrix X;          // Аргумент
    CMatrix Y;          // Функция
    CMatrix K;          // Матрица пересчета координат
    CRect RW;           // Прямоугольник в окне
    CRectD RS;          // Прямоугольник области в МСК
    CMyPen PenLine;     // Перо для линий
    CMyPen PenAxis;     // Перо для осей

public:
    CPlot2D() { K.RedimMatrix(3, 3); }

    double CalculateNiceStep(double range);
    void DrawAxisLabels(CDC& dc);
    void SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX, int);
    void SetWindowRect(CRect& RWX);
    void GetWindowCoords(double xs, double ys, int& xw, int& yw);
    void SetPenLine(CMyPen& PLine);
    void SetPenAxis(CMyPen& PAxis);
    void Draw(CDC& dc, int DrawAxes = 1, int DrawRect = 1);

private:
    void CalculateWorldRect();
};