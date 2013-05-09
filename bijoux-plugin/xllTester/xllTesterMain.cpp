

#include <iostream>

#include <ObjIdl.h>
#include <Objbase.h>
#include <atlbase.h>

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


int main(void)
{
	
	return (0);
}