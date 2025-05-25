
// GXMusicDlg.h: 头文件
//
#pragma once
#include "TransparentButton.h"
#include "TransparentImage.h"
#include "TransparentStatic.h"
#include "TransparentList.h"
#include "CTransparentHeaderCtrl.h"


#include <iostream>
//#include <codecvt>
#include <mmsystem.h>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>

#pragma comment(lib, "winmm.lib")

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
	Gdiplus::Bitmap* m_pBackgroundBmp = nullptr;
	CTransparentButton m_CloseButton;
	CTransparentButton m_MaximizeButton;
	CTransparentButton m_MinimizeButton;
	BOOL			   m_bIsWindowMaximized;	// for maximize window
	HCURSOR m_hHandCursor;  // 手形光标
	TransparentImage m_imageIcon;
	TransparentStatic m_staticTitle; // GX Music
	std::vector<CRect> m_vecControlRects;
	std::vector<int> m_vecControlIDs;
	std::vector<CRect> m_vecScaleControlRects;
	std::vector<int> m_vecScaleControlIDs;
	std::vector<Gdiplus::Rect> m_vecTransparentBlockRects;
	int m_opacity = 25;
	CRect m_initialDialogRect;

	std::vector<GXSongInfo> m_SongInfoList;
	TransparentList m_SongInfoListCtrl;

	CString m_strCurMusicFilePath;
	bool m_bPause;
	CString m_strDefaultDirectoryPath;
	std::vector<CString> m_vecMusicFilePaths;

	void PlayMusic();
	void PauseOrPlayMusic(bool bPause);
	void StopMusic();
	void SetMusicVolume(int nVolume);
	HRESULT LoadDefaultMusics();

	void InitSongList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnMainButtonClicked();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMusicListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};
