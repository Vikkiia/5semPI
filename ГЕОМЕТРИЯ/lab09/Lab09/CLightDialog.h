#pragma once
#include "resource.h"
#include "CMatrix.h"
#include <afxwin.h>

class CLightDialog : public CDialog
{
public:
    CLightDialog(CMatrix& light, COLORREF& color, CWnd* pParent = nullptr);

    // Данные диалога
    CMatrix& m_Light;   // Ссылка на координаты источника света
    COLORREF& m_Color;   // Ссылка на цвет пирамиды

    enum { IDD = IDD_LIGHT_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV поддержка

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnOK();

    // Синхронизация при изменении полей
    afx_msg void OnKillFocusHex();   // hex поле
    afx_msg void OnKillFocusRgb();   // любое из R/G/B

private:
    COLORREF m_CurrentColor;         // текущий цвет в диалоге

    // Парсинг и обновление
    bool ParseHexString(const CString& s, COLORREF& outColor);
    bool ParseRgbControls(COLORREF& outColor);

    void UpdateRgbFromCurrent();     // m_CurrentColor -> R,G,B
    void UpdateHexFromCurrent();     // m_CurrentColor -> hex
};
