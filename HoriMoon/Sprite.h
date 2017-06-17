#ifndef __sprite_h__
#define __sprite_h__

#include <ddraw.h>
#include "Bitmap.h"

class CSprite {
private:
	int m_nFrame;
	int m_nWidth;
	int m_nHeight;
	LPDIRECTDRAWSURFACE7 *m_ppSurface;
	CBITMAP *m_pBMPArray;

public:
	CSprite();
	~CSprite();
	int GetNumberOfFrame();
	bool InitSprite(int nFrame, int nWidth, int nHeight, int nColorKey, LPDIRECTDRAW7 pDirectDraw);
	bool LoadFrame(int nFrame, char *filename);
	bool Drawing(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Drawing2(int nFrame, int x, int y, LPDIRECTDRAWSURFACE7 pSurface, bool bUsingColorKey = true);
	bool Restore();
	bool ReleaseAll();
};
#endif // !__sprite_h__
