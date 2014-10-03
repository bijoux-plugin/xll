///***************************************************************************
// File:		Metrics.h
// Description:	Header file for XLL stock metrics
//
// File contains the definitions of the different metric functions
// used by the sample XLL to grade a stock
//
///***************************************************************************


BOOL ComputeEMA(double* pddata, int cdata, int cperiod, double* pdret, int fStore);

BOOL MetrADX(double* pdhigh, double* pdlow, double* pdclose, int cclose, int cper, double* dret);

BOOL MetrMACD(double* pdclose, int cclose, int clo, int chi, double* dret);

BOOL MetrRSI(double* pdclose, int cclose, int cper, double* dret);
