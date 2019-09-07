#pragma once

//#define LOGFILE "c:\\imesample.log"

#define GETLPCOMPSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwCompStrOffset)
#define GETLPRESULTSTR(lpcs) (LPTSTR)((LPBYTE)(lpcs) + (lpcs)->dwResultStrOffset)

void Helper_Trace(char * pszFormat,...);

void Helper_DrawBMP(HDC hdc,RECT rc,UINT bmpID);

BOOL Helper_IsIMEMessage(UINT message);

void Helper_CenterWindow(HWND hWnd);
