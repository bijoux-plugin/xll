
#include "UDFs.h"

#include <Windows.h>
#include <vector>
#include "xllMain.h"
#include "xlCall.h"
#include "XLUtil.h"

extern HMODULE hCalypso2Excel;
extern std::vector<FUNC_INFO*> all_functions;

//extern "C" void *call_function ( void *f_ptr, int n_args, void *args );
extern "C" LPXLOPER call_function ( void *f_ptr, int n_args, LPXLOPER *args );

/*
LPXLOPER call_custom_function ( int number_of_parameters, void *parameters, void *func_ptr ) {

	LPXLOPER result;
	int i;

	int s_ret = sizeof ( result );
	int s_void = sizeof ( parameters );
	int s_f_ptr = sizeof ( func_ptr );
#ifdef _WIN64
#else
	__asm
	{
		mov i, 0
		mov ecx, number_of_parameters
		loopstart:
			; must copy values[i] into eax and push
			mov eax, parameters
			add eax, i
			mov eax, [eax]
			push eax
			add i, 4
			dec ecx
			jnz loopstart
		call func_ptr
		mov result, eax
		mov ecx, number_of_parameters
		loopclean:
			pop eax
			dec ecx
			jnz loopclean
	}
#endif

	return result;
}
*/

#ifdef __cplusplus
extern "C"
{
#endif

LPXLOPER WINAPI FSExecuteNumber(int funcNumber, 
				LPXLOPER v0, LPXLOPER v1, LPXLOPER v2, LPXLOPER v3, LPXLOPER v4,
				LPXLOPER v5, LPXLOPER v6, LPXLOPER v7, LPXLOPER v8, LPXLOPER v9, LPXLOPER v10,
				LPXLOPER v11, LPXLOPER v12, LPXLOPER v13, LPXLOPER v14, LPXLOPER v15, LPXLOPER v16,
				LPXLOPER v17, LPXLOPER v18, LPXLOPER v19, LPXLOPER v20, LPXLOPER v21, LPXLOPER v22,
				LPXLOPER v23, LPXLOPER v24, LPXLOPER v25, LPXLOPER v26, LPXLOPER v27, LPXLOPER v28)
{
	LPXLOPER xOpers[] = {v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
						 v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
						 v20, v21, v22, v23, v24, v25, v26, v27, v28};
	LPXLOPER xRes = new XLOPER;

	if (funcNumber <= all_functions.size()) {
		FUNC_INFO* fInfo = all_functions.at(funcNumber-1);
		void *func_ptr = GetProcAddress ( hCalypso2Excel, fInfo->dllFuncName );
		bool allParametersPresent = true;
		for ( int i=0; i < fInfo->numParams; i++ ) {
			if ( xOpers[i]->xltype == xltypeMissing ) {
				allParametersPresent = false;
				XLUtil::MakeString(xRes, "Missing Parameter(s)");
				return xRes;
				break;
			}
		}
		xRes = call_function ( func_ptr, fInfo->numParams, xOpers );
	} else {
		XLUtil::MakeString(xRes, "Invalid Function");
	}

	return xRes;
}

#ifdef __cplusplus
}
#endif