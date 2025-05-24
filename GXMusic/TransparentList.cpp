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
        // 设置透明背景和文本颜色
        pLVCD->clrText = RGB(255, 255, 255); // 白色文本
        pLVCD->clrTextBk = CLR_NONE;         // 透明背景

        // 不跳过默认绘制！允许系统绘制文本
        *pResult = CDRF_NEWFONT;
        break;
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
