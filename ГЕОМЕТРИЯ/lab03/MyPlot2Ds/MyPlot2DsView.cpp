#include "pch.h"
#include "MyPlot2Ds.h"
#include "MyPlot2DsDoc.h"
#include "MyPlot2DsView.h"
#include "Plot2D.h"

IMPLEMENT_DYNCREATE(CMyPlot2DView, CView)

BEGIN_MESSAGE_MAP(CMyPlot2DView, CView)
    ON_COMMAND(ID_TEST_F1, &CMyPlot2DView::OnTestsF1)
    ON_COMMAND(ID_TEST_F2, &CMyPlot2DView::OnTestsF2)
    ON_COMMAND(ID_TEST_F3, &CMyPlot2DView::OnTestsF3)
END_MESSAGE_MAP()

CMyPlot2DView::CMyPlot2DView() : m_currentTest(0)
{
}

//void CMyPlot2DView::OnDraw(CDC* pDC)
//{
//    if (m_currentTest != 0) {
//        m_plot.Draw(*pDC, 1, 1);
//    }
//    if (m_currentTest == 3) {
//        CMyPen circlePen;
//        circlePen.Set(PS_SOLID, 2, RGB(0, 0, 255)); // Синий, толщина 2
//        DrawCircle(pDC, 0, 0, 10.0, circlePen); // Окружность с центром (0,0) и радиусом 10
//    }
//}

void CMyPlot2DView::OnDraw(CDC* pDC)
{
    // Очищаем область белым цветом
    CRect clientRect;
    GetClientRect(&clientRect);
    pDC->FillSolidRect(&clientRect, RGB(255, 255, 255));

    if (m_currentTest != 0) {
        // Рисуем график
     

        if (m_currentTest == 3) {
          
            m_plot.Draw(*pDC, 0, 1);
            CMyPen circlePen;
            circlePen.Set(PS_SOLID, 2, RGB(0, 0, 255)); // Синий, толщина 2
            DrawCircle(pDC, 0, 0, 5.0, circlePen); // Используем DrawCircle
        }
        else {
               m_plot.Draw(*pDC, 1, 1);
        }


        // Для теста F3 дополнительно рисуем окружность
        //if (m_currentTest == 3) {
        //    CMyPen circlePen;
        //    circlePen.Set(PS_SOLID, 2, RGB(0, 0, 255)); // Синий, толщина 2

        //    CPen pen;
        //    pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
        //    CPen* oldPen = pDC->SelectObject(&pen);

        //    // Рисуем окружность вручную
        //    CRect plotRect;
        //    GetClientRect(&clientRect);
        //    int size = min(clientRect.Width(), clientRect.Height()) - 100;
        //    int centerX = clientRect.Width() / 2;
        //    int centerY = clientRect.Height() / 2;
        //    int circleRadius = size / 2 - 10;

        //    pDC->Ellipse(centerX - circleRadius, centerY - circleRadius,
        //        centerX + circleRadius, centerY + circleRadius);

        //    pDC->SelectObject(oldPen);
        //}
    }
    else {
        // Отладочный текст
        pDC->TextOut(10, 10, _T("Выберите тест из меню Test_F"));
    }
}

void CMyPlot2DView::DrawCircle(CDC* pDC, double centerX, double centerY, double radius, CMyPen& pen)
{
    // Отладочная информация
   /* CString debugText;
    debugText.Format(_T("DrawCircle called: center(%.1f, %.1f), radius=%.1f"), centerX, centerY, radius);
    pDC->TextOut(10, 70, debugText);*/

    CPen circlePen;
    circlePen.CreatePen(pen.PenStyle, pen.PenWidth, pen.PenColor);
    CPen* oldPen = pDC->SelectObject(&circlePen);

    // ВАЖНО: устанавливаем прозрачную кисть!
    CBrush* oldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

    // Преобразуем координаты центра и радиуса в оконные координаты
    int centerXw, centerYw;
    m_plot.GetWindowCoords(centerX, centerY, centerXw, centerYw);

  /*  debugText.Format(_T("Window coords: center(%d, %d)"), centerXw, centerYw);
    pDC->TextOut(10, 90, debugText);*/

    int radiusXw, radiusYw;
    m_plot.GetWindowCoords(centerX + radius, centerY, radiusXw, radiusYw);
    int radiusW = radiusXw - centerXw;

   /* debugText.Format(_T("Radius in window: %d"), radiusW);
    pDC->TextOut(10, 110, debugText);*/

    // Рисуем окружность (только контур, без заливки)
    pDC->Ellipse(centerXw - radiusW, centerYw - radiusW,
        centerXw + radiusW, centerYw + radiusW);

 /*   debugText = _T("Ellipse drawn");
    pDC->TextOut(10, 130, debugText);*/

    // Восстанавливаем старые объекты
    pDC->SelectObject(oldPen);
    pDC->SelectObject(oldBrush);
}

//void CMyPlot2DView::DrawCircle(CDC* pDC, double centerX, double centerY, double radius, CMyPen& pen)
//{
//    CPen circlePen;
//    circlePen.CreatePen(pen.PenStyle, pen.PenWidth, pen.PenColor);
//    CPen* oldPen = pDC->SelectObject(&circlePen);
//
//    CBrush* oldbrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
//
//    // Преобразуем координаты центра и радиуса в оконные координаты
//    int centerXw, centerYw;
//    m_plot.GetWindowCoords(centerX, centerY, centerXw, centerYw);
//
//    int radiusXw, radiusYw;
//    m_plot.GetWindowCoords(centerX + radius, centerY, radiusXw, radiusYw);
//    int radiusW = radiusXw - centerXw;
//
//    // Рисуем окружность
//    pDC->Ellipse(centerXw - radiusW, centerYw - radiusW,
//        centerXw + radiusW, centerYw + radiusW);
//
//    pDC->SelectObject(oldPen);
//    pDC->SelectObject(oldbrush);
//}

double CMyPlot2DView::MyF1(double x)
{
    if (fabs(x) < 1e-10) // x близко к 0
        return 1.0;       // предел sin(x)/x при x→0 равен 1
    return sin(x) / x;  // Пример функции F1
}

double CMyPlot2DView::MyF2(double x)
{
    return sqrt(abs(x)) * sin(x);  // Пример функции F2
}

void CMyPlot2DView::TestF1()
{
    // Рассчитываем значения функции для x ∈ [0, 4π]
    int number = 1;
    int n = 100;
    CMatrix X(n, 1), Y(n, 1);
    double step = 4* 3.14159 / (n-1);

    for (int i = 0; i < n; i++) {
        X(i, 0) = i * step;
        Y(i, 0) = MyF1(X(i, 0));
    }

    // Устанавливаем параметры пера для графика
    CMyPen penLine;
    penLine.Set(PS_SOLID, 1, RGB(255, 0, 0));  // Красный, толщина 1

    // Устанавливаем параметры пера для осей
    CMyPen penAxis;
    penAxis.Set(PS_SOLID, 2, RGB(0, 0, 255));  // Синий, толщина 2

    // Устанавливаем область отображения
    CRect clientRect;
    GetClientRect(&clientRect);
    CRect plotRect = clientRect;
    plotRect.DeflateRect(150, 150);  // Отступы от краев

    m_plot.SetPenLine(penLine);
    m_plot.SetPenAxis(penAxis);
    m_plot.SetParams(X, Y, plotRect, number);
}

void CMyPlot2DView::TestF2()
{
    // Рассчитываем значения функции для x ∈ [0, 4π]
    int number = 2;
    int n = 100;
    CMatrix X(n, 1), Y(n, 1);
    double step = 4 * 3.14159 / (n - 1);

    for (int i = 0; i < n; i++) {
        X(i, 0) = i * step;
        Y(i, 0) = MyF2(X(i, 0));
    }

    // Устанавливаем параметры пера для графика
    CMyPen penLine;
    penLine.Set(PS_DASHDOT, 3, RGB(255, 0, 0));  // Красный, толщина 3, штрих-пунктир

    // Устанавливаем параметры пера для осей
    CMyPen penAxis;
    penAxis.Set(PS_SOLID, 2, RGB(0, 0, 0));  // Черный, толщина 2

    // Устанавливаем область отображения
    CRect clientRect;
    GetClientRect(&clientRect);
    CRect plotRect = clientRect;
    plotRect.DeflateRect(50, 50);

    m_plot.SetPenLine(penLine);
    m_plot.SetPenAxis(penAxis);
    m_plot.SetParams(X, Y, plotRect, number);
}

void CMyPlot2DView::TestF3()
{
    // Создаем правильный восьмиугольник
    int number = 3;
    int n = 8;
    CMatrix X(n + 1, 1), Y(n + 1, 1);
    double radius = 5.0;  // Уменьшили радиус для лучшего отображения

    for (int i = 0; i <= n; i++) {
        double angle = 2 * 3.14159 * i / n;
        X(i, 0) = radius * cos(angle);
        Y(i, 0) = radius * sin(angle);
    }

    // Перо для восьмиугольника
    CMyPen penLine;
    penLine.Set(PS_SOLID, 3, RGB(255, 0, 0));  // Красный, толщина 3

    // Перо для осей
    CMyPen penAxis;
    penAxis.Set(PS_SOLID, 1, RGB(0, 0, 0));  // Черный, толщина 1

    CRect clientRect;
    GetClientRect(&clientRect);

    // Создаем квадратную область в центре
    int size = min(clientRect.Width(), clientRect.Height()) - 100;
    CRect plotRect(
        (clientRect.Width() - size) / 2,
        (clientRect.Height() - size) / 2,
        (clientRect.Width() + size) / 2,
        (clientRect.Height() + size) / 2
    );

    m_plot.SetPenLine(penLine);
    m_plot.SetPenAxis(penAxis);
    m_plot.SetParams(X, Y, plotRect, number);
}

//void CMyPlot2DView::TestF3()
//{
//    // Создаем правильный восьмиугольник
//     // Создаем правильный восьмиугольник
//    int n = 8;
//    CMatrix X(n + 1, 1), Y(n + 1, 1);
//    double radius = 10;
//
//    for (int i = 0; i <= n; i++) {
//        double angle = 2 * 3.14159 * i / n;
//        X(i, 0) = radius * cos(angle);
//        Y(i, 0) = radius * sin(angle);
//    }
//
//    // Перо для восьмиугольника
//    CMyPen penLine;
//    penLine.Set(PS_SOLID, 3, RGB(255, 0, 0));  // Красный, толщина 3
//
//    // Перо для окружности
//    CMyPen penCircle;
//    penCircle.Set(PS_SOLID, 2, RGB(0, 0, 255));  // Синий, толщина 2
//
//    CRect clientRect;
//    GetClientRect(&clientRect);
//    CRect plotRect = clientRect;
//    plotRect.DeflateRect(0, 0);
//
//   
//    m_plot.SetPenAxis(penCircle);  // Используем перо осей для окружности
//    m_plot.SetPenLine(penLine);
//    m_plot.SetParams(X, Y, plotRect);
//}



void CMyPlot2DView::OnTestsF1()
{
    m_currentTest = 1;
    TestF1();
    Invalidate();
}

void CMyPlot2DView::OnTestsF2()
{
    m_currentTest = 2;
    TestF2();
    Invalidate();
}

void CMyPlot2DView::OnTestsF3()
{
    m_currentTest = 3;
    TestF3();
    Invalidate();
}

BOOL CMyPlot2DView::PreCreateWindow(CREATESTRUCT& cs)
{
    // Вызываем базовый класс
    return CView::PreCreateWindow(cs);
}