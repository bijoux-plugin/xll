///***************************************************************************
// File:        stock.cpp
//
// Purpose:     Stock Web Service XLL
//
// Platform:    Microsoft Windows
//
// Functions:		
//              xlAutoOpen
//              xlAddInManagerInfo
//              FuncStockInternal
//              FuncStockOld
//              FuncStock
//              GetStockMetricInternal
//              GetStockMetricOld
//              GetStockMetric
//              GetStockScoreInternal
//              GetStockScoreOld
//              GetStockScore
//              SetWebPolling
//
///***************************************************************************

//Define _WIN32_WINDOWS if not already defined
//for web service proxy

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

//
// Includes
//

#include <winsock2.h>
#include "xlcall.h"
#include "framewrk.h"
#include "wchar.h"
#include "StockServiceProxy.h"
#include "Metrics.h"

//
// Table of all the functions to register
// 
// Special symbols used in the types:
//    % - follows a type to indicate the 2007 versions
//        should be used           
//    $ - at the end of the types, signifies the function
//        can be computed using MTR
//    ! - at the end of the types, signifies the function
//        is volatile
//

#define WKSHT_FUNC_ROWS 6
#define WKSHT_FUNC_COLS 10
#define WKSHT_FUNC12_START 0
#define WKSHT_FUNC12_END 3
#define WKSHT_FUNC11_START 3
#define WKSHT_FUNC11_END 6

static LPSTR g_rgWorksheetFuncs
[WKSHT_FUNC_ROWS][WKSHT_FUNC_COLS] =
{
	{ "FuncStock",
		"UC%C%J$!",
		"FuncStock",
		"Type text, ticker symbol, date",
		"1",
		"Stock Add-In",
		"",
		"",
		"Finds opening price of stock",
		"type, ticker symbol, date"
	},
	{ "GetStockMetric",
		"UC%C%JJ$!",
		"GetStockMetric",
		"Ticker symbol, metric text, start and stop dates",
		"1",
		"Stock Add-In",
		"",
		"",
		"Computes a stock metric",
		"Name of stock, name of metric, start and stop ranges"
	},
	{ "GetStockScore",
		"UK%K%K%$",
		"GetStockScore",
		"MACD Score, RSI Score, ADX Score",
		"1",
		"Stock Add-In",
		"",
		"",
		"Computes a buy/sell/hold rating",
		"Test variable"
	},
	{ "FuncStockOld",
		"RCCJ!",
		"FuncStock",
		"Type text, ticker symbol, date",
		"1",
		"Stock Add-In",
		"",
		"",
		"Finds opening price of stock",
		"type, ticker symbol, date"
	},
	{ "GetStockMetricOld",
		"RCCJJ!",
		"GetStockMetric",
		"Ticker symbol, metric text, start and stop dates",
		"1",
		"Stock Add-In",
		"",
		"",
		"Computes a stock metric",
		"Name of stock, name of metric, start and stop ranges"
	},
	{ "GetStockScoreOld",
		"RKKK",
		"GetStockScore",
		"MACD Score, RSI Score, ADX Score",
		"1",
		"Stock Add-In",
		"",
		"",
		"Computes a buy/sell/hold rating",
		"Test variable"
	},
};

//
// handles registering of functions
//

__declspec(dllexport) int WINAPI xlAutoOpen(void)
{

	static XLOPER xDLL;	   // name of this DLL //
	int vnum;
	int i;

	vnum = XLCallVer();

	vnum >>= 8;

	Excel(xlGetName, &xDLL, 0);

	for (i=(vnum == 12 ? WKSHT_FUNC12_START : WKSHT_FUNC11_START); 
		i<(vnum == 12 ? WKSHT_FUNC12_END : WKSHT_FUNC11_END); i++)
	{
		Excel(xlfRegister, 0, 1+ WKSHT_FUNC_COLS,
			  (LPXLOPER) &xDLL,
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][0]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][1]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][2]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][3]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][4]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][5]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][6]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][7]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][8]),
			  (LPXLOPER) TempStrConst(g_rgWorksheetFuncs[i][9]));
	}

	// Free the XLL filename //
	Excel(xlFree, 0, 1, (LPXLOPER) &xDLL);

	InitStockService();

	return 1;
}

//
// Provides information for the addin manager
//

__declspec(dllexport) LPXLOPER WINAPI xlAddInManagerInfo(LPXLOPER xAction)
{
	static XLOPER xInfo, xIntAction;

	Excel(xlCoerce, &xIntAction, 2, xAction, TempInt(xltypeInt));

	if (xIntAction.val.w == 1)
	{
		xInfo.xltype = xltypeStr;
		xInfo.val.str = "\020Stock Add-In XLL";
	}
	else
	{
		xInfo.xltype = xltypeErr;
		xInfo.val.err = xlerrValue;
	}

	return(LPXLOPER) &xInfo;
}

///***************************************************************************
// FuncStockInternal()
//
// Purpose:
//
//      This function is called by both FuncStock and FuncStockOld.  Given a
//      stock symbol, an info type, and a date, it will poll the web service
//      for the value and return an error value.
//
// Parameters:
//
//      LPWSTR stval    The type of the value desired.
//                      One of [OPEN, CLOSE, LOW, HIGH]
//
//      LPWSTR sttkr    The ticker symbol of the stock
//
//      int date        The date as an integer
//
//      double *dret    Value to store the web services response in.
//
// Returns: 
//
//      BOOL            TRUE if error, FALSE otherwise
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************
int FuncStockInternal(LPWSTR stval, LPWSTR sttkr, int date, double* dret)
{
	int wtype;
	BOOL fError;

	fError = FALSE;
	wtype = -1;

	if (!wcscmp(stval, L"OPEN"))
		fError = OpenStockValue(sttkr, date, dret);
	else if (!wcscmp(stval, L"CLOSE"))
		fError = CloseStockValue(sttkr, date, dret);
	else if (!wcscmp(stval, L"LOW"))
		fError = LowStockValue(sttkr, date, dret);
	else if (!wcscmp(stval, L"HIGH"))
		fError = HighStockValue(sttkr, date, dret);
	else
		fError = TRUE;

	if (!fError && *dret < 0)
		fError = TRUE;

	return fError;
}

///***************************************************************************
// FuncStockOld()
//
// Purpose:
//
//      This function is exported for use in versions of Excel other than 2007.
//      It converts the strings Excel gives it into Unicode strings, and then
//      calls FuncStockInternal.
//
// Parameters:
//
//      LPSTR stval     The type of the value desired.
//                      One of [OPEN, CLOSE, LOW, HIGH]
//
//      LPSTR sttkr     The ticker symbol of the stock
//
//      int date        The date as an integer
//
// Returns: 
//
//      LPXLOPER        XLOPER of xltypeNum containing the queried value, or
//                      xltypeErr of xlerrNull on failure
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

__declspec(dllexport) LPXLOPER WINAPI FuncStockOld(LPSTR stval, LPSTR ststk, int date)
{
	BOOL fError;
	double dret;
	WCHAR wstval[10];
	WCHAR wststk[10];
	LPXLOPER pxlret;

	dret = 0;

	pxlret = (LPXLOPER) malloc(sizeof(XLOPER));

	fError = !MultiByteToWideChar(CP_ACP, 0, stval, strlen(stval) + 1, wstval, 10);
	fError|= !MultiByteToWideChar(CP_ACP, 0, ststk, strlen(ststk) + 1, wststk, 10);

	if (!fError)
		fError = FuncStockInternal(wstval, wststk, date, &dret);

	if (fError)
	{
		pxlret->xltype = xltypeErr;
		pxlret->val.err = xlerrNull;
	}
	else
	{
		pxlret->xltype = xltypeNum;
		pxlret->val.num = dret;
	}

	pxlret->xltype |= xlbitXLFree;

	return pxlret;
}

///***************************************************************************
// FuncStock()
//
// Purpose:
//
//      This function is exported for use in Excel 2007. It calls
//      FuncStockInternal to get the requested value.
//
// Parameters:
//
//      LPWSTR stval    The type of the value desired.
//                      One of [OPEN, CLOSE, LOW, HIGH]
//
//      LPWSTR sttkr    The ticker symbol of the stock
//
//      int date        The date as an integer
//
// Returns: 
//
//      LPXLOPER12      XLOPER12 of xltypeNum containing the queried value, or
//                      xltypeErr of xlerrNull on failure
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

__declspec(dllexport) LPXLOPER12 WINAPI FuncStock(LPWSTR stval, LPWSTR sttkr, int date)
{
	BOOL fError;
	double dret;
	LPXLOPER12 pxlret;

	pxlret = (LPXLOPER12) malloc(sizeof(XLOPER12));

	fError = FuncStockInternal(stval, sttkr, date, &dret);

	if (fError)
	{
		pxlret->xltype = xltypeErr;
		pxlret->val.err = xlerrNull;
	}
	else
	{
		pxlret->xltype = xltypeNum;
		pxlret->val.num = dret;
	}

	pxlret->xltype |= xlbitXLFree;

	return pxlret;
}

///***************************************************************************
// GetStockMetricInternal()
//
// Purpose:
//
//      Function called by both GetStockMetric and GetStockMetricOld.  Will
//      query the web service for a stocks metric, given a start and an end
//      date.  Can fail when the web service is not responding, on invalid
//      input, and on a per-metric basis if too small a range is given.
//
// Parameters:
//
//      LPWSTR ststk    The ticker symbol of the stock
//
//      LPWSTR stmet    A string identifying the metric.
//                      One of [RSI, MACD, ADX].
//
//      DWORD dwstart   The starting date
//
//      DWORD dwend     The ending date
//
//      double* dret    Value to store the computed metric in
//
// Returns: 
//
//      BOOL            TRUE if error, FALSE otherwise
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

BOOL GetStockMetricInternal(LPWSTR ststk, LPWSTR stmet, DWORD dwstart, DWORD dwend, double* dret)
{
	int cprc;
	double *pdprc;
	double *pdprl;
	double *pdprh;

	BOOL fError;

	fError = FALSE;

	if (dwstart > dwend)
		fError = TRUE;

	if (!fError)
	{
		cprc = ((long int)dwend - (long int)dwstart);

		if (!wcscmp(stmet, L"RSI"))
		{
			pdprc = new double[cprc];
			fError = RangeStockValue(ststk, L"CLOSE", dwstart, cprc, &pdprc);
			if (!fError)
				fError = MetrRSI(pdprc, cprc, 7, dret);
			delete [] pdprc;
		}
		else if (!wcscmp(stmet, L"MACD"))
		{
			pdprc = new double[cprc];
			fError = RangeStockValue(ststk, L"CLOSE", dwstart, cprc, &pdprc);
			if (!fError)
				fError = MetrMACD(pdprc, cprc, 7, 16, dret);
			delete [] pdprc;
		}
		else if (!wcscmp(stmet, L"ADX"))
		{
			pdprc = new double[cprc];
			pdprl = new double[cprc];
			pdprh = new double[cprc];
			fError = RangeStockValue(ststk, L"CLOSE", dwstart, cprc, &pdprc);
			fError|= RangeStockValue(ststk, L"LOW", dwstart, cprc, &pdprl);
			fError|= RangeStockValue(ststk, L"HIGH", dwstart, cprc, &pdprh);
			if (!fError)
				fError = MetrADX(pdprh, pdprl, pdprc, cprc, 6, dret);
			delete [] pdprc;
			delete [] pdprl;
			delete [] pdprh;
		}
	}
	else
		fError = TRUE;

	return fError;
}

///***************************************************************************
// GetStockMetricOld()
//
// Purpose:
//
//      This function is exported for use in versions of Excel other than 2007.
//      It converts the strings Excel gives it into Unicode strings, and then
//      calls GetStockMetricInternal.
//
// Parameters:
//
//      LPSTR ststk     The ticker symbol of the stock
//
//      LPSTR stmet     A string identifying the metric.
//                      One of [RSI, MACD, ADX].
//
//      DWORD dwstart   The starting date
//
//      DWORD dwend     The ending date
//
// Returns: 
//
//      LPXLOPER        XLOPER of xltypeNum containing the queried value, or
//                      xltypeErr of xlerrNull on failure
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

__declspec(dllexport) LPXLOPER GetStockMetricOld(LPSTR ststk, LPSTR stmet, DWORD dwstart, DWORD dwend)
{
	LPXLOPER pxlret;
	WCHAR wststk[10];
	WCHAR wstmet[10];
	double dret;
	BOOL fError;

	dret = 0;

	pxlret = (LPXLOPER) malloc(sizeof(XLOPER));

	fError = !MultiByteToWideChar(CP_ACP, 0, ststk, strlen(ststk) + 1, wststk, 10);
	fError|= !MultiByteToWideChar(CP_ACP, 0, stmet, strlen(stmet) + 1, wstmet, 10);

	if (!fError)
		fError = GetStockMetricInternal(wststk, wstmet, dwstart, dwend, &dret);

	if (fError)
	{
		pxlret->xltype = xltypeErr;
		pxlret->val.err = xlerrNum;
	}
	else
	{
		pxlret->xltype = xltypeNum;
		pxlret->val.num = dret;
	}

	pxlret->xltype |= xlbitXLFree;

	return pxlret;
}

///***************************************************************************
// GetStockMetric()
//
// Purpose:
//
//      This function is exported for use in Excel 2007. It calls
//      GetStockMetricInternal to get the requested value.
//
// Parameters:
//
//      LPWSTR ststk    The ticker symbol of the stock
//
//      LPWSTR stmet    A string identifying the metric.
//                      One of [RSI, MACD, ADX].
//
//      DWORD dwstart   The starting date
//
//      DWORD dwend     The ending date
//
// Returns: 
//
//      LPXLOPER12      XLOPER12 of xltypeNum containing the queried value, or
//                      xltypeErr of xlerrNull on failure
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

__declspec(dllexport) LPXLOPER12 GetStockMetric(LPWSTR ststk, LPWSTR stmet, DWORD dwstart, DWORD dwend)
{
	LPXLOPER12 pxlret;
	double dret;
	BOOL fError;

	pxlret = (LPXLOPER12) malloc(sizeof(XLOPER12));

	dret = 0;

	fError = GetStockMetricInternal(ststk, stmet, dwstart, dwend, &dret);

	if (fError)
	{
		pxlret->xltype = xltypeErr;
		pxlret->val.err = xlerrNum;
	}
	else
	{
		pxlret->xltype = xltypeNum;
		pxlret->val.num = dret;
	}

	pxlret->xltype |= xlbitXLFree;

	return pxlret;
}


// A somewhat arbitrary stock score calculation...
//
// The steps:
//      1: Look for MACD crossovers.  Going from negative to positive can signal a buy,
//         and a change from positive to negative can signal a sell.		   
//
//      2: Look for RSI signals.  If RSI drops below ~25 and then rises steadily, it
//         can signal a buy.  If RSI goes above ~75, and then drops steadily, it can
//         signal a sell.
//
// Some notes:
//      +  There is a somewhat arbitrary decision to take into acount ADX as a modifier
//         in almost all cases - it measures trend strength, so it is of use.  Generally,
//         it is used to verify a buy/sell signal, as well as used when the signal is far
//         in the past.
// 
//      +  When a buy/sell signal is not found, or it is far in the past, there is an
//         arbitrary decision for scoring the stock that is different in each case for
//         each metric.  They seem to give decent results without actually knowing stock
//         price data.

short ComputeStockScore(double* pdmacd, double* pdrsi, double* pdadx, INT32 cv)
{
	double* pdtmp; //pointer used through to index into paramaters
	double dmacd; //value between -1 and 1 that rates MACD
	double drsi; //value between -1 and 1 that rates RSI
	int wmacdx;	//most recent MACD crossover
	int wrsix; //most recent RSI crossover
	int wrsixp;	//previous RSI crossover
	int i;
	short wret;

	wmacdx = 0;
	wrsix = 0;
	wrsixp = 0;
	wret = 0;

	//Start with looking for MACD crossovers
	//
	//A MACD crossover occurs when the MACD goes from negative to positive
	//or positive to negative.  A move above 0 tends to indicate a coming
	//increase in stock price - a buy signal.  It is the opposite for a move
	//below 0.

	pdtmp = pdmacd;
	dmacd = *pdtmp;
	pdtmp++;

	for (i = 1; i < cv; i++)
	{
		if (dmacd < 0 && *pdtmp > 0)
		{
			dmacd = *pdtmp;
			wmacdx = i;
		}
		else if (dmacd > 0 && *pdtmp < 0)
		{
			dmacd = *pdtmp;
			wmacdx = -i;
		}
		pdtmp++;
	}

	//Rate the MACD performance
	//
	//	-If there was a recent by signal and the trend was less
	//	 than 0.4, a lower trend is better (the stock is much more
	//	 likely to reverse direction and confirm the buy/sell signal)
	//
	//	-If, however, the trend was above 0.4, we ignore the buy/sell
	//	 signal. For instance, a sell signal can trigger when the stock
	//   has been steadily increasing for a long time. Not until that
	//   uptrend fails should a sell be considered
	//
	//  -If the signal was older than 7 days, it is ignored, and the
	//   current rating is the trend
	//
	//	-If no signal is given, use the trend strength

	pdtmp = pdadx;

	if (wmacdx != 0)
	{
		pdtmp += abs(wmacdx);
		if (wmacdx < 0)
			dmacd = 1;
		else
			dmacd = -1;

		if (cv - abs(wmacdx) > 7)
		{
			dmacd *= -pdadx[cv - 1];
			wmacdx = cv - 6;
		}
		else if (*pdtmp < 0.4)
		{
			dmacd *= (*pdtmp - 0.3) / 0.3;
		}
	}
	else if (dmacd < 0)
	{
		dmacd = (1.0 - pdtmp[cv - 1]) / 2.0;
		wmacdx = cv - 6;
	}
	else
	{
		dmacd = (pdtmp[cv - 1] - 1.0) / 2.0;
		wmacdx = cv - 6;
	}

	//Look for RSI signals
	//
	//RSI signals occur when the RSI drops from above 70 or rises from
	//below 30. Dropping from above 70 is considered a sell signal.
	//Conversely, rising about 30 is considered a buy signal.  The
	//method also looks for a quick drop back below 30 or above 70.
	//If this happens within a close time frame to the signal, the
	//signal is ignored.

	pdtmp = pdrsi;
	drsi = *pdtmp;
	pdtmp++;

	for (i = 1; i < cv; i++)
	{
		if (drsi < 30.0 && *pdtmp > 30.0)
		{
			wrsixp = wrsix;
			wrsix = i;
		}
		else if (drsi > 30.0 && *pdtmp < 30.0 && i - abs(wrsix) < 4) //catch whipsaws
		{
			wrsix = wrsixp;
		}
		else if (drsi > 70.0 && *pdtmp < 70.0)
		{
			wrsixp = wrsix;
			wrsix = -i;
		}
		else if (drsi < 70.0 && *pdtmp > 70.0 && i - abs(wrsix) < 4)
		{
			wrsix = wrsixp;
		}
		drsi = *pdtmp;
		pdtmp++;
	}

	//Rate the RSI performance
	//
	//	-If the stock is not trending, the lower a trend, the more
	//	 the original signal is used
	//
	//	-If the stock is highly trending at the signal, high RSI's
	//	 are used as buy, and low RSI's as a sell
	//
	//	-If the signal is more than 7 days old and was a sell, check
	//	 the current RSI - a low value means one should consider getting
	//	 back into the market
	//
	//	-Similarly, an old buy signal coupled with a current RSI value
	//	 above 70 is a warning that it might be time to leave the market
	//
	//	-Otherwise, an old signal is treated the same as no signal - buy
	//	 when low, sell when high

	pdtmp = pdadx;

	if (wrsix != 0)
	{
		pdtmp += abs(wrsix);
		if (wrsix < 0)
			drsi = 1;
		else
			drsi = -1;
		if (cv - abs(wrsix) > 7)
		{
			pdtmp = pdadx + cv - 1;
			if (wrsix < 0 && pdrsi[cv-1] < 30.0)
			{
				drsi = 0.5 - *pdtmp;
			}
			else if (wrsix > 0 && pdrsi[cv-1] > 70.0)
			{
				drsi = *pdtmp - 0.5;
			}
			else
			{
				drsi *= (50.0 - pdrsi[cv-1]) / 100.0;
			}
			wrsix = cv - 6;
		}
		else if (*pdtmp < 0.4)
		{
			drsi *= (*pdtmp - 0.4) / 0.4;
		}
		else
		{
			drsi = (50.0 - pdrsi[abs(wrsix)]) / 100.0;
		}
	}
	else
	{
		drsi = (drsi - 50.0) / 50.0 * pdadx[cv-1];
		wrsix = cv - 6;
	}

	//Final computation just stratifies the ratings on a scale
	//of 1-10
	wret = (short)((dmacd * 5.0 + 5.0) / 2.0 + 0.5);
	wret = wret + (short)((drsi * 5.0 + 5.0) / 2.0 + 0.5);
	if (cv - abs(wmacdx) > 5 || cv - abs(wrsix) > 5)
		wret--;
	if (wret < 1)
		wret = 1;

	return wret;
}

///***************************************************************************
// GetStockScoreOld()
//
// Purpose:
//
//      This function is exported for use in versions of Excel other than 2007.
//      It uses the FP8 structure to receive several arrays of doubles, verifies
//      their lengths, and then calls GetStockScoreInternal
//
// Parameters:
//
//      FP* pxmacd      FP structure with all the MACD scores
//
//      FP* pxrsi       FP structure with all the RSI scores
//
//      FP* pxadx       FP structure with all the ADX scores
//
// Returns: 
//
//      LPXLOPER        XLOPER of xltypeInt containing the score given to
//                      the stock (between 1 and 10), or an xltypeErr on
//                      failure.
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

__declspec(dllexport) LPXLOPER GetStockScoreOld(FP* pxmacd, FP* pxrsi, FP* pxadx)
{
	LPXLOPER pxlret;
	BOOL fError;
	short wret;
	unsigned short int cv;

	pxlret = (LPXLOPER) malloc(sizeof(XLOPER));
	wret = cv = 0;
	fError = FALSE;

	cv = pxmacd->rows * pxmacd->columns;
	fError = (cv != (pxrsi->rows * pxrsi->columns));
	fError|= (cv != (pxadx->rows * pxadx->columns));

	if (!fError)
	{
		wret = ComputeStockScore(pxmacd->array, pxrsi->array, pxadx->array, cv);
	}

	if (fError)
	{
		pxlret->xltype = xltypeErr;
		pxlret->val.err = xlerrNum;
	}
	else
	{
		pxlret->xltype = xltypeInt;
		pxlret->val.w = wret;
	}

	pxlret->xltype |= xlbitXLFree;

	return pxlret;
}

///***************************************************************************
// GetStockScore()
//
// Purpose:
//
//      This function is exported for use in Excel 2007. It uses the FP12
//      structure to receive several arrays of doubles, verifies
//      their lengths, and then calls GetStockScoreInternal
//
// Parameters:
//
//      FP12* pxmacd    FP12 structure with all the MACD scores
//
//      FP12* pxrsi     FP12 structure with all the RSI scores
// 
//      FP12* pxadx     FP12 structure with all the ADX scores
//
// Returns: 
//
//      LPXLOPER12      XLOPER12 of xltypeInt containing the score given to
//                      the stock (between 1 and 10), or an xltypeErr on
//                      failure.
//
// Comments:
//
// History:  Date       Author        Reason
///***************************************************************************

__declspec(dllexport) LPXLOPER12 GetStockScore(FP12* pxmacd, FP12* pxrsi, FP12* pxadx)
{
	LPXLOPER12 pxlret;
	BOOL fError;
	int wret;
	INT32 cv;

	pxlret = (LPXLOPER12) malloc(sizeof(XLOPER12));
	cv = wret = 0;
	fError = FALSE;


	cv = pxmacd->rows * pxmacd->columns;
	fError = (cv != (pxrsi->rows * pxrsi->columns));
	fError|= (cv != (pxadx->rows * pxadx->columns));

	if (!fError)
	{
		wret = ComputeStockScore(pxmacd->array, pxrsi->array, pxadx->array, cv);
	}

	if (fError)
	{
		pxlret->xltype = xltypeErr;
		pxlret->val.err = xlerrNum;
	}
	else
	{
		pxlret->xltype = xltypeInt;
		pxlret->val.w = wret;
	}

	pxlret->xltype |= xlbitXLFree;

	return pxlret;
}

// A simple method that enables or disables web polling; any non-zero
// parameter enables online polling

__declspec(dllexport) void pascal SetWebPolling(int flag)
{
	ProxyOnlineMode(flag);
}

__declspec(dllexport) int pascal GetWebPolling()
{
	return GetProxyMode();
}
