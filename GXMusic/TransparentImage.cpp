#include "pch.h"
#include "TransparentImage.h"

BEGIN_MESSAGE_MAP(TransparentImage, CStatic)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

TransparentImage::TransparentImage() : m_pImage(nullptr) {}

TransparentImage::~TransparentImage()
{
    delete m_pImage;
}

void TransparentImage::SetImage(const CString& path)
{
    m_pImage = new Gdiplus::Image(path);
}

void TransparentImage::OnPaint()
{
    CPaintDC dc(this);
    Gdiplus::Graphics graphics(dc);

    if (m_pImage)
    {
        // 获取控件的客户区
        CRect rect;
        GetClientRect(&rect);

        // 获取图片的原始尺寸
        UINT imgWidth = m_pImage->GetWidth();
        UINT imgHeight = m_pImage->GetHeight();

        // 计算等比例缩放
        float aspectRatio = static_cast<float>(imgWidth) / imgHeight;
        int drawWidth = rect.Width();
        int drawHeight = static_cast<int>(drawWidth / aspectRatio);

        if (drawHeight > rect.Height())
        {
            drawHeight = rect.Height();
            drawWidth = static_cast<int>(drawHeight * aspectRatio);
        }

        // 计算居中位置
        int x = (rect.Width() - drawWidth) / 2;
        int y = (rect.Height() - drawHeight) / 2;

        // 绘制图片
        graphics.DrawImage(m_pImage, x, y, drawWidth, drawHeight);
    }
}

BOOL TransparentImage::OnEraseBknd(CDC* pDC)
{
    return TRUE;
}