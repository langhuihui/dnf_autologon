
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
//	���뷨����
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

//��������
#define CLSNAME_UI			_T("ISUI")		//UI
#define CLSNAME_STATUS		_T("ISSTATUS")	//״̬��
#define CLSNAME_INPUT		_T("ISINPUT")	//���봰��
#define CLSNAME_COMP		_T("ISCOMP")	//���봰��
#define CLSNAME_CAND		_T("ISCAND")	//��ѡ����
#define CLSNAME_SPCHAR		_T("ISSPCHAR")	//�����ַ�����

#define MAX_PRIVATEDATA			100
#define MAX_CAND                256
#define	CANDPERPAGE				5


typedef struct tagUIPRIV {      // IME private UI data
    HWND    hStatusWnd;         // status window
    HWND    hInputWnd;          // ���봰�ڣ��������봰�ںͺ�ѡ���������Ӵ���
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

