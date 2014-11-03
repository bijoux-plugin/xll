
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

int function_four ( int one, int two, int three, int four ) {
	return one + two + three + four;
}

int function_five ( int one, int two, int three, int four, int five ) {
	return one + two + three + four + five;
}

int function_six ( int one, int two, int three, int four, int five, int six ) {
	return one + two + three + four + five + six;
}

int function_twelve ( int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten, int eleven, int twelve ) {
	return one + two + three + four + five + six + seven + eight + nine + ten + eleven + twelve;
}

extern "C" int func_0 ( void );
extern "C" int func_1 ( int );
extern "C" int func_2 ( int, int );
extern "C" int func_3 ( int, int, int );
extern "C" int func_4 ( int, int, int, int );
extern "C" int func_5 ( int, int, int, int, int );
extern "C" int func_6 ( int, int, int, int, int, int );

extern "C" int call_function ( void *f_ptr, int n_args, int *args );

TEST_CASE ( "Calling C++ version of each function", "[c-all-methods]" ) {
    REQUIRE ( function_zero ( ) == 200 );
	REQUIRE ( function_one ( 10 ) == 30 );
	REQUIRE ( function_two ( 10, 20 ) == 30 );
	REQUIRE ( function_three ( 10, 20, 30 ) == 60 );
	REQUIRE ( function_four ( 10, 20, 30, 40 ) == 100 );
	REQUIRE ( function_five ( 10, 20, 30, 40, 50 ) == 150 );
	REQUIRE ( function_six ( 10, 20, 30, 40, 50, 60 ) == 210 );
}

TEST_CASE ( "Calling 0-parameter assembly function", "[c-method-0-parm]" ) {
 	REQUIRE ( func_0 ( ) == 300 );
}

TEST_CASE ( "Calling 1-parameter assembly function", "[c-method-1-parm]" ) {
	REQUIRE ( func_1 ( 10 ) == 30 );
	REQUIRE ( func_1 ( 20 ) == 60 );
	REQUIRE ( func_1 ( 30 ) == 90 );
}

TEST_CASE ( "Calling 2-parameter assembly function", "[c-method-2-parm]" ) {
	REQUIRE ( func_2 ( 1, 2 ) == 3 );
	REQUIRE ( func_2 ( 100, 200 ) == 300 );
	REQUIRE ( func_2 ( 18, 1 ) == 19 );
}

TEST_CASE ( "Calling 3-parameter assembly function", "[c-method-3-parm]" ) {
	REQUIRE ( func_3 ( 1, 2, 3 ) == 6 );
	REQUIRE ( func_3 ( 100, 200, 300 ) == 600 );
	REQUIRE ( func_3 ( 18, 1, 2 ) == 21 );
}

TEST_CASE ( "Calling 4-parameter assembly function", "[c-method-3-parm]" ) {
	REQUIRE ( func_4 ( 1, 2, 3, 4 ) == 10 );
	REQUIRE ( func_4 ( 100, 200, 300, 150 ) == 750 );
	REQUIRE ( func_4 ( 18, 1, 2, -10 ) == 11 );
}

TEST_CASE ( "Calling 5-parameter assembly function", "[c-method-3-parm]" ) {
	REQUIRE ( func_5 ( 1, 2, 3, 4, 5 ) == 15 );
	REQUIRE ( func_5 ( 100, 200, 300, 450, 150 ) == 1200 );
	REQUIRE ( func_5 ( 18, 1, 2, 10, 11 ) == 42 );
}

TEST_CASE ( "Calling 6-parameter assembly function", "[c-method-3-parm]" ) {
	REQUIRE ( func_6 ( 1, 2, 3, 4, 5, 6 ) == 21 );
	REQUIRE ( func_6 ( 100, 200, 300, 450, 150, 99 ) == 1299 );
	REQUIRE ( func_6 ( 18, 1, 2, 10, 11, 9 ) == 51 );
}

TEST_CASE ( "Calling C function from assembly function", "[c-asm-c-0-parm]" ) {
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

TEST_CASE ( "Calling C function with 4 parameters from assembly function", "[c-asm-c-4-parm]" ) {
	int args [ ] = { 1, 4, 6, 7 };
	REQUIRE ( call_function ( &function_four, 4, args ) == 18 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	args [ 2 ] = 222;
	args [ 3 ] = 111;
	REQUIRE ( call_function ( &function_four, 4, args ) == 348 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	args [ 2 ] = -22;
	args [ 3 ] = 100;
	REQUIRE ( call_function ( &function_four, 4, args ) == 143 );
}

TEST_CASE ( "Calling C function with 5 parameters from assembly function", "[c-asm-c-5-parm]" ) {
	int args [ ] = { 1, 4, 6, 7, 20 };
	REQUIRE ( call_function ( &function_five, 5, args ) == 38 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	args [ 2 ] = 222;
	args [ 3 ] = 111;
	args [ 4 ] = 100;
	REQUIRE ( call_function ( &function_five, 5, args ) == 448 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	args [ 2 ] = -22;
	args [ 3 ] = 100;
	args [ 4 ] = 123;
	REQUIRE ( call_function ( &function_five, 5, args ) == 266 );
}

TEST_CASE ( "Calling C function with 6 parameters from assembly function", "[c-asm-c-6-parm]" ) {
	int args [ ] = { 1, 4, 6, 7, 20, 100 };
	REQUIRE ( call_function ( &function_six, 6, args ) == 138 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	args [ 2 ] = 222;
	args [ 3 ] = 111;
	args [ 4 ] = 100;
	args [ 5 ] = 1000;
	REQUIRE ( call_function ( &function_six, 6, args ) == 1448 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	args [ 2 ] = -22;
	args [ 3 ] = 100;
	args [ 4 ] = 123;
	args [ 5 ] = 2000;
	REQUIRE ( call_function ( &function_six, 6, args ) == 2266 );
}

TEST_CASE ( "Calling C function with 12 parameters from assembly function", "[c-asm-c-12-parm]" ) {
	int args [ ] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	REQUIRE ( call_function ( &function_twelve, 12, args ) == 78 );
	args [ 0 ] = 5;
	args [ 1 ] = 10;
	args [ 2 ] = 222;
	args [ 3 ] = 111;
	args [ 4 ] = 100;
	args [ 5 ] = 1000;
	REQUIRE ( call_function ( &function_twelve, 12, args ) == 1505 );
	args [ 0 ] = 55;
	args [ 1 ] = 10;
	args [ 2 ] = -22;
	args [ 3 ] = 100;
	args [ 4 ] = 123;
	args [ 5 ] = 2000;
	REQUIRE ( call_function ( &function_twelve, 12, args ) == 2323 );
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