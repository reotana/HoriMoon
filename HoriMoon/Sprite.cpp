//--------------------------------------------------------------------------------------------
//sprite.cpp	CSprite class - DiretX 7.0 이상
//--------------------------------------------------------------------------------------------

#include<windows.h>
#include"Sprite.h"

static RECT destRect;
static RECT destRect2;

CSprite::CSprite() {
	destRect.left = 0;
	destRect.top = 0;
	destRect.right = 1366;
	destRect.bottom = 768;

	destRect2.left = 0;
	destRect2.top = -768;
	destRect2.right = 1366;
	destRect2.bottom = 0;

	m_ppSurface = NULL;
	m_nFrame = 0;
	m_pBMPArray = NULL;
}

CSprite::~CSprite() {

	if (m_ppSurface)
		delete[]m_ppSurface;
	if (m_pBMPArray)
		delete[]m_pBMPArray;
}
//--------------------------------------------------------------------------------------------
//  Sprite 초기화
//--------------------------------------------------------------------------------------------

bool CSprite::InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw) {
	if (m_ppSurface)
		delete[]m_ppSurface;
	if (m_pBMPArray)
		delete[]m_pBMPArray;

	//DirectDrawSurface를 nFrame 갯수 만큼 준비한다.
	m_ppSurface = new LPDIRECTDRAWSURFACE7[nFrame];

	if (!m_ppSurface)
		return false;

	//CBMP를 nFrame 갯수 만큼 준비한다.
	m_pBMPArray = new CBITMAP[nFrame];

	if (!m_pBMPArray)
		return false;

	//한 프레임에 해당하는 Surface를 만들 준비를 한다.
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = nHeight;
	ddsd.dwWidth = nWidth;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ddck.dwColorSpaceHighValue = nColorKey;

	for (int i = 0; i < nFrame; ++i) {
		if (FAILED(pDirectDraw->CreateSurface(&ddsd, &m_ppSurface[i], NULL))) {
			m_ppSurface[i] = NULL;
			return false;
		}
		else
			m_ppSurface[i]->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nFrame = nFrame;

	return true;
}

bool CSprite::LoadFrame(int nFrame, char *filename) {

	if (!m_pBMPArray[nFrame].LoadBMPFile(filename))
		return false;
	if (!m_pBMPArray[nFrame].CopyBufferToSurface(m_ppSurface[nFrame]))
		return false;

	return true;
}

bool CSprite::Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) {
	RECT destRect;

	destRect.left = x - (m_nWidth >> 1);
	destRect.top = y - (m_nHeight >> 1);
	destRect.right = destRect.left + m_nWidth;
	destRect.bottom = destRect.top + m_nHeight;

	HRESULT hResult;

	if (bUsingColorKey)
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL))) {

			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
		}
		else
			if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL))) {
				if (hResult == DDERR_SURFACELOST)
					return (Restore());
				else
					return false;
			}

	return false;
}

bool CSprite::Restore() {

	if (!m_ppSurface)
		return false;

	for (int i = 0; i < m_nFrame; ++i)
	{
		if (!m_ppSurface[i])
			return false;
		if (FAILED(m_ppSurface[i]->Restore()))
			return false;
		if (!m_pBMPArray[i].CopyBufferToSurface(m_ppSurface[i]))
			return false;
	}

	return true;
}

int CSprite::GetNumberOfFrame() {
	return m_nFrame;
}

bool CSprite::ReleaseAll() {
	if (!m_ppSurface)
		return false;

	for (int i = 0; i < m_nFrame; ++i)
		if (m_ppSurface[i])
			m_ppSurface[i]->Release();

	return false;
}

//화면 종 스크롤
bool CSprite::Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey) {

	HRESULT hResult;
	if (bUsingColorKey) {
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return false;
	}
	else
	{
		if (FAILED(hResult = pSurface->Blt(&destRect, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL)))
			if (hResult == DDERR_SURFACELOST)
				return (Restore());

		if (FAILED(hResult = pSurface->Blt(&destRect2, m_ppSurface[nFrame], NULL, DDBLT_WAIT, NULL))) {
			if (hResult == DDERR_SURFACELOST)
				return (Restore());
			else
				return false;
		}
	}
	return true;
}
