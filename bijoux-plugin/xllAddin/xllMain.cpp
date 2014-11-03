
#include <Windows.h>

#include <tchar.h>

#include "UDFs.h"
#include "xllMain.h"
#include "xlCall.h"
#include "Framework.h"
#include "XLUtil.h"

#include <vector>
#include <iostream>
#include <string>

HMODULE hCalypso2Excel = NULL;
std::vector<FUNC_INFO*> all_functions;

void RegisterExcelFunction (LPXLOPER xDLL, const char* xllFunctionName, const char* excelFunctionName,
	const char* pxTypeText, char* pxFuncDescription, char* excelCategory, char* argumentDesc)
{
	const int argumentCount = 10;
	// [0]pxModuleText, [1]pxProcedure, [2]pxTypeText
	// [3]pxFunctionText, [4]pxArgumentText
	// [5]pxMacroType, [6]pxCategory, [7]pxShortcutText, [8]pxHelpTopic, [9]pxFunctionHelp
	static LPXLOPER input[argumentCount];
	input[0] = (LPXLOPER FAR) xDLL;
	input[1] = (LPXLOPER FAR) XLUtil::MakeExcelString2(xllFunctionName);
	input[2] = (LPXLOPER FAR) XLUtil::MakeExcelString2(pxTypeText);
	input[3] = (LPXLOPER FAR) XLUtil::MakeExcelString2(excelFunctionName);
	input[4] = (LPXLOPER FAR) XLUtil::MakeExcelString2(argumentDesc);
	input[5] = (LPXLOPER FAR) XLUtil::MakeExcelString2("1");
	input[6] = (LPXLOPER FAR) XLUtil::MakeExcelString2(excelCategory);
	input[7] = (LPXLOPER FAR) XLUtil::MakeExcelString3(0);
	input[8] = (LPXLOPER FAR) XLUtil::MakeExcelString3(0);
	input[9] = (LPXLOPER FAR) XLUtil::MakeExcelString2(pxFuncDescription);

	int result = Excel4v (xlfRegister, NULL, argumentCount, (LPXLOPER FAR*)input);

	for ( int i = 1; i < argumentCount; i++)
	{
		XLUtil::FreeContents ( input[i] );
		free ( input[i] );
	}
}

void loadListOfFunctions(LPXLOPER xDLL)
{
	int numberOfFunctions = 0;
	typedef int (WINAPIV *NUM_FUNC_PROC)(void);
	NUM_FUNC_PROC fpNumberOfFunctions = (NUM_FUNC_PROC) GetProcAddress ( hCalypso2Excel, "GetNumberOfFunctions" );
	numberOfFunctions = (fpNumberOfFunctions)();

	typedef void (WINAPIV *FUNC_INFO_PROC)(int, int, char *, int);
	FUNC_INFO_PROC fpFunctionInfo = (FUNC_INFO_PROC) GetProcAddress ( hCalypso2Excel, "GetFunctionInfo" );
	char memory[100];
	for (int i=0; i < numberOfFunctions; i++) {
		FUNC_INFO *func_info = new FUNC_INFO;

		(fpFunctionInfo)(i, 0, func_info->excelFuncName, 100);
		(fpFunctionInfo)(i, 1, func_info->dllFuncName, 100);
		(fpFunctionInfo)(i, 2, func_info->excelCategory, 100);
		(fpFunctionInfo)(i, 3, func_info->funcDescription, 100);
		(fpFunctionInfo)(i, 4, func_info->argumentDesc, 100);
		(fpFunctionInfo)(i, 5, memory, 10);
		func_info->numParams = std::atoi(memory);
		all_functions.push_back(func_info);

		// xllFunctionName, excelFunctionName
		char xllFunctionName[20] = { 0 };
		sprintf(xllFunctionName, "FS%d", i+1); // Function1, Function2 ...
		RegisterExcelFunction (xDLL, xllFunctionName, func_info->excelFuncName, "RPPPPPPPPPPPPPPPPPPPPPPPPPPPPP", 
			func_info->funcDescription, func_info->excelCategory, func_info->argumentDesc);
	}
}

void loadReferenceToDLL(LPXLOPER xDLL)
{
	// Load Reference to our DLL
	TCHAR szDir[260];
	GetCurrentDirectory(sizeof(szDir), szDir);
	LPCTSTR lpFileName = _T("C:\\xllFunctions.dll");
	hCalypso2Excel = LoadLibrary ( lpFileName );

	if ( hCalypso2Excel == NULL )
	{
		int lastError = GetLastError();
		//cout << "\nCould not load the DLL: " << lastError;
		int i=0;
	}

	loadListOfFunctions(xDLL);
}

#ifdef __cplusplus
extern "C"
{
#endif


BOOL __stdcall xlAutoOpen(void)
{
	static XLOPER xDLL;
	Excel4 ( xlGetName, &xDLL, 0 );

	loadReferenceToDLL(&xDLL);

	Excel4 ( xlFree, 0, 1, (LPXLOPER) &xDLL );

    return 1;
}

// Called when Excel closes XLL
BOOL __stdcall xlAutoClose(void)
{
	// Free Reference to our DLL
	BOOL status = FreeLibrary ( hCalypso2Excel );
	if ( status == FALSE )
	{
		int lastError = GetLastError();
		//std::cout << "\nCould not unload the DLL: " << lastError;
	}

	return 1;
}

__declspec(dllexport) LPXLOPER WINAPI xlAutoRegister(LPXLOPER pxName)
{
	return NULL;
}

// Called when user activates the XLL from the Add-In Manager
__declspec(dllexport) int WINAPI xlAutoAdd(void)
{
	return 1;
}

// Called when user deactivates XLL from the Add-In Manager
__declspec(dllexport) int WINAPI xlAutoRemove(void)
{
	return 1;
}
// Called by Excel just after an XLL worksheet function returns an XLOPER12 with a flag set
// that tells it there is memory that the XLL still needs to release
// Good for returning dynamically allocated arrays, strings and external references
// to worksheet without memory leaks
__declspec(dllexport) void WINAPI xlAutoFree(LPXLOPER px)
{
	int free_me=0;
}

// Support for descriptive information about the add-in(s)
// You can add a new customized title for the user, but
// unfortunately, only an add-in written in Microsoft Visual Basic
// can add a description string.
__declspec(dllexport) LPXLOPER WINAPI xlAddInManagerInfo ( LPXLOPER xAction ) {

	static XLOPER xInfo, xIntAction;

	XLOPER xlInt;
	xlInt.xltype = xltypeInt;
	xlInt.val.w;
	Excel4 ( xlCoerce, &xIntAction, 2, xAction, xlInt );

	if ( xIntAction.val.w == 1) {
		xInfo.xltype = xltypeStr;
		xInfo.val.str = "\10xllAddin";
	} else {
		xInfo.xltype = xltypeErr;
		xInfo.val.err = xlerrValue;
	}

	return (LPXLOPER) &xInfo;
}

#define DECLARE_EXCEL_FUNCTION(number) \
__declspec(dllexport) LPXLOPER WINAPI FS##number (LPXLOPER v0, LPXLOPER v1, LPXLOPER v2 \
	,LPXLOPER v3, LPXLOPER v4, LPXLOPER v5, LPXLOPER v6, LPXLOPER v7, LPXLOPER v8 \
	,LPXLOPER v9, LPXLOPER v10, LPXLOPER v11, LPXLOPER v12, LPXLOPER v13, LPXLOPER v14, LPXLOPER v15, LPXLOPER v16 \
	,LPXLOPER v17, LPXLOPER v18, LPXLOPER v19, LPXLOPER v20, LPXLOPER v21, LPXLOPER v22, LPXLOPER v23, LPXLOPER v24, LPXLOPER v25 \
	,LPXLOPER v26, LPXLOPER v27, LPXLOPER v28) \
{ \
	return FSExecuteNumber(number, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28); \
} 

DECLARE_EXCEL_FUNCTION(0)
DECLARE_EXCEL_FUNCTION(1)
DECLARE_EXCEL_FUNCTION(2)
DECLARE_EXCEL_FUNCTION(3)


#ifdef __cplusplus
}
#endif