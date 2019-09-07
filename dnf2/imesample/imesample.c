// imesample.cpp : Defines the entry point for the DLL application.
//

#include "imesample.h"

HINSTANCE	g_hInst=NULL;

//窗口过程回调
/*
extern "C"
{
__declspec(dllexport) LRESULT WINAPI UIWndProc(HWND hWnd,	UINT message,WPARAM wParam,	LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI InputWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI CandWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI CompWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
__declspec(dllexport) LRESULT WINAPI StatusWndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);
}
*/

//************************************************************
//	注册用户界面窗口类
//************************************************************
BOOL ImeClass_Register( HINSTANCE hInstance )
{
    WNDCLASSEX wc;
	
    //
    // register class of UI window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = UIWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 2 * sizeof(LONG);
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPTSTR)NULL;
    wc.lpszClassName  = CLSNAME_UI;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	
    //
    // register class of input window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = InputWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = CLSNAME_INPUT;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;	

    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
    //
    // register class of status window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = StatusWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = CLSNAME_STATUS;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
    //
    // register class of composition window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = CompWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = CLSNAME_COMP;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
    //
    // register class of candidate window.
    //
    wc.cbSize         = sizeof(WNDCLASSEX);
    wc.style          = CS_INPUTSTAR | CS_IME;
    wc.lpfnWndProc    = CandWndProc;
    wc.cbClsExtra     = 0;
    wc.cbWndExtra     = 0;
    wc.hInstance      = hInstance;
    wc.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wc.hIcon          = NULL;
    wc.lpszMenuName   = (LPSTR)NULL;
    wc.lpszClassName  = CLSNAME_CAND;
    wc.hbrBackground  = NULL;
    wc.hIconSm        = NULL;
	
    if( !RegisterClassEx( (LPWNDCLASSEX)&wc ) )
        return FALSE;
	return TRUE;
}


//**************************************************************
//	注销输入之星注册的窗口类
//**************************************************************
void ImeClass_Unregister(HINSTANCE hInstance)
{
	UnregisterClass(CLSNAME_UI    ,hInstance);
	UnregisterClass(CLSNAME_INPUT    ,hInstance);
	UnregisterClass(CLSNAME_STATUS,hInstance);
	UnregisterClass(CLSNAME_COMP,hInstance);
	UnregisterClass(CLSNAME_CAND,hInstance);
}


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			attach_dnf();
			if(!ImeClass_Register((HINSTANCE)hModule)) return FALSE;
			g_hInst=(HINSTANCE)hModule;
#ifdef LOGFILE
			{
				FILE *f=fopen(LOGFILE,"rb");
				if(f)
				{
					long lSize=0;
					fseek(f,0,SEEK_END);
					lSize=ftell(f);
					fclose(f);
					if(lSize>(1<<12)) DeleteFile(LOGFILE);
				}
			}
#endif
			break;
		case DLL_PROCESS_DETACH:
			ImeClass_Unregister((HINSTANCE)hModule);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
    }
    return TRUE;
}

