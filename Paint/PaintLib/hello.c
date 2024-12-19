/*
Copyright 2024 Taichi Murakami.
Hello World クラスを実装します。
*/

#include "PaintLib.h"
#include "hello.h"

/* Hello World */
typedef
struct HelloWorld
{
	const IHelloWorldVtbl *lpVtbl;
	ULONG_PTR uCounter;
	BSTR bstrGreeting;
}
HelloWorld;

static
STDMETHODIMP_(ULONG) HelloWorld_AddRef(
	_In_ IHelloWorld *This);

static
STDMETHODIMP HelloWorld_GetGreeting(
	_In_ IHelloWorld *This,
	_Out_ BSTR *pbstrGreeting);

static
STDMETHODIMP HelloWorld_Initialize(
	_In_ HelloWorld *This);

static
STDMETHODIMP HelloWorld_QueryInterface(
	_In_ IHelloWorld *This,
	_In_ REFIID riid,
	_Out_ void **ppvObject);

static
HRESULT CALLBACK HelloWorld_QueryInterfaceProc(
	_In_ REFIID riid,
	_In_ UINT uOffset);

static
STDMETHODIMP_(ULONG) HelloWorld_Release(
	_In_ IHelloWorld *This);

static
BOOL CALLBACK HelloWorld_ReleaseProc(
	_In_ _Post_ptr_invalid_ LPVOID This);

static
STDMETHODIMP HelloWorld_SetGreeting(
	_In_ IHelloWorld *This,
	_In_ BSTR bstrGreeting);

static const
IHelloWorldVtbl HelloWorldVtbl =
{
	/* IUnknown */
	HelloWorld_QueryInterface,
	HelloWorld_AddRef,
	HelloWorld_Release,
	/* IHelloWorld */
	HelloWorld_GetGreeting,
	HelloWorld_SetGreeting,
};

STDAPI CreateHelloWorld(
	_Out_ IHelloWorld **ppHelloWorld)
{
	HelloWorld *pHelloWorld;
	HRESULT hResult;

	if (ppHelloWorld)
	{
		pHelloWorld = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(HelloWorld));
		*ppHelloWorld = (IHelloWorld*)pHelloWorld;

		if (pHelloWorld)
		{
			pHelloWorld->lpVtbl = &HelloWorldVtbl;
			pHelloWorld->uCounter = DEFAULT_UNKNOWN_COUNT;
			hResult = HelloWorld_Initialize(pHelloWorld);
		}
		else
		{
			hResult = E_OUTOFMEMORY;
		}
	}
	else
	{
		hResult = E_POINTER;
	}

	return hResult;
}

static
STDMETHODIMP_(ULONG) HelloWorld_AddRef(
	_In_ IHelloWorld *This)
{
	return SelfAddRef(&((HelloWorld*)This)->uCounter);
}

static
STDMETHODIMP HelloWorld_GetGreeting(
	_In_ IHelloWorld *This,
	_Out_ BSTR *pbstrGreeting)
{
	return SelfGetString(((HelloWorld*)This)->bstrGreeting, pbstrGreeting);
}

static
STDMETHODIMP HelloWorld_Initialize(
	_In_ HelloWorld *This)
{
	HRESULT hResult;

	if (This->bstrGreeting)
	{
		hResult = E_ILLEGAL_METHOD_CALL;
	}
	else if (This->bstrGreeting = SysAllocString(TEXT("Hello, world!")))
	{
		hResult = S_OK;
	}
	else
	{
		hResult = E_OUTOFMEMORY;
	}

	return hResult;
}

static
STDMETHODIMP HelloWorld_QueryInterface(
	_In_ IHelloWorld *This,
	_In_ REFIID riid,
	_Out_ void **ppvObject)
{
	return SelfQueryInterface((IUnknown*)This, riid, ppvObject, HelloWorld_QueryInterfaceProc);
}

static
HRESULT CALLBACK HelloWorld_QueryInterfaceProc(
	_In_ REFIID riid,
	_In_ UINT uOffset)
{
	return (IsEqualIID(riid, &IID_IHelloWorld) || IsEqualIID(riid, &IID_IUnknown)) ? S_OK : E_NOINTERFACE;
}

static
STDMETHODIMP_(ULONG) HelloWorld_Release(
	_In_ IHelloWorld *This)
{
	return SelfRelease(This, &((HelloWorld*)This)->uCounter, HelloWorld_ReleaseProc);
}

static
BOOL CALLBACK HelloWorld_ReleaseProc(
	_In_ _Post_ptr_invalid_ LPVOID This)
{
	return HeapFree(GetProcessHeap(), 0, This);
}

static
STDMETHODIMP HelloWorld_SetGreeting(
	_In_ IHelloWorld *This,
	_In_ BSTR bstrGreeting)
{
	return SelfSetString(&((HelloWorld*)This)->bstrGreeting, bstrGreeting);
}
