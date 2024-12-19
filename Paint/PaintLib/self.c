/*
Copyright 2024 Taichi Murakami.
クラスのプロパティを取得または設定する方法を実装します。
*/

#include "PaintLib.h"

STDAPI_(ULONG) SelfAddRef(
	_Inout_ PULONG_PTR pCounter)
{
	const ULONG_PTR uCounter = UnknownIncrement(pCounter);

	if (!uCounter)
	{
		ExitProcess(EXIT_OVERFLOW);
	}

	return (ULONG)uCounter;
}

STDAPI SelfCreateInstance(
	_In_opt_ IUnknown *pOuter,
	_In_ REFIID riid,
	_Out_ void **ppvObject,
	_In_ SELFCREATEINSTANCEPROC lpfnCreate)
{
	IUnknown *pInstance;
	HRESULT hResult;

	if (!ppvObject)
	{
		hResult = E_POINTER;
	}
	else if (pOuter)
	{
		*ppvObject = NULL;
		hResult = CLASS_E_NOAGGREGATION;
	}
	else if (SUCCEEDED(hResult = lpfnCreate(&pInstance)))
	{
		hResult = UNKNOWN_QUERYINTERFACE(pInstance, riid, ppvObject);
		UNKNOWN_RELEASE(pInstance);
	}
	else
	{
		*ppvObject = NULL;
	}

	return hResult;
}

STDAPI SelfGetString(
	_In_opt_ BSTR bstrSource,
	_Out_ BSTR *pbstrDestination)
{
	HRESULT hResult;

	if (!pbstrDestination)
	{
		hResult = E_POINTER;
	}
	else if (bstrSource)
	{
		*pbstrDestination = SysAllocStringLen(bstrSource, SysStringLen(bstrSource));
		hResult = *pbstrDestination ? S_OK : E_OUTOFMEMORY;
	}
	else
	{
		*pbstrDestination = NULL;
		hResult = S_FALSE;
	}

	return hResult;
}


STDAPI SelfQueryInterface(
	_In_ LPUNKNOWN This,
	_In_ REFIID riid,
	_Out_ void **ppvObject,
	_In_ SELFQUERYINTERFACEPROC lpfnCompare)
{
	HRESULT hResult;
	UINT uOffset;

	if (ppvObject)
	{
		for (uOffset = 0;;)
		{
			hResult = lpfnCompare(riid, uOffset);

			switch (hResult)
			{
			case S_OK:
				UNKNOWN_ADDREF(This);
				*ppvObject = This + uOffset;
				goto EXIT;
			case S_FALSE:
				uOffset++;
				continue;
			default:
				*ppvObject = NULL;
				goto EXIT;
			}
		}
	}
	else
	{
		hResult = E_POINTER;
	}

EXIT:
	return hResult;
}

STDAPI_(ULONG) SelfRelease(
	_In_ LPVOID This,
	_Inout_ ULONG_PTR *pCounter,
	_In_ SELFRELEASEPROC lpfnDelete)
{
	const ULONG_PTR uCounter = UnknownDecrement(pCounter);

	switch (uCounter)
	{
	case 0:
		lpfnDelete(This);
		break;
	case MAXULONG_PTR:
		ExitProcess(EXIT_OVERFLOW);
		break;
	}

	return (ULONG)uCounter;
}

STDAPI SelfSetString(
	_Inout_ BSTR *pbstrDestination,
	_In_opt_ BSTR bstrSource)
{
	HRESULT hResult;
	UINT uLength;

	if (bstrSource)
	{
		uLength = SysStringLen(bstrSource);

		if (*pbstrDestination)
		{
			uLength = SysReAllocStringLen(pbstrDestination, bstrSource, uLength);
			hResult = uLength ? S_OK : E_OUTOFMEMORY;
		}
		else
		{
			*pbstrDestination = SysAllocStringLen(bstrSource, uLength);
			hResult = *pbstrDestination ? S_OK : E_OUTOFMEMORY;
		}
	}
	else
	{
		SysFreeString(*pbstrDestination);
		*pbstrDestination = NULL;
		hResult = S_FALSE;
	}

	return hResult;
}

