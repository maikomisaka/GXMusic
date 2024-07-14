
// GXMusicDlg.h: 头文件
//
#pragma once
#include "TransparentButton.h"
#include "TransparentImage.h"
#include "TransparentStatic.h"
#include "CTransparentColorBlock.h"

#include <vector>

struct GXSongInfo {
	CString m_title;
	//CString m_filePath;
	CString m_artist;
	CString m_album;
	CString m_duration;
};

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
	CImage m_backgroundImage; // 用于加载背景图片
	CTransparentButton m_CloseButton;
	CTransparentButton m_MaximizeButton;
	CTransparentButton m_MinimizeButton;
	BOOL			   m_bIsWindowMaximized;	// for maximize window
	HCURSOR m_hHandCursor;  // 手形光标
	TransparentImage m_imageIcon;
	TransparentStatic m_staticTitle; // GX Music
	CTransparentColorBlock m_blockMain;
	std::vector<CRect> m_vecControlRects;
	std::vector<int> m_vecControlIDs;
	std::vector<CRect> m_vecScaleControlRects;
	std::vector<int> m_vecScaleControlIDs;
	CRect m_initialDialogRect;

	std::vector<GXSongInfo> m_SongInfoList;
	CListCtrl m_SongInfoListCtrl;

	void SetBackgroundAndTransparency(BYTE alpha);
	void InitSongList();

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
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};
