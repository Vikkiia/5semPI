#include "pch.h"
#include "Plot2D.h"
#include "CoordinateTransform.h"

void CPlot2D::SetParams(CMatrix& XX, CMatrix& YY, CRect& RWX, int n)
{
    X = XX;
    Y = YY;
    RW = RWX;
    CalculateWorldRect();
    K = SpaceToWindow(RS, RW);
}

void CPlot2D::SetWindowRect(CRect& RWX)
{
    RW = RWX;
    K = SpaceToWindow(RS, RW);
}

//void CPlot2D::GetWindowCoords(double xs, double ys, int& xw, int& yw)
//{
//    if (K.GetRowCount() == 3) {
//        xw = (int)(K(0, 0) * xs + K(0, 2));
//        yw = (int)(K(1, 0) * xs + K(1, 1) * ys + K(1, 2));
//    }
//}


void CPlot2D::GetWindowCoords(double xs, double ys, int& xw, int& yw)
{
    double sx = (RW.right - RW.left) / (RS.right - RS.left);
    double sy = (RW.bottom - RW.top) / (RS.top - RS.bottom); // инверсия Y

    xw = static_cast<int>(RW.left + (xs - RS.left) * sx);
    yw = static_cast<int>(RW.bottom - (ys - RS.bottom) * sy);
}
void CPlot2D::SetPenLine(CMyPen& PLine)
{
    PenLine = PLine;
}

void CPlot2D::SetPenAxis(CMyPen& PAxis)
{
    PenAxis = PAxis;
}

void CPlot2D::CalculateWorldRect()
{
    if (X.GetRowCount() == 0) return;

    double minX = X(0, 0), maxX = X(0, 0);
    double minY = Y(0, 0), maxY = Y(0, 0);

    for (int i = 1; i < X.GetRowCount(); i++) {
        if (X(i, 0) < minX) minX = X(i, 0);
        if (X(i, 0) > maxX) maxX = X(i, 0);
        if (Y(i, 0) < minY) minY = Y(i, 0);
        if (Y(i, 0) > maxY) maxY = Y(i, 0);
    }

    // Добавляем отступы
   
    double dx = (maxX - minX) * 0.1;
    double dy = (maxY - minY) * 0.1;

    RS.left = minX - dx;
    RS.right = maxX + dx;
    RS.top = maxY + dy;
    RS.bottom = minY - dy;
}



void CPlot2D::DrawAxisLabels(CDC& dc)
{
    CFont* oldFont = dc.GetCurrentFont();
    COLORREF oldColor = dc.GetTextColor();
    int oldBkMode = dc.GetBkMode();

    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(0, 0, 0));

    CFont font;
    font.CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    dc.SelectObject(&font);

    // Автоматически определяем хороший шаг для делений
    double xRange = RS.right - RS.left;
    double yRange = RS.top - RS.bottom;

    // Выбираем "красивый" шаг (1, 2, 5, 10, etc.)
    double xStep = CalculateNiceStep(xRange);
    double yStep = CalculateNiceStep(yRange);

    // Рисуем подписи для оси X
    double startX = ceil(RS.left / xStep) * xStep;
    for (double xValue = startX; xValue <= RS.right; xValue += xStep) {
        int xPos, yPos;
        GetWindowCoords(xValue, 0, xPos, yPos);

        // Засечка
        dc.MoveTo(xPos, yPos - 3);
        dc.LineTo(xPos, yPos + 3);

        // Подпись
        CString label;
        if (fabs(xValue) < 1e-10) xValue = 0;  // Избегаем -0.0
        label.Format(_T("%g"), xValue);
        CSize textSize = dc.GetTextExtent(label);
        dc.TextOut(xPos - textSize.cx / 2, yPos + 8, label);
    }

    // Рисуем подписи для оси Y
    double startY = ceil(RS.bottom / yStep) * yStep;
    for (double yValue = startY; yValue <= RS.top; yValue += yStep) {
        if (fabs(yValue) < 1e-10) continue;  // Пропускаем 0 на оси Y (уже есть на оси X)

        int xPos, yPos;
        GetWindowCoords(0, yValue, xPos, yPos);

        // Засечка
        dc.MoveTo(xPos - 3, yPos);
        dc.LineTo(xPos + 3, yPos);

        // Подпись
        CString label;
        label.Format(_T("%g"), yValue);
        CSize textSize = dc.GetTextExtent(label);
        dc.TextOut(xPos - textSize.cx - 5, yPos - textSize.cy / 2, label);
    }

    // Подписываем оси
    int originX, originY;
    GetWindowCoords(0, 0, originX, originY);
    dc.TextOut(RW.right - 15, originY + 15, _T("X"));
    dc.TextOut(originX + 5, RW.top + 5, _T("Y"));

    dc.SelectObject(oldFont);
    dc.SetTextColor(oldColor);
    dc.SetBkMode(oldBkMode);
}

// Вспомогательная функция для вычисления "красивого" шага
double CPlot2D::CalculateNiceStep(double range)
{
    double exponent = floor(log10(range));
    double fraction = range / pow(10.0, exponent);

    double niceFraction;
    if (fraction < 1.5) {
        niceFraction = 1.0;
    }
    else if (fraction < 3.0) {
        niceFraction = 2.0;
    }
    else if (fraction < 7.0) {
        niceFraction = 5.0;
    }
    else {
        niceFraction = 10.0;
    }

    return niceFraction * pow(10.0, exponent - 1);
}





void CPlot2D::Draw(CDC& dc, int DrawAxes, int DrawRect)
{
    // Сохраняем текущие настройки
    CPen* oldPen = dc.GetCurrentPen();

    // Рисуем прямоугольник области
  /*  if (DrawRect) {
        CPen rectPen;
        rectPen.CreatePen(PS_DASH, 1, RGB(200, 200, 200));
        dc.SelectObject(&rectPen);
        dc.Rectangle(&RW);
    }*/

    // Рисуем оси координат


    if (DrawAxes) {
        CPen axisPen;
        axisPen.CreatePen(PenAxis.PenStyle, PenAxis.PenWidth, PenAxis.PenColor);
        dc.SelectObject(&axisPen);

        int x0, y0, x1, y1;
        // Ось X
        GetWindowCoords(RS.left, 0, x0, y0);
        GetWindowCoords(RS.right, 0, x1, y1);
        dc.MoveTo(x0, y0);
        dc.LineTo(x1, y1);

        // Ось Y
        GetWindowCoords(0, RS.bottom, x0, y0);
        GetWindowCoords(0, RS.top, x1, y1);
        dc.MoveTo(x0, y0);
        dc.LineTo(x1, y1);

        DrawAxisLabels(dc);
    }

    // Рисуем график
    if (X.GetRowCount() > 0) {
        CPen linePen;
        linePen.CreatePen(PenLine.PenStyle, PenLine.PenWidth, PenLine.PenColor);
        dc.SelectObject(&linePen);

        int x_prev, y_prev;
        GetWindowCoords(X(0, 0), Y(0, 0), x_prev, y_prev);

        for (int i = 1; i < X.GetRowCount(); i++) {
            int x, y;
            GetWindowCoords(X(i, 0), Y(i, 0), x, y);
            dc.MoveTo(x_prev, y_prev);
            dc.LineTo(x, y);
            x_prev = x;
            y_prev = y;
        }
    }



    // Восстанавливаем перо
    dc.SelectObject(oldPen);
}