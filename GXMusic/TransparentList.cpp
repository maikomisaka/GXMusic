#include "pch.h"
#include "TransparentList.h"

IMPLEMENT_DYNAMIC(TransparentList, CListCtrl)

BEGIN_MESSAGE_MAP(TransparentList, CListCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomDraw)
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

void TransparentList::OnPaint()
{
    CPaintDC dc(this);
    CRect rcClient;
    GetClientRect(rcClient);

    // 双缓冲绘制
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
    CBitmap* pOldBmp = memDC.SelectObject(&memBitmap);

    // 1. 绘制父窗口背景（覆盖整个区域）
    DrawParentBackground(&memDC, rcClient);

    // 2. 绘制列表内容（排除标题行区域）
    DefWindowProc(WM_PAINT, (WPARAM)memDC.GetSafeHdc(), 0);

    // 3. 复制到屏幕
    dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBmp);
    //CListCtrl::OnPaint();
}

void TransparentList::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();
    ModifyStyle(0, WS_CLIPCHILDREN);
}

void TransparentList::DrawParentBackground(CDC* pDC, CRect rcArea)
{
    CWnd* pParent = GetParent();
    if (!pParent || !pDC) return;

    // 保存设备上下文状态
    int nSaveDC = pDC->SaveDC();

    // 设置裁剪区域
    pDC->IntersectClipRect(rcArea);

    // 计算控件在父窗口中的实际位置
    CRect rcList;
    GetWindowRect(rcList);
    pParent->ScreenToClient(rcList);

    // 偏移原点
    pDC->SetWindowOrg(
        rcList.left - rcArea.left,
        rcList.top - rcArea.top
    );

    // 强制父窗口绘制背景
    pParent->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->GetSafeHdc());
    //pParent->SendMessage(WM_PRINTCLIENT, (WPARAM)pDC->GetSafeHdc(),
        //PRF_CLIENT | PRF_ERASEBKGND);

    // 恢复设备上下文
    pDC->RestoreDC(nSaveDC);
}