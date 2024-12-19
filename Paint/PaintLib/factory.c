/*
Copyright 2024 Taichi Murakami.
クラス ファクトリを実装します。
*/

#include "PaintLib.h"
#include "hello.h"

STDAPI CreateHelloWorld(
	_Out_ IHelloWorld **ppHelloWorld);

static
STDMETHODIMP_(ULONG) ClassFactory_AddRef(
	_In_ IClassFactory *This);

static
STDMETHODIMP ClassFactory_LockServer(
	_In_ IClassFactory *This,
	_In_ BOOL bLock);

static
STDMETHODIMP ClassFactory_QueryInterface(
	_In_ IClassFactory *This,
	_In_ REFIID riid,
	_Out_ void **ppvObject);

static
HRESULT CALLBACK ClassFactory_QueryInterfaceProc(
	_In_ REFIID riid,
	_In_ UINT uOffset);

static
STDMETHODIMP_(ULONG) ClassFactory_Release(
	_In_ IClassFactory *This);

static
STDMETHODIMP HelloWorldClassFactory_CreateInstance(
	_In_ IClassFactory *This,
	_In_opt_ IUnknown *pOuter,
	_In_ REFIID riid,
	_Out_ void **ppvObject);

static const
IClassFactoryVtbl HelloWorldClassFactoryVtbl =
{
	/* IUnknown */
	ClassFactory_QueryInterface,
	ClassFactory_AddRef,
	ClassFactory_Release,
	/* IClassFactory */
	HelloWorldClassFactory_CreateInstance,
	ClassFactory_LockServer,
};

ClassFactory HelloWorldClassFactory = { &HelloWorldClassFactoryVtbl };

static
STDMETHODIMP_(ULONG) ClassFactory_AddRef(
	_In_ IClassFactory *This)
{
	return DllAddRef(&((ClassFactory*)This)->uCounter);
}

static
STDMETHODIMP ClassFactory_LockServer(
	_In_ IClassFactory *This,
	_In_ BOOL bLock)
{
	return DllLockServer(bLock);
}

static
STDMETHODIMP ClassFactory_QueryInterface(
	_In_ IClassFactory *This,
	_In_ REFIID riid,
	_Out_ void **ppvObject)
{
	return SelfQueryInterface((IUnknown*)This, riid, ppvObject, ClassFactory_QueryInterfaceProc);
}

static
HRESULT CALLBACK ClassFactory_QueryInterfaceProc(
	_In_ REFIID riid,
	_In_ UINT uOffset)
{
	return (IsEqualIID(riid, &IID_IClassFactory) || IsEqualIID(riid, &IID_IUnknown)) ? S_OK : E_NOINTERFACE;
}

static
STDMETHODIMP_(ULONG) ClassFactory_Release(
	_In_ IClassFactory *This)
{
	return DllRelease(&((ClassFactory*)This)->uCounter);
}

static
STDMETHODIMP HelloWorldClassFactory_CreateInstance(
	_In_ IClassFactory *This,
	_In_opt_ IUnknown *pOuter,
	_In_ REFIID riid,
	_Out_ void **ppvObject)
{
	return SelfCreateInstance(pOuter, riid, ppvObject, (SELFCREATEINSTANCEPROC)CreateHelloWorld);
}
