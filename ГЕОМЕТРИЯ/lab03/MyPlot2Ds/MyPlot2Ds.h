
// MyPlot2Ds.h: основной файл заголовка для приложения MyPlot2Ds
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CMyPlot2DsApp:
// Сведения о реализации этого класса: MyPlot2Ds.cpp
//

class CMyPlot2DsApp : public CWinApp
{
public:
	CMyPlot2DsApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyPlot2DsApp theApp;
