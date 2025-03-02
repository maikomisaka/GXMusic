#include "pch.h"
#include "CTransparentHeaderCtrl.h"

IMPLEMENT_DYNAMIC(CTransparentHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CTransparentHeaderCtrl, CHeaderCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomDraw)
END_MESSAGE_MAP()

CTransparentHeaderCtrl::CTransparentHeaderCtrl() {}
CTransparentHeaderCtrl::~CTransparentHeaderCtrl() {}

BOOL CTransparentHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}

void CTransparentHeaderCtrl::OnPaint()
{
    //CHeaderCtrl::OnPaint();
    CPaintDC dc(this);
    CRect rect;
    GetClientRect(&rect);

    // 创建GDI+ Graphics对象
    Gdiplus::Graphics graphics(dc.m_hDC);
    Gdiplus::Bitmap buffer(rect.Width(), rect.Height());
    Gdiplus::Graphics bufferGraphics(&buffer);

    // 绘制父窗口背景到缓冲区
    CWnd* pParent = GetParent();
    if (pParent)
    {
        CPoint pt(0, 0);
        MapWindowPoints(pParent, &pt, 1);

        HDC hdcBuffer = bufferGraphics.GetHDC();
        CDC* pDC = CDC::FromHandle(hdcBuffer);
        pDC->SetWindowOrg(-pt.x, -pt.y);
        pParent->SendMessage(WM_ERASEBKGND, (WPARAM)hdcBuffer);
        bufferGraphics.ReleaseHDC(hdcBuffer);
    }

    // 绘制透明文本（带抗锯齿）
    Gdiplus::Font font(L"Arial", 16, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255)); // 黑色文本

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
        bufferGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
        // 创建对齐格式（垂直居中）
        Gdiplus::StringFormat format;
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直居中

        bufferGraphics.DrawString(
            CStringW(szText), -1, &font, gdiRect,
            &format, &textBrush
        );
    }

    // 输出到屏幕
    graphics.DrawImage(&buffer, 0, 0);
}

void CTransparentHeaderCtrl::OnNMCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pNMCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    *pResult = CDRF_DODEFAULT;

    //switch (pNMCD->nmcd.dwDrawStage)
    //{
    //case CDDS_PREPAINT:
    //    *pResult = CDRF_NOTIFYITEMDRAW;
    //    break;

    //case CDDS_ITEMPREPAINT:
    //    //pNMCD->clrText = RGB(255, 255, 255); // 白色文本
    //    pNMCD->clrTextBk = CLR_NONE;         // 透明背景
    //    ::SetBkMode(pNMCD->nmcd.hdc, TRANSPARENT);
    //    *pResult = CDRF_NEWFONT;
    //    break;
    //}

}
