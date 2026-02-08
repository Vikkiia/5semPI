
// MyPlot2DsDoc.cpp: реализация класса CMyPlot2DsDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "MyPlot2Ds.h"
#endif

#include "MyPlot2DsDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMyPlot2DsDoc

IMPLEMENT_DYNCREATE(CMyPlot2DsDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyPlot2DsDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CMyPlot2DsDoc

CMyPlot2DsDoc::CMyPlot2DsDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CMyPlot2DsDoc::~CMyPlot2DsDoc()
{
}

BOOL CMyPlot2DsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}




// Сериализация CMyPlot2DsDoc

void CMyPlot2DsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
	}
	else
	{
		// TODO: добавьте код загрузки
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CMyPlot2DsDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CMyPlot2DsDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CMyPlot2DsDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CMyPlot2DsDoc

#ifdef _DEBUG
void CMyPlot2DsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyPlot2DsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CMyPlot2DsDoc
