
// GXMusicDlg.h: 头文件
//
#include "TransparentButton.h"

#pragma once


// CGXMusicDlg 对话框
class CGXMusicDlg : public CDialogEx
{
// 构造
public:
	CGXMusicDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GXMUSIC_DIALOG };
#endif


private:
	// for UI degign
	CImage m_bgImage; // 用于加载背景图片
	CTransparentButton m_CloseButton;
	CTransparentButton m_MaximizeButton;
	CTransparentButton m_MinimizeButton;
	BOOL			   m_bIsWindowMaximized;	// for maximize window

	void SetBackgroundAndTransparency(BYTE alpha);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMainButtonClicked();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};
