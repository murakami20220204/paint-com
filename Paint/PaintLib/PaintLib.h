/*
Copyright 2024 Taichi Murakami.
このファイルに記載されたシンボルはエクスポートされません。
*/

#pragma once
#include <windows.h>
#define DEFAULT_UNKNOWN_COUNT 1
#define EXIT_OVERFLOW -1
#define UNKNOWN_INTERLOCKED FALSE

typedef
struct ClassFactory
{
	const IClassFactoryVtbl *lpVtbl;
	ULONG_PTR uCounter;
}
ClassFactory;

typedef HRESULT(WINAPI *SELFCREATEINSTANCEPROC)(void**);
typedef HRESULT(CALLBACK *SELFQUERYINTERFACEPROC)(REFIID, UINT);
typedef BOOL(CALLBACK *SELFRELEASEPROC)(LPVOID);

EXTERN_C ClassFactory HelloWorldClassFactory;

STDAPI_(ULONG) DllAddRef(
	_Inout_ PULONG_PTR pCounter);

STDAPI DllInstall(
	_In_ BOOL bInstall,
	_In_opt_z_ LPCWSTR pwszCmdLine);

STDAPI DllLockServer(
	_In_ BOOL bLock);

STDAPI_(ULONG) DllRelease(
	_Inout_ PULONG_PTR pCounter);

STDAPI_(ULONG) SelfAddRef(
	_Inout_ PULONG_PTR pCounter);

STDAPI SelfCreateInstance(
	_In_opt_ IUnknown *pOuter,
	_In_ REFIID riid,
	_Out_ void **ppvObject,
	_In_ SELFCREATEINSTANCEPROC lpfnCreate);

STDAPI SelfGetString(
	_In_opt_ BSTR bstrSource,
	_Out_ BSTR *pbstrDestination);

STDAPI SelfQueryInterface(
	_In_ LPUNKNOWN This,
	_In_ REFIID riid,
	_Out_ void **ppvObject,
	_In_ SELFQUERYINTERFACEPROC lpfnCompare);

STDAPI_(ULONG) SelfRelease(
	_In_ LPVOID This,
	_Inout_ ULONG_PTR *pCounter,
	_In_ SELFRELEASEPROC lpfnDelete);

STDAPI SelfSetString(
	_Inout_ BSTR *pbstrDestination,
	_In_opt_ BSTR bstrSource);

#ifdef _WIN64
#define InterlockedDecrementPtr InterlockedDecrement64
#define InterlockedIncrementPtr InterlockedIncrement64
#else
#define InterlockedDecrementPtr InterlockedDecrement
#define InterlockedIncrementPtr InterlockedIncrement
#endif

#if UNKNOWN_INTERLOCKED
#define UnknownDecrement(pCount) ((ULONG_PTR)InterlockedDecrementPtr((PLONG_PTR)(pCount)))
#define UnknownIncrement(pCount) ((ULONG_PTR)InterlockedIncrementPtr((PLONG_PTR)(pCount)))
#else
#define UnknownDecrement(pCount) (--(*(pCount)))
#define UnknownIncrement(pCount) (++(*(pCount)))
#endif

#ifdef __cplusplus
#define UNKNOWN_ADDREF(This) ((This)->AddRef())
#define UNKNOWN_QUERYINTERFACE(This, riid, ppvObject) ((This)->QueryInterface((riid), (ppvObject)))
#define UNKNOWN_RELEASE(This) ((This)->Release())
#else
#define UNKNOWN_ADDREF(This) ((This)->lpVtbl->AddRef(This))
#define UNKNOWN_QUERYINTERFACE(This, riid, ppvObject) ((This)->lpVtbl->QueryInterface((This), (riid), (ppvObject)))
#define UNKNOWN_RELEASE(This) ((This)->lpVtbl->Release(This))
#endif
