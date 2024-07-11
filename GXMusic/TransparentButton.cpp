#include "pch.h"
#include "TransparentButton.h"

BEGIN_MESSAGE_MAP(CTransparentButton, CButton)
	ON_WM_ERASEBKGND()
    ON_WM_DRAWITEM()
END_MESSAGE_MAP()

CTransparentButton::CTransparentButton()
    : m_pImage(NULL)
{
}

CTransparentButton::~CTransparentButton()
{
    delete m_pImage;
}

BOOL CTransparentButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTransparentButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;

    //// 如果按钮被按下，改变背景色
    //if (lpDrawItemStruct->itemState & ODS_SELECTED)
    //{
    //    pDC->FillSolidRect(&rect, RGB(0, 0, 0)); // 灰色背景
    //}

    // 绘制PNG图片
    if (m_pImage)
    {
        Gdiplus::Graphics graphics(pDC->GetSafeHdc());
        // 计算图片的目标矩形，保持纵横比
        int imgWidth = m_pImage->GetWidth();
        int imgHeight = m_pImage->GetHeight();
        float imgAspect = static_cast<float>(imgWidth) / imgHeight;

        int targetWidth = rect.Width();
        int targetHeight = static_cast<int>(targetWidth / imgAspect);

        if (targetHeight > rect.Height())
        {
            targetHeight = rect.Height();
            targetWidth = static_cast<int>(targetHeight * imgAspect);
        }

        int x = (rect.Width() - targetWidth) / 2;
        int y = (rect.Height() - targetHeight) / 2;

        graphics.DrawImage(m_pImage, x, y, targetWidth, targetHeight);
    }
}

BOOL CTransparentButton::LoadPngImage(UINT nIDResource)
{
    HRSRC hResource = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDResource), _T("PNG"));
    if (!hResource)
        return FALSE;

    DWORD imageSize = ::SizeofResource(AfxGetInstanceHandle(), hResource);
    if (!imageSize)
        return FALSE;

    const void* pResourceData = ::LockResource(::LoadResource(AfxGetInstanceHandle(), hResource));
    if (!pResourceData)
        return FALSE;

    HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (!hBuffer)
        return FALSE;

    void* pBuffer = ::GlobalLock(hBuffer);
    if (!pBuffer)
    {
        ::GlobalFree(hBuffer);
        return FALSE;
    }

    CopyMemory(pBuffer, pResourceData, imageSize);

    IStream* pStream = NULL;
    if (::CreateStreamOnHGlobal(hBuffer, TRUE, &pStream) != S_OK)
    {
        ::GlobalUnlock(hBuffer);
        ::GlobalFree(hBuffer);
        return FALSE;
    }

    m_pImage = Gdiplus::Bitmap::FromStream(pStream);
    pStream->Release();

    if (m_pImage->GetLastStatus() != Gdiplus::Ok)
    {
        delete m_pImage;
        m_pImage = NULL;
        return FALSE;
    }

    return TRUE;
}
