

#ifdef _WIN64
	#pragma comment(lib, "lib\\frmwrk32.lib")
	#pragma comment(lib, "lib64\\xlcall32.lib")
#else
	#pragma comment(lib, "lib\\xlcall32.lib")
#endif