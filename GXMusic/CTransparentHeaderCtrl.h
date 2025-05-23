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
    afx_msg void OnHeaderBeginTrack(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

private:
    void DrawHeaderText(Gdiplus::Graphics& graphics);           // ÎÄ×Ö»æÖÆ
};

