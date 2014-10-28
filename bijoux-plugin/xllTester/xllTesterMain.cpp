
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>

//#include <ObjIdl.h>
//#include <Objbase.h>
//#include <atlbase.h>

/*
// Whooper finds its instance by creating and saving a new workbook
// with a random number in the name to avoid collisions
// I want to iterate through all IDispatch interfaces
// and to be able to determine if it is indeed an Excel interface
int connectToAllInstances()
{
	IBindCtx* pbc;
	HRESULT hr;

	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) {
		std::cout << "\nError calling CoInitializeEx";
		return 1;
	}

	// Get a BindCtx - Bind Context Object
	hr = CreateBindCtx(0, &pbc);
	if (FAILED(hr)) {
		std::cout << "\nError calling CreateBindCtx";
		return 1;
	}

	// Gets the Running Object Table (ROT)
	IRunningObjectTable* prot;
	hr = pbc->GetRunningObjectTable(&prot);
	if (FAILED(hr)) {
		pbc->Release();
		return 1;
	}

	// Get Enumeration Interface
	IEnumMoniker* pem;
	hr = prot->EnumRunning(&pem);
	if (FAILED(hr)) {
		prot->Release();
		pbc->Release();
		return 1;
	}

	// http://support.microsoft.com/kb/190985
	// http://support.microsoft.com/kb/238975
	pem->Reset();
	ULONG fetched;
	IMoniker* pmon;
	int n = 0;
	while (pem->Next(1, &pmon, &fetched) == S_OK) {
		// Get DisplayName
		LPOLESTR pName;
		pmon->GetDisplayName(pbc, NULL, &pName);

		// Convert to ASCII
		char szName[512];
		WideCharToMultiByte(CP_ACP, 0, pName, -1, szName, 512, NULL, NULL);
		std::cout << "\nName: " << szName;

		IDispatch* pDispObj;
		hr = pmon->BindToObject(pbc, NULL, IID_IDispatch, (void**)&pDispObj);
		if (hr == S_OK) {
			//std::cout << "\nIDispatch interface supported";
			UINT infoCount;
			hr = pDispObj->GetTypeInfoCount(&infoCount);
			for (UINT curEnum(0); curEnum < infoCount; ++curEnum) {
				CComPtr<ITypeInfo> pTypeInfo;
				pDispObj->GetTypeInfo(curEnum, 0, &pTypeInfo);
				CComBSTR interfaceName;
				pTypeInfo->GetDocumentation(-1, &interfaceName, 0, 0, 0);
				std::wcout << "\n --" << interfaceName.m_str;
			}
			//LPOLESTR szDoc = 
			//hr = pDisp->GetIDsOfNames(
		} else if (hr == E_NOINTERFACE) {
			std::cout << "\nIDispatch interface not supported";
		}
		
		LPCLSID pclsid;
		pclsid = new CLSID;
		hr = CLSIDFromString(L"Excel.Application", pclsid);
		if (FAILED(hr)) {
			std::cout << "\nError";
			//return 1;
		}
		
		
	}

	pmon->Release();

	pem->Release();
	prot->Release();
	pbc->Release();

	CoUninitialize();
}

*/

int function_zero ( void ) {
	return 200;
}
int function_one ( int one ) {
	return one * 3;
}

int function_two ( int one, int two ) {
	return one + two;
}

int function_three ( int one, int two, int three ) {
	return one + two + three;
}

extern "C" int __cdecl func_0 ( void );
extern "C" int __cdecl func_1 ( int );
extern "C" int __cdecl func_2 ( int, int );
extern "C" int __cdecl func_3 ( int, int, int );

extern "C" int __cdecl  call_function ( int, int );

TEST_CASE ( "Calling C++ version of each function", "[c-method]" ) {
    REQUIRE ( function_zero ( ) == 200 );
	REQUIRE ( function_one ( 10 ) == 30 );
	REQUIRE ( function_two ( 10, 20 ) == 30 );
	REQUIRE ( function_three ( 10, 20, 30 ) == 60 );
}

TEST_CASE ( "Calling 0-parameter assembly function with C-Calling conventions", "[c-method-0-parm]" ) {
    REQUIRE ( func_0 ( ) == 300 );
}

TEST_CASE ( "Calling 1-parameter assembly function with C-Calling conventions", "[c-method-1-parm]" ) {
	REQUIRE ( func_1 ( 10 ) == 30 );
	REQUIRE ( func_1 ( 20 ) == 60 );
	REQUIRE ( func_1 ( 30 ) == 90 );
}

TEST_CASE ( "Calling 2-parameter assembly function with C-Calling conventions", "[c-method-2-parm]" ) {
	REQUIRE ( func_2 ( 1, 2 ) == 3 );
	REQUIRE ( func_2 ( 100, 200 ) == 300 );
	REQUIRE ( func_2 ( 18, 1 ) == 19 );
}

TEST_CASE ( "Calling 3-parameter assembly function with C-Calling conventions", "[c-method-3-parm]" ) {
	REQUIRE ( func_3 ( 1, 2, 3 ) == 6 );
	REQUIRE ( func_3 ( 100, 200, 300 ) == 600 );
	REQUIRE ( func_3 ( 18, 1, 2 ) == 21 );
}

TEST_CASE ( "Calling assembly function from assembly function with C-Calling convention", "[c-asm-asm-0-parm]" ) {
	REQUIRE ( call_function ( 11, 20 ) == 31 );
}