#include "pch.h"
#include "TransparentList.h"

IMPLEMENT_DYNAMIC(TransparentList, CListCtrl)

BEGIN_MESSAGE_MAP(TransparentList, CListCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomDraw)
    ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
    ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()

BOOL TransparentList::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void TransparentList::OnNMCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    *pResult = CDRF_DODEFAULT;

    switch (pLVCD->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT:
        *pResult = CDRF_NOTIFYITEMDRAW;
        break;

    case CDDS_ITEMPREPAINT:
    {
        // 创建GDI+绘图对象
        Gdiplus::Graphics graphics(pLVCD->nmcd.hdc);
        int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

        // 获取绘制区域
        CRect rcItem;
        GetItemRect(pLVCD->nmcd.dwItemSpec, &rcItem, LVIR_BOUNDS);
        Gdiplus::Rect gdiRect(rcItem.left, rcItem.top, rcItem.Width(), rcItem.Height());

        // 判断选中状态
        BOOL bSelected = GetItemState(nItem, LVIS_SELECTED) & LVIS_SELECTED;

        // 绘制透明背景
        if (bSelected)
        {
            Gdiplus::Color translucentColor(22, 255, 255, 255);
            Gdiplus::SolidBrush brush(translucentColor);
            graphics.FillRectangle(&brush, gdiRect);
        }

        // 绘制文本
        Gdiplus::Font font(L"Arial", 18, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
        Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255, 255));

        // 创建对齐格式（垂直居中）
        Gdiplus::StringFormat format;
        format.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 垂直居中

        int nColCount = m_listHeaderCtrl.GetItemCount();

        for (int ii = 0; ii < nColCount; ii++)
        {
            CRect rcCol;           
            CString strText = GetItemText(nItem, ii);

            GetSubItemRect(nItem, ii, LVIR_LABEL, rcCol);

            Gdiplus::RectF textRect(
                rcCol.left,
                rcCol.top,
                rcCol.Width(),
                rcCol.Height()
            );

            graphics.DrawString(
                strText.GetString(), -1, &font, textRect,
                &format, &textBrush
            );
        }       

        *pResult = CDRF_SKIPDEFAULT; // 跳过默认绘制
        break;
    }
    }
}

void TransparentList::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();

    CHeaderCtrl* pHeader = GetHeaderCtrl();
    if (pHeader && pHeader->GetSafeHwnd())
    {
        m_listHeaderCtrl.SubclassWindow(pHeader->GetSafeHwnd());
    }
        
}

// 垂直滚动处理
void TransparentList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CRect rcClient;
    GetClientRect(&rcClient);

    // 计算滚动偏移量
    int scrollOffset = GetScrollPos(SB_VERT) - nPos;

    SetRedraw(FALSE); // 临时禁止绘制
    // 调用基类处理滚动逻辑
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

    SetRedraw(TRUE);

    // 仅重绘受影响区域
    if (scrollOffset != 0) {
        CRect rcUpdate = rcClient;
        rcUpdate.top += scrollOffset;
        InvalidateRect(&rcUpdate, FALSE); // FALSE表示不擦除背景
        UpdateWindow();
    }
}

// 水平滚动处理
void TransparentList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SetRedraw(FALSE);
    CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
    SetRedraw(TRUE);
    RedrawWindow(
        NULL,
        NULL,
        RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN
    );
}

// 鼠标滚轮处理
BOOL TransparentList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    //SetRedraw(FALSE);
    BOOL bResult = CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
    //SetRedraw(TRUE);
    /*RedrawWindow(
        NULL,
        NULL,
        RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE
    );*/

    return bResult;
}

LRESULT TransparentList::OnPrintClient(WPARAM wParam, LPARAM lParam) {
    // 直接调用OnPaint进行绘制
    CDC* pDC = CDC::FromHandle((HDC)wParam);
    if (pDC) {
        OnPaint();
    }
    return 0;
}

void TransparentList::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
    // 直接返回，不计算非客户区
    //CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}
