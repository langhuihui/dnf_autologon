
#pragma once



#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers





#include <windows.h>
#include <stdio.h>
#include <imm.h>
#include <tchar.h>
#include <crtdbg.h>
#include "helper.h"
#include "resource.h"
#include "attach_dnf.h"

////////////////////////
//	输入法导出
//
/////////////////////
//__declspec(dllexport) BOOL ImeClass_Register( HINSTANCE hInstance );
//__declspec(dllexport) void ImeClass_Unregister(HINSTANCE hInstance);
extern "C"
{
__declspec(dllexport) LRESULT WINAPI UIWndProc(HWND hWnd,	UINT message,WPARAM wParam,	LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI InputWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI CandWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI CompWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI StatusWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
}




#define CS_INPUTSTAR			(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS)

//窗口类名
#define CLSNAME_UI			_T("ISUI")		//UI
#define CLSNAME_STATUS		_T("ISSTATUS")	//状态栏
#define CLSNAME_INPUT		_T("ISINPUT")	//输入窗口
#define CLSNAME_COMP		_T("ISCOMP")	//编码窗口
#define CLSNAME_CAND		_T("ISCAND")	//候选窗口
#define CLSNAME_SPCHAR		_T("ISSPCHAR")	//特殊字符窗口

#define MAX_PRIVATEDATA			100
#define MAX_CAND                256
#define	CANDPERPAGE				5


typedef struct tagUIPRIV {      // IME private UI data
    HWND    hStatusWnd;         // status window
    HWND    hInputWnd;          // 输入窗口，包含编码窗口和候选窗口两个子窗口
}UIPRIV;

typedef UIPRIV      *PUIPRIV;
typedef UIPRIV NEAR *NPUIPRIV;
typedef UIPRIV FAR  *LPUIPRIV;

typedef struct tagTRANSMSG {
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
} TRANSMSG, *PTRANSMSG, NEAR *NPTRANSMSG, FAR *LPTRANSMSG;

extern HINSTANCE	g_hInst;

