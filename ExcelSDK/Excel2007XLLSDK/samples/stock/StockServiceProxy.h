///***************************************************************************
// File:		StockServiceProxy.h
// Description:	Header file for WebService wrappers 
//
// These methods are called by the XLL when it polls the web service.  The
// proxy handles where the data is retrieved from based on the current
// settings in the XLL
//
///***************************************************************************

#include "StockService.h"

void InitStockService();
void UninitStockService();
void ProxyOnlineMode(int flag);
int GetProxyMode();

BOOL OpenStockValue(LPWSTR sttkr, int date, double* dret);
BOOL CloseStockValue(LPWSTR sttkr, int date, double* dret);
BOOL LowStockValue(LPWSTR sttkr, int date, double* dret);
BOOL HighStockValue(LPWSTR sttkr, int date, double* dret);
BOOL RangeStockValue(LPWSTR sttkr, LPWSTR sttyp, int date, int cdate, double** pdret);
