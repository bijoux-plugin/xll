///***************************************************************************
// File:            Metrics.cpp
// Description:	    Implementation of XLL stock metrics
//
// These methods compute several different stock metrics from given input.
// The different paramaters to the metrics (such as length of time period)
// are left as user-defined in case one would like to call them with
// different values to better gauge shorter (or longer) time periods
//
///***************************************************************************

#include <stdlib.h>
#include <windows.h>

///***************************************************************************
// ComputeEMA()
//
// Purpose:
//
//      This function computes an Exponential Moving Average with user-specified
//      paramaters and data.  Optionally, it stores every computed value instead
//      of only the last one.
//
//      The n-day EMA is defined as:
//          (previous n-day EMA) + (new value - previous n-day EMA) * (2 / (n + 1))
//      Where the first EMA is a simple average of the first n days
//
// Parameters:
//
//      double* pddata	Pointer to array of doubles that represents the data
//
//      int cdata       Number of elements in pddata
//
//      int cperiod     Length of the averaging period
//
//      double* pdret   Either one double to store the last value in, or
//                      an array of doubles to store every computed value in
//
//      int fStore      TRUE if each computed value should be stored
//
// Returns: 
//
//      int             TRUE if success, FALSE otherwise
//
// Comments:
//
//      If fStore is TRUE, pdret needs to have space to store
//      (cdata - cperiod + 1) doubles
//
// History:  Date       Author        Reason
///***************************************************************************

BOOL ComputeEMA(double* pddata, int cdata, int cperiod, double* pdret, int fStore)
{
	double dsum;
	double dmult;
	int i;
	BOOL fError;

	dsum = 0;
	fError = FALSE;
	dmult = 2.f / (cperiod + 1);

	if (cperiod >= cdata || pdret == NULL)
		fError = TRUE;
	else
	{
		for (i = 0; i < cperiod; i++)
		{
			dsum += *pddata;
			pddata++;
		}
		dsum /= cperiod;
		if (fStore)
		{
			*pdret = dsum;
			pdret++;
		}

		for (i = 0; i < cdata - cperiod; i++)
		{
			dsum = dsum + (*pddata - dsum) * dmult;
			pddata++;
			if (fStore)
			{
				*pdret = dsum;
				pdret++;
			}
		}
		if (!fStore)
		{
			*pdret = dsum;
		}
	}
	return fError;
}

///
/// This simple method computes an average used in the ADX calculation
///

void AdxAve(double* pdcurAve, int cper, double dnewVal)
{
	*pdcurAve = dnewVal / (double)cper + *pdcurAve * (double)(cper - 1) / (double)cper;
}

///***************************************************************************
// MetrADX()
//
// Purpose:
//
//      This function computes the ADX (Average Directional Index) of a stock
//      by this formula:
//          1) Calculate two values: +DM and -DM
//             +DM = Current high price - last high price
//             -DM = Last low price - current low price
//          2) Throw away the smaller of the two, or both if neither is positive
//          3) Calculate the True Range, defined as the largest of
//             * Current high price - current low price
//             * Current high price - last close price
//             * Last close price - current low price
//          4) For each of those three values (+DM, -DM, and TR), take an average
//             over n-days as defined in AdxAve.  These are called +DM14, -DM14, and TR14
//          5) Calculate +DI14 by taking +DM14 / TR14
//          6) Calculate -DI14 by taking -DM14 / TR14
//          7) Calculate DX as |(+DI14) - (-DI14)| / ((+DI14) + (-DI14))
//          8) Take the n-day average of DX using the AdxAve function; this
//             value is the ADX
//
// Parameters:
//
//          double* pdhigh      Pointer to array of the daily high prices
//
//          double* pdlow       Pointer to array of the daily low prices
//
//          double* pdclose     Pointer to array of the daily closing prices
//
//          int cclose          Number of periods in each of the data sets
//
//          int cper            Length of averaging period
//
//          double* dret        Pointer to double in which the final value is stored
//
// Returns: 
//
//          int                 TRUE if success, FALSE otherwise
//
// Comments:
//
//
// History:  Date       Author        Reason
///***************************************************************************

BOOL MetrADX(double* pdhigh, double* pdlow, double* pdclose, int cclose, int cper, double* dret)
{
	double ddmp;  //running calc of +DM14 / TR14
	double ddmm;  //running calc of -DM14 / TR14
	double detr;  //running calc of True Range
	double dadx;  //current ADX

	double dtp;   //temp used in computing +DM
	double dtm;   //temp used in computing -DM
	double dtr;   //temp used in compute True Range
	double dtrhl; //difference of high and low
	double dtrhc; //difference of high and close
	double dtrcl; //difference of close and low
	int i;
	BOOL fError;

	fError = FALSE;

	ddmp = 0.0;
	ddmm = 0.0;
	detr = 0.0;
	dadx = 0.0;

	if (pdclose == NULL || dret == NULL || cper < 1)
		fError = TRUE;
	else
	{
		//for the first n-days, just compute a running total of
		//+DM, -DM, and the True Range
		for (i = 1; i <= cper; i++)
		{
			dtp = pdhigh[i] - pdhigh[i-1];
			dtm = pdlow[i-1] - pdlow[i];

			if (dtp < 0 && dtm < 0)
			{
				dtp = 0;
				dtm = 0;
			}
			else if (dtp > dtm)
				dtm = 0;
			else
				dtp = 0;

			dtrhl = pdhigh[i] - pdlow[i];
			dtrhc = pdhigh[i] - pdclose[i-1];
			dtrcl = pdclose[i-1] - pdlow[i];

			dtr = dtrhl > dtrhc ? dtrhl : dtrhc;
			dtr = dtr > dtrcl ? dtr : dtrcl;

			ddmp += dtp;
			ddmm += dtm;
			detr += dtr;
		}

		//the first average is just a simple average
		ddmp /= (double)cper;
		ddmm /= (double)cper;
		detr /= (double)cper;

		//divide +DM14 and -DM14 by TR14
		ddmp /= detr;
		ddmm /= detr;

		//compute the first ADX
		dadx = (ddmp > ddmm ? ddmp - ddmm : ddmm - ddmp);
		dadx /= ddmp + ddmm;

		//the rest of the days can compute averages using AdxAve
		for (i = cper+1; i < cclose; i++)
		{
			dtp = pdhigh[i] - pdhigh[i-1];
			dtm = pdlow[i-1] - pdlow[i];

			if (dtp < 0 && dtm < 0)
			{
				dtp = 0;
				dtm = 0;
			}
			else if (dtp > dtm)
				dtm = 0;
			else
				dtp	= 0;

			dtrhl = pdhigh[i] - pdlow[i];
			dtrhc = pdhigh[i] - pdclose[i-1];
			dtrcl = pdclose[i-1] - pdlow[i];

			dtr = dtrhl > dtrhc ? dtrhl : dtrhc;
			dtr = dtr > dtrcl ? dtr : dtrcl;

			AdxAve(&detr, cper, dtr);

			AdxAve(&ddmp, cper, dtp/detr);
			AdxAve(&ddmm, cper, dtm/detr);

			dtr = (ddmp > ddmm ? ddmp - ddmm : ddmm - ddmp);
			dtr /= ddmp + ddmm;
			AdxAve(&dadx, cper, dtr);
		}
	}

	*dret = dadx;

	return fError;
}

///***************************************************************************
// MetrMACD()
//
// Purpose:
//
//      This function computes the MACD (Moving Average Convergence/Divergence)
//      of a stock given by the formula:
//          1) Calculate a "low" exponential moving average of n-days on the closing price
//          2) Calculate a "high" exponential moving average of a set number of days
//             that is larger than n; usually around 2n
//          3) The MACD is the difference of the high and low averages
//
// Parameters:
//
//      double* pdclose	Pointer to array of the daily closing prices
//
//      int cclose      Number of periods in each of the data sets
//
//      int clo         Length of the smaller averaging period
//
//      int chi         Length of the larger averaging period
//
//      double* dret    Pointer to double in which the final value is stored
//
// Returns: 
//
//      int             TRUE if success, FALSE otherwise
//
// Comments:
//
//
// History:  Date       Author        Reason
///***************************************************************************

BOOL MetrMACD(double* pdclose, int cclose, int clo, int chi, double* dret)
{
	double dloEMA;
	double dhiEMA;
	BOOL fError;

	fError = FALSE;
	if (pdclose == NULL || dret == NULL || clo < 1 || clo > chi || cclose - chi < 2)
		fError = TRUE;
	else
	{
		fError = ComputeEMA(pdclose, cclose, clo, &dloEMA, 0);
		fError|= ComputeEMA(pdclose, cclose, chi, &dhiEMA, 0);

		if (!fError)
			*dret = dhiEMA - dloEMA;
	}
	return fError;
}

///***************************************************************************
// MetrRSI()
//
// Purpose:
//
//      This function computes the RSI (Relative Strength Index) of a stock given
//      by the formula:
//          1) Define the first Average Gain over n-days as a simple average of the
//             total gains (increases only) over n-days
//          2) Similarly define an Average Loss over n-days
//          3) Subsequent Average Gains are defined as
//               (previous Average Gain * (n-1) + current gain) / n
//             with Average Losses being computed similarly
//          4) Define the RS as (Average Gains) / (Average Losses)
//          5) The RSI is defined as 100 - (100 / (1 + RS))
//          6) If the average losses every becomes 0, the RSI is defined
//             as 100
//
// Parameters:
//
//      double* pdclose	Pointer to array of the daily closing prices
//
//      int cclose      Number of periods in each of the data sets
//
//      int cper        Length of the averaging period
//
//      double* dret    Pointer to double in which the final value is stored
//
// Returns: 
//
//      int             TRUE if success, FALSE otherwise
//
// Comments:
//
//
// History:  Date       Author        Reason
///***************************************************************************

BOOL MetrRSI(double* pdclose, int cclose, int cper, double* dret)
{
	double dt; //current difference in close
	double dtg;	//running average gains
	double dtl;	//running average losses
	double drsi; //computed rsi
	int i;
	BOOL fError;

	fError = FALSE;

	dtg = 0.0;
	dtl = 0.0;
	dt = 0.0;
	drsi = 0.0;

	if (pdclose == NULL || dret == NULL || cper < 1 || cclose <= cper)
		fError = TRUE;

	if (!fError)
	{
		for (i = 1; i <= cper; i++)
		{
			dt = pdclose[i] - pdclose[i-1];
			if (dt < 0)
				dtl -= dt;
			else
				dtg += dt;
		}

		dtg /= cper;
		dtl /= cper;

		drsi = dtg / dtl;
		drsi = 100.0 - 100.0 / (1 + drsi);

		for (i = cper+1; i < cclose; i++)
		{
			dt = pdclose[i] - pdclose[i-1];

			if (dtl == 0.0)
				drsi = 100.0;
			else
			{
				dtg = dtg * (cper - 1) + (dt > 0 ? dt : 0);
				dtl = dtl * (cper - 1) - (dt < 0 ? dt : 0);

				dtg /= cper;
				dtl /= cper;

				drsi = dtg / dtl;

				drsi = 100.0 - 100.0 / (1 + drsi);
			}
		}
	}
	*dret = drsi;

	return fError;
}
