#include "define.h"
#include <ddraw.h>	
#include <dinput.h>	

#include "Bitmap.h"	
#include "Sprite.h"
#include "Timer.h"

#include "stdio.h"	
#include <windows.h>

#include <dsound.h>	
#include "dsutil.h"
#include <time.h>
#include <stdlib.h>

extern LPDIRECTDRAWSURFACE7	    g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7		g_lpSecondarySurface;
extern LPDIRECTINPUT8			g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8		g_lpDirectInputKeyboard;
extern CTimer					g_Timer;

extern CSprite g_GameSetSprite;
extern void Fliping();

extern int mainID;
extern int mainChoose;
extern bool gamereset;

extern BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);

void MainScreen(void) {
	g_GameSetSprite.Drawing2(0, 683, 384, g_lpSecondarySurface, false);

	Fliping();
}