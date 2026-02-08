#pragma once
#define _USE_MATH_DEFINES
#include "resource.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibPyramid.h"
#include "CLightDialog.h"
#include <cmath>  // для M_PI, cos, sin
#include <math.h>

class CMainWnd : public CFrameWnd
{
private:
	CPyramid PIR;   // Пирамида
	CRect WinRect;  // Область в окне
	CMatrix PView;  // Точка наблюдения (сферические координаты)
	CMatrix PLight; // Источник света (сферические координаты)
	COLORREF PyramidColor; // Цвет пирамиды
	int Index;
	CMenu menu;

	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

	// Функция перевода сферических координат в декартовы
	CMatrix SphericalToDecart(CMatrix& spherical)
	{
		double r = spherical(0);
		double fi = spherical(1) * M_PI / 180.0;
		double q = spherical(2) * M_PI / 180.0;

		double x = r * cos(q) * cos(fi);
		double y = r * cos(q) * sin(fi);
		double z = r * sin(q);

		CMatrix result(3);
		result(0) = x;
		result(1) = y;
		result(2) = z;
		return result;
	}

public:
	CMainWnd::CMainWnd()
	{
		Create(NULL, L"Lab09", WS_OVERLAPPEDWINDOW, CRect(10, 10, 700, 800), NULL, NULL);
		Index = 0;

		// Камера (сферические координаты)
		PView.RedimMatrix(3);
		PView(0) = 100; // r
		PView(1) = 45;  // fi
		PView(2) = 45;  // q

		//  Источник света
		PLight.RedimMatrix(3);
		PLight(0) = 25; // r
		PLight(1) = 45; // fi
		PLight(2) = 60; // q

		// Цвет пирамиды (по умолчанию RGB(236, 100, 134))
		PyramidColor = RGB(236, 100, 134);	 
	}

	void OnPaint();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSize(UINT nType, int cx, int cy);
	void OnPyramid();
	void OnLightSettings();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_30002, OnPyramid)
	ON_COMMAND(ID_LIGHT_SETTINGS, OnLightSettings)
	ON_COMMAND(ID_EXIT, Exit)
END_MESSAGE_MAP()

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu);            // Установить меню
	return 0;
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1)
	{
		// Отрисовка пирамиды
		PIR.ColorDraw(dc, PView, WinRect, PyramidColor, PLight);

		// === Вывод координат камеры ===
		CString str;
		str.Format(L"Камера: r = %.2f, fi = %.2f°, q = %.2f°",
			PView(0), PView(1), PView(2));
		dc.TextOutW(10, 10, str);

		// === Вывод координат источника света ===
		CMatrix LightDec = SphericalToDecart(PLight);
		CString strLight;
		strLight.Format(L"Источник света: X = %.2f, Y = %.2f, Z = %.2f (r=%.2f, fi=%.2f°, q=%.2f°)",
			LightDec(0), LightDec(1), LightDec(2),
			PLight(0), PLight(1), PLight(2));
		dc.TextOutW(10, 30, strLight);
	}
}

void CMainWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (Index == 1)
	{
		switch (nChar)
		{
			// === Управление камерой ===
		case VK_DOWN:
			if (PView(2) > 0) PView(2) -= 3;
			break;
		case VK_UP:
			if (PView(2) < 180) PView(2) += 3;
			break;
		case VK_LEFT:
			PView(1) -= 3;
			if (PView(1) < -180) PView(1) += 360;
			break;
		case VK_RIGHT:
			PView(1) += 3;
			if (PView(1) > 180) PView(1) -= 360;
			break;

			// === Управление источником света (W/A/S/D) ===
		case 'A':
			PLight(1) -= 5; // влево
			if (PLight(1) < -180) PLight(1) += 360;
			break;
		case 'D':
			PLight(1) += 5; // вправо
			if (PLight(1) > 180) PLight(1) -= 360;
			break;
		case 'W':
			if (PLight(2) < 180) PLight(2) += 5; // вверх
			break;
		case 'S':
			if (PLight(2) > 0) PLight(2) -= 5; // вниз
			break;
		}

		Invalidate(); // перерисовать окно
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(100, 100, cx - 100, cy - 100);
}

void CMainWnd::OnPyramid()
{
	Index = 1;
	Invalidate();
}

void CMainWnd::OnLightSettings()
{
	// Создать и показать диалог для настройки координат источника света и цвета
	CLightDialog dlg(PLight, PyramidColor, this);
	if (dlg.DoModal() == IDOK)
	{
		// Координаты и цвет обновлены в диалоге, перерисовать окно
		Invalidate();
	}
}

void CMainWnd::Exit()
{
	DestroyWindow();
}
