/*
Copyright 2024 Taichi Murakami.
DLL がエクスポートする関数を実装します。
*/

#include "PaintLib.h"
#include "hello.h"
#include <olectl.h>
#include <pathcch.h>
#include <strsafe.h>
#define DLL_CMDLINE TEXT("USER")
#define DLL_VERSION 1

#ifdef _WIN64
#define DLL_SYSKIND SYS_WIN64
#elif _WIN32
#define DLL_SYSKIND SYS_WIN32
#endif

static HMODULE Dll;
static ULONG_PTR DllLock;

static
HRESULT WINAPI DllRegisterTypeLib(
	_In_ BOOL bForUser);

static
HRESULT WINAPI DllUnregisterTypeLib(
	_In_ BOOL bForUser);

STDAPI_(ULONG) DllAddRef(
	_Inout_ PULONG_PTR pCounter)
{
	const ULONG_PTR uCounter = UnknownIncrement(pCounter);

	switch (uCounter)
	{
	case 0:
		ExitProcess(EXIT_OVERFLOW);
		break;
	case 1:
		DllLockServer(TRUE);
		break;
	}

	return (ULONG)uCounter;
}

__control_entrypoint(DllExport)
STDAPI DllCanUnloadNow(
	void)
{
	return DllLock ? S_FALSE : S_OK;
}

_Check_return_
STDAPI DllGetClassObject(
	_In_ REFCLSID rclsid,
	_In_ REFIID riid,
	_Outptr_ LPVOID *ppvObject)
{
	HRESULT hResult;

	if (!ppvObject)
	{
		hResult = E_POINTER;
	}
	else if (IsEqualCLSID(rclsid, &CLSID_HelloWorld))
	{
		hResult = UNKNOWN_QUERYINTERFACE((LPUNKNOWN)&HelloWorldClassFactory, riid, ppvObject);
	}
	else
	{
		*ppvObject = NULL;
		hResult = CLASS_E_CLASSNOTAVAILABLE;
	}

	return hResult;
}

STDAPI DllInstall(
	_In_ BOOL bInstall,
	_In_opt_z_ LPCWSTR pwszCmdLine)
{
	SIZE_T uCmdLine;
	HRESULT hResult;
	BOOL bForUser;
	bForUser = FALSE;

	if (pwszCmdLine)
	{
		hResult = StringCchLength(pwszCmdLine, MAX_PATH, &uCmdLine);

		if (SUCCEEDED(hResult))
		{
			bForUser = !CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, pwszCmdLine, (int)uCmdLine, DLL_CMDLINE, ARRAYSIZE(DLL_CMDLINE));
		}
	}
	else
	{
		hResult = S_OK;
	}
	if (SUCCEEDED(hResult))
	{
		hResult = bInstall ?
			DllRegisterTypeLib(bForUser) :
			DllUnregisterTypeLib(bForUser);
	}

	return hResult;
}

STDAPI DllLockServer(
	_In_ BOOL bLock)
{
	BOOL bOverflow;

	if (bLock)
	{
		bOverflow = !UnknownIncrement(&DllLock);
	}
	else
	{
		bOverflow = !~UnknownDecrement(&DllLock);
	}
	if (bOverflow)
	{
		ExitProcess(EXIT_OVERFLOW);
	}

	return S_OK;
}

EXTERN_C
BOOL WINAPI DllMain(
	_In_ HMODULE hDLL,
	_In_ DWORD dwReason,
	_In_opt_ LPVOID lpReserved)
{
	BOOL bResult;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		Dll = hDLL;
		//bResult = SUCCEEDED(DllRegisterTypeLib(TRUE));
		bResult = TRUE;
		break;
	case DLL_THREAD_ATTACH:
		bResult = TRUE;
		break;
	case DLL_THREAD_DETACH:
		bResult = TRUE;
		break;
	case DLL_PROCESS_DETACH:
		//bResult = lpReserved ? FALSE : SUCCEEDED(DllUnregisterTypeLib(TRUE));
		bResult = !lpReserved;
		break;
	default:
		bResult = FALSE;
		break;
	}

	return bResult;
}

STDAPI DllRegisterServer(
	void)
{
	return DllRegisterTypeLib(FALSE);
}

static
HRESULT WINAPI DllRegisterTypeLib(
	_In_ BOOL bForUser)
{
	ITypeLib *pTypeLib;
	LPTSTR lpFileName;
	HANDLE hHeap;
	HRESULT hResult;
	hHeap = GetProcessHeap();
	lpFileName = HeapAlloc(hHeap, 0, sizeof(TCHAR) * PATHCCH_MAX_CCH);

	if (lpFileName)
	{
		if (GetModuleFileName(Dll, lpFileName, PATHCCH_MAX_CCH) != PATHCCH_MAX_CCH)
		{
			hResult = LoadTypeLib(lpFileName, &pTypeLib);

			if (SUCCEEDED(hResult))
			{
				hResult = bForUser ?
					RegisterTypeLibForUser(pTypeLib, lpFileName, NULL) :
					RegisterTypeLib(pTypeLib, lpFileName, NULL);
				UNKNOWN_RELEASE(pTypeLib);
			}
		}
		else
		{
			hResult = HRESULT_FROM_WIN32(GetLastError());
		}

		HeapFree(hHeap, 0, lpFileName);
	}
	else
	{
		hResult = E_OUTOFMEMORY;
	}

	return hResult;
}

STDAPI_(ULONG) DllRelease(
	_Inout_ PULONG_PTR pCounter)
{
	const ULONG_PTR uCounter = UnknownDecrement(pCounter);

	switch (uCounter)
	{
	case 0:
		DllLockServer(FALSE);
		break;
	case MAXULONG_PTR:
		ExitProcess(EXIT_OVERFLOW);
		break;
	}

	return (ULONG)uCounter;
}

STDAPI DllUnregisterServer(
	void)
{
	return DllUnregisterTypeLib(FALSE);
}

static
HRESULT WINAPI DllUnregisterTypeLib(
	_In_ BOOL bForUser)
{
	return bForUser ?
		UnRegisterTypeLibForUser(&LIBID_PaintLibrary, DLL_VERSION, 0, LOCALE_NEUTRAL, DLL_SYSKIND) :
		UnRegisterTypeLib(&LIBID_PaintLibrary, DLL_VERSION, 0, LOCALE_NEUTRAL, DLL_SYSKIND);
}
