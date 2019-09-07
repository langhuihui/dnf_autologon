// 守护程序.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "守护程序.h"

//#include "ks.h"
#define MAX_LOADSTRING 100
#define TIMEREVENT_CHECK 1
// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
RECT windowSize;

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	G_Log(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HANDLE hDevice = CreateFile("\\\\192.168.238.130\\ProtectProcess", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
#pragma warning( disable : 4996)
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
	Log(szTitle);
	mailInfo.m_pcUserName = "dexter_1985@163.com";
	mailInfo.m_pcIPName = "smtp.163.com";
	mailInfo.m_pcUserPassWord = "rhy08917";
	mailInfo.m_pcSender = "dexter_1985@163.com";
	mailInfo.m_pcSenderName = "dnf_log";
	mailInfo.m_pcTitle = "DNF_守护程序Log";
	mailInfo.m_pcBody = "看附件";
	mailInfo.m_pcReceiver = "178529795@qq.com";
	mailInfo.m_pcIPAddr = "";
	sender.AddFilePath("Log.txt");
	sender.AddFilePath("login.log"); 
	/*ksLoadDLLs();
	KSInit();
	if (KSLogin(147, "X1L4O55LrJp198SyD481a13bK3xQ89N8")){
		Log("可可登录成功！");
	}
	else{
		Log("可可登录失败！");
		return -1;
	}*/
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	ReadConfig();
	ReadAccountList();
	ReadUnusualAccountList();
	GetNetCardInfo();
	//currentAccount = &AccountList.GetHead();
	//currentAccount = &AccountList.GetAt(AccountList.GetHeadPosition() + currentNetCardIndex);
	LaunchAutoLogin();
	MSG msg;
	HACCEL hAccelTable;
	srand(atoi(currentAccount->QQ));
	
	
	//进程通讯
	/*if (IPC_Start(12345, 1) == 0){
		Log("进程通讯服务器启动失败！");
		return -1;
	}*/
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));
	//tryLaunchAutoLogin();
	SetTimer(hWnd, TIMEREVENT_CHECK, 10000, NULL);
	WndProc(hWnd, WM_TIMER, TIMEREVENT_CHECK, NULL);
	// 主消息循环: 
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//cout << "GetMessage" << msg.message;
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			//cout << " --> TranslateAccelerator Done!";
			TranslateMessage(&msg);
			//cout << "-->TranslateMessage";
			DispatchMessage(&msg);
			//cout << "-->DispatchMessage";
		}
		//cout <<"!"<< endl;s
	}
	Log("退出守护程序！");
	KillAutoLogin();
	logOfStream.close();
	/*ks_exit();
	ksFreeDLLs();*/
	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   GetSystemMetrics(SM_CXSCREEN)-350, GetSystemMetrics(SM_CYSCREEN)-290, 350, 250, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }

   GetClientRect(hWnd,&windowSize);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   HWND _hWnd = GetConsoleWindow();
  /* RECT rect;
   GetWindowRect(_hWnd,&rect);*/
   MoveWindow(_hWnd, GetSystemMetrics(SM_CXSCREEN) - 750, GetSystemMetrics(SM_CYSCREEN) - 250, 400, 250, false);
   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	 RECT rect;
	 GetWindowRect(hWnd, &rect);
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	int i = 0;
	int j = 0;
	int k = 0;
	CString txt = "";
	txt.Format("windows消息%x %x %x", message,wParam,lParam);
	Print(txt.GetBuffer(0));
	std::string _log;
	GETNOW
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_32771://重启
			if (tryLaunchAutoLogin()){
				lastRestartMsg.Format("上次重启原因：%s", "菜单命令");
			}
			break;
		case ID_32772://开关
			if (AutoProcessHandler == NULL){
				currentAccount->CoolDown = 0;
				if (tryLaunchAutoLogin()){
					lastRestartMsg.Format("上次重启原因：%s", "菜单命令");
				}
			}
			else{
				currentAccount->CoolDown = INT64_MAX;
				KillAutoLogin();
			}
			break;
		case ID_32773://提交log
			logOfStream.close();
			if (sender.SendMail(mailInfo)){
				MessageBox(hWnd, "发送成功！", "提交log", 0);
			}
			else{
				MessageBox(hWnd, "发送失败！", "提交log", 0);
			}
			break;
		case ID_32774://提交g_log
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, G_Log);
			
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MYMESSAGE_SAFEMODE:
		AccountLog("安全模式");
		CurrentLogInfo.SafeModeCount++;
		InvalidateRect(hWnd, NULL, true);
		SaveSafeModeAccount(currentAccount->QQ);
		break;
	case WM_MYMESSAGE_THIRDPART:
		AccountLog("第三方");
		CurrentLogInfo.ThirdPartCount++;
		InvalidateRect(hWnd, NULL, true);
		SetUnusualAccount(1);
		if (tryLaunchAutoLogin()){
			lastRestartMsg.Format("上次重启原因：%s", "第三方");
		}
		break;
	case WM_MYMESSAGE_FORBIDDEN:
		AccountLog("关禁闭");
		CurrentLogInfo.ForbiddenCount++;
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MYMESSAGE_BLOCK:
		AccountLog("封号");
		CurrentLogInfo.BlockCount++;
		InvalidateRect(hWnd, NULL, true);
		SaveBlockAccount(currentAccount->QQ);
		AccountList.RemoveHead();
		if (AccountList.GetCount() > 0){
			currentAccount = &AccountList.GetHead();
			if (tryLaunchAutoLogin()){
				lastRestartMsg.Format("上次重启原因：%s", "封号");
			}
		}
		else{
			Log("已无可用帐号,停止使用");
			KillAutoLogin();
			KillTimer(hWnd, TIMEREVENT_CHECK);
		}
		break;
	case WM_MYMESSAGE_UNUSUAL:
		AccountLog("数据异常");
		CurrentLogInfo.UnusualCount++;
		currentAccount->CoolDown = now+ 60 * 65;
		//SetNextAccount();
		if (tryLaunchAutoLogin()){
			lastRestartMsg.Format("上次重启原因：%s", "数据异常");
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MYMESSAGE_COMPLETE:
		AccountLog("帐号刷完");
		now = Next6();
		currentAccount->CoolDown = now;
		/*currentAccount->CoolDown = IsInGameTime(now) == -1 ? NextGameTime(now) : now + (rand() % 10) *60;*/
		/*currentAccount->CoolDown = -1;
		/*SetNextAccount();
		if (tryLaunchAutoLogin()){
			lastRestartMsg.Format("上次重启原因：%s", "帐号刷完");
		}*/
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_MYMESSAGE_PLUSE:
		timeout = wParam == 0 ? 300 : wParam;
		if(lParam != 0)restartFlag = lParam;
		txt.Format("收到心跳包%lld %d %d", timeout, restartFlag, now - lastPluse);
		Log(txt.GetBuffer(0));
		//std::cout << "pluse" << " " << timeout << " " << restartFlag << " " << now - lastPluse<<std::endl;
		lastPluse = now;
		break;
	case WM_MYMESSAGE_RESTARTWITHOUTLOGIN:
		lastRestartMsg.Format("上次重启原因：%s", "收到非登录重启的命令");
		RestartWithoutLogin();
		break;
	case WM_MYMESSAGE_CHANGEROLE:
		/*roleNum++;
		_log = "第";
		_log +=itoa(roleNum, " ", 10);
		_log += "角色刷完";
		AccountLog(_log);
		if (roleNum >= 4){
			currentAccount->CoolDown = now+ 60*60;
			SetNextAccount();
			tryLaunchAutoLogin();
			InvalidateRect(hWnd, NULL, true);
		}*/
		break;
	case WM_MYMESSAGE_RESTART:
		Log("收到强制重启命令");
		lastRestartMsg.Format("上次重启原因：%s", "收到强制重启命令");
		KillAutoLogin();
		LaunchAutoLogin();
		break;
	case WM_TIMER:
		if (wParam == TIMEREVENT_CHECK){
			
			if (IsIconic(hWnd)){
				Log("侦测到被最小化");
				ShowWindow(hWnd,SW_NORMAL);
				ShowWindow(GetConsoleWindow(), SW_NORMAL);
		
				Log("恢复窗口");
			}
			if (rect.left!=windowSize.left||rect.right!=windowSize.right||rect.top!=windowSize.top||rect.bottom!=windowSize.bottom){
				MoveWindow(hWnd, GetSystemMetrics(SM_CXSCREEN) - 350, GetSystemMetrics(SM_CYSCREEN) - 290, 350, 250, true);
			}
			if (AutoProcessHandler == NULL){
#ifdef REST
				if ((now > currentAccount->CoolDown && currentAccount->CoolDown != 0) || (currentAccount->CoolDown == 0 && IsInGameTime() != -1))
				{
					/*if (CheckUnusualAccount()){
						Log("使用异常帐号登录");
						currentAccount = &AccountList.GetHead();
					}*/
					Log("进入游戏时间段！");
					SetRestTime();
					if (tryLaunchAutoLogin()){
						lastRestartMsg.Format("上次重启原因：%s", "进入游戏时间段");
					}
					InvalidateRect(hWnd, NULL, true);
				}
#else
				if (now > currentAccount->CoolDown && currentAccount->CoolDown!=-1)
				{
					localtime_s(&nowTime, &currentAccount->CoolDown);
					txt.Format("侦测到进程不存在，但满足自动启动时间%02d:%02d:%02d", nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
					Log(txt.GetBuffer(0));
					if (tryLaunchAutoLogin()){
						lastRestartMsg.Format("上次重启原因：%s", "侦测到进程不存在！");
					}
					InvalidateRect(hWnd, NULL, true);
				}
#endif
			}
			else{
#ifdef REST
				if (now > restTime){
					now = NextGameTime(now);
					localtime_s(&nowTime, &now);
					txt.Format("进入休息时间!下次启动时间为%d:%d:%d", nowTime.tm_hour,nowTime.tm_min,nowTime.tm_sec);
					AccountLog(txt);
					currentAccount->CoolDown = now;
					KillAutoLogin();
					InvalidateRect(hWnd, NULL, true);
				}else 
#endif
				if (FindWindow(NULL, "<错误> 插件版本:5.1412")){
					Log("侦测到错误窗口：<错误> 插件版本:5.1412");
					lastRestartMsg.Format("上次重启原因：%s", "侦测到错误窗口 <错误> 插件版本:5.1412");
					KillAutoLogin();
					LaunchAutoLogin();
				}
				else if (FindWindow(NULL, "dnf2异常")){
					Log("侦测到错误窗口：dnf2异常");
					lastRestartMsg.Format("上次重启原因：%s", "侦测到错误窗口 dnf2异常");
					restart();
				}
				else if (FindWindow(NULL, "dnf2­_no_login异常")){
					Log("侦测到错误窗口：dnf2­_no_login异常");
					lastRestartMsg.Format("上次重启原因：%s", "侦测到错误窗口 dnf2­_no_login异常");
					restart();
				}
				else if (now > lastPluse + timeout ){
					if (currentAccount->CoolDown <now){
						localtime_s(&nowTime, &lastPluse);
						txt.Format("心跳超时!超时时间:%lld,标志位:%d,上次心跳时间：%02d:%02d:%02d", timeout, restartFlag, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
						lastRestartMsg.Format("上次重启原因:心跳超时!超时时间:%lld, 标志位 : %d, 上次心跳时间：%02d : %02d : %02d", timeout, restartFlag, nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
						Log(txt);
						restart();
					}
					else{
						Log("心跳超时，但已刷完。");
					}
				}
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		txt.Format("安全模式次数：   %d", CurrentLogInfo.SafeModeCount);
		TextOut(hdc, 0, i, txt.GetBuffer(0), txt.GetLength());
		txt.Format("出第三方次数：   %d", CurrentLogInfo.ThirdPartCount);
		TextOut(hdc, 0, i+=25, txt.GetBuffer(0), txt.GetLength());
		txt.Format("关禁闭次数：       %d", CurrentLogInfo.ForbiddenCount);
		TextOut(hdc, 0, i += 25, txt.GetBuffer(0), txt.GetLength());
		txt.Format("封号次数：           %d", CurrentLogInfo.BlockCount);
		TextOut(hdc, 0, i += 25, txt.GetBuffer(0), txt.GetLength());
		txt.Format("刷完帐号次数：   %d", CurrentLogInfo.CompleteCount);
		TextOut(hdc, 0, i += 25, txt.GetBuffer(0), txt.GetLength());
		txt.Format("数据异常次数：   %d", CurrentLogInfo.UnusualCount);
		TextOut(hdc, 0, i += 25, txt.GetBuffer(0), txt.GetLength());
		k = i;
		for (i = 0; i < AccountList_Original.GetCount(); i++){
			AccountInfo &info = AccountList_Original.GetAt(i);
			txt.Format("(%d)%s", i + 1, info.QQ);
			TextOut(hdc, 150, j, txt.GetBuffer(), txt.GetLength());
			j += 25;
			if (info.QQ == currentAccount->QQ){
				txt.Format("当前帐号：(%d)%s(%s)", i + 1, currentAccount->QQ, AutoProcessHandler ? "运行" : currentAccount->CoolDown >now?"刷完":"休息");
				TextOut(hdc, 0,k, txt.GetBuffer(0), txt.GetLength());
			}
		}
		TextOut(hdc, 0, k + 25, lastRestartMsg.GetBuffer(0), lastRestartMsg.GetLength());
		ValidateRect(hWnd, NULL);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOVE:
	case WM_SIZE:
		break;
	default:
		//cout <<"default" <<message ;
		i = DefWindowProc(hWnd, message, wParam, lParam);
		//cout << ":"<<i << endl;
		return i;
	}
	return 0;
}
INT_PTR CALLBACK G_Log(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	
	switch (message)
	{
	case WM_INITDIALOG:
		[&hDlg](){
			ifstream reason("reason.txt", ios::in);
			char buffer[256];
			while (!reason.eof()){
				reason.getline(buffer, sizeof(buffer));
				SendDlgItemMessage(hDlg, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)buffer);
			}
		}();
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK){
				[&hDlg](){
					char buffer[256];
					
					SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETLBTEXT, SendDlgItemMessage(hDlg, IDC_COMBO1, CB_GETCURSEL, 0, 0), (LPARAM)buffer);
					sMailInfo mailInfo; CSendMail sender;
					mailInfo.m_pcUserName = "dexter_1985@163.com";
					mailInfo.m_pcIPName = "smtp.163.com";
					mailInfo.m_pcUserPassWord = "rhy08917";
					mailInfo.m_pcSender = "dexter_1985@163.com";
					mailInfo.m_pcSenderName = "dnf_log";
					mailInfo.m_pcTitle = "DNF_Log";
					mailInfo.m_pcBody = buffer;
					mailInfo.m_pcReceiver = "2221249130@qq.com";
					mailInfo.m_pcIPAddr = "";
					sender.AddFilePath("g_log.log");
					if (AutoProcessHandler!=NULL)KillAutoLogin();
					if (sender.SendMail(mailInfo)){
						MessageBox(hDlg, "发送成功！", "提交log", 0);
					}
					else{
						MessageBox(hDlg, "发送失败！", "提交log", 0);
					}
				}();
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
