#pragma once
class TransparentImage : public CStatic
{
public:
	TransparentImage();
	~TransparentImage();

	void SetImage(const CString& path);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBknd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

private:
	Gdiplus::Image* m_pImage;
};

