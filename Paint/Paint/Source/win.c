/*
Copyright 2024 Taichi Murakami.
アプリケーションのメイン エントリ ポイントです。
*/

#include <windows.h>
#include <tchar.h>
#include "noexport.h"
#include "resource.h"
#define LOADSTRING_MAX 32

static HWND WINAPI CreateFrameWindow(
	_In_opt_ HINSTANCE hInstance);

static BOOL WINAPI RegisterPrivateClasses(
	_In_opt_ HINSTANCE hInstance);

extern int WINAPI _tWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInst,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	int nResult = -1;

	if (
		RegisterPrivateClasses(hInstance) &&
		(hWnd = CreateFrameWindow(hInstance)))
	{
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		nResult = (int)msg.wParam;
	}

	return nResult;
}

static HWND WINAPI CreateFrameWindow(
	_In_opt_ HINSTANCE hInstance)
{
	TCHAR strCaption[LOADSTRING_MAX];
	LoadString(hInstance, IDS_APPTITLE, strCaption, LOADSTRING_MAX);
	return CreateWindow(FRAMECLASSNAME, strCaption, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
}

static BOOL WINAPI RegisterPrivateClasses(
	_In_opt_ HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof wc);
	wc.cbSize = sizeof wc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = FrameWindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_FRAME);
	wc.lpszClassName = FRAMECLASSNAME;
	return !!RegisterClassEx(&wc);
}
