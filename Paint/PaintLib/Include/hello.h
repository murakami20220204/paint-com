/*
Copyright 2024 Taichi Murakami.
*/

#pragma once
#include <windows.h>

/* Library Identifier */
EXTERN_C const IID LIBID_PaintLibrary;

/* Interface Identifiers */
EXTERN_C const IID IID_IHelloWorld;

/* Class Identifiers */
EXTERN_C const CLSID CLSID_HelloWorld;

#undef INTERFACE
#define INTERFACE IHelloWorld
DECLARE_INTERFACE_IID_(IHelloWorld, IUnknown, "6E6803DC-26C5-483A-9A8B-6AC8D9F68852")
{
	/* IUnknown */
	STDMETHOD(QueryInterface)(THIS_ _In_ REFIID riid, _Out_ void **ppvObject) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;
	/* IHelloWorld */
	STDMETHOD(GetGreeting)(THIS_ _Out_ BSTR *pbstrGreeting) PURE;
	STDMETHOD(SetGreeting)(THIS_ _In_ BSTR bstrGreeting) PURE;
};
