#pragma once
#include "CTransparentHeaderCtrl.h"

class TransparentList : public CListCtrl
{
    DECLARE_DYNAMIC(TransparentList)
protected:
    DECLARE_MESSAGE_MAP()

    virtual void PreSubclassWindow();

public:
    CTransparentHeaderCtrl m_listHeaderCtrl;

public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnNMCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
};

