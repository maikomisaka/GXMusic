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
	return TRUE; // 阻止默认背景擦除
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
        pLVCD->nmcd.uItemState &= ~CDIS_SELECTED; // 可选：取消选中状态的高亮
        pLVCD->clrText = RGB(255, 255, 255);      // 设置文本颜色
        pLVCD->clrTextBk = CLR_NONE;              // 设置透明背景
        *pResult = CDRF_NEWFONT;
        break;
    }
}

void TransparentList::OnPaint()
{
    CPaintDC dc(this);

    // 使用内存DC防止闪烁
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    CRect rcClient;
    GetClientRect(rcClient);
    memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
    CBitmap* pOldBmp = memDC.SelectObject(&memBitmap);

    // 绘制父窗口背景
    DrawParentBackground(&memDC);

    // 绘制列表内容
    DefWindowProc(WM_PAINT, (WPARAM)memDC.GetSafeHdc(), 0);

    // 复制到屏幕
    dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
    memDC.SelectObject(pOldBmp);
}

void TransparentList::DrawParentBackground(CDC* pDC)
{
    CWnd* pParent = GetParent();
    if (!pParent || !pDC) return;

    // 获取列表控件在父窗口中的位置
    CRect rcList;
    GetWindowRect(rcList);
    pParent->ScreenToClient(rcList); // 转换为父窗口坐标系

    // 保存原始设备上下文状态
    int nSaveDC = pDC->SaveDC();

    // 设置裁剪区域防止绘制到控件外
    CRect rcClient;
    GetClientRect(rcClient);
    pDC->IntersectClipRect(rcClient);

    // 偏移设备上下文原点至控件位置
    pDC->SetWindowOrg(rcList.left, rcList.top);

    // 强制父窗口绘制背景到此DC
    pParent->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->GetSafeHdc());
    pParent->SendMessage(WM_PRINTCLIENT, (WPARAM)pDC->GetSafeHdc(),
        PRF_CLIENT | PRF_CHILDREN | PRF_ERASEBKGND);

    // 恢复设备上下文
    pDC->RestoreDC(nSaveDC);
}