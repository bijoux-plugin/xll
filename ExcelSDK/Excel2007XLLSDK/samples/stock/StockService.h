//
// sproxy.exe generated file
// do not modify this file
//
// Created: 06/14/2007@14:01:25
//

#pragma once


#if !defined(_WIN32_WINDOWS) && !defined(_WIN32_WINNT) && !defined(_WIN32_WCE)
#pragma message("warning: defining _WIN32_WINDOWS = 0x0410")
#define _WIN32_WINDOWS 0x0410
#endif

#include <atlsoap.h>

namespace Service
{

struct StockInfoHeader
{
	double open;
	double close;
	double low;
	double high;
};

template <typename TClient = CSoapSocketClientT<> >
class CServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://localhost:1618/StockService/Service.asmx"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT GetClose(
		BSTR stSymbol, 
		int date, 
		double* GetCloseResult
	);

	HRESULT GetRange(
		BSTR stSymbol, 
		BSTR infType, 
		int start, 
		int length, 
		double** GetRangeResult, int* GetRangeResult_nSizeIs
	);

	HRESULT GetOpen(
		BSTR stSymbol, 
		int date, 
		double* GetOpenResult
	);

	HRESULT GetHigh(
		BSTR stSymbol, 
		int date, 
		double* GetHighResult
	);

	HRESULT GetAll(
		BSTR stSymbol, 
		int date, 
		StockInfoHeader* GetAllResult
	);

	HRESULT GetLow(
		BSTR stSymbol, 
		int date, 
		double* GetLowResult
	);
};

typedef CServiceT<> CService;

__if_not_exists(__StockInfoHeader_entries)
{
extern __declspec(selectany) const _soapmapentry __StockInfoHeader_entries[] =
{
	{ 
		0x003EC7F2, 
		"open", 
		L"open", 
		sizeof("open")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_FIELD, 
		offsetof(StockInfoHeader, open),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x073C9A36, 
		"close", 
		L"close", 
		sizeof("close")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_FIELD, 
		offsetof(StockInfoHeader, close),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x0001DA32, 
		"low", 
		L"low", 
		sizeof("low")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_FIELD, 
		offsetof(StockInfoHeader, low),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0x003AD3C0, 
		"high", 
		L"high", 
		sizeof("high")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_FIELD, 
		offsetof(StockInfoHeader, high),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __StockInfoHeader_map =
{
	0x45D9CA19,
	"StockInfoHeader",
	L"StockInfoHeader",
	sizeof("StockInfoHeader")-1,
	sizeof("StockInfoHeader")-1,
	SOAPMAP_STRUCT,
	__StockInfoHeader_entries,
	sizeof(StockInfoHeader),
	4,
	-1,
	SOAPFLAG_NONE,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};
}

struct __CService_GetClose_struct
{
	BSTR stSymbol;
	int date;
	double GetCloseResult;
};

extern __declspec(selectany) const _soapmapentry __CService_GetClose_entries[] =
{

	{
		0xCD5D5E9D, 
		"stSymbol", 
		L"stSymbol", 
		sizeof("stSymbol")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetClose_struct, stSymbol),
		NULL,
		NULL,
		-1,
	},
	{
		0x003881DE, 
		"date", 
		L"date", 
		sizeof("date")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetClose_struct, date),
		NULL,
		NULL,
		-1,
	},
	{
		0x523B0C35, 
		"GetCloseResult", 
		L"GetCloseResult", 
		sizeof("GetCloseResult")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetClose_struct, GetCloseResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CService_GetClose_map =
{
	0xCCD60DC5,
	"GetClose",
	L"GetCloseResponse",
	sizeof("GetClose")-1,
	sizeof("GetCloseResponse")-1,
	SOAPMAP_FUNC,
	__CService_GetClose_entries,
	sizeof(__CService_GetClose_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};


struct __CService_GetRange_struct
{
	BSTR stSymbol;
	BSTR infType;
	int start;
	int length;
	double *GetRangeResult;
	int __GetRangeResult_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CService_GetRange_entries[] =
{

	{
		0xCD5D5E9D, 
		"stSymbol", 
		L"stSymbol", 
		sizeof("stSymbol")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetRange_struct, stSymbol),
		NULL,
		NULL,
		-1,
	},
	{
		0x9AA0451F, 
		"infType", 
		L"infType", 
		sizeof("infType")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetRange_struct, infType),
		NULL,
		NULL,
		-1,
	},
	{
		0x086249AE, 
		"start", 
		L"start", 
		sizeof("start")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetRange_struct, start),
		NULL,
		NULL,
		-1,
	},
	{
		0x034EE602, 
		"length", 
		L"length", 
		sizeof("length")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetRange_struct, length),
		NULL,
		NULL,
		-1,
	},
	{
		0x6EA3030C, 
		"GetRangeResult", 
		L"GetRangeResult", 
		sizeof("GetRangeResult")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetRange_struct, GetRangeResult),
		NULL,
		NULL,
		4+1,
	},
	{
		0x6EA3030C,
		"__GetRangeResult_nSizeIs",
		L"__GetRangeResult_nSizeIs",
		sizeof("__GetRangeResult_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CService_GetRange_struct, __GetRangeResult_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CService_GetRange_map =
{
	0xA317165C,
	"GetRange",
	L"GetRangeResponse",
	sizeof("GetRange")-1,
	sizeof("GetRangeResponse")-1,
	SOAPMAP_FUNC,
	__CService_GetRange_entries,
	sizeof(__CService_GetRange_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};


struct __CService_GetOpen_struct
{
	BSTR stSymbol;
	int date;
	double GetOpenResult;
};

extern __declspec(selectany) const _soapmapentry __CService_GetOpen_entries[] =
{

	{
		0xCD5D5E9D, 
		"stSymbol", 
		L"stSymbol", 
		sizeof("stSymbol")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetOpen_struct, stSymbol),
		NULL,
		NULL,
		-1,
	},
	{
		0x003881DE, 
		"date", 
		L"date", 
		sizeof("date")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetOpen_struct, date),
		NULL,
		NULL,
		-1,
	},
	{
		0x0740AAF1, 
		"GetOpenResult", 
		L"GetOpenResult", 
		sizeof("GetOpenResult")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetOpen_struct, GetOpenResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CService_GetOpen_map =
{
	0xD9BE4B81,
	"GetOpen",
	L"GetOpenResponse",
	sizeof("GetOpen")-1,
	sizeof("GetOpenResponse")-1,
	SOAPMAP_FUNC,
	__CService_GetOpen_entries,
	sizeof(__CService_GetOpen_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};


struct __CService_GetHigh_struct
{
	BSTR stSymbol;
	int date;
	double GetHighResult;
};

extern __declspec(selectany) const _soapmapentry __CService_GetHigh_entries[] =
{

	{
		0xCD5D5E9D, 
		"stSymbol", 
		L"stSymbol", 
		sizeof("stSymbol")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetHigh_struct, stSymbol),
		NULL,
		NULL,
		-1,
	},
	{
		0x003881DE, 
		"date", 
		L"date", 
		sizeof("date")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetHigh_struct, date),
		NULL,
		NULL,
		-1,
	},
	{
		0xC635D93F, 
		"GetHighResult", 
		L"GetHighResult", 
		sizeof("GetHighResult")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetHigh_struct, GetHighResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CService_GetHigh_map =
{
	0x2AB8454F,
	"GetHigh",
	L"GetHighResponse",
	sizeof("GetHigh")-1,
	sizeof("GetHighResponse")-1,
	SOAPMAP_FUNC,
	__CService_GetHigh_entries,
	sizeof(__CService_GetHigh_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};


struct __CService_GetAll_struct
{
	BSTR stSymbol;
	int date;
	StockInfoHeader GetAllResult;
};

extern __declspec(selectany) const _soapmapentry __CService_GetAll_entries[] =
{

	{
		0xCD5D5E9D, 
		"stSymbol", 
		L"stSymbol", 
		sizeof("stSymbol")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetAll_struct, stSymbol),
		NULL,
		NULL,
		-1,
	},
	{
		0x003881DE, 
		"date", 
		L"date", 
		sizeof("date")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetAll_struct, date),
		NULL,
		NULL,
		-1,
	},
	{
		0x85780B38, 
		"GetAllResult", 
		L"GetAllResult", 
		sizeof("GetAllResult")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetAll_struct, GetAllResult),
		NULL,
		&__StockInfoHeader_map,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CService_GetAll_map =
{
	0xC34ED988,
	"GetAll",
	L"GetAllResponse",
	sizeof("GetAll")-1,
	sizeof("GetAllResponse")-1,
	SOAPMAP_FUNC,
	__CService_GetAll_entries,
	sizeof(__CService_GetAll_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};


struct __CService_GetLow_struct
{
	BSTR stSymbol;
	int date;
	double GetLowResult;
};

extern __declspec(selectany) const _soapmapentry __CService_GetLow_entries[] =
{

	{
		0xCD5D5E9D, 
		"stSymbol", 
		L"stSymbol", 
		sizeof("stSymbol")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL | SOAPFLAG_NULLABLE,
		offsetof(__CService_GetLow_struct, stSymbol),
		NULL,
		NULL,
		-1,
	},
	{
		0x003881DE, 
		"date", 
		L"date", 
		sizeof("date")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetLow_struct, date),
		NULL,
		NULL,
		-1,
	},
	{
		0x9A570131, 
		"GetLowResult", 
		L"GetLowResult", 
		sizeof("GetLowResult")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		offsetof(__CService_GetLow_struct, GetLowResult),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CService_GetLow_map =
{
	0x8BC331C1,
	"GetLow",
	L"GetLowResponse",
	sizeof("GetLow")-1,
	sizeof("GetLowResponse")-1,
	SOAPMAP_FUNC,
	__CService_GetLow_entries,
	sizeof(__CService_GetLow_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
	0xFA446C1D,
	"http://imaginarystockservice/",
	L"http://imaginarystockservice/",
	sizeof("http://imaginarystockservice/")-1
};

extern __declspec(selectany) const _soapmap * __CService_funcs[] =
{
	&__CService_GetClose_map,
	&__CService_GetRange_map,
	&__CService_GetOpen_map,
	&__CService_GetHigh_map,
	&__CService_GetAll_map,
	&__CService_GetLow_map,
	NULL
};

template <typename TClient>
inline HRESULT CServiceT<TClient>::GetClose(
		BSTR stSymbol, 
		int date, 
		double* GetCloseResult
	)
{
    if ( GetCloseResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CService_GetClose_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.stSymbol = stSymbol;
	__params.date = date;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://imaginarystockservice/GetClose\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetCloseResult = __params.GetCloseResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CServiceT<TClient>::GetRange(
		BSTR stSymbol, 
		BSTR infType, 
		int start, 
		int length, 
		double** GetRangeResult, int* __GetRangeResult_nSizeIs
	)
{
    if ( GetRangeResult == NULL )
		return E_POINTER;
	if( __GetRangeResult_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CService_GetRange_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.stSymbol = stSymbol;
	__params.infType = infType;
	__params.start = start;
	__params.length = length;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://imaginarystockservice/GetRange\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetRangeResult = __params.GetRangeResult;
	*__GetRangeResult_nSizeIs = __params.__GetRangeResult_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CServiceT<TClient>::GetOpen(
		BSTR stSymbol, 
		int date, 
		double* GetOpenResult
	)
{
    if ( GetOpenResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CService_GetOpen_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.stSymbol = stSymbol;
	__params.date = date;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://imaginarystockservice/GetOpen\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetOpenResult = __params.GetOpenResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CServiceT<TClient>::GetHigh(
		BSTR stSymbol, 
		int date, 
		double* GetHighResult
	)
{
    if ( GetHighResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CService_GetHigh_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.stSymbol = stSymbol;
	__params.date = date;

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://imaginarystockservice/GetHigh\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetHighResult = __params.GetHighResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CServiceT<TClient>::GetAll(
		BSTR stSymbol, 
		int date, 
		StockInfoHeader* GetAllResult
	)
{
    if ( GetAllResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CService_GetAll_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.stSymbol = stSymbol;
	__params.date = date;

	__atlsoap_hr = SetClientStruct(&__params, 4);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://imaginarystockservice/GetAll\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetAllResult = __params.GetAllResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CServiceT<TClient>::GetLow(
		BSTR stSymbol, 
		int date, 
		double* GetLowResult
	)
{
    if ( GetLowResult == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CService_GetLow_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.stSymbol = stSymbol;
	__params.date = date;

	__atlsoap_hr = SetClientStruct(&__params, 5);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"http://imaginarystockservice/GetLow\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*GetLowResult = __params.GetLowResult;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CServiceT<TClient>::GetFunctionMap()
{
	return __CService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CService_GetClose_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CService_GetClose_atlsoapheader_map = 
	{
		0xCCD60DC5,
		"GetClose",
		L"GetCloseResponse",
		sizeof("GetClose")-1,
		sizeof("GetCloseResponse")-1,
		SOAPMAP_HEADER,
		__CService_GetClose_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xFA446C1D,
		"http://imaginarystockservice/",
		L"http://imaginarystockservice/",
		sizeof("http://imaginarystockservice/")-1
	};

	static const _soapmapentry __CService_GetRange_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CService_GetRange_atlsoapheader_map = 
	{
		0xA317165C,
		"GetRange",
		L"GetRangeResponse",
		sizeof("GetRange")-1,
		sizeof("GetRangeResponse")-1,
		SOAPMAP_HEADER,
		__CService_GetRange_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xFA446C1D,
		"http://imaginarystockservice/",
		L"http://imaginarystockservice/",
		sizeof("http://imaginarystockservice/")-1
	};

	static const _soapmapentry __CService_GetOpen_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CService_GetOpen_atlsoapheader_map = 
	{
		0xD9BE4B81,
		"GetOpen",
		L"GetOpenResponse",
		sizeof("GetOpen")-1,
		sizeof("GetOpenResponse")-1,
		SOAPMAP_HEADER,
		__CService_GetOpen_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xFA446C1D,
		"http://imaginarystockservice/",
		L"http://imaginarystockservice/",
		sizeof("http://imaginarystockservice/")-1
	};

	static const _soapmapentry __CService_GetHigh_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CService_GetHigh_atlsoapheader_map = 
	{
		0x2AB8454F,
		"GetHigh",
		L"GetHighResponse",
		sizeof("GetHigh")-1,
		sizeof("GetHighResponse")-1,
		SOAPMAP_HEADER,
		__CService_GetHigh_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xFA446C1D,
		"http://imaginarystockservice/",
		L"http://imaginarystockservice/",
		sizeof("http://imaginarystockservice/")-1
	};

	static const _soapmapentry __CService_GetAll_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CService_GetAll_atlsoapheader_map = 
	{
		0xC34ED988,
		"GetAll",
		L"GetAllResponse",
		sizeof("GetAll")-1,
		sizeof("GetAllResponse")-1,
		SOAPMAP_HEADER,
		__CService_GetAll_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xFA446C1D,
		"http://imaginarystockservice/",
		L"http://imaginarystockservice/",
		sizeof("http://imaginarystockservice/")-1
	};

	static const _soapmapentry __CService_GetLow_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CService_GetLow_atlsoapheader_map = 
	{
		0x8BC331C1,
		"GetLow",
		L"GetLowResponse",
		sizeof("GetLow")-1,
		sizeof("GetLowResponse")-1,
		SOAPMAP_HEADER,
		__CService_GetLow_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_PID | SOAPFLAG_DOCUMENT | SOAPFLAG_LITERAL,
		0xFA446C1D,
		"http://imaginarystockservice/",
		L"http://imaginarystockservice/",
		sizeof("http://imaginarystockservice/")-1
	};


	static const _soapmap * __CService_headers[] =
	{
		&__CService_GetClose_atlsoapheader_map,
		&__CService_GetRange_atlsoapheader_map,
		&__CService_GetOpen_atlsoapheader_map,
		&__CService_GetHigh_atlsoapheader_map,
		&__CService_GetAll_atlsoapheader_map,
		&__CService_GetLow_atlsoapheader_map,
		NULL
	};
	
	return __CService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CServiceT<TClient>::GetNamespaceUri()
{
	return L"http://imaginarystockservice/";
}

template <typename TClient>
ATL_NOINLINE inline const char * CServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CServiceT<TClient>::GetNamespaceUriA()
{
	return "http://imaginarystockservice/";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace Service

template<>
inline HRESULT AtlCleanupValue<Service::StockInfoHeader>(Service::StockInfoHeader *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->open);
	AtlCleanupValue(&pVal->close);
	AtlCleanupValue(&pVal->low);
	AtlCleanupValue(&pVal->high);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<Service::StockInfoHeader>(Service::StockInfoHeader *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->open, pMemMgr);
	AtlCleanupValueEx(&pVal->close, pMemMgr);
	AtlCleanupValueEx(&pVal->low, pMemMgr);
	AtlCleanupValueEx(&pVal->high, pMemMgr);
	return S_OK;
}
