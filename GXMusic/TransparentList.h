#pragma once

class TransparentList : public CListCtrl
{
    DECLARE_DYNAMIC(TransparentList)
protected:
    DECLARE_MESSAGE_MAP()
private:
    void DrawParentBackground(CDC* pDC);
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnNMCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};

