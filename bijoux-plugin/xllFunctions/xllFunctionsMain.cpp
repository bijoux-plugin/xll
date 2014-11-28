// xllFunctions.dll is the library that contains the latest version of our "Addin"

// xllAddin.xll is the Excel Addin that would download the latest version of xllFunctions.dll from
// the server each time when starting up or whenever necessary


#include <Windows.h>
#include <vector>
#include <string>
#include "string.h"
#include "xlCall.h"

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

enum optionType {
	call,
	put
};

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
		/*float valPut = priceAmericanOption(2.0, 20, 30, 0.05, 0.20, 0, 500, op);
	assert( (valPut - 0.4659212) < 0.0000001 );
	std::cout << "\nAmerican Option Price (Put Option): " << valPut << std::endl;

	float valCall = priceAmericanOption(2.0, 20, 10, 0.05, 0.20, 0, 500, op);
	assert( (valCall - 10.954246) < 0.0000001 );
	std::cout << "\nAmerican Option Price (Call Option): " << valCall << std::endl;*/
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = 100;

	float timeToExpirationT = _timeToExpirationT->val.num;
	float stockPriceS = _stockPriceS->val.num;
	float strikePriceK = _strikePriceK->val.w;
	float r = _r->val.w; // 
	float sigma = _sigma->val.w; // volatility
	float q = _q->val.w; // Dividend Yield
	int heightOfBinomialTreeN = _heightOfBinomialTreeN->val.num;
	enum optionType opType;

	if (_opType->val.w == 1) {
		opType = optionType::call;
	} else {
		opType = optionType::put;
	}


	////
	float deltaT = timeToExpirationT / heightOfBinomialTreeN;
	float up = exp(sigma * sqrt(deltaT));

	float p0 = (up * exp(-r * deltaT) - exp(-q * deltaT)) * up / (up*up - 1);
	float p1 = exp(-r * deltaT) - p0;

	float *p = new float[heightOfBinomialTreeN+1];
	for (int i=0; i <= heightOfBinomialTreeN; i++)
	{
		if (opType == put)
			p[i] = strikePriceK - stockPriceS * pow(up, (2*i - heightOfBinomialTreeN));
		else if (opType == call)
			p[i] = stockPriceS * pow(up, (2*i - heightOfBinomialTreeN)) - strikePriceK;
		if (p[i] < 0)
			p[i] = 0;
	}

	float exercise = 0.0;
	for (int j= heightOfBinomialTreeN-1; j >= 0; j--)
	{
		for (int i1=0; i1 <= j; i1++)
		{
			p[i1] = p0 * p[i1] + p1 * p[i1 + 1]; // binomial value
			if (opType == put)
				exercise = strikePriceK - stockPriceS * pow(up,(2*i1 - j)); // exercise value
			else if (opType == call)
				exercise = stockPriceS * pow(up,(2*i1 - j)) - strikePriceK ; // exercise value
			if (p[i1] < exercise)
				p[i1] = exercise;
		}
	}

	//return p[0];
	////
	return xRes;
}

__declspec(dllexport) LPXLOPER WINAPIV PriceEuropean(LPXLOPER parm1, LPXLOPER parm2, LPXLOPER parm3, LPXLOPER parm4)
{
	LPXLOPER xRes = new XLOPER;
	xRes->xltype = xltypeInt;
	xRes->val.w = parm1->val.num + parm2->val.num + parm3->val.num + parm4->val.num;
	return xRes;
}