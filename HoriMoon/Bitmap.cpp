
#include <Windows.h>
#include <windowsx.h>
#include "Bitmap.h"

CBITMAP::CBITMAP() {
	m_pBuffer = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
}
CBITMAP::~CBITMAP() {
	if (m_pBuffer)
		delete[] m_pBuffer;
}
int CBITMAP::GetHeight() {
	return (m_nHeight);
}
int CBITMAP::GetWidth() {
	return (m_nWidth);
}

bool CBITMAP::LoadBMPFile(char *filename) {
	//파일 열기
	HANDLE hFile;
	DWORD actualRead;

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// 1.BITMAPFILEHEADER 읽기
	BITMAPFILEHEADER BmpFileHead;

	if (!ReadFile(hFile, &BmpFileHead, sizeof(BmpFileHead), &actualRead, NULL)) {
		CloseHandle(hFile);
		return false;
	}

	// bitmap file인지 점검한다.
	if (BmpFileHead.bfType != 0x4D42) {
		CloseHandle(hFile);
		return false;
	}

	// 2.BITMAPINFOHEADER 읽기
	BITMAPINFOHEADER Bmpinfohead;

	if (!ReadFile(hFile, &Bmpinfohead, sizeof(Bmpinfohead), &actualRead, NULL)) {
		CloseHandle(hFile);
		return false;
	}

	//24bits RGB bmp file인지 점검한다.
	if (Bmpinfohead.biBitCount != 24) {
		CloseHandle(hFile);
		return false;
	}

	//bottom-up 순서로 저장하고 있는 bmp file 인가를 점검한다
	int nHeight = Bmpinfohead.biHeight;
	bool bBottomUp; // bmp file	저장순서가 bottom-up이면 true, 아니면 false

	if (nHeight > 0)
		bBottomUp = true;
	else {
		bBottomUp = false;
		nHeight = -nHeight;
	}

	m_nHeight = nHeight;
	m_nWidth = Bmpinfohead.biWidth;

	// 3.Pixel data를 buffer에 읽어 들이기
	// buffer를 생성하고 pixel data를 읽을 준비를 한다.
	if (m_pBuffer)
		delete[] m_pBuffer;

	m_pBuffer = new unsigned char[m_nHeight *m_nWidth * 4];

	// bmp파일로부터 pixel data를 읽는다.
	struct RGBstruct {
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct RGBstruct rgb24;

	int nStoredLine = (m_nWidth * 3 + 3)&~3;		//bmp file에 저장된 한 줄의 byte 수
	unsigned char temp[4];							//bmp file의 한 줄의 끝 부분 값 읽음.
	int nRemainder = nStoredLine - (m_nWidth * 3);	//bmp file의 한 줄 끝 부분에 추가한 byte 수

	int nDestY, nDeltaY;							//bottom-up or top-down에 따라 값 결정.

	if (bBottomUp) {
		nDestY = nHeight - 1;
		nDeltaY = -1;
	}
	else {
		nDestY = 0;
		nDeltaY = 1;
	}

	for (int y = 0; y<nHeight; y++) {
		for (int x = 0; x<m_nWidth; x++) {
			if (!ReadFile(hFile, &rgb24, 3, &actualRead, NULL)) {
				CloseHandle(hFile);
				delete[]m_pBuffer;
				return false;
			}
			CopyMemory(m_pBuffer + (x << 2) + nDestY *(m_nWidth << 2), &rgb24, 3);
			*(m_pBuffer + (x << 2) + nDestY *(m_nWidth << 2) + 3) = 0;				// Alpha 값을 0으로
		}
		nDestY += nDeltaY;

		if (!ReadFile(hFile, temp, nRemainder, &actualRead, NULL)) {
			CloseHandle(hFile);
			delete[]m_pBuffer;
			return false;
		}
	}

	// 파일 닫기
	CloseHandle(hFile);
	return true;
}

bool CBITMAP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 IpSurface) {
	if (!m_pBuffer) {																// 버퍼 비면 동작 중지
		return false;
	}

	// 1. Lock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (FAILED(IpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) {
		return false;
	}

	// 2. buffer에서 Surface로 복사
	unsigned char *pDest, *pSrc;

	pDest = (unsigned char *)ddsd.lpSurface;
	pSrc = m_pBuffer;

	for (int y = 0; y < m_nHeight; y++) {
		CopyMemory(pDest, pSrc, m_nWidth << 2);
		pDest += ddsd.lPitch;
		pSrc += (m_nWidth << 2);
	}

	// 3.Unlock()
	IpSurface->Unlock(NULL);

	return true;
}