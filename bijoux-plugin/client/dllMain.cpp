
#include <Windows.h>
#include <vector>
#include <string>
#include <xlCall.h>

int __declspec(dllexport) GetNumberOfFunctions(void)
{
	return 2;
}

void __declspec(dllexport) GetFunctionInfo(int FunctionIndex, int ParameterIndex, char value[], int len)
{
	// Excel Name, Dll Name
	static char funcNames [][6][100] = {
	//   Excel Function,   DLL Function,    Category,        Description,              Argument Descriptions, # of parameters
		{"Price.American", "PriceAmerican", "OptionPricing", "Price American Options", "T, S, K, r, sigma, q, heightN, optionType, ",      "8"},
		{"Price.European", "PriceEuropean", "OptionPricing", "Price European Options", "Strike Price, ",      "4"} };

	strcpy(value, funcNames[FunctionIndex][ParameterIndex]);
}
/*
__declspec(dllexport) LPXLOPER WINAPIV PriceAmerican(
	LPXLOPER _timeToExpirationT,
	LPXLOPER _stockPriceS,
	LPXLOPER _strikePriceK,
	LPXLOPER _r, // 
	LPXLOPER _sigma, // volatility
	LPXLOPER _q, // Dividend Yield
	LPXLOPER _heightOfBinomialTreeN,
	LPXLOPER _opType)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xl
	return xRes;
}

*/