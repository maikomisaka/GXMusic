#include "pch.h"
#include "TransparentStatic.h"

BEGIN_MESSAGE_MAP(TransparentStatic, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

TransparentStatic::TransparentStatic() {}

TransparentStatic::~TransparentStatic() {}

void TransparentStatic::OnPaint()
{
    CPaintDC dc(this); // 设备上下文

    // 获取控件的边界
    CRect rect;
    GetClientRect(&rect);

    // 设置透明背景模式
    dc.SetBkMode(TRANSPARENT);

    // 获取文本
    CString text;
    GetWindowText(text);

    // 获取文本颜色
    COLORREF textColor = RGB(255, 255, 255);
    dc.SetTextColor(textColor);

    // 获取字体
    CFont* pFont = GetFont();
    LOGFONT lf = { 0 };
    lf.lfHeight = 35;
    //lf.lfWeight = FW_BOLD;
    _tcscpy_s(lf.lfFaceName, _T("Arial"));
    pFont->CreateFontIndirectW(&lf);
    CFont* pOldFont = dc.SelectObject(pFont);

    // 绘制文本
    dc.DrawText(text, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    dc.SelectObject(pOldFont);
}

BOOL TransparentStatic::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // 不擦除背景
}