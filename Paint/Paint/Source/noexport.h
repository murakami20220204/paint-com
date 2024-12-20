/*
Copyright 2024 Taichi Murakami.
*/

#pragma once
#define FRAMECLASSNAME TEXT("Frame")
#define FRAME_ABOUT 0x1000

typedef INT_PTR (WINAPI *WINDLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef LRESULT (WINAPI *WINWNDPROC)(HWND, UINT, WPARAM, LPARAM);

EXTERN_C
LRESULT CALLBACK FrameWindowProc(
	_In_ HWND hWnd,
	_In_ UINT uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam);
