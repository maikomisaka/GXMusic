#pragma once
#include <afxcview.h>

class CTransparentHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNAMIC(CTransparentHeaderCtrl)

public:
    CTransparentHeaderCtrl();
    virtual ~CTransparentHeaderCtrl();

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnNMCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};

