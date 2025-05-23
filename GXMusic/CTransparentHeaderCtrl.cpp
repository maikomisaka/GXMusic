#include "pch.h"
#include "CTransparentHeaderCtrl.h"

IMPLEMENT_DYNAMIC(CTransparentHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CTransparentHeaderCtrl, CHeaderCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_NOTIFY_REFLECT(HDN_BEGINTRACK, OnHeaderBeginTrack)
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()

CTransparentHeaderCtrl::CTransparentHeaderCtrl() {}
CTransparentHeaderCtrl::~CTransparentHeaderCtrl() {}

BOOL CTransparentHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CTransparentHeaderCtrl::OnPaint()
{
    CPaintDC dc(this);
    Gdiplus::Graphics graphics(dc.m_hDC);

    // 绘制标题文字
    DrawHeaderText(graphics);
}

void CTransparentHeaderCtrl::DrawHeaderText(Gdiplus::Graphics& graphics)
{
    // 绘制透明文本（带抗锯齿）
    Gdiplus::Font font(L"Arial", 18, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));

    int nItems = GetItemCount();
    for (int i = 0; i < nItems; ++i)
    {
        CRect itemRect;
        GetItemRect(i, &itemRect);

        TCHAR szText[256];
        HDITEM hdi = { HDI_TEXT };
        hdi.pszText = szText;
        hdi.cchTextMax = _countof(szText);
        GetItem(i, &hdi);

        // 转换到GDI+坐标系
        Gdiplus::RectF gdiRect(
            (Gdiplus::REAL)itemRect.left,
            (Gdiplus::REAL)itemRect.top,
            (Gdiplus::REAL)itemRect.Width(),
            (Gdiplus::REAL)itemRect.Height()
        );

        // 绘制文本
        graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        // 创建对齐格式（垂直居中）
        Gdiplus::StringFormat format;
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直居中

        graphics.DrawString(
            CStringW(szText), -1, &font, gdiRect,
            &format, &textBrush
        );
    }
}

void CTransparentHeaderCtrl::OnHeaderBeginTrack(NMHDR* pNMHDR, LRESULT* pResult)
{
    *pResult = 1; // 设置 pResult 为 1 阻止调整
}

BOOL CTransparentHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
    return TRUE;
}
