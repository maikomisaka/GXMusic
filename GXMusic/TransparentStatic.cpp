#include "pch.h"
#include "TransparentStatic.h"

BEGIN_MESSAGE_MAP(TransparentStatic, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

TransparentStatic::TransparentStatic() 
{
    m_font.CreatePointFont(160, _T("Arial"));
}

TransparentStatic::~TransparentStatic() {}

void TransparentStatic::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Draw the static text
    CRect rect;
    GetClientRect(&rect);

    dc.SetBkMode(TRANSPARENT); // Set background mode to transparent
    dc.SetTextColor(RGB(255, 255, 255));

    CFont* pOldFont = dc.SelectObject(&m_font);

    CString text;
    GetWindowText(text);
    dc.DrawText(text, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Restore old font
    dc.SelectObject(pOldFont);
}

BOOL TransparentStatic::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // ²»²Á³ý±³¾°
}