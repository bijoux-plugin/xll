
#include <Windows.h>
#include <xlCall.64bit.h>

#include "catch.hpp"

__declspec(dllexport) LPXLOPER WINAPIV Four_XLOPER_Parameters(LPXLOPER parm1, LPXLOPER parm2, LPXLOPER parm3, LPXLOPER parm4)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = parm1->val.w + parm2->val.w + parm3->val.w + parm4->val.w;
	return xRes;
}

__declspec(dllexport) LPXLOPER WINAPIV Five_XLOPER_Parameters(LPXLOPER parm1, LPXLOPER parm2, LPXLOPER parm3, LPXLOPER parm4, LPXLOPER parm5)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = parm1->val.w + parm2->val.w + parm3->val.w + parm4->val.w + parm5->val.w;
	return xRes;
}

__declspec(dllexport) LPXLOPER WINAPIV Six_XLOPER_Parameters(LPXLOPER parm1, LPXLOPER parm2, LPXLOPER parm3, LPXLOPER parm4, LPXLOPER parm5, LPXLOPER parm6)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = parm1->val.w + parm2->val.w + parm3->val.w + parm4->val.w + parm5->val.w + parm6->val.w;
	return xRes;
}

__declspec(dllexport) LPXLOPER WINAPIV Twelve_XLOPER_Parameters(LPXLOPER parm1, LPXLOPER parm2, LPXLOPER parm3, LPXLOPER parm4, LPXLOPER parm5, LPXLOPER parm6, LPXLOPER parm7, LPXLOPER parm8, LPXLOPER parm9, LPXLOPER parm10, LPXLOPER parm11, LPXLOPER parm12)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = parm1->val.w + parm2->val.w + parm3->val.w + parm4->val.w + parm5->val.w + parm6->val.w + parm7->val.w + parm8->val.w + parm9->val.w + parm10->val.w + parm11->val.w + parm12->val.w;
	return xRes;
}

extern "C" void *call_function64 ( void *f_ptr, int n_args, void *args );

TEST_CASE ( "Validate sizes of types for 64-bit Excel using XLOPER", "[validate-xloper-4]" ) {
	REQUIRE ( sizeof ( int ) == 4 );
	REQUIRE ( sizeof ( LPXLOPER ) == 8 );
	XLOPER testXlOper;
	testXlOper.val.w = 10;
	testXlOper.xltype = xltypeInt;
	REQUIRE ( sizeof ( XLOPER ) == 24 );
	REQUIRE ( sizeof ( testXlOper ) == 24 );
	REQUIRE ( sizeof ( testXlOper.val.w ) == 2 ); // short
	REQUIRE ( sizeof ( testXlOper.val.num ) == 8 ); // double
	REQUIRE ( sizeof ( testXlOper.val.str ) == 8 ); // LPSTR
	REQUIRE ( sizeof ( testXlOper.val.xbool ) == 2 ); // WORD
	REQUIRE ( sizeof ( testXlOper.xltype ) == 2 ); // WORD
}

TEST_CASE ( "Validate sizes of types for 64-bit Excel using XLOPER12", "[validate-xloper-12]" ) {
	REQUIRE ( sizeof ( int ) == 4 );
	REQUIRE ( sizeof ( LPXLOPER12 ) == 8 );
	XLOPER12 testXlOper;
	testXlOper.val.w = 10;
	testXlOper.xltype = xltypeInt;
	REQUIRE ( sizeof ( XLOPER12 ) == 32 );
	REQUIRE ( sizeof ( testXlOper ) == 32 );
	REQUIRE ( sizeof ( testXlOper.val.w ) == 4 ); // short
	REQUIRE ( sizeof ( testXlOper.val.num ) == 8 ); // double
	REQUIRE ( sizeof ( testXlOper.val.str ) == 8 ); // LPSTR
	REQUIRE ( sizeof ( testXlOper.val.xbool ) == 4 ); // WORD
	REQUIRE ( sizeof ( testXlOper.xltype ) == 4 ); // WORD
}

TEST_CASE ( "Call function with 4 LPXLOPER types from assembly" "[c-asm-c-xl-4-parm]" ) {
	void *f_ptr = &Four_XLOPER_Parameters;
	LPXLOPER *args = new LPXLOPER [ 4 ];
	args[0] = new XLOPER;
	args[0]->xltype = xltypeInt;
	args[0]->val.w = 1;
	args[1] = new XLOPER;
	args[1]->xltype = xltypeInt;
	args[1]->val.w = 4;
	args[2] = new XLOPER;
	args[2]->xltype = xltypeInt;
	args[2]->val.w = 6;
	args[3] = new XLOPER;
	args[3]->xltype = xltypeInt;
	args[3]->val.w = 8;

	LPXLOPER xlResult = (LPXLOPER)call_function64 ( f_ptr, 4, args );
	REQUIRE ( xlResult->val.w == 19 );
	REQUIRE ( xlResult->xltype == xltypeInt );
}

TEST_CASE ( "Call function with 5 LPXLOPER types from assembly" "[c-asm-c-xl-5-parm]" ) {
	int total = 0;
	void *f_ptr = &Five_XLOPER_Parameters;
	LPXLOPER *args = new LPXLOPER [ 5 ];
	for ( int i=0; i < 5; i++ ) {
		args[i] = new XLOPER;
		args[i]->xltype = xltypeInt;
		args[i]->val.w = i;
		total += i;
	}
	LPXLOPER xlResult = (LPXLOPER)call_function64 ( f_ptr, 5, args );
	REQUIRE ( xlResult->val.w == total );
	REQUIRE ( xlResult->xltype == xltypeInt );
}

TEST_CASE ( "Call function with 6 LPXLOPER types from assembly" "[c-asm-c-xl-6-parm]" ) {
	int num_of_parms = 6;
	int total = 0;
	void *f_ptr = &Six_XLOPER_Parameters;
	LPXLOPER *args = new LPXLOPER [ num_of_parms ];
	for ( int i=0; i < num_of_parms; i++ ) {
		args[i] = new XLOPER;
		args[i]->xltype = xltypeInt;
		args[i]->val.w = i;
		total += i;
	}
	LPXLOPER xlResult = (LPXLOPER)call_function64 ( f_ptr, num_of_parms, args );
	REQUIRE ( xlResult->val.w == total );
	REQUIRE ( xlResult->xltype == xltypeInt );
}

TEST_CASE ( "Call function with 12 LPXLOPER types from assembly" "[c-asm-c-xl-12-parm]" ) {
	int num_of_parms = 12;
	int total = 0;
	void *f_ptr = &Twelve_XLOPER_Parameters;
	LPXLOPER *args = new LPXLOPER [ num_of_parms ];
	for ( int i=0; i < num_of_parms; i++ ) {
		args[i] = new XLOPER;
		args[i]->xltype = xltypeInt;
		args[i]->val.w = i;
		total += i;
	}
	LPXLOPER xlResult = (LPXLOPER)call_function64 ( f_ptr, num_of_parms, args );
	REQUIRE ( xlResult->val.w == total );
	REQUIRE ( xlResult->xltype == xltypeInt );
}