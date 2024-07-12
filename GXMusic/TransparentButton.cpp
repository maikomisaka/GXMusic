#include "pch.h"
#include "TransparentButton.h"

BEGIN_MESSAGE_MAP(CTransparentButton, CButton)
	ON_WM_ERASEBKGND()
    ON_WM_DRAWITEM()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

CTransparentButton::CTransparentButton()
    : m_pNormalImage(NULL), m_pHoverImage(NULL), m_bHover(FALSE)
{
}

CTransparentButton::~CTransparentButton()
{
    delete m_pNormalImage;
    delete m_pHoverImage;
}

void CTransparentButton::SetNormalImage(UINT nIDResource)
{
    LoadImageFromResource(nIDResource, &m_pNormalImage);
}

void CTransparentButton::SetHoverImage(UINT nIDResource)
{
    LoadImageFromResource(nIDResource, &m_pHoverImage);
}

void CTransparentButton::LoadImageFromResource(UINT nIDResource, Gdiplus::Image** ppImage)
{
    HINSTANCE hInst = AfxGetInstanceHandle();
    HRSRC hResource = FindResource(hInst, MAKEINTRESOURCE(nIDResource), _T("PNG"));
    if (hResource)
    {
        DWORD imageSize = SizeofResource(hInst, hResource);
        if (imageSize)
        {
            const void* pResourceData = LockResource(LoadResource(hInst, hResource));
            if (pResourceData)
            {
                HGLOBAL hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
                if (hBuffer)
                {
                    void* pBuffer = GlobalLock(hBuffer);
                    if (pBuffer)
                    {
                        CopyMemory(pBuffer, pResourceData, imageSize);

                        IStream* pStream = NULL;
                        if (CreateStreamOnHGlobal(hBuffer, FALSE, &pStream) == S_OK)
                        {
                            *ppImage = Gdiplus::Image::FromStream(pStream);
                            pStream->Release();
                        }

                        GlobalUnlock(hBuffer);
                    }
                    GlobalFree(hBuffer);
                }
            }
        }
    }
}

BOOL CTransparentButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTransparentButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    Gdiplus::Graphics graphics(pDC->GetSafeHdc());

    // 计算图片的目标矩形，保持纵横比
    int imgWidth = m_pHoverImage->GetWidth();
    int imgHeight = m_pHoverImage->GetHeight();
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

    if (m_bHover && m_pHoverImage)
    {
        graphics.DrawImage(m_pHoverImage, x, y, targetWidth, targetHeight);
    }
    else if (m_pNormalImage)
    {
        graphics.DrawImage(m_pNormalImage, x, y, targetWidth, targetHeight);
    }
}

void CTransparentButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bHover)
    {
        m_bHover = TRUE;
        Invalidate();

        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        TrackMouseEvent(&tme);
    }

    CButton::OnMouseMove(nFlags, point);
}

void CTransparentButton::OnMouseLeave()
{
    m_bHover = FALSE;
    Invalidate();

    CButton::OnMouseLeave();
}