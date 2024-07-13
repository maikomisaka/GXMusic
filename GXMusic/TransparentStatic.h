#pragma once
class TransparentStatic : public CStatic
{
public:
    TransparentStatic();
    ~TransparentStatic();

protected:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()
};

