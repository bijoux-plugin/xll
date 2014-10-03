///***************************************************************************
// File:				GENERIC.H
//
// Purpose:			Header file for Generic.c
// 
// Platform:    Microsoft Windows
//
// Development Team: Andy Steenson
//
// Updated by Microsoft Product Support Services, Windows Developer Support.
// From the Microsoft Excel Developer's Kit, Version 5
// Copyright (c) 1996 Microsoft Corporation. All rights reserved.
///***************************************************************************

// 
// Function prototypes
//

void cwCenter(HWND, int);
BOOL CALLBACK DIALOGMsgProc(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL GetHwnd(HWND * pHwnd);
int lpwstricmp(LPWSTR s, LPWSTR t);

//
// identifier for controls
//
#define FREE_SPACE                  104
#define EDIT                        101
#define TEST_EDIT                   106
