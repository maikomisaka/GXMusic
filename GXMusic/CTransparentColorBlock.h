#pragma once
class CTransparentColorBlock : public CStatic
{
public:
    CTransparentColorBlock();
    virtual ~CTransparentColorBlock();

    void SetColor(COLORREF color);
    void SetOpacity(int opacity); // 0 - 255, 0 means fully transparent, 255 means fully opaque

protected:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    DECLARE_MESSAGE_MAP()

private:
    COLORREF m_color;
    int m_opacity;
};

