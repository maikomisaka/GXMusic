
// GXMusicDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GXMusic.h"
#include "GXMusicDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGXMusicDlg 对话框



CGXMusicDlg::CGXMusicDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GXMUSIC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bIsWindowMaximized = FALSE;
	m_hHandCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
}

void CGXMusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GX_MAIN_CLOSE, m_CloseButton);
	DDX_Control(pDX, IDC_GX_MAIN_MAX, m_MaximizeButton);
	DDX_Control(pDX, IDC_GX_MAIN_MIN, m_MinimizeButton);
	DDX_Control(pDX, IDC_GX_MAIN_ICON, m_imageIcon);
	DDX_Control(pDX, IDC_GX_MAIN_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_GX_SONG_LIST, m_SongInfoListCtrl);
	//DDX_Control(pDX, IDC_GX_MAIN_BLOCK, m_blockMain);
}

BEGIN_MESSAGE_MAP(CGXMusicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_GX_MAIN_CLOSE, OnMainButtonClicked)	
	ON_BN_CLICKED(IDC_GX_MAIN_MAX, OnMainButtonClicked)
	ON_BN_CLICKED(IDC_GX_MAIN_MIN, OnMainButtonClicked)
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_GX_SONG_LIST, OnMusicListDoubleClick)
END_MESSAGE_MAP()


// CGXMusicDlg 消息处理程序

BOOL CGXMusicDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// init control ids
	m_vecControlIDs = {
		IDC_GX_MAIN_CLOSE,
		IDC_GX_MAIN_MAX,
		IDC_GX_MAIN_MIN,
		IDC_GX_MAIN_ICON,
		IDC_GX_MAIN_TITLE
	};
	for (int id : m_vecControlIDs) {
		CRect rect;
		GetDlgItem(id)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_vecControlRects.push_back(rect);
	}
	m_vecScaleControlIDs = {
		IDC_GX_MAIN_BLOCK
	};
	for (int id : m_vecScaleControlIDs) {
		CRect rect;
		GetDlgItem(id)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_vecScaleControlRects.push_back(rect);

		Gdiplus::Rect gdiRect(rect.left, rect.top, rect.Width(), rect.Height());
		m_vecTransparentBlockRects.push_back(gdiRect);
	}
	GetClientRect(&m_initialDialogRect);

	// Background setting
	m_pBackgroundBmp = Gdiplus::Bitmap::FromFile(L".\\res\\GX_App_Background.png");

	// 设置窗口区域为圆角
	CRect rect;
	GetWindowRect(&rect);
	SetWindowRgn(CreateRoundRectRgn(0, 0, rect.Width(), rect.Height(), 20, 20), TRUE);

	m_CloseButton.SetNormalImage(IDB_GX_MAIN_CLOSE);
	m_CloseButton.SetHoverImage(IDB_GX_MAIN_CLOSE_HOVER);
	m_MaximizeButton.SetNormalImage(IDB_GX_MAIN_MAX);
	m_MaximizeButton.SetHoverImage(IDB_GX_MAIN_MAX_HOVER);
	m_MinimizeButton.SetNormalImage(IDB_GX_MAIN_MIN);
	m_MinimizeButton.SetHoverImage(IDB_GX_MAIN_MIN_HOVER);

	m_imageIcon.SetImage(_T(".\\res\\GX_Music_Icon.png"));

	ModifyStyle(WS_CLIPCHILDREN, 0);

	InitSongList();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGXMusicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

HCURSOR CGXMusicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGXMusicDlg::InitSongList()
{
	CFont font;
	CRect listRect;

	m_strCurMusicFilePath = "E:/ceobe/study/GXMusic/GXMusic/musicRes/会呼吸的痛.mp3";
	m_bPause = false;
	m_strDefaultDirectoryPath = "E:/ceobe/study/GXMusic/GXMusic/musicRes/*.mp3";

	font.CreatePointFont(250, _T("Arial"));

	m_SongInfoListCtrl.SetExtendedStyle(m_SongInfoListCtrl.GetExtendedStyle() | LVS_EX_TRANSPARENTBKGND | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	
	m_SongInfoListCtrl.SetFont(&font);
	m_SongInfoListCtrl.SetView(LV_VIEW_DETAILS);

	m_vecMusicFilePaths.clear();

	LoadDefaultMusics();
}

LRESULT CGXMusicDlg::OnNcHitTest(CPoint point) {
	if (!::IsWindow(m_hWnd))
		return HTERROR;
	// trans screen coordinate to client coordinate
	ScreenToClient(&point);

	// define region for window dragging
	CRect dragRegion;
	GetClientRect(&dragRegion);
	if (point.y >= 0 && point.y <= 30) {
		return HTCAPTION;
	}
	return CDialogEx::OnNcHitTest(point);
}

void CGXMusicDlg::OnMainButtonClicked()
{
	CWnd* pWnd = GetFocus();
	UINT nID = pWnd->GetDlgCtrlID();
	if (nID == IDC_GX_MAIN_CLOSE) {
		EndDialog(IDCANCEL);
	}
	else if (nID == IDC_GX_MAIN_MAX) {
		if (m_bIsWindowMaximized) {
			ShowWindow(SW_RESTORE);
			m_bIsWindowMaximized = FALSE;
		}
		else {
			ShowWindow(SW_MAXIMIZE);
			m_bIsWindowMaximized = TRUE;
		}
		
	}
	else if (nID == IDC_GX_MAIN_MIN) {
		ShowWindow(SW_MINIMIZE);
	}
}

void CGXMusicDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CDialogEx::OnGetMinMaxInfo(lpMMI);

	// 获取工作区大小
	RECT rcWorkArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);

	// 设置最大尺寸和最大位置
	lpMMI->ptMaxPosition.x = rcWorkArea.left;
	lpMMI->ptMaxPosition.y = rcWorkArea.top;
	lpMMI->ptMaxSize.x = rcWorkArea.right - rcWorkArea.left;
	lpMMI->ptMaxSize.y = rcWorkArea.bottom - rcWorkArea.top;
}

BOOL CGXMusicDlg::OnEraseBkgnd(CDC* pDC)
{
	if (m_pBackgroundBmp) {
		CRect rect;
		GetClientRect(&rect);

		Gdiplus::Graphics graphics(pDC->GetSafeHdc());
		Gdiplus::SolidBrush transparentBrush(Gdiplus::Color(m_opacity, 255, 255, 255));

		graphics.DrawImage(m_pBackgroundBmp,
			Gdiplus::Rect(0, 0, rect.Width(), rect.Height()),
			0, 0, m_pBackgroundBmp->GetWidth(), m_pBackgroundBmp->GetHeight(),
			Gdiplus::UnitPixel);

		// 绘制透明层
		for (auto& rect : m_vecTransparentBlockRects)
		{
			graphics.FillRectangle(&transparentBrush, rect);
		}
		return TRUE;
	}
	return CDialogEx::OnEraseBkgnd(pDC);
}

BOOL CGXMusicDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// 检查是否在按钮上
	if (pWnd == GetDlgItem(IDC_GX_MAIN_CLOSE) ||
		pWnd == GetDlgItem(IDC_GX_MAIN_MAX) || 
		pWnd == GetDlgItem(IDC_GX_MAIN_MIN) )
	{
		// 设置手形光标
		::SetCursor(m_hHandCursor);
		return TRUE;
	}

	// 使用默认光标
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CGXMusicDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)
	{
		// 更新窗口区域为圆角
		if (nType != SIZE_MAXIMIZED)
		{
			SetWindowRgn(CreateRoundRectRgn(0, 0, cx, cy, 20, 20), TRUE);
		}
		else
		{
			SetWindowRgn(NULL, TRUE); // 最大化时移除圆角
		}

		CRect rectClient;
		GetClientRect(&rectClient);

		// 计算宽度和高度的比例
		double widthRatio = static_cast<double>(rectClient.Width()) / m_initialDialogRect.Width();
		double heightRatio = static_cast<double>(rectClient.Height()) / m_initialDialogRect.Height();

		// 调整控件的位置
		for (size_t i = 0; i < m_vecControlIDs.size(); ++i)
		{
			CRect rcNew = m_vecControlRects[i];

			// 根据比例调整控件位置
			rcNew.left = static_cast<int>(m_vecControlRects[i].left * widthRatio);
			rcNew.top = static_cast<int>(m_vecControlRects[i].top * heightRatio);
			rcNew.right = rcNew.left + m_vecControlRects[i].Width();
			rcNew.bottom = rcNew.top + m_vecControlRects[i].Height();

			GetDlgItem(m_vecControlIDs[i])->MoveWindow(rcNew, FALSE);
		}
		// 调整控件的位置和大小
		for (size_t i = 0; i < m_vecScaleControlIDs.size(); ++i)
		{
			CRect rcNew = m_vecScaleControlRects[i];

			// 根据比例调整控件位置和大小
			rcNew.left = static_cast<int>(m_vecScaleControlRects[i].left * widthRatio);
			rcNew.top = static_cast<int>(m_vecScaleControlRects[i].top * heightRatio);
			rcNew.right = static_cast<int>(m_vecScaleControlRects[i].right * widthRatio);
			rcNew.bottom = static_cast<int>(m_vecScaleControlRects[i].bottom * heightRatio);

			GetDlgItem(m_vecScaleControlIDs[i])->MoveWindow(rcNew, FALSE);
		}

		Invalidate();
	}
}

#pragma region music control related

static std::string Data2String(TagLib::String str)
{
	std::wstring wstr = str.toWString();

	int size = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &result[0], size, nullptr, nullptr);
	result.pop_back();

	return result;
}

static std::string Data2String(CString cstr)
{
	int size = WideCharToMultiByte(CP_ACP, 0, cstr.GetString(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_ACP, 0, cstr.GetString(), -1, &result[0], size, nullptr, nullptr);
	result.pop_back();

	return result;
}

static std::string GetMusicDuration(const TagLib::FileRef& file)
{
	std::string strTime, strMinute, strSecond;
	int totalSeconds = 0;
	int minutes = 0;
	int seconds = 0;

	TagLib::AudioProperties* audioProps = file.audioProperties();

	if (!audioProps) {
		std::cerr << "Error: No audio properties found." << std::endl;
		goto wrapup;
	}

	totalSeconds = audioProps->lengthInSeconds();
	minutes = totalSeconds / 60;
	seconds = totalSeconds % 60;

	strMinute = minutes < 10 ? ("0" + std::to_string(minutes)) : std::to_string(minutes);
	strSecond = seconds < 10 ? ("0" + std::to_string(seconds)) : std::to_string(seconds);
	strTime = strMinute + ":" + strSecond;
wrapup:
	return strTime;
}

HRESULT CGXMusicDlg::LoadDefaultMusics()
{
	HRESULT hr = S_OK;
	std::string strTitle;
	std::string strArtist;
	std::string strAlbum;
	std::string strYear;
	std::string strTime;
	CRect rect;
	int nWidth = 0;
	int nCounter = 0;
	CFileFind musicFinder;

	TagLib::ID3v2::FrameFactory::instance()->setDefaultTextEncoding(TagLib::String::UTF8);

	m_SongInfoListCtrl.GetClientRect(&rect);

	BOOL bWorking = musicFinder.FindFile(m_strDefaultDirectoryPath);

	nWidth = (int)(rect.Width() * 0.3);
	m_SongInfoListCtrl.InsertColumn(0, _T("歌名"), LVCFMT_LEFT, nWidth);
	nWidth = (int)(rect.Width() * 0.3);
	m_SongInfoListCtrl.InsertColumn(1, _T("歌手"), LVCFMT_LEFT, nWidth);
	nWidth = (int)(rect.Width() * 0.3);
	m_SongInfoListCtrl.InsertColumn(2, _T("专辑"), LVCFMT_LEFT, nWidth);
	nWidth = (int)(rect.Width() * 0.1);
	m_SongInfoListCtrl.InsertColumn(3, _T("时长"), LVCFMT_LEFT, nWidth);

	while (bWorking)
	{
		bWorking = musicFinder.FindNextFileW();

		if (musicFinder.IsDirectory())
		{
			continue;
		}

		CString cstrFile = musicFinder.GetFilePath();
		std::string strFile = Data2String(cstrFile);

		if (!strFile.empty())
		{

			TagLib::FileRef file(strFile.c_str());

			if (!file.isNull() && file.tag())
			{
				TagLib::Tag* tag = file.tag();

				strTitle = Data2String(tag->title());
				strAlbum = Data2String(tag->album());
				strArtist = Data2String(tag->artist());		
				strTime = GetMusicDuration(file);
			}
			else {
				std::cerr << "无法读取文件或文件没有有效标签" << std::endl;
			}

			m_SongInfoListCtrl.InsertItem(nCounter, (CString)strTitle.c_str());
			m_SongInfoListCtrl.SetItemText(nCounter, 1, (CString)strArtist.c_str());
			m_SongInfoListCtrl.SetItemText(nCounter, 2, (CString)strAlbum.c_str());
			m_SongInfoListCtrl.SetItemText(nCounter, 3, (CString)strTime.c_str());

			m_vecMusicFilePaths.push_back(cstrFile);

			nCounter++;
		}
	}

	return hr;
}

void CGXMusicDlg::PlayMusic()
{
	std::string openCommand = "open \"" + std::string(CT2A(m_strCurMusicFilePath)) +
		"\" type mpegvideo alias music";
	mciSendStringA(openCommand.c_str(), NULL, 0, NULL);

	mciSendStringA("play music", NULL, 0, NULL);
}

void CGXMusicDlg::PauseOrPlayMusic(bool bPause)
{
	if (bPause)
	{
		mciSendStringA("stop music", NULL, 0, NULL);
	}
	else
	{
		mciSendStringA("play music", NULL, 0, NULL);
	}
}

void CGXMusicDlg::StopMusic()
{
	mciSendStringA("stop music", NULL, 0, NULL);
	mciSendStringA("close music", NULL, 0, NULL);
}
void CGXMusicDlg::SetMusicVolume(int nVolume)
{
	std::string cmd = "setaudio music volume to " + std::to_string(nVolume);
	mciSendStringA(cmd.c_str(), NULL, 0, NULL);
}

void CGXMusicDlg::OnMusicListDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	int nItem = pNMListView->iItem;  // 获取双击的行号
	if (nItem != -1) {
		StopMusic();

		m_strCurMusicFilePath = m_vecMusicFilePaths[nItem];

		PlayMusic();
	}

	*pResult = 0;
}
#pragma endregion
