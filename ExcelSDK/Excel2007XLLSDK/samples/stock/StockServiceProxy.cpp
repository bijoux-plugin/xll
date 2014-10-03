///***************************************************************************
// File:            StockServiceProxy.cpp
//
// Description:	    Provides access to C++ generated web service
//
// File contains wrapper methods for instantiating and accessing the
// web service, as well as error detection and reporting.  Also allows
// for the web service to be shut off, and data polled from the in-file
// data.  This data is easy to modify - see the comments surrounding it.
//
// If you wish to host the included web service, you must add the web reference
// in Visual Studio to it and name it "StockService"
//
///***************************************************************************

#include "stdafx.h"
#include "StockServiceProxy.h"

static BOOL g_fOnline = TRUE;

//number of locally-defined stocks
#define TICKER_COLS 2

//the names of the stocks included in the local data set
static LPWSTR g_rgTickerSymbol [TICKER_COLS] =
{
	L"STOCK1",
	L"STOCK2",
};

//number of days of locally-defined data
#define DATES_COLS 65

//stock index x for day index y is located at [x][2*y] for open
//and [x][2*y+1] for close
static double g_rgStockPrices [TICKER_COLS][DATES_COLS*2] =
{
	{
		40.84, 36.94, 38.01, 39.40, 40.60, 44.09, 44.09, 42.70, 37.82, 39.64, 35.39, 33.05, 35.00, 31.91,
		28.80, 33.23, 37.01, 38.69, 42.10, 44.80, 43.33, 39.87, 41.80, 39.80, 40.97, 38.69, 36.90, 32.92,
		33.20, 30.68, 24.87, 23.76, 19.80, 14.68, 17.00, 14.81, 16.80, 16.98, 15.00, 10.55, 12.10, 10.27,
		11.10, 13.49, 17.60, 20.59, 17.90, 17.71, 17.20, 21.79, 18.70, 21.94, 21.70, 21.91, 20.50, 24.14,
		23.00, 22.07, 16.93, 15.20, 15.35, 17.89, 21.49, 19.48, 16.80, 16.40, 20.39, 15.34, 13.30, 12.65,
		10.40, 12.86, 14.10, 16.54, 12.90, 18.13, 14.46, 14.56, 12.30, 17.23, 13.60, 17.43, 17.40, 19.45,
		17.70, 21.01, 25.50, 29.56, 33.10, 33.94, 38.40, 36.25, 36.49, 39.32, 43.70, 42.18, 36.80, 34.57,
		33.20, 32.20, 35.61, 38.02, 39.60, 36.33, 30.80, 27.41, 27.30, 30.77, 33.90, 32.76, 34.40, 33.98,
		38.30, 40.60, 44.74, 41.91, 37.70, 41.56, 41.20, 45.62, 41.90, 45.88, 44.82, 45.67, 48.10, 46.81,
	},
	{
		38.13, 36.30, 34.02, 30.77, 34.60, 32.53, 30.56, 32.58, 34.26, 31.60, 29.41, 24.71, 28.00, 28.26,
		26.21, 24.78, 24.51, 27.01, 25.40, 23.12, 23.80, 20.63, 21.91, 18.76, 15.44, 15.72, 12.00, 13.89,
		11.24, 11.48, 13.39, 10.23, 12.77, 14.33, 17.88, 19.44, 22.00, 23.91, 24.55, 27.27, 27.05, 24.36,
		24.04, 27.81, 27.09, 33.01, 33.92, 31.79, 28.38, 31.50, 35.84, 34.45, 37.62, 41.82, 46.83, 46.65,
		47.43, 50.42, 49.80, 54.00, 56.11, 54.70, 55.38, 59.38, 58.94, 64.10, 63.17, 59.83, 54.46, 52.96,
		50.49, 54.06, 56.24, 59.33, 58.20, 63.54, 68.38, 64.90, 67.52, 62.71, 60.87, 56.54, 57.14, 57.80,
		54.67, 53.44, 58.24, 56.03, 53.79, 57.36, 56.67, 61.20, 65.07, 63.51, 58.42, 58.62, 54.02, 54.51,
		53.35, 52.35, 54.29, 49.26, 48.63, 50.51, 55.62, 54.03, 50.89, 50.03, 46.49, 50.30, 52.53, 54.52,
		57.97, 55.41, 57.72, 57.16, 60.14, 55.32, 57.13, 58.26, 57.77, 58.83, 54.19, 50.04, 45.23, 44.66,
	},
};

//stock index x for day index y is located at [x][2*y] for high
//and [x][2*y+1] for low
static double g_rgStockLimits [TICKER_COLS][DATES_COLS*2] =
{
	{
		41.41, 33.31, 40.53, 37.98, 53.45, 38.93, 46.08, 42.31, 39.89, 37.19, 38.01, 23.26, 44.80, 31.86,
		41.27, 28.56, 40.83, 37.01, 52.68, 41.77, 43.33, 37.14, 45.81, 39.76, 50.04, 38.64, 39.02, 30.75,
		33.48, 30.44, 31.04, 18.06, 21.22, 12.95, 18.13, 05.18, 23.63, 15.93, 23.77, 00.56, 12.38, 10.25,
		22.77, 09.51, 24.26, 13.47, 23.02, 16.23, 21.98, 15.31, 23.70, 10.59, 30.17, 17.64, 26.10, 18.21,
		23.15, 21.41, 21.93, 14.38, 19.14, 13.40, 22.18, 19.46, 16.82, 12.97, 20.52, 09.50, 14.75, 04.50,
		12.87, 07.75, 26.42, 11.74, 23.02, 09.11, 16.64, 09.06, 23.38, 09.27, 22.04, 13.38, 20.24, 17.34,
		25.86, 10.77, 30.91, 23.14, 34.50, 32.29, 40.31, 34.13, 39.69, 32.81, 45.98, 39.67, 42.05, 34.53,
		35.17, 24.11, 38.32, 32.75, 40.46, 32.72, 34.77, 27.26, 31.49, 24.25, 33.99, 24.96, 40.03, 30.89,
		48.38, 29.55, 44.74, 38.98, 42.61, 27.85, 52.43, 31.93, 53.78, 41.45, 45.98, 44.58, 53.14, 46.23,
	},
	{
		39.66, 34.95, 34.75, 29.73, 42.08, 31.53, 32.58, 30.10, 34.32, 31.41, 32.48, 23.22, 28.83, 27.14,
		27.37, 24.34, 31.72, 24.21, 25.43, 17.52, 32.74, 19.86, 23.24, 11.45, 24.29, 14.57, 18.94, 08.01,
		18.93, 11.10, 13.39, 09.59, 18.56, 12.59, 28.93, 17.86, 30.91, 21.06, 27.72, 22.60, 35.62, 24.29,
		28.61, 15.54, 34.02, 22.07, 38.08, 31.69, 36.98, 26.05, 41.09, 33.83, 44.48, 37.25, 46.84, 41.19,
		59.85, 45.60, 61.52, 46.37, 56.20, 54.70, 66.93, 48.40, 64.71, 58.50, 71.31, 55.63, 62.68, 50.16,
		57.99, 50.42, 59.48, 54.14, 63.60, 57.63, 69.63, 54.93, 76.40, 61.37, 63.87, 53.97, 61.16, 54.95,
		56.08, 53.39, 63.19, 50.66, 57.98, 51.33, 71.16, 46.87, 68.20, 62.48, 65.45, 54.29, 55.85, 44.24,
		53.61, 51.01, 54.50, 40.39, 51.11, 47.60, 60.38, 51.47, 53.03, 49.00, 54.05, 40.34, 58.22, 43.73,
		58.07, 54.83, 57.73, 48.41, 60.35, 55.31, 64.27, 54.29, 63.34, 55.23, 59.16, 41.77, 47.91, 37.97,
	},
};

//
// Sets if the proxy should use local data or if it should
// poll a webservice for data
//
void ProxyOnlineMode(int flag)
{
	g_fOnline = (flag==0 ? FALSE : TRUE);
}

//
// Returns FALSE if local data is being used, TRUE if web service
// is being polled
//
int GetProxyMode() 
{
	return g_fOnline;
}

//
// Scans through the g_rgTickerSymbol array until one
// of the symbols matches the input and returns that index;
// -1 indicates it was not found
//
int GetTickerIndexFromLPWSTR(LPWSTR sttkr)
{
	int i;

	for (i = 0; i < TICKER_COLS; i++)
	{
		if (!wcscmp(g_rgTickerSymbol[i], sttkr))
			return i;
	}

	return -1;
}

///***************************************************************************
// GetStockValue()
//
// Purpose:
//
//      Used for local data retrieval when g_fOnline is FALSE.  Indexes into
//      the array g_rgStockPrices, which contains both open and close prices
//      for all stocks. Checks for invalid arguments and returns -1 if one is
//      provided.
//
// Parameters:
//
//      int tkr         Index into first dimensions of g_rgStockPrices that
//                      indicates which stock is being requested; number should
//                      be obtained from GetTickerIndexFromLPWSTR()
//
//      int date        A non-negative number indicating the date
//
//      BOOL fOpen      TRUE if the opening price is requested, FALSE for the 
//                      closing price
//
// Returns: 
//
//      double          The requested value, or a negative number if an index
//                      was out of bounds
//
// Comments:
//
///***************************************************************************

double GetStockValue(int tkr, int date, BOOL fOpen)
{
	if (tkr >= 0 && date >= 0 && tkr < TICKER_COLS && date < DATES_COLS)
		return g_rgStockPrices[tkr][date*2 + (fOpen ? 0 : 1)];
	else
		return -1;
}

//
// Convenience function for offline data retrieval 
//
double GetClosingValue(LPWSTR sttkr, int date)
{
	return GetStockValue(GetTickerIndexFromLPWSTR(sttkr), date, FALSE);
}

//
// Convenience function for offline data retrieval
//
double GetOpeningValue(LPWSTR sttkr, int date)
{
	return GetStockValue(GetTickerIndexFromLPWSTR(sttkr), date, TRUE);
}


///***************************************************************************
// GetLimitsValue()
//
// Purpose:
//
//      Used for local data retrieval when g_fOnline is FALSE.  Indexes into
//      the array g_rgStockLimits, which contains both low and high prices
//      for all stocks. Checks for invalid arguments and returns -1 if one is
//      provided.
//
// Parameters:
//
//      int tkr         Index into first dimensions of g_rgStockLimits that
//                      indicates which stock is being requested; number should
//                      be obtained from GetTickerIndexFromLPWSTR()
//
//      int date        A non-negative number indicating the date
//
//      BOOL fLow       TRUE if the low price is requested, FALSE for the high
//                      price
//
// Returns: 
//
//      double          The requested value, or a negative number if an index
//                      was out of bounds
//
// Comments:
//
///***************************************************************************

double GetLimitsValue(int tkr, int date, BOOL fLow)
{
	if (tkr >= 0 && date >= 0 && tkr < TICKER_COLS && date < DATES_COLS)
		return g_rgStockLimits[tkr][date*2 + (fLow ? 0 : 1)];
	else
		return -1;
}

//
// Convenience function for offline data retrieval
//
double GetLowValue(LPWSTR sttkr, int date)
{
	return GetLimitsValue(GetTickerIndexFromLPWSTR(sttkr), date, TRUE);
}

//
// Convenience function for offline data retrieval
//
double GetHighValue(LPWSTR sttkr, int date)
{
	return GetLimitsValue(GetTickerIndexFromLPWSTR(sttkr), date, FALSE);
}

//
// Essentially a memcpy that skips every other number in an array because
// low/high and open/close are stored in the same array in alternating fashion 
//
BOOL CopyRangeValue(int sttkr, double* pddata, int date, int cdate, double* pdret)
{
	int i;
	BOOL fError;

	if (sttkr < 0 || sttkr > TICKER_COLS || date < 0 || cdate < 0 || date + cdate > DATES_COLS)
		fError = TRUE;
	else
	{
		pddata+=date * 2;
		fError = FALSE;
		for (i = 0; i < cdate; i++)
		{
			*pdret = *pddata;
			pdret++;
			pddata+=2;
		}
	}

	return fError;
}

///***************************************************************************
// GetRangeValue()
//
// Purpose:
//
//      Used for local data retrieval when g_fOnline is FALSE.  Indexes into
//      the proper array to copy the data. Uses CopyRangeValue to account for
//      the alternating prices in the local array.
//
// Parameters:
//
//      LPWSTR sttkr    A string representing the desired ticker symbol
//
//      LPWSTR sttyp    A string that indicates what type of data to retrieve
//                      Either LOW, HIGH, CLOSE, or OPEN
//
//      int date        A non-negative number indicating the first date
//
//      int cdate       A non-negative number indicating how many days of
//                      data should be fetched
//
//      double *pdret   Array in which the values are stored
//
// Returns: 
//
//      BOOL            FALSE if there are no errors; TRUE otherwise
//
// Comments:
//
///***************************************************************************
BOOL GetRangeValue(LPWSTR sttkr, LPWSTR sttyp, int date, int cdate, double* pdret)
{
	BOOL fError;
	int tkr;

	fError = FALSE;

	tkr = GetTickerIndexFromLPWSTR(sttkr);

	if (tkr < 0 || tkr > TICKER_COLS)
		fError = TRUE;
	else if (!wcscmp(sttyp, L"OPEN"))
		fError = CopyRangeValue(tkr, &(g_rgStockPrices[tkr][0]), date, cdate, pdret);
	else if (!wcscmp(sttyp, L"CLOSE"))
		fError = CopyRangeValue(tkr, &(g_rgStockPrices[tkr][1]), date, cdate, pdret);
	else if (!wcscmp(sttyp, L"HIGH"))
		fError = CopyRangeValue(tkr, &(g_rgStockLimits[tkr][0]), date, cdate, pdret);
	else if (!wcscmp(sttyp, L"LOW"))
		fError = CopyRangeValue(tkr, &(g_rgStockLimits[tkr][1]), date, cdate, pdret);
	else
		fError = TRUE;

	return fError;
}

//
// Makes sure CoInitialize has been called at least once to enable
// the web service
//
void InitStockService()
{
	CoInitialize(NULL);
}

//
// Called when the web service is done being used by the client
//
void UninitStockService()
{
	CoUninitialize();
}

///***************************************************************************
// RangeStockValue()
//
// Purpose:
//
//      Function will get a range of data for a stock.  It can get either
//      HIGH, LOW, OPEN, or CLOSE.  The user passes in a pointer to an
//      array where the range should be stored; there should be at least
//      cdate cells in the array or behavior is undefined. If g_fOnline 
//      is set, it will attempt to poll the stock web service instead of
//      from the local arrays.
//
// Parameters:
//
//      LPWSTR sttkr    A string representing the desired ticker symbol
//
//      LPWSTR sttyp    A string that indicates what type of data to retrieve
//                      Either LOW, HIGH, CLOSE, or OPEN
//
//      int date        A non-negative number indicating the first date
//
//      int cdate       A non-negative number indicating how many days of
//                      data should be fetched
//
//      double **pdret  Pointer to array in which the values are stored
//
// Returns: 
//
//      BOOL            FALSE if there are no errors; TRUE otherwise
//
// Comments:
//
///***************************************************************************

BOOL RangeStockValue(LPWSTR sttkr, LPWSTR sttyp, int date, int cdate, double** pdret)
{
	int cr;
	BOOL fError;

	fError = FALSE;

	if (g_fOnline)
	{
		if (SUCCEEDED(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)))
		{
			Service::CService serv;
			serv.AddRef();
			if (SUCCEEDED(serv.GetRange(sttkr, sttyp, date, cdate, pdret, &cr)))
			{
				if (cr != cdate) //if we received a different number of values than we asked for
				{
					fError = TRUE;
				}
			}
			else
			{
				fError = TRUE;
			}
			serv.Release();
			CoUninitialize();
		}
	}
	else
	{
		fError = GetRangeValue(sttkr, sttyp, date, cdate, *pdret);
	}

	return fError;
}

///***************************************************************************
// OpenStockValue()
//
// Purpose:
//
//      Function will get the opening price of a stock on the requested day.
//      In case of a failure, it returns TRUE and does not modify the store
//      variable. Otherwise, it modifies the variable and returns false.
//      If g_fOnline is set, it will attempt to poll the stock web service
//      instead of from the local arrays.
//
// Parameters:
//
//      LPWSTR sttkr    A string representing the desired ticker symbol
//
//      int date        A non-negative number indicating the date
//
//      double *dret    Pointer to where to store the opening price
//
// Returns: 
//
//      BOOL            FALSE if there are no errors; TRUE otherwise
//
// Comments:
//
///***************************************************************************

BOOL OpenStockValue(LPWSTR sttkr, int date, double* dret)
{
	BOOL fError;

	fError = FALSE;

	if (g_fOnline)
	{
		if (SUCCEEDED(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)))
		{
			Service::CService serv;
			serv.AddRef();
			if (FAILED(serv.GetOpen(sttkr, date, dret)))
				fError = TRUE;
			serv.Release();
			CoUninitialize();
		}
		else
		{
			fError = FALSE;
		}
	}
	else
	{
		*dret = GetOpeningValue(sttkr, date);
		if (*dret < 0)
			fError = TRUE;
	}

	return fError;
}

///***************************************************************************
// CloseStockValue()
//
// Purpose:
//
//      Function will get the closing price of a stock on the requested day.
//      In case of a failure, it returns TRUE and does not modify the store
//      variable. Otherwise, it modifies the variable and returns false.
//      If g_fOnline is set, it will attempt to poll the stock web service
//      instead of from the local arrays.
//
// Parameters:
//
//      LPWSTR sttkr    A string representing the desired ticker symbol
//
//      int date        A non-negative number indicating the date
//
//      double *dret    Pointer to where to store the closing price
//
// Returns: 
//
//      BOOL            FALSE if there are no errors; TRUE otherwise
//
// Comments:
//
///***************************************************************************

BOOL CloseStockValue(LPWSTR sttkr, int date, double* dret)
{
	BOOL fError;

	fError = FALSE;

	if (g_fOnline)
	{
		if (SUCCEEDED(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)))
		{
			Service::CService serv;
			serv.AddRef();
			if (FAILED(serv.GetClose(sttkr, date, dret)))
				fError = TRUE;
			serv.Release();
			CoUninitialize();
		}
		else
		{
			fError = FALSE;
		}
	}
	else
	{
		*dret = GetClosingValue(sttkr, date);
		if (*dret < 0)
			fError = TRUE;
	}

	return fError;
}

///***************************************************************************
// LowStockValue()
//
// Purpose:
//
//      Function will get the low price of a stock on the requested day.
//      In case of a failure, it returns TRUE and does not modify the store
//      variable. Otherwise, it modifies the variable and returns false.
//      If g_fOnline is set, it will attempt to poll the stock web service
//      instead of from the local arrays.
//
// Parameters:
//
//      LPWSTR sttkr    A string representing the desired ticker symbol
//
//      int date        A non-negative number indicating the date
//
//      double *dret    Pointer to where to store the low price
//
// Returns: 
//
//      BOOL            FALSE if there are no errors; TRUE otherwise
//
// Comments:
//
///***************************************************************************

BOOL LowStockValue(LPWSTR sttkr, int date, double* dret)
{
	BOOL fError;

	fError = FALSE;

	if (g_fOnline)
	{
		if (SUCCEEDED(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)))
		{
			Service::CService serv;
			serv.AddRef();
			if (FAILED(serv.GetLow(sttkr, date, dret)))
				fError = TRUE;
			serv.Release();
			CoUninitialize();
		}
		else
		{
			fError = FALSE;
		}
	}
	else
	{
		*dret = GetLowValue(sttkr, date);
		if (*dret < 0)
			fError = TRUE;
	}

	return fError;
}

///***************************************************************************
// HighStockValue()
//
// Purpose:
//
//      Function will get the high price of a stock on the requested day.
//      In case of a failure, it returns TRUE and does not modify the store
//      variable. Otherwise, it modifies the variable and returns false.
//      If g_fOnline is set, it will attempt to poll the stock web service
//      instead of from the local arrays.
//
// Parameters:
//
//      LPWSTR sttkr    A string representing the desired ticker symbol
//
//      int date        A non-negative number indicating the date
//
//      double *dret    Pointer to where to store the high price
//
// Returns: 
//
//      BOOL            FALSE if there are no errors; TRUE otherwise
//
// Comments:
//
///***************************************************************************

BOOL HighStockValue(LPWSTR sttkr, int date, double* dret)
{
	BOOL fError;

	fError = FALSE;

	if (g_fOnline)
	{
		if (SUCCEEDED(CoInitializeEx(NULL,COINIT_APARTMENTTHREADED)))
		{
			Service::CService serv;
			serv.AddRef();
			if (FAILED(serv.GetHigh(sttkr, date, dret)))
				fError = TRUE;
			serv.Release();
			CoUninitialize();
		}
		else
		{
			fError = FALSE;
		}
	}
	else
	{
		*dret = GetHighValue(sttkr, date);
		if (*dret < 0)
			fError = TRUE;
	}

	return fError;
}

