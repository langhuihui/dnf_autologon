//***********************************************************************
//	这个文件处理UI消息，导出UIWndProc接口,一些和界面相关的工具函数也在此
//	文件中定义。
//***********************************************************************
#include "imesample.h"

/**********************************************************************/
/* 显示状态栏                                                       */
/**********************************************************************/
void PASCAL UIWnd_OpenStatus( HWND hUIWnd ,BOOL bShow)
{
    HGLOBAL        hUIPrivate;
    LPUIPRIV       lpUIPrivate;
    HIMC           hIMC;
    int            nShowCmd=bShow?SW_SHOWNOACTIVATE:SW_HIDE;

    hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate)           // can not darw status window
       return;
    

    lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
    if (!lpUIPrivate)          // can not draw status window
       return;
    

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
    if (!hIMC) {
       nShowCmd = SW_HIDE;
    } 
    else if (!ImmLockIMC(hIMC))
	{
        nShowCmd = SW_HIDE;
    }else
	{
		ImmUnlockIMC(hIMC);
	}
	
	if(!lpUIPrivate->hStatusWnd)
	{
		RECT rcWorkArea;
	    LPINPUTCONTEXT lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
		SystemParametersInfo(SPI_GETWORKAREA,0,&rcWorkArea,0);
        lpUIPrivate->hStatusWnd = CreateWindowEx(
                                     0,
                                     CLSNAME_STATUS, NULL, 
                                     WS_POPUP|WS_DISABLED,
                                     rcWorkArea.right -200,rcWorkArea.bottom-25,
                                     200,25,
                                     hUIWnd, (HMENU)NULL, g_hInst, NULL);
	}

	ShowWindow( lpUIPrivate->hStatusWnd, nShowCmd);
    GlobalUnlock(hUIPrivate);
    return;
}

/**********************************************************************/
/* 显示状态栏                                                       */
/**********************************************************************/
void PASCAL UIWnd_OpenInput( HWND hUIWnd ,BOOL bShow)
{
    HGLOBAL        hUIPrivate;
    LPUIPRIV       lpUIPrivate;
    HIMC           hIMC;
	LPINPUTCONTEXT lpIMC;
	POINT		   pt;
    int            nShowCmd=bShow?SW_SHOWNOACTIVATE:SW_HIDE;

    hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate)           // can not darw status window
       return;
    

    lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
    if (!lpUIPrivate)          // can not draw status window
       return;
    

    hIMC = (HIMC)GetWindowLong(hUIWnd, IMMGWL_IMC);
 	lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
	pt.x=LOWORD(lpIMC->dwReserve[0]);
	pt.y=HIWORD(lpIMC->dwReserve[0]);
	ImmUnlockIMC(hIMC);
	
	if(!lpUIPrivate->hInputWnd)
	{
        lpUIPrivate->hInputWnd = CreateWindowEx(
                                     0,
                                     CLSNAME_INPUT, NULL, 
                                     WS_POPUP|WS_DISABLED,
                                     pt.x ,pt.y ,
                                     200,50,
                                     hUIWnd, (HMENU)NULL, g_hInst, NULL);
	}else if(bShow)
	{
		SetWindowPos(lpUIPrivate->hInputWnd,NULL,pt.x,pt.y,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}

	ShowWindow( lpUIPrivate->hInputWnd, nShowCmd);
    GlobalUnlock(hUIPrivate);
    return;
}

/**********************************************************************/
/* UIWnd_GetStatusWnd                                                         */
/* Return Value :                                                     */
/*      window handle of candidatte                                   */
/**********************************************************************/
HWND PASCAL UIWnd_GetStatusWnd(
    HWND hUIWnd)                // UI window
{
    HGLOBAL  hUIPrivate;
    LPUIPRIV lpUIPrivate;
    HWND     hWnd;

    hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate) {          // can not darw candidate window
        return (HWND)NULL;
    }

    lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
    if (!lpUIPrivate) {         // can not draw candidate window
        return (HWND)NULL;
    }

    hWnd = lpUIPrivate->hStatusWnd;

    GlobalUnlock(hUIPrivate);
    return (hWnd);
}

/**********************************************************************/
/* UIWnd_GetInputWnd                                                         */
/* Return Value :                                                     */
/*      window handle of candidatte                                   */
/**********************************************************************/
HWND PASCAL UIWnd_GetInputWnd(
    HWND hUIWnd)                // UI window
{
    HGLOBAL  hUIPrivate;
    LPUIPRIV lpUIPrivate;
    HWND     hWnd;

    hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate) {          // can not darw candidate window
        return (HWND)NULL;
    }

    lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
    if (!lpUIPrivate) {         // can not draw candidate window
        return (HWND)NULL;
    }

    hWnd = lpUIPrivate->hInputWnd;

    GlobalUnlock(hUIPrivate);
    return (hWnd);
}

//设置输入窗口位置
void UIWnd_SetInputWndPos(HWND hUIWnd,LPINPUTCONTEXT lpIMC)
{
	POINT pt=lpIMC->cfCompForm.ptCurrentPos;
	int	 hLineHei=16;
	HFONT hUIFont=(HFONT)SendMessage(lpIMC->hWnd,WM_GETFONT,0,0);
	ClientToScreen(lpIMC->hWnd,&pt);
	if(hUIFont)
	{
		HDC hDC = GetDC(lpIMC->hWnd);
		HFONT oldFont = (HFONT)SelectObject(hDC, hUIFont);
		SIZE sizeChar;
		GetTextExtentPoint(hDC,"A",1,&sizeChar);
		SelectObject(hDC, oldFont);
		ReleaseDC(lpIMC->hWnd,hDC);
		hLineHei=sizeChar.cy;
	}
	lpIMC->dwReserve[0]=MAKELONG(pt.x,pt.y);	//将坐标保存在lpIMC->dwReserve[0],在显示输入窗口时再从它取数据
}

/**********************************************************************/
/*                                                                    */
/* NotifyHandle()                                                     */
/*                                                                    */
/* Handle WM_IME_NOTIFY messages.                                     */
/*                                                                    */
/**********************************************************************/

LONG UIWnd_Notify(HIMC hIMC, HWND hUIWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LONG lRet = 0L;
    LPINPUTCONTEXT lpIMC;

    if (!(lpIMC = ImmLockIMC(hIMC)))
        return 0L;

	
    switch (wParam)
    {
	case IMN_OPENSTATUSWINDOW:
		UIWnd_OpenStatus(hUIWnd,TRUE);
		break;
	case IMN_CLOSESTATUSWINDOW:
		UIWnd_OpenStatus(hUIWnd,FALSE);
		break;
    case IMN_OPENCANDIDATE:
        UIWnd_OpenInput(hUIWnd,TRUE);
        break;
    case IMN_CLOSECANDIDATE:
        UIWnd_OpenInput(hUIWnd,FALSE);
        break;
    case IMN_CHANGECANDIDATE:
        break;
	case IMN_SETOPENSTATUS:
		break;
	case IMN_SETCONVERSIONMODE://freepy do like this
		break;
	case IMN_SETSENTENCEMODE:
		break;
	case IMN_SETCOMPOSITIONWINDOW:
		UIWnd_SetInputWndPos(hUIWnd,lpIMC);
		break;
	case IMN_SETSTATUSWINDOWPOS:
		break;
	default:
		break;
    }
    ImmUnlockIMC(hIMC);

    return lRet;
}

/**********************************************************************/
/* UIPaint()                                                          */
/**********************************************************************/
LRESULT PASCAL UIWnd_Paint(HWND   hUIWnd)
{
    PAINTSTRUCT ps;
	MSG sMsg;
    // for safety
    BeginPaint(hUIWnd, &ps);
    EndPaint(hUIWnd, &ps);

    // some application will not remove the WM_PAINT messages
    PeekMessage(&sMsg, hUIWnd, WM_PAINT, WM_PAINT, PM_REMOVE|PM_NOYIELD);
    return (0L);
}

/**********************************************************************/
/* UIWnd_Create()                                                   */
/**********************************************************************/
void PASCAL UIWnd_Create( HWND hUIWnd)
{
    HGLOBAL hUIPrivate;

    // create storage for UI setting
    hUIPrivate = GlobalAlloc(GHND, sizeof(UIPRIV));
    if (!hUIPrivate) {     
        return;
    }

    SetWindowLong(hUIWnd, IMMGWL_PRIVATE, (LONG)hUIPrivate);

    // set the default position for UI window, it is hide now
    SetWindowPos(hUIWnd, NULL, 0, 0, 0, 0, SWP_NOACTIVATE|SWP_NOZORDER);

    ShowWindow(hUIWnd, SW_SHOWNOACTIVATE);

    return;
}

/**********************************************************************/
/* DestroyUIWindow()                                                  */
/**********************************************************************/
void PASCAL UIWnd_Destroy(HWND hUIWnd)
{
    HGLOBAL  hUIPrivate;
    LPUIPRIV lpUIPrivate;

    hUIPrivate = (HGLOBAL)GetWindowLong(hUIWnd, IMMGWL_PRIVATE);
    if (!hUIPrivate) {     
        return;
    }

    lpUIPrivate = (LPUIPRIV)GlobalLock(hUIPrivate);
    if (!lpUIPrivate) {    
        return;
    }
    // status window need to be destroyed
    if (lpUIPrivate->hStatusWnd) {
        DestroyWindow(lpUIPrivate->hStatusWnd);
    }

    // status window need to be destroyed
    if (lpUIPrivate->hInputWnd) {
        DestroyWindow(lpUIPrivate->hInputWnd);
    }

    GlobalUnlock(hUIPrivate);

    // free storage for UI settings
    GlobalFree(hUIPrivate);

    return;
}



/**********************************************************************/
/*                                                                    */
/* UIWndProc()                                                   */
/*                                                                    */
/* IME UI window procedure                                            */
/*                                                                    */
/**********************************************************************/
LRESULT WINAPI UIWndProc(HWND hUIWnd,	UINT message,WPARAM wParam,	LPARAM lParam)
{
    LONG           lRet = 0L;
    HIMC hIMC = (HIMC)GetWindowLong(hUIWnd,IMMGWL_IMC);
    //**************************************************************
    // Even if there is no current UI. these messages should not be pass to 
    // DefWindowProc().
    //**************************************************************
    if (!hIMC)
    {
		if(Helper_IsIMEMessage(message)) return 0;
    }
	
    switch (message)
    {
	case WM_CREATE://做窗口的初始化工作
		UIWnd_Create(hUIWnd);
		break;
	case WM_DESTROY:
		UIWnd_Destroy(hUIWnd);
		break;
	case WM_IME_NOTIFY:
		lRet = UIWnd_Notify(hIMC, hUIWnd,message,wParam,lParam);
		break; 
    case WM_PAINT:
        lRet= UIWnd_Paint(hUIWnd);
		break;
    case WM_IME_STARTCOMPOSITION:
        UIWnd_OpenInput(hUIWnd,TRUE);
        break;
    case WM_IME_ENDCOMPOSITION:
        UIWnd_OpenInput(hUIWnd,FALSE);
        break;
	case WM_IME_COMPOSITION:
		if(lParam & GCS_COMPSTR) InvalidateRect(UIWnd_GetInputWnd(hUIWnd),NULL,FALSE);
		break;
	case WM_IME_SETCONTEXT://更新当前的状态
		break;
    case WM_MOUSEACTIVATE:
        return MA_NOACTIVATE;
	default:
		return DefWindowProc(hUIWnd,message,wParam,lParam);
    }
    return lRet;
}