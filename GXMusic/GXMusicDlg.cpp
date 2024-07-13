
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
}

BEGIN_MESSAGE_MAP(CGXMusicDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_GX_MAIN_CLOSE, OnMainButtonClicked)	
	ON_BN_CLICKED(IDC_GX_MAIN_MAX, OnMainButtonClicked)
	ON_BN_CLICKED(IDC_GX_MAIN_MIN, OnMainButtonClicked)
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
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

	// Background setting
	m_bgImage.Load(_T(".\\res\\GX_App_Background.png"));

	m_CloseButton.SetNormalImage(IDB_GX_MAIN_CLOSE);
	m_CloseButton.SetHoverImage(IDB_GX_MAIN_CLOSE_HOVER);
	m_MaximizeButton.SetNormalImage(IDB_GX_MAIN_MAX);
	m_MaximizeButton.SetHoverImage(IDB_GX_MAIN_MAX_HOVER);
	m_MinimizeButton.SetNormalImage(IDB_GX_MAIN_MIN);
	m_MinimizeButton.SetHoverImage(IDB_GX_MAIN_MIN_HOVER);

	m_imageIcon.SubclassDlgItem(IDC_GX_MAIN_ICON, this);
	m_imageIcon.SetImage(_T(".\\res\\GX_Music_Icon.png"));
	m_staticTitle.SubclassDlgItem(IDC_GX_MAIN_TITLE, this);
	// Transparent Block Setting
	m_blockMain.SubclassDlgItem(IDC_GX_MAIN_BLOCK, this);

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

void CGXMusicDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		BYTE alpha = 255;	// 0-255
		SetBackgroundAndTransparency(alpha);
		
	}
}

void CGXMusicDlg::SetBackgroundAndTransparency(BYTE alpha)
{
	CPaintDC dc(this); // device context for painting

	// 获取客户区尺寸
	CRect rect;
	GetClientRect(&rect);

	// 创建一个内存 DC
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	// 创建一个兼容的位图
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	// 绘制背景图片
	m_bgImage.Draw(memDC.GetSafeHdc(), 0, 0, rect.Width(), rect.Height());

	// 设置透明度（假设透明度为 128，即 50% 透明）
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = alpha; // 透明度值 (0-255)
	bf.AlphaFormat = 0;

	// 将内存 DC 中的内容拷贝到屏幕 DC
	dc.AlphaBlend(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), bf);

	memDC.SelectObject(pOldBmp);
}

LRESULT CGXMusicDlg::OnNcHitTest(CPoint point) {
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
	return TRUE;
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