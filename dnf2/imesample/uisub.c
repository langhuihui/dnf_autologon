//除uiWnd外所有窗口处理
//提示信息显示在编码窗口中
//输入法创建的所有窗口（不含对话框）都应该是WS_DISABLE的，也就是说不能响应鼠标消息
//为了响应鼠标消息需要在WM_SETCURSOR中获取鼠标按钮状态来模拟
#include "imesample.h"

BOOL WINAPI AboutDialogProc(HWND hWnd ,	UINT message ,WPARAM wParam ,LPARAM lParam )
{

	switch(message) {
	case WM_INITDIALOG:
		{
			Helper_CenterWindow(hWnd);
		}
		return TRUE;
	case WM_COMMAND: 
		switch(wParam) { 
			break;
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd,TRUE);
			break;
		default:
			return FALSE;
			break;
		}
		break;
	default:
		return FALSE;
		break;
	}
	return TRUE;
}

LRESULT WINAPI InputWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_CREATE:
		{
			RECT rc;
			GetClientRect(hWnd,&rc);
			rc.left+=10;
			rc.bottom/=2;
			CreateWindow(CLSNAME_COMP,NULL,WS_CHILD|WS_VISIBLE,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,hWnd,(HMENU)0,g_hInst,NULL);
			OffsetRect(&rc,0,rc.bottom);
			CreateWindow(CLSNAME_CAND,NULL,WS_CHILD|WS_VISIBLE,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,hWnd,(HMENU)0,g_hInst,NULL);
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			HBRUSH hr=CreateSolidBrush(RGB(255,0,0));
			GetClientRect(hWnd,&rc);
			rc.right=rc.left+10;
			FillRect(hdc,&rc,hr);
			DeleteObject(hr);
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if(!Helper_IsIMEMessage(message))
			lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

LRESULT WINAPI CandWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			GetClientRect(hWnd,&rc);
			FillRect(hdc,&rc,(HBRUSH)GetStockObject(DKGRAY_BRUSH));
			TextOut(hdc,0,0,"Cand",4);
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if(!Helper_IsIMEMessage(message))
			lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

LRESULT WINAPI CompWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			RECT rc;
			HWND hUIWnd=GetWindow(GetParent(hWnd),GW_OWNER);
			HIMC hIMC=(HIMC)GetWindowLong(hUIWnd,IMMGWL_IMC);
			LPINPUTCONTEXT lpIMC=(LPINPUTCONTEXT)ImmLockIMC(hIMC);
			GetClientRect(hWnd,&rc);
			FillRect(hdc,&rc,(HBRUSH)GetStockObject(GRAY_BRUSH));
			if(lpIMC)
			{
				LPCOMPOSITIONSTRING lpCompStr=(LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
				if(lpCompStr)
				{
					LPCTSTR pComp=GETLPCOMPSTR(lpCompStr);
					TextOut(hdc,0,1,pComp,lpCompStr->dwCompStrLen);
					ImmUnlockIMCC(lpIMC->hCompStr);
				}
				ImmUnlockIMC(hIMC);
			}
			EndPaint(hWnd,&ps);
		}
		break;
	default:
		if(!Helper_IsIMEMessage(message))
			lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

void UIStatus_OnPaint(HWND hWnd,HDC hdc)
{
	RECT rc;
	//pic box
	GetClientRect(hWnd,&rc);
	Rectangle(hdc,rc.left,rc.top,rc.right,rc.bottom);
	DrawText(hdc,"",-1,&rc,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	TextOut(hdc,1,1,"",1);
}

LRESULT WINAPI StatusWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	LRESULT lRet=0;
	switch(message)
	{
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc=BeginPaint(hWnd,&ps);
			UIStatus_OnPaint(hWnd,hdc);
			EndPaint(hWnd,&ps);
			lRet=1;
		}
		break;
	case WM_SETCURSOR:
		{//模拟鼠标消息
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd,&pt);
			SendMessage(hWnd,HIWORD(lParam),0,MAKELONG(pt.x,pt.y));
		}
		break;
	case WM_LBUTTONDOWN:
		{
	       DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), GetActiveWindow(), (DLGPROC)AboutDialogProc);
		}
		break;
	default:
		lRet=DefWindowProc(hWnd,message,wParam,lParam);
		break;
	}
	return lRet;
}

