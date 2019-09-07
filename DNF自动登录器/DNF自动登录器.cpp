// DNF自动登录器.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AutoLogin.h"
#define WM_MYMESSAGE_UNUSUAL WM_USER+100
#define WM_MYMESSAGE_COMPLETE WM_USER+101
#define WM_MYMESSAGE_PLUSE WM_USER+102
#define WM_MYMESSAGE_SAFEMODE  WM_USER+103
#define WM_MYMESSAGE_THIRDPART  WM_USER+104
#define WM_MYMESSAGE_FORBIDDEN  WM_USER+105
#define WM_MYMESSAGE_BLOCK  WM_USER+106

HWND keeperHandle;
int _tmain(int argc, _TCHAR* argv[])
{
	Dm dm;
	AutoLogin autoLogin(dm);
	
	if (argc > 4){
		keeperHandle = (HWND)atoi(argv[1]);
		autoLogin.KeeperHandle = keeperHandle;
		autoLogin.Login(argv[2], argv[3], argv[4]);
	}
	else{
		keeperHandle = (HWND)atoi(argv[0]);
		autoLogin.KeeperHandle = keeperHandle;
		autoLogin.Login(argv[1], argv[2], argv[3]);
	}
	
	/*autoLogin._dm._dm.Capture(424, 297, 553, 350, "v.bmp");
	autoLogin._dm.unbind_wnd();
	char buffer[32];
	int ret = D2File("387437d579612e97f9257db36be579ea", "test", "test", "v.bmp", 30, 57342, buffer);*/
	return 0;
}
