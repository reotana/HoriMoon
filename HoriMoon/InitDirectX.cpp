//--------------------------------------------------------------------------------------------
//DirectDraw
//DirectInput
//--------------------------------------------------------------------------------------------

#include <windows.h>
#include <windowsx.h>

#include <dsound.h>
#include "DSUTIL.h"

#include <ddraw.h>
#include <dinput.h>

#include "define.h"

//--------------------------------------------------------------------------------------------
// 함수 원형 선언
//--------------------------------------------------------------------------------------------
bool InitDirectDraw(void);
bool InitDirectInput(void);

//--------------------------------------------------------------------------------------------
// 외부 전역 변수 선언
//--------------------------------------------------------------------------------------------
extern HWND		 hWnd;
extern HINSTANCE g_hInstance;

extern LPDIRECTDRAW7		g_lpDirectDrawObject;
extern LPDIRECTDRAWSURFACE7 g_lpPrimarySurface;
extern LPDIRECTDRAWSURFACE7 g_lpSecondarySurface;

extern LPDIRECTINPUT8		g_lpDirectInputObject;
extern LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard;

LPDIRECTSOUND		SoundOBJ;
LPDIRECTSOUNDBUFFER SoundDSB;
DSBUFFERDESC		DSB_desc;
HSNDOBJ				Sound[10];

//--------------------------------------------------------------------------------------------
// DirectX 초기화, 실패하면 false, 수행 성공은 true
//--------------------------------------------------------------------------------------------
bool InitializeDirectX(void) {
	if (!InitDirectDraw())		return false;
	if (!InitDirectInput())		return false;

	return true;
}

bool InitDirectDraw(void) {
	// DirectDraw Object 생성
	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&g_lpDirectDrawObject, IID_IDirectDraw7, NULL)))		return false;
	// 협력수준 설정
	if (FAILED(g_lpDirectDrawObject->SetCooperativeLevel(hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)))		return false;
	// 화면 해상도 설정
	if (FAILED(g_lpDirectDrawObject->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH, 0, 0)))		return false;

	// 각종 DirectDrawSurface 생성
	// Primary Surface 생성
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;

	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;

	if (FAILED(g_lpDirectDrawObject->CreateSurface(&ddsd, &g_lpPrimarySurface, NULL)))		return false;

	// Secondary Surface 생성
	DDSCAPS2 ddscaps;
	ZeroMemory(&ddscaps, sizeof(ddscaps));
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

	if (FAILED(g_lpPrimarySurface->GetAttachedSurface(&ddscaps, &g_lpSecondarySurface)))		return false;

	// Clipper 생성 및 설정
	LPDIRECTDRAWCLIPPER lpClipper;

	if (FAILED(g_lpDirectDrawObject->CreateClipper(NULL, &lpClipper, NULL)))		return false;
	if (FAILED(lpClipper->SetHWnd(NULL, hWnd)))		return false;
	if (FAILED(g_lpSecondarySurface->SetClipper(lpClipper)))		return false;

	return true;
}
bool InitDirectInput(void) {
	// DirectInput Object 생성
	if (FAILED(DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID *)&g_lpDirectInputObject, NULL)))		return false;
	// DirectInput Device 생성
	if (FAILED(g_lpDirectInputObject->CreateDevice(GUID_SysKeyboard, &g_lpDirectInputKeyboard, NULL)))		return false;
	// 협력 수준 설정
	if (FAILED(g_lpDirectInputKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))		return false;
	// Data 형식 설정
	if (FAILED(g_lpDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard)))		return false;
	// Acquire
	g_lpDirectInputKeyboard->Acquire();

	return true;
}

//--------------------------------------------------------------------------------------------
//  Sound
//--------------------------------------------------------------------------------------------
//BOOL _InitDirectSound(void) {
//	if (DirectSoundCreate(NULL, &SoundOBJ, NULL) == DS_OK) {
//		if (SoundOBJ->SetCooperativeLevel(hWnd, DSSCL_PRIORITY) != DS_OK)			return FALSE;
//		memset(&DSB_desc, 0, sizeof(DSBUFFERDESC));
//		DSB_desc.dwSize = sizeof(DSBUFFERDESC);
//		DSB_desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
//
//		if (SoundOBJ->CreateSoundBuffer(&DSB_desc, &SoundDSB, NULL) != DS_OK)			return FALSE;
//		SoundDSB->SetVolume(0);
//		SoundDSB->SetPan(0);
//
//		//----- 사운드 배열에 각각의 BGM을 넣어놓는다. -----//
//		Sound[0] = SndObjCreate(SoundOBJ, "sound\\main2.wav", 1);			// 배경 음악
//		Sound[1] = SndObjCreate(SoundOBJ, "sound\\gun.wav", 2);			// 플레이어 총알 발사
//		Sound[2] = SndObjCreate(SoundOBJ, "sound\\explosion.wav", 3);	// 폭발음
//		Sound[3] = SndObjCreate(SoundOBJ, "sound\\gameend.wav", 2);		// 게임 엔드
//
//		return TRUE;
//	}
//	return FALSE;
//}