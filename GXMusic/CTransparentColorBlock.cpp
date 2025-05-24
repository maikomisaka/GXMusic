#include "pch.h"
#include "CTransparentColorBlock.h"

BEGIN_MESSAGE_MAP(CTransparentColorBlock, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CTransparentColorBlock::CTransparentColorBlock()
    : m_color(RGB(255, 255, 255)),
    m_opacity(22) // 默认不透明度为128，半透明
{}

CTransparentColorBlock::~CTransparentColorBlock()
{
}

void CTransparentColorBlock::SetColor(COLORREF color)
{
    m_color = color;
    Invalidate(); // 触发重绘
}

void CTransparentColorBlock::SetOpacity(int opacity)
{
    m_opacity = opacity;
    Invalidate(); // 触发重绘
}

void CTransparentColorBlock::OnPaint()
{
    CPaintDC dc(this); // 设备上下文

    CRect rect;
    GetClientRect(&rect);

    // 创建内存DC和兼容位图
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    // 设置透明度
    memDC.FillSolidRect(&rect, RGB(255, 255, 255)); // 白色背景
    CBrush brush;
    brush.CreateSolidBrush(m_color);
    CBrush* pOldBrush = memDC.SelectObject(&brush);
    memDC.FillSolidRect(&rect, m_color);
    memDC.SelectObject(pOldBrush);

    // AlphaBlend的BLENDFUNCTION结构体设置
    BLENDFUNCTION blendFunc;
    blendFunc.BlendOp = AC_SRC_OVER;
    blendFunc.BlendFlags = 0;
    blendFunc.SourceConstantAlpha = m_opacity;
    blendFunc.AlphaFormat = 0;

    dc.AlphaBlend(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, rect.Width(), rect.Height(), blendFunc);

    memDC.SelectObject(pOldBitmap);
}

BOOL CTransparentColorBlock::OnEraseBkgnd(CDC* pDC)
{
    return TRUE; // 不擦除背景
}
