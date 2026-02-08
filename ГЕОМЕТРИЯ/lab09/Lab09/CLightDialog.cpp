#include "CLightDialog.h"
#include "resource.h"

CLightDialog::CLightDialog(CMatrix& light, COLORREF& color, CWnd* pParent)
    : CDialog(IDD_LIGHT_DIALOG, pParent)
    , m_Light(light)
    , m_Color(color)
    , m_CurrentColor(color)
{
}

void CLightDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLightDialog, CDialog)
    ON_BN_CLICKED(IDOK, OnOK)
    ON_EN_KILLFOCUS(IDC_EDIT_COLOR2, &CLightDialog::OnKillFocusHex)  // HEX
    ON_EN_KILLFOCUS(IDC_EDIT_COLOR3, &CLightDialog::OnKillFocusRgb)  // R
    ON_EN_KILLFOCUS(IDC_EDIT_COLOR4, &CLightDialog::OnKillFocusRgb)  // G
    ON_EN_KILLFOCUS(IDC_EDIT_COLOR, &CLightDialog::OnKillFocusRgb)  // B
END_MESSAGE_MAP()

// --------------------- Вспомогательные функции -----------------------------

bool CLightDialog::ParseHexString(const CString& src, COLORREF& outColor)
{
    CString s = src;
    s.Trim();
    if (s.IsEmpty())
        return false;

    if (s.GetAt(0) == L'#')
        s = s.Mid(1);

    if (s.GetLength() != 6)
        return false;

    wchar_t* endPtr = nullptr;
    unsigned long val = wcstoul(s, &endPtr, 16);
    if (*endPtr != 0)
        return false;

    BYTE r = static_cast<BYTE>((val >> 16) & 0xFF);
    BYTE g = static_cast<BYTE>((val >> 8) & 0xFF);
    BYTE b = static_cast<BYTE>(val & 0xFF);

    outColor = RGB(r, g, b);
    return true;
}

bool CLightDialog::ParseRgbControls(COLORREF& outColor)
{
    CString sR, sG, sB;
    GetDlgItemText(IDC_EDIT_COLOR3, sR); // R
    GetDlgItemText(IDC_EDIT_COLOR4, sG); // G
    GetDlgItemText(IDC_EDIT_COLOR, sB); // B

    wchar_t* endPtr = nullptr;

    long r = wcstol(sR, &endPtr, 10);
    if (*endPtr != 0 || r < 0 || r > 255)
        return false;

    endPtr = nullptr;
    long g = wcstol(sG, &endPtr, 10);
    if (*endPtr != 0 || g < 0 || g > 255)
        return false;

    endPtr = nullptr;
    long b = wcstol(sB, &endPtr, 10);
    if (*endPtr != 0 || b < 0 || b > 255)
        return false;

    outColor = RGB(static_cast<BYTE>(r),
        static_cast<BYTE>(g),
        static_cast<BYTE>(b));
    return true;
}

void CLightDialog::UpdateRgbFromCurrent()
{
    CString s;
    BYTE r = GetRValue(m_CurrentColor);
    BYTE g = GetGValue(m_CurrentColor);
    BYTE b = GetBValue(m_CurrentColor);

    s.Format(L"%d", static_cast<int>(r));
    SetDlgItemText(IDC_EDIT_COLOR3, s);

    s.Format(L"%d", static_cast<int>(g));
    SetDlgItemText(IDC_EDIT_COLOR4, s);

    s.Format(L"%d", static_cast<int>(b));
    SetDlgItemText(IDC_EDIT_COLOR, s);
}

void CLightDialog::UpdateHexFromCurrent()
{
    BYTE r = GetRValue(m_CurrentColor);
    BYTE g = GetGValue(m_CurrentColor);
    BYTE b = GetBValue(m_CurrentColor);

    CString hex;
    hex.Format(L"%02X%02X%02X", r, g, b);
    SetDlgItemText(IDC_EDIT_COLOR2, hex);
}

// --------------------------- Инициализация ---------------------------------

BOOL CLightDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Координаты источника света
    CString str;

    str.Format(L"%.2f", m_Light(0));
    SetDlgItemText(IDC_EDIT_R, str);

    str.Format(L"%.2f", m_Light(1));
    SetDlgItemText(IDC_EDIT_FI, str);

    str.Format(L"%.2f", m_Light(2));
    SetDlgItemText(IDC_EDIT_Q, str);

    // Цвет
    m_CurrentColor = m_Color;
    UpdateRgbFromCurrent();
    UpdateHexFromCurrent();

    return TRUE;
}

// --------------------- Реакция на изменение полей --------------------------

void CLightDialog::OnKillFocusHex()
{
    CString sHex;
    GetDlgItemText(IDC_EDIT_COLOR2, sHex);

    COLORREF col;
    if (!ParseHexString(sHex, col))
    {
        MessageBox(L"Invalid color hex format!Use RRGGBB or #RRGGBB",
            L"Ошибка", MB_OK | MB_ICONERROR);
        // вернуть старое значение
        UpdateHexFromCurrent();
        return;
    }

    m_CurrentColor = col;
    UpdateRgbFromCurrent();   // синхронизируем R,G,B
}

void CLightDialog::OnKillFocusRgb()
{
    COLORREF col;
    if (!ParseRgbControls(col))
    {
        MessageBox(L"Компоненты R, G, B должны быть целыми числами от 0 до 255.",
            L"Ошибка", MB_OK | MB_ICONERROR);
        // вернуть старые значения
        UpdateRgbFromCurrent();
        return;
    }

    m_CurrentColor = col;
    UpdateHexFromCurrent();   // синхронизируем hex
}

// ------------------------------ OK -----------------------------------------

void CLightDialog::OnOK()
{
    // ---------- источник света ----------
    CString strR, strFi, strQ;

    GetDlgItemText(IDC_EDIT_R, strR);
    GetDlgItemText(IDC_EDIT_FI, strFi);
    GetDlgItemText(IDC_EDIT_Q, strQ);

    double r = _wtof(strR);
    double fi = _wtof(strFi);
    double q = _wtof(strQ);

    if (r <= 0)
    {
        MessageBox(L"Радиус должен быть положительным числом!",
            L"Ошибка", MB_OK | MB_ICONERROR);
        return;
    }

    if (q < 0 || q > 180)
    {
        MessageBox(L"Угол q должен быть в диапазоне от 0 до 180 градусов!",
            L"Ошибка", MB_OK | MB_ICONERROR);
        return;
    }

    m_Light(0) = r;
    m_Light(1) = fi;
    m_Light(2) = q;

    // ---------- цвет ----------
    // m_CurrentColor к этому моменту уже синхронизирован обработчиками
    // (но на всякий случай ещё раз валидируем RGB)
    COLORREF col;
    if (!ParseRgbControls(col))
    {
        MessageBox(L"Компоненты R, G, B должны быть целыми числами от 0 до 255.",
            L"Ошибка", MB_OK | MB_ICONERROR);
        return;
    }

    m_CurrentColor = col;
    m_Color = m_CurrentColor;

    CDialog::OnOK();
}
