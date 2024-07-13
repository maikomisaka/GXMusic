
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
	ON_WM_SIZE()
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
	}
	GetClientRect(&m_initialDialogRect);

	// Background setting
	m_bgImage.Load(_T(".\\res\\GX_App_Background.png"));
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
	BYTE alpha = 255;	// 0-255
	SetBackgroundAndTransparency(alpha);
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

void CGXMusicDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType != SIZE_MINIMIZED)
	{
		// 更新窗口区域为圆角
		SetWindowRgn(CreateRoundRectRgn(0, 0, cx, cy, 20, 20), TRUE);
		Invalidate();

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

			GetDlgItem(m_vecControlIDs[i])->MoveWindow(rcNew);
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

			GetDlgItem(m_vecScaleControlIDs[i])->MoveWindow(rcNew);
		}
	}
}