
#include <Windows.h>
#include <xlCall.64bit.h>

#include "catch.hpp"

__declspec(dllexport) LPXLOPER WINAPIV PriceEuropean(LPXLOPER parm1, LPXLOPER parm2, LPXLOPER parm3, LPXLOPER parm4)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = parm1->val.num + parm2->val.num + parm3->val.num + parm4->val.num;
	return xRes;
}

extern "C" void *call_function ( void *f_ptr, int n_args, void *args );

/*
LPXLOPER call_custom_function ( void *func_ptr, int number_of_parameters, void *parameters ) {
	LPXLOPER xlResult = new XLOPER;
	xlResult->xltype = xltypeInt;
	xlResult->val.w = 19;

	return xlResult;
}
*/

TEST_CASE ( "Call function with LPXLOPER types from assembly" "[c-asm-c-xl-4-parm]" ) {
	void *f_ptr = &PriceEuropean;
	LPXLOPER args = new XLOPER [ 4 ];
	args[0].xltype = xltypeInt;
	args[0].val.w = 1;
	args[1].xltype = xltypeInt;
	args[1].val.w = 4;
	args[2].xltype = xltypeInt;
	args[2].val.w = 6;
	args[3].xltype = xltypeInt;
	args[4].val.w = 8;

	int s_1 = sizeof ( f_ptr );
	int s_2 = sizeof ( LPXLOPER );
	LPXLOPER xlResult = (LPXLOPER)call_function ( f_ptr, 4, args );
	REQUIRE ( xlResult->val.w == 19 );
	REQUIRE ( xlResult->xltype == xltypeInt );
}