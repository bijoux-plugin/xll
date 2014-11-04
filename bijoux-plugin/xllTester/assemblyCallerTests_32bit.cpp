
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

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

extern "C" int __cdecl  call_function ( void *f_ptr, int n_args, int *args );

TEST_CASE ( "Calling C++ version of each function", "[c-all-methods]" ) {
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

TEST_CASE ( "Calling C function from assembly function with C-Calling convention", "[c-asm-c-0-parm]" ) {
	REQUIRE ( call_function ( &function_zero, 0, NULL ) == 200 );
}

TEST_CASE ( "Calling C function with 1 parameter from assembly function with C-Calling convention", "[c-asm-c-1-parm]" ) {
	int args [ ] = { 1 };
	REQUIRE ( call_function ( &function_one, 1, args ) == 3 );
	args [ 0 ] = 5;
	REQUIRE ( call_function ( &function_one, 1, args ) == 15 );
	args [ 0 ] = 15;
	REQUIRE ( call_function ( &function_one, 1, args ) == 45 );

	// Allocate a new array to make sure the stack is uncorrupted
	int args_2 [ ] = { 20 };
	REQUIRE ( call_function ( &function_one, 1, args_2 ) == 60 );
}

TEST_CASE ( "Calling C function with 2 parameters from assembly function with C-Calling convention", "[c-asm-c-2-parm]" ) {
	int args [ ] = { 1, 4 };
	REQUIRE ( call_function ( &function_two, 2, args ) == 5 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	REQUIRE ( call_function ( &function_two, 2, args ) == 15 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	REQUIRE ( call_function ( &function_two, 2, args ) == 65 );
}

TEST_CASE ( "Calling C function with 3 parameters from assembly function with C-Calling convention", "[c-asm-c-3-parm]" ) {
	int args [ ] = { 1, 4, 6 };
	REQUIRE ( call_function ( &function_three, 3, args ) == 11 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	args [ 2 ] = 222;
	REQUIRE ( call_function ( &function_three, 3, args ) == 237 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	args [ 2 ] = -22;
	REQUIRE ( call_function ( &function_three, 3, args ) == 43 );
}

TEST_CASE ( "Calling assembly function from assembly function with C-Calling convention with 0 parameters", "[c-asm-asm-0-parm]" ) {
	REQUIRE ( call_function ( &func_0, 0, NULL ) == 300 );
}

TEST_CASE ( "Calling assembly function with 1 parameter from assembly function with C-Calling convention", "[c-asm-asm-1-parm]" ) {
	int args [ ] = { 1 };
	REQUIRE ( call_function ( &func_1, 1, args ) == 3 );
	args [ 0 ] = 5;
	REQUIRE ( call_function ( &func_1, 1, args ) == 15 );
	args [ 0 ] = 15;
	REQUIRE ( call_function ( &func_1, 1, args ) == 45 );

	// Allocate a new array to make sure the stack is uncorrupted
	int args_2 [ ] = { 20 };
	REQUIRE ( call_function ( &func_1, 1, args_2 ) == 60 );
}

TEST_CASE ( "Calling assembly function with 2 parameters from assembly function with C-Calling convention", "[c-asm-asm-2-parm]" ) {
	int args [ ] = { 1, 4 };
	REQUIRE ( call_function ( &func_2, 2, args ) == 5 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	REQUIRE ( call_function ( &func_2, 2, args ) == 15 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	REQUIRE ( call_function ( &func_2, 2, args ) == 65 );
}

TEST_CASE ( "Calling assembly function with 3 parameters from assembly function with C-Calling convention", "[c-asm-asm-3-parm]" ) {
	int args [ ] = { 1, 4, 6 };
	REQUIRE ( call_function ( &func_3, 3, args ) == 11 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	args [ 2 ] = 222;
	REQUIRE ( call_function ( &func_3, 3, args ) == 237 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	args [ 2 ] = -22;
	REQUIRE ( call_function ( &func_3, 3, args ) == 43 );
}