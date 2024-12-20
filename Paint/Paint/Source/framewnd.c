/*
Copyright 2024 Taichi Murakami.
フレーム ウィンドウを実装します。
*/

#include <windows.h>
#include "noexport.h"
#include "resource.h"
#include "../../PaintLib/Include/hello.h"

static
LRESULT WINAPI DefProc(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

static
LRESULT WINAPI OnAbout(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

static
LRESULT WINAPI OnCreate(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

static
LRESULT WINAPI OnDestroy(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);

LRESULT CALLBACK FrameWindowProc(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	WINWNDPROC lpProc;

	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_ABOUT:
			lpProc = SendMessage;
			uMsg = FRAME_ABOUT;
			wParam = 0;
			lParam = 0;
			break;
		case IDM_EXIT:
			lpProc = SendMessage;
			uMsg = WM_CLOSE;
			wParam = 0;
			lParam = 0;
			break;
		default:
			lpProc = DefProc;
			break;
		}

		break;
	case WM_CREATE:
		lpProc = OnCreate;
		break;
	case WM_DESTROY:
		lpProc = OnDestroy;
		break;
	case FRAME_ABOUT:
		lpProc = OnAbout;
		break;
	default:
		lpProc = DefProc;
		break;
	}

	return lpProc(hWnd, uMsg, wParam, lParam);
}

static
LRESULT WINAPI DefProc(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	return DefFrameProc(hWnd, NULL, uMsg, wParam, lParam);
}

static
LRESULT WINAPI OnAbout(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	IHelloWorld *pHello;
	BSTR bstrGreeting;
	HRESULT hResult;
	hResult = CoCreateInstance(&CLSID_HelloWorld, NULL, CLSCTX_ALL, &IID_IHelloWorld, &pHello);

	if (SUCCEEDED(hResult))
	{
		hResult = pHello->lpVtbl->GetGreeting(pHello, &bstrGreeting);

		if (SUCCEEDED(hResult))
		{
			MessageBox(hWnd, bstrGreeting, TEXT("Greeting"), 0);
			SysFreeString(bstrGreeting);
		}

		pHello->lpVtbl->Release(pHello);
	}

	return 0;
}

static
LRESULT WINAPI OnCreate(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	return DefProc(hWnd, uMsg, wParam, lParam);
}

static
LRESULT WINAPI OnDestroy(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	PostQuitMessage(0);
	return DefProc(hWnd, uMsg, wParam, lParam);
}
