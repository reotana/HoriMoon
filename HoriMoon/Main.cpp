//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN

#pragma region  Include 해와야 하는 것들

#include <Windows.h>	
#include <windowsx.h>

#include <ddraw.h>
#include <dinput.h>

#include "Bitmap.h"
#include "Timer.h"
#include "Sprite.h"

#include "Define.h"

#pragma endregion

//----------------------------------------------------------------------------------
// 선언 파트
//----------------------------------------------------------------------------------

#pragma region  기본적인 선언

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode);

HWND hWnd;
HINSTANCE g_hInstance;
bool g_bActiveApp = false;
CTimer g_Timer;	

bool InitializeDirectX(void);
void MainScreen(void);
void Fliping();
bool InitObject();
bool LoadBMPandInitSurface();

int mainID = 0;
int mainChoose = 0;

LPDIRECTDRAW7 g_lpDirectDrawObject = NULL;
LPDIRECTDRAWSURFACE7 g_lpPrimarySurface = NULL;
LPDIRECTDRAWSURFACE7 g_lpSecondarySurface = NULL;

LPDIRECTINPUT8 g_lpDirectInputObject = NULL;
LPDIRECTINPUTDEVICE8 g_lpDirectInputKeyboard = NULL;

#pragma endregion

#pragma region Sprite 선언

CSprite g_TitleBarSprite;
CSprite g_TitleSprite;

CSprite g_GameMenuSprite1;
CSprite g_GameMenuSprite2;

CSprite g_GameSetSprite;

#pragma endregion

//----------------------------------------------------------------------------------
// WINMAIN
//----------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevlnstance, LPSTR lpCmdLine, int nCmdShow) {
	//WindowClass 등록
	WNDCLASSEX WndClass;
	MSG Msg;

	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = Class_N;
	WndClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

	if (RegisterClassEx(&WndClass) == 0)
		return 0;

	hWnd = CreateWindowEx(WS_EX_TOPMOST, Class_N, Class_N, WS_POPUP | WS_VISIBLE, 0, 0,
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)	return 0;

	g_hInstance = hInstance;
	SetFocus(hWnd);
	ShowCursor(FALSE);

	if (!InitializeDirectX())		return 0;

	g_Timer.start();

	if (!LoadBMPandInitSurface())		return 0;
	if (!InitObject())		return 0;

	while (true) {
		if (PeekMessage(&Msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!GetMessage(&Msg, NULL, 0, 0))
				return (int)Msg.wParam;
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		else if (g_bActiveApp) {
			switch (mainID) {
			case 0: MainScreen(); break;
			case 1: break;
			case 2: DestroyWindow(hWnd); break;
			}
		}
		else WaitMessage();
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage)
	{
	case WM_ACTIVATEAPP:
		if (wParam)	g_bActiveApp = true;
		else g_bActiveApp = false;
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		if (g_lpDirectDrawObject) {
			if (g_lpSecondarySurface)
				g_lpSecondarySurface->Release();
			if (g_lpPrimarySurface)
				g_lpPrimarySurface->Release();

			g_TitleSprite.ReleaseAll();
			g_TitleBarSprite.ReleaseAll();

			g_lpDirectDrawObject->Release();
		}

		if (g_lpDirectInputObject) {
			if (g_lpDirectInputKeyboard) {
				g_lpDirectInputKeyboard->Unacquire();
				g_lpDirectInputKeyboard->Release();
			}
			g_lpDirectInputObject->Release();
		}

		ShowCursor(TRUE);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}
	return 0;	//*/
}

HRESULT hResult;

bool LoadBMPandInitSurface() {

	if (!g_TitleSprite.InitSprite(1, 1366, 768, COLOR_KEY, g_lpDirectDrawObject))		return false;	// 각자 구현하고자 하는 해상도 적용
	if (!g_TitleSprite.LoadFrame(0, "data\\title\\title.bmp"))		return false;
	g_TitleSprite.Drawing(0, 683, 384, g_lpSecondarySurface, false);
	Fliping();
		
	// GameSet 스프라이트
	if (!g_GameSetSprite.InitSprite(1, 1366, 768, COLOR_KEY, g_lpDirectDrawObject))		return false;
	if (!g_GameSetSprite.LoadFrame(0, "data\\background2.bmp"))		return false;

		return true;
}


bool InitObject() {
	return true;
}

void Fliping() {
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT)))
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Release();
}

BOOL DirectInputKeyboardDown(LPDIRECTINPUTDEVICE8 lpKeyboard, int dikcode)
{
	char KeyBuffer[256];
	HRESULT hResult;
	// 키보드 받은 값이 오류라면
	if (lpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer) == DIERR_INPUTLOST) {
		while (hResult = lpKeyboard->Acquire() == DIERR_INPUTLOST)						// 계속 다시 해준다.
			hResult = lpKeyboard->Acquire();
		lpKeyboard->GetDeviceState(256, (LPVOID)KeyBuffer);
	}

	return (KeyBuffer[dikcode] & 0x80);
}