/*
Copyright 2024 Taichi Murakami.
*/

#pragma once
#include <windows.h>

/* Library Identifier */
//EXTERN_C const IID LIBID_PaintLibrary;
const IID DECLSPEC_SELECTANY LIBID_PaintLibrary = { 0x7BE73173, 0x79FB, 0x4F30, 0x91, 0xF0, 0x6B, 0xDC, 0x26, 0xE0, 0xC6, 0x25 };

/* Interface Identifiers */
//EXTERN_C const IID IID_IHelloWorld;
const IID DECLSPEC_SELECTANY IID_IHelloWorld = { 0x6E6803DC, 0x26C5, 0x483A, 0x9A, 0x8B, 0x6A, 0xC8, 0xD9, 0xF6, 0x88, 0x52 };

/* Class Identifiers */
//EXTERN_C const CLSID CLSID_HelloWorld;
const CLSID DECLSPEC_SELECTANY CLSID_HelloWorld = { 0xCBE40A9F, 0xCE73, 0x46C8, 0xAD, 0xC4, 0x17, 0xCE, 0x59, 0x2B, 0x76, 0x5C };

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
