
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
	//���� ����
	HANDLE hFile;
	DWORD actualRead;

	hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, (LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// 1.BITMAPFILEHEADER �б�
	BITMAPFILEHEADER BmpFileHead;

	if (!ReadFile(hFile, &BmpFileHead, sizeof(BmpFileHead), &actualRead, NULL)) {
		CloseHandle(hFile);
		return false;
	}

	// bitmap file���� �����Ѵ�.
	if (BmpFileHead.bfType != 0x4D42) {
		CloseHandle(hFile);
		return false;
	}

	// 2.BITMAPINFOHEADER �б�
	BITMAPINFOHEADER Bmpinfohead;

	if (!ReadFile(hFile, &Bmpinfohead, sizeof(Bmpinfohead), &actualRead, NULL)) {
		CloseHandle(hFile);
		return false;
	}

	//24bits RGB bmp file���� �����Ѵ�.
	if (Bmpinfohead.biBitCount != 24) {
		CloseHandle(hFile);
		return false;
	}

	//bottom-up ������ �����ϰ� �ִ� bmp file �ΰ��� �����Ѵ�
	int nHeight = Bmpinfohead.biHeight;
	bool bBottomUp; // bmp file	��������� bottom-up�̸� true, �ƴϸ� false

	if (nHeight > 0)
		bBottomUp = true;
	else {
		bBottomUp = false;
		nHeight = -nHeight;
	}

	m_nHeight = nHeight;
	m_nWidth = Bmpinfohead.biWidth;

	// 3.Pixel data�� buffer�� �о� ���̱�
	// buffer�� �����ϰ� pixel data�� ���� �غ� �Ѵ�.
	if (m_pBuffer)
		delete[] m_pBuffer;

	m_pBuffer = new unsigned char[m_nHeight *m_nWidth * 4];

	// bmp���Ϸκ��� pixel data�� �д´�.
	struct RGBstruct {
		unsigned char B;
		unsigned char G;
		unsigned char R;
	};

	struct RGBstruct rgb24;

	int nStoredLine = (m_nWidth * 3 + 3)&~3;		//bmp file�� ����� �� ���� byte ��
	unsigned char temp[4];							//bmp file�� �� ���� �� �κ� �� ����.
	int nRemainder = nStoredLine - (m_nWidth * 3);	//bmp file�� �� �� �� �κп� �߰��� byte ��

	int nDestY, nDeltaY;							//bottom-up or top-down�� ���� �� ����.

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
			*(m_pBuffer + (x << 2) + nDestY *(m_nWidth << 2) + 3) = 0;				// Alpha ���� 0����
		}
		nDestY += nDeltaY;

		if (!ReadFile(hFile, temp, nRemainder, &actualRead, NULL)) {
			CloseHandle(hFile);
			delete[]m_pBuffer;
			return false;
		}
	}

	// ���� �ݱ�
	CloseHandle(hFile);
	return true;
}

bool CBITMAP::CopyBufferToSurface(LPDIRECTDRAWSURFACE7 IpSurface) {
	if (!m_pBuffer) {																// ���� ��� ���� ����
		return false;
	}

	// 1. Lock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (FAILED(IpSurface->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL))) {
		return false;
	}

	// 2. buffer���� Surface�� ����
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