/*
Copyright 2024 Taichi Murakami.
DLL がエクスポートする関数を実装します。
*/

#include "PaintLib.h"
#include "hello.h"

static HMODULE Dll;
static ULONG_PTR DllLock;

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
	return S_OK;
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
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		Dll = hDLL;
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

STDAPI DllRegisterServer(
	void)
{
	return DllInstall(TRUE, NULL);
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
	return DllInstall(FALSE, NULL);
}
