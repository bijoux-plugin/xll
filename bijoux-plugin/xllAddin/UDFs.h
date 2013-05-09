
#pragma once

#include <Windows.h>
#include "xlCall.h"

typedef struct funcInfo {
	char excelFuncName[100];
	char dllFuncName[100];
	char excelCategory[100];
	char funcDescription[255];
	char argumentDesc[100];
	int numParams;
} FUNC_INFO;

#ifdef __cplusplus
extern "C"
{
#endif

LPXLOPER WINAPI FSExecuteNumber(int number, 
				LPXLOPER v0, LPXLOPER v1, LPXLOPER v2, LPXLOPER v3, LPXLOPER v4,
				LPXLOPER v5, LPXLOPER v6, LPXLOPER v7, LPXLOPER v8, LPXLOPER v9, LPXLOPER v10,
				LPXLOPER v11, LPXLOPER v12, LPXLOPER v13, LPXLOPER v14, LPXLOPER v15, LPXLOPER v16,
				LPXLOPER v17, LPXLOPER v18, LPXLOPER v19, LPXLOPER v20, LPXLOPER v21, LPXLOPER v22,
				LPXLOPER v23, LPXLOPER v24, LPXLOPER v25, LPXLOPER v26, LPXLOPER v27, LPXLOPER v28);

#ifdef __cplusplus
}
#endif