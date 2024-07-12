#pragma once
class CTransparentButton : public CButton
{
public:
	CTransparentButton();
	~CTransparentButton();

	void SetNormalImage(UINT nIDResource);
	void SetHoverImage(UINT nIDResource);

private:
	void LoadImageFromResource(UINT nIDResource, Gdiplus::Image** ppImage);

private:
	Gdiplus::Image* m_pNormalImage;
	Gdiplus::Image* m_pHoverImage;
	BOOL m_bHover;

	

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

