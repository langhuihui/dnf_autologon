#include "stdafx.h"
#include "AutoLogin.h"
#include <fstream>  
#include "AutoProcess.h"
AutoLogin::AutoLogin(Dm&dm) :_dm(dm)
{
	std::ifstream fin("ServerMap.txt", std::ios::in);
	char  line[1024] = { 0 };
	string area,server;
	while (fin.getline(line, sizeof(line)))
	{
		std::stringstream  word(line);
		word >> area;
		printf("读取大区数据:%s", area.c_str());
		ServerListArray servers;
		while (!word.eof()){
			word >> server;
			servers.Add(server.c_str());
			_serverMap.SetAt(server.c_str(), area.c_str());
			printf(server.c_str());
		}
		printf("\r\n");
		_serverLists.Add(servers);
		_areaList.Add(area.c_str());
	}
	printf("读取服务器映射文件完成\r\n");
	long ret = _dm.reg();
	int regCount = 0;
	while (ret != 1){
		regCount++;
		if (regCount > 5){
			cout << "注册尝试次数超过5次，5秒后退出" << endl;
			Sleep(5000);
			ExitProcess(-1);
			return;
		}
		cout << "注册失败：" << ret<<endl;
		Sleep(2000);
		ret = _dm.reg();
	}
	_dm.set_wnd_name("地下城与勇士登录程序");
	_dm.find_wnd();
	while (_dm.wnd_hwnd == NULL){
		printf("未侦测到游戏程序窗口，2秒后再次侦测！\r\n");
		Sleep(2000);
		_dm.find_wnd();
	}
	printf("侦测到游戏程序窗口，开始绑定窗口！\r\n");
	BindDNFLoginWindow();
	_dm._cdnf.ActivateWin((long)_dm.wnd_hwnd);
	
}
void AutoLogin::Login(char * qq,char *password,char * area){
	
	cout << "读取打码兔帐号";
	std::ifstream fin("dama2Account.txt", std::ios::in);
	char  line[256] = { 0 };
	fin.getline(line, sizeof(line));
	std::stringstream  word(line);
	cout << "完成" << endl;
	WaitUpdate waitUpdate;
	ExitSelfProcess exitSelf;
	SelectArea selectArea;
	SelectServer selectServer;
	FullAlert fullAlert;
	DelayAlert delayAlert;
	LoginAdv loginAdv;
	InputVerify inputVerify;
	InputAccount inputAccount;
	InputWrong inputWrong;
	Logined logined;
	CompleteProcess completeProcess;
	inputAccount.username = qq;
	inputAccount.password = password;
	string username_v, password_v;
	word >> username_v;
	inputVerify.username = (char *)username_v.c_str();
	word >> password_v;
	inputVerify.password = (char *)password_v.c_str();
	cout << "初始化状态机" << endl;
	waitUpdate.timeout = -1;
	selectArea.timeout = 10;
	selectArea.readyTimeout = 100;
	selectServer.timeout = 10;
	fullAlert.timeout = 5;
	loginAdv.timeout = 5;
	delayAlert.timeout = 5;
	inputVerify.timeout = 30;
	inputAccount.timeout = 30;
	inputWrong.timeout = 2;
	inputAccount.readyTimeout = 30;
	logined.timeout = 30;
	completeProcess.dm = logined.dm = inputWrong.dm = inputAccount.dm = inputVerify.dm = loginAdv.dm = delayAlert.dm = fullAlert.dm = selectServer.dm = selectArea.dm = waitUpdate.dm = &_dm;
	exitSelf.KeeperHandle = KeeperHandle;
	selectServer.serverPointInfo = &GetServerPoint(area);

	selectArea.CheckProcess.AddTail(&waitUpdate);
	waitUpdate.NextProcess.AddTail(&exitSelf);
	waitUpdate.NextProcess.AddTail(&selectArea);
	selectArea.NextProcess.AddTail(&selectServer);
	
	selectServer.NextProcess.AddTail(&inputAccount);
	
	selectServer.NextProcess.AddTail(&fullAlert);
	selectServer.NextProcess.AddTail(&delayAlert);
	
	fullAlert.NextProcess.AddTail(&delayAlert);
	fullAlert.NextProcess.AddTail(&inputAccount);
	
	delayAlert.NextProcess.AddTail(&inputAccount);
	loginAdv.NextProcess.AddTail(&inputAccount);
	inputAccount.CheckProcess.AddTail(&loginAdv);
	inputAccount.NextProcess.AddTail(&logined);
	inputAccount.NextProcess.AddTail(&inputVerify);
	inputAccount.NextProcess.AddTail(&inputWrong);
	inputVerify.NextProcess.AddTail(&logined);
	inputVerify.NextProcess.AddTail(&inputWrong);
	inputVerify.NextProcess.AddTail(&inputVerify);
	inputWrong.NextProcess.AddTail(&inputAccount);
	logined.CheckProcess.AddTail(&inputVerify);
	logined.NextProcess.AddTail(&completeProcess);
	AutoProcess *currentProcess = &selectArea;
	cout << qq << " 开始登录 " << area << endl;
	SendMessage(KeeperHandle, WM_USER + 102, NULL, NULL);
	while (currentProcess->NextProcess.GetCount() > 0){
		AutoProcess * oldProcess = currentProcess;
		int timeout = 0;
		while (!currentProcess->IsReady()){
			timeout++;
			if (timeout >= currentProcess->readyTimeout){
				cout << "超过"<<timeout<<"秒无响应" << typeid(*currentProcess).name() << endl;
				ExitProcess(1);
			}
			if (currentProcess->CheckProcess.GetCount() > 0){
				POSITION p = currentProcess->CheckProcess.GetHeadPosition();
				do{
					AutoProcess *next = currentProcess->CheckProcess.GetNext(p);
					if (next->IsReady()){
						currentProcess = next;
					}
				} while (p != NULL);
				if (oldProcess != currentProcess){
					break;
				}
			}
			Sleep(1000);
		}
		currentProcess->startTime = time(NULL);
		cout << "开始运行" << typeid(*currentProcess).name() << endl;
		currentProcess->Run(); currentProcess->tryTimes = 0;
		Sleep(1000);
		while (!currentProcess->IsComplete()){
			oldProcess = currentProcess;
			if (currentProcess->NextProcess.GetCount()>1){
				POSITION p = currentProcess->NextProcess.GetHeadPosition();
				AutoProcess *next = currentProcess->NextProcess.GetNext(p);
				 while (p != NULL){
					next = currentProcess->NextProcess.GetNext(p);
					if (next->IsReady()){
						currentProcess = next;
						cout << "开始运行" << typeid(*currentProcess).name() << endl;
						currentProcess->startTime = time(NULL);
						next->Run();
						break;
					}
					else{
						cout << "尝试使用步骤失败" << typeid(*next).name() << endl;
					}
				};
			}
			if (currentProcess == oldProcess && currentProcess->CheckTimeout()){
				cout << "侦测到超时" << typeid(*currentProcess).name() << endl;
				currentProcess->tryTimes++;
				if (currentProcess->tryTimes > 2){
					cout << "尝试多次失败" << typeid(*currentProcess).name() << endl;
					ExitProcess(1);
				}
				if (currentProcess->IsReady()){
					cout << "重新开始运行" << typeid(*currentProcess).name() << endl;
					currentProcess->startTime = time(NULL);
					currentProcess->ReRun();
				}
				else{
					currentProcess->startTime = time(NULL);
					cout << "等待" << endl;
				}	
			}
			Sleep(1000);
		}
		currentProcess = currentProcess->NextProcess.GetHead();
	}
}

AutoLogin::~AutoLogin()
{
	_dm.unbind_wnd();
	_serverLists.RemoveAll();
	_areaList.RemoveAll();
	_serverMap.RemoveAll();
}

ServerPointInfo AutoLogin::GetServerPoint(CString serverName){
	ServerPointInfo serverPointInfo;
	CString &area = _serverMap[serverName];
	CPoint k(188,242+100),k2(210,445+100);
	int areaIndex = 0;
	while (_areaList[areaIndex]!=area){
		areaIndex++;
	}
	int serverIndex = 0;
	while (_serverLists[areaIndex][serverIndex] != serverName){
		serverIndex++;
	}
	serverPointInfo.AreaPoint = CPoint(k.x + (areaIndex % 5) * 136, k.y + (areaIndex / 5)*45);
	serverPointInfo.ServerPoint = CPoint(k2.x + (serverIndex % 5 * 136), k2.y + (serverIndex/5)*45);
	return serverPointInfo;
}


void AutoLogin::BindDNFLoginWindow(){
	_dm._cdnf.GameHwnd = (long)_dm.wnd_hwnd;
	//long bind_ret=dm.BindWindowEx(winHwnd,"dx2","dx","dx","dx.public.memory",101);
	long bind_ret = _dm._dm.BindWindowEx(_dm._cdnf.GameHwnd, "normal", "normal", "normal", "", 0);
	//long bind_ret = _dm._dm.BindWindowEx(_dm._cdnf.GameHwnd, "dx2", "normal", "dx", "dx.public.memory", 0);
	if (1 == bind_ret)  //"normal","normal","normal","dx.public.memory",101
	{
		_dm._cdnf.Bind_ret = true;

		_dm._dm.DmGuard(1, "memory");
		//_dm._dm.MoveWindow(_dm._cdnf.GameHwnd, -10, 0);
		_dm._dm.FreeProcessMemory(_dm._cdnf.GameHwnd);
		//TR
	}
	else
	{
		long  err = _dm._dm.DmGetLastError();
		CString errStr;
		errStr.Format("绑定游戏窗口失败：错误号[%d]", err);
		_dm._cdnf.Bind_ret = false;

		MessageBox(_dm.wnd_hwnd, errStr, "绑定错误号", MB_OK);
	}

	_dm._dm.SetPicPwd("lh7812119");
	_dm._dm.SetDictPwd("lh7812119");

	//dm.SetShowErrorMsg(0);
	//获取窗口大小
	CRect rect;
	::GetClientRect(_dm.wnd_hwnd, &rect);
	_dm._cdnf.m_WinArea = rect;// CRect(0, 0,rect.Width()-16, rect.Height()-38);

	_dm._cdnf.m_WinLeftArea = CRect(0, 0, _dm._cdnf.m_WinArea.right / 2, _dm._cdnf.m_WinArea.bottom);
	_dm._cdnf.m_WinRightArea = CRect(_dm._cdnf.m_WinArea.right / 2, 0, _dm._cdnf.m_WinArea.right, _dm._cdnf.m_WinArea.bottom);

	_dm._cdnf.m_WinTopArea = _dm._cdnf.m_WinArea;
	_dm._cdnf.m_WinTopArea.bottom = _dm._cdnf.m_WinArea.bottom / 2;

	_dm._cdnf.m_WinBottomArea = _dm._cdnf.m_WinArea;
	_dm._cdnf.m_WinBottomArea.top = _dm._cdnf.m_WinArea.bottom / 2;

	_dm._cdnf.m_WinLeftTopArea = _dm._cdnf.m_WinLeftArea;
	_dm._cdnf.m_WinLeftTopArea.bottom = _dm._cdnf.m_WinLeftArea.bottom / 2;

	_dm._cdnf.m_WinLeftBottomArea = _dm._cdnf.m_WinLeftArea;
	_dm._cdnf.m_WinLeftBottomArea.top = _dm._cdnf.m_WinLeftArea.bottom / 2;

	_dm._cdnf.m_WinRightTopArea = _dm._cdnf.m_WinRightArea;
	_dm._cdnf.m_WinRightTopArea.bottom = _dm._cdnf.m_WinRightArea.bottom / 2;

	_dm._cdnf.m_WinRightBottomArea = _dm._cdnf.m_WinRightArea;
	_dm._cdnf.m_WinRightBottomArea.top = _dm._cdnf.m_WinRightArea.bottom / 2;
}