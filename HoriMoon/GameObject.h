
#ifndef  __gobject_h__
#define __gobject_h__

#include "sprite.h"
#include "timer.h"
#include <ddraw.h>
#include <cmath>
#include <stdlib.h>
#include <time.h>

class GameObject {
public:
	int m_x;
	int m_y;
protected:
	CSprite *m_pSprite;
	int m_nFrameInterval;
	int m_nCurrentFrame;
	int m_nLastFrameTime;
	CTimer* m_pTimer;
	bool m_blsLive;

public:
	GameObject();
	~GameObject();
	void Initialize(CSprite *pSprite, int x, int y, CTimer *timer, int CurrentFrame, int FrameInterval);
	void Draw(int x, int y, LPDIRECTDRAWSURFACE7 IpSurface);
	void DrawFrame(int x, int y, int nFrame, LPDIRECTDRAWSURFACE7 IpSurface);
	bool IsLive();
	void Kill();
	int GetX();
	int GetY();
};
#endif // ! __gobject_h__
