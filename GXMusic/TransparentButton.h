#pragma once
class CTransparentButton : public CButton
{
public:
	CTransparentButton();
	~CTransparentButton();

	BOOL LoadPngImage(UINT nIDResource);


private:
	Gdiplus::Bitmap* m_pImage;

protected:
	afx_msg void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

