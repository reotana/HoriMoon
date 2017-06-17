
#ifndef __bmp_h__
#define __bmp_h__

#include <ddraw.h>

class CBITMAP {
private:
	int m_nWidth;
	int m_nHeight;
	unsigned char * m_pBuffer;

public:
	CBITMAP();
	~CBITMAP();
	int GetWidth();
	int GetHeight();
	bool LoadBMPFile(char* filename);
	bool CopyBufferToSurface(LPDIRECTDRAWSURFACE7 IpSurface);
};


#endif // !__bmp_h__
