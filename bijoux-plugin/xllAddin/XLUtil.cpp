/*******************************************************************************
* This program and the accompanying materials
* are made available under the terms of the Common Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/cpl-v10.html
*
* Contributors:
*     Peter Smith
*******************************************************************************/

//#include "stdafx.h"
#include <windows.h>
#include "xlcall.h"
#include "XLUtil.h"

void XLUtil::MakeNumber(LPXLOPER lpXLOper, const int number)
{
	lpXLOper->xltype = xltypeNum;
	lpXLOper->val.num = number;
}

void XLUtil::MakeString(LPXLOPER lpXLOper, const char* string)
{
	lpXLOper->xltype = xltypeStr;
	lpXLOper->val.str = XLUtil::MakeExcelString(string);
}

void XLUtil::MakeMissing(LPXLOPER lpXLOper)
{
	lpXLOper->xltype = xltypeNil;
}
void XLUtil::MakeBoolean(LPXLOPER lpXLOper, const bool boolean_value)
{
	lpXLOper->xltype = xltypeBool;
	if ( boolean_value )
		lpXLOper->val.xbool = TRUE;
	else
		lpXLOper->val.xbool = FALSE;
}

LPXLOPER XLUtil::MakeBoolean(const bool boolean_value)
{
	LPXLOPER xl = new XLOPER;
	xl->xltype = xltypeBool;
	if ( boolean_value )
		xl->val.xbool = TRUE;
	else
		xl->val.xbool = FALSE;

	return xl;
}
LPXLOPER XLUtil::MakeMissing()
{
	LPXLOPER xl = new XLOPER;
	xl->xltype = xltypeMissing;
	return xl;
}
LPXLOPER XLUtil::MakeNumber(const int number)
{
	LPXLOPER xl = new XLOPER;
	xl->xltype = xltypeNum;
	xl->val.num = number;
	return xl;
}
// Make an excel string
LPSTR XLUtil::MakeExcelString(const char* string)
{
	if(string == NULL) return NULL;
	size_t len = strlen(string);
	if(len > 255) len = 255; // Excel strings are limited to 255 chars
	char* temp = (char*) malloc(len + 2);
	memcpy(temp + 1, string, len);
	temp[0] = (BYTE) len;
	temp[len+1] = 0;
	return temp;
}

LPXLOPER XLUtil::MakeExcelString2(const char* string)
{
	LPXLOPER xl = new XLOPER;
	if(string == NULL) {
		xl->xltype = xltypeMissing;
	} else {
		xl->xltype = xltypeStr;
		xl->val.str = MakeExcelString(string);
	}

	return xl;
}

LPXLOPER XLUtil::MakeExcelString3(char* lcstr)
{
	LPXLOPER xl = new XLOPER;
	if(lcstr == NULL) {
		xl->xltype = xltypeMissing;
	} else {
		xl->xltype = xltypeStr;
		xl->val.str = lcstr;
		//xl->val.str = 0;
	}
	return xl;
}

// A helper function used to register a function
int XLUtil::RegisterFunction(LPXLOPER xllName,
					  const char* procedure, const char* typeText, const char* functionText,
					  const char* argumentText, const char* macroType, const char* category,
					  const char* shortcutText, const char* helpTopic,
					  const char* functionHelp, const char* argumentHelp, bool command)
{
	static XLOPER args[10];
	for(int i = 0; i < 10; i++) {
		args[i].val.str = NULL;
		args[i].xltype = xltypeStr;
	}
	args[0].val.str = MakeExcelString(procedure);
	args[1].val.str = MakeExcelString(typeText);
	args[2].val.str = MakeExcelString(functionText);
	args[3].val.str = MakeExcelString(argumentText);
	args[4].val.str = MakeExcelString(macroType);
	args[5].val.str = MakeExcelString(category);
	args[6].val.str = MakeExcelString(shortcutText);
	if(!command) {
		args[7].val.str = MakeExcelString(helpTopic);
		args[8].val.str = MakeExcelString(functionHelp);
		args[9].val.str = MakeExcelString(argumentHelp);
	}

	// Check types for NULL
	for(int i = 0; i < 10; i++) {
		if(args[i].val.str == NULL)
			args[i].xltype = xltypeMissing;
	}

	int res = 0;
	if(!command) {
		res = Excel4(xlfRegister, 0, 11, (LPXLOPER) xllName, (LPXLOPER) &args[0],
			(LPXLOPER) &args[1], (LPXLOPER) &args[2], (LPXLOPER) &args[3],
			(LPXLOPER) &args[4], (LPXLOPER) &args[5], (LPXLOPER) &args[6],
			(LPXLOPER) &args[7], (LPXLOPER) &args[8], (LPXLOPER) &args[9]);
	} else {
		res = Excel4(xlfRegister, 0, 8, (LPXLOPER) xllName, (LPXLOPER) &args[0],
			(LPXLOPER) &args[1], (LPXLOPER) &args[2], (LPXLOPER) &args[3],
			(LPXLOPER) &args[4], (LPXLOPER) &args[5], (LPXLOPER) &args[6]);
	}


	// Free strings
	for(int i = 0; i < 10; i++) {
		if(!args[i].val.str == NULL)
			free(args[i].val.str);
	}

	return res;
}

int XLUtil::RegisterCommand(LPXLOPER xllName,
					  const char* procedure, const char* typeText, const char* functionText,
					  const char* argumentText, const char* macroType, const char* category,
					  const char* shortcutText)
{
	return RegisterFunction(xllName, procedure, typeText, functionText, argumentText,
		macroType, category, shortcutText, NULL, NULL, NULL, true);
}

void XLUtil::CopyValue(LPXLOPER xloperSrc, LPXLOPER xloperDst)
{
	memcpy(xloperDst, xloperSrc, sizeof(XLOPER));
	xloperDst->xltype = (xloperSrc->xltype & ~(xlbitXLFree | xlbitDLLFree));
}

void XLUtil::FreeContents(LPXLOPER px)
{
	switch(px->xltype & ~(xlbitXLFree | xlbitDLLFree))
	{
		case xltypeMulti:
			for ( int i = (px->val.array.rows) * (px->val.array.columns) - 1; 
							i >=0; i--)
			{
				FreeContents ( &px->val.array.lparray[i] );
			}
			free(px->val.array.lparray);
			break;
		case xltypeStr:
			if ( px != NULL && px->val.str != NULL )
			{
				free(px->val.str);
				px->val.str = NULL;
			}
			break;
		case xltypeBigData:
			if ( px != NULL && px->val.bigdata.h.lpbData != NULL )
			{
				free(px->val.bigdata.h.lpbData);
				px->val.bigdata.h.lpbData = NULL;
			}
			break;
		default:
			break;
	}
}

void XLUtil::MakeDLLFree(LPXLOPER px)
{
	if ( px != NULL )
		px->xltype |= xlbitDLLFree;
}

void XLUtil::MakeXLFree(LPXLOPER px)
{
	if ( px != NULL )
		px->xltype |= xlbitXLFree;
}

// Assumes a two-column array with key/value on each row
LPXLOPER XLMap::get(LPXLOPER pmap, const char* key)
{
	if(key == NULL) return NULL;
	int len = strlen(key);
	int rows = pmap->val.array.rows;
	if(rows == 0) return NULL;
	int cols = pmap->val.array.columns;
	if(cols != 2) return NULL;
	int cells = rows * cols;
	for(int i =0; i < cells; i += 2)
	{
		LPXLOPER k = &pmap->val.array.lparray[i];
		if((k->xltype & ~(xlbitXLFree | xlbitDLLFree)) != xltypeStr)
			continue;
		int l = k->val.str[0];
		if(len != l)
			continue;
		if(strncmp(key, &k->val.str[1], l) == 0)
			return &pmap->val.array.lparray[i+1];
	}
	return NULL;
}

char* XLMap::getString(LPXLOPER pmap, const char* key)
{
	LPXLOPER px = get(pmap, key);
	if(px != NULL && (px->xltype & ~(xlbitXLFree | xlbitDLLFree)) == xltypeStr) {
		return px->val.str;
	}
	return NULL;
}

bool XLMap::getBoolean(LPXLOPER pmap, const char* key)
{
	LPXLOPER px = get(pmap, key);
	if(px != NULL && (px->xltype & ~(xlbitXLFree | xlbitDLLFree)) == xltypeBool)
	{
		return (px->val.xbool == 1 );
	}
	return false;
}

int XLMap::getInteger(LPXLOPER pmap, const char* key)
{
	LPXLOPER px = get(pmap, key);
	if(px != NULL && (px->xltype & ~(xlbitXLFree | xlbitDLLFree)) == xltypeInt) {
		return px->val.w;
	}
	return -1;
}

void XLUtil::WriteMessageToStatusBar ( const char* format, ... )
{
	static XLOPER result;
	static XLOPER argLogical;
	static XLOPER argText;

	char message[255];
	va_list args;
	va_start ( args, format );
//	vsprintf_s ( message, 255, format, args );
	va_end ( args );

	XLUtil::MakeBoolean ( &argLogical, true );
	XLUtil::MakeString ( &argText, message );

	Excel4 ( xlcMessage, &result, 2, &argLogical, &argText );

	XLUtil::FreeContents ( &argText );
}

void XLUtil::RemoveLastMessageFromStatusBar ( )
{
	static XLOPER result;
	static XLOPER argLogical;

	XLUtil::MakeBoolean ( &argLogical, false );
	
	Excel4 ( xlcMessage, &result, 1, &argLogical );
}