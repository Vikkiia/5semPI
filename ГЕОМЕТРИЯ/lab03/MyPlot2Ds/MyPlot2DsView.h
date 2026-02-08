#pragma once
#include "Plot2D.h"

class CMyPlot2DView : public CView  // Убедитесь, что имя класса правильное
{
private:
    CPlot2D m_plot;
    int m_currentTest;

    double MyF1(double x);
    double MyF2(double x);
    void TestF1();
    void TestF2();
    void TestF3();
    void DrawCircle(CDC* pDC, double centerX, double centerY, double radius, CMyPen& pen);

public:
    CMyPlot2DView();  // Конструктор
    DECLARE_DYNCREATE(CMyPlot2DView)

    virtual void OnDraw(CDC* pDC);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnTestsF1();
    afx_msg void OnTestsF2();
    afx_msg void OnTestsF3();
};