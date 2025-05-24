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
    CDC memDC;
    CBitmap memBitmap;
    CRect rectHeader;
    CRect rectDialog;
    CRect rectHeaderInDialog;
    CWnd* pListCtrl = NULL;
    CWnd* pDialog = NULL;

    GetClientRect(&rectHeader);

    // 创建内存DC用于双缓冲
    memDC.CreateCompatibleDC(&dc);
    memBitmap.CreateCompatibleBitmap(&dc, rectHeader.Width(), rectHeader.Height());
    memDC.SelectObject(&memBitmap);

    // 创建临时的GDI+ Bitmap
    Gdiplus::Bitmap gdiBitmap(L".\\res\\GX_App_Background.png");
    Gdiplus::Graphics graphics(memDC.GetSafeHdc());


    // 计算标题栏区域在原始位图中对应的背景
    pListCtrl = GetParent();
    pDialog = pListCtrl->GetParent();
    GetWindowRect(&rectHeaderInDialog);
    pDialog->ScreenToClient(&rectHeaderInDialog);
    pDialog->GetClientRect(&rectDialog);

    double scaleX = gdiBitmap.GetWidth() * 1.0 / rectDialog.Width();
    double scaleY = gdiBitmap.GetHeight() * 1.0 / rectDialog.Height();
    int srcX = (int)((rectHeaderInDialog.left - rectDialog.left) * scaleX);
    int srcY = (int)((rectHeaderInDialog.top - rectDialog.top) * scaleY);
    int srcWidth = (int)(rectHeader.Width() * scaleX);
    int srcHeight = (int)(rectHeader.Height() * scaleY);
    
    // 绘制标题栏区域的背景
    graphics.DrawImage(
        &gdiBitmap,
        Gdiplus::Rect(0, 0, rectHeader.Width(), rectHeader.Height()),
        srcX, srcY, srcWidth, srcHeight,
        Gdiplus::UnitPixel
    );

    // 绘制标题文字
    DrawHeaderText(&memDC);

    dc.BitBlt(0, 0, rectHeader.Width(), rectHeader.Height(), &memDC, 0, 0, SRCCOPY);
}

void CTransparentHeaderCtrl::DrawHeaderText(CDC* pDC)
{
    Gdiplus::Graphics graphics(pDC->GetSafeHdc());
    // 绘制透明文本（带抗锯齿）
    Gdiplus::Font font(L"Arial", 18, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255, 255));

    // 绘制透明层
    CRect rectHeader;
    Gdiplus::SolidBrush transparentBrush(Gdiplus::Color(25, 255, 255, 255));
    GetClientRect(&rectHeader);
    graphics.FillRectangle(
        &transparentBrush, 
        rectHeader.left, 
        rectHeader.top, 
        rectHeader.Width(),
        rectHeader.Height()
    );

    int nItems = GetItemCount();
    for (int i = 0; i < nItems; ++i)
    {
        CRect itemRect;
        GetItemRect(i, &itemRect);

        TCHAR szText[256] = { 0 };
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
