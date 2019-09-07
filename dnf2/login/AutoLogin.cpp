
#include "AutoLogin.h"
#include <fstream>  
#include "AutoProcess.h"

_DNF_BEGIN
LoginLog LoginLog::Instance = LoginLog();
ofstream LoginLog::LogFile;

AutoLogin::AutoLogin(Dm&dm) 
:_dm(dm)
{
	LoginLog::LogFile.open("login.log", ios::out);
	std::ifstream fin("ServerMap.txt", std::ios::in);
	char  line[1024] = { 0 };
	string area,server;
	while (fin.getline(line, sizeof(line)))
	{
		std::stringstream  word(line);
		word >> area;
		Log("��ȡ��������:" << area.c_str())
		ServerListArray servers;
		while (!word.eof()){
			word >> server;
			servers.Add(server.c_str());
			_serverMap.SetAt(server.c_str(), area.c_str());
			Log(server.c_str())
		}
		LogLine("")
		_serverLists.Add(servers);
		_areaList.Add(area.c_str());
	}
	LogLine("��ȡ������ӳ���ļ����")
}
void AutoLogin::Login(char * qq,char *password,char * area){
	long ret = _dm.reg();
	int regCount = 0;
	while (ret != 1){
		regCount++;
		if (regCount > 5){
			LogLine("ע�᳢�Դ�������5�Σ�5����˳�")
			Sleep(5000);
			ExitProcess(-1);
			return;
		}
		LogLine("ע��ʧ�ܣ�" << ret)
		Sleep(2000);
		ret = _dm.reg();
	}
	_dm.set_wnd_name("���³�����ʿ��¼����");
	_dm.find_wnd();
	while (_dm.wnd_hwnd == NULL){
		LogLine("δ��⵽��Ϸ���򴰿ڣ�2����ٴ���⣡")
		Sleep(2000);
		_dm.find_wnd();
	}
	LogLine("��⵽��Ϸ���򴰿ڣ���ʼ�󶨴��ڣ�")
	BindDNFLoginWindow();
	_dm._cdnf.ActivateWin((long)_dm.wnd_hwnd);
	Log("��ȡ�������ʺ�")
	std::ifstream fin("dama2Account.txt", std::ios::in);
	char  line[256] = { 0 };
	fin.getline(line, sizeof(line));
	std::stringstream  word(line);
	LogLine ( "���")
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
	LogLine("��ʼ��״̬��")
	waitUpdate.timeout = -1;
	const size_t times = 3;
	selectArea.timeout = 10 * times;
	selectArea.readyTimeout = 100 * times;
	selectServer.timeout = 20 * times;
	fullAlert.timeout = 5 * times;
	loginAdv.timeout = 5 * times;
	delayAlert.timeout = 5 * times;
	selectServer.timeout = 5 * times;
	inputVerify.timeout = 2 * times;
	inputAccount.timeout = 2 * times;
	inputWrong.timeout = 2 * times;
	logined.timeout = 60*times;
	inputAccount.readyTimeout = 30 * times;
	completeProcess.dm =logined.dm= inputWrong.dm = inputAccount.dm = inputVerify.dm = loginAdv.dm = delayAlert.dm = fullAlert.dm = selectServer.dm = selectArea.dm = waitUpdate.dm= &_dm;
	exitSelf.KeeperHandle = KeeperHandle;
	selectServer.serverPointInfo = &GetServerPoint(area);

	selectArea.CheckProcess.AddTail(&waitUpdate);
	waitUpdate.NextProcess.AddTail(&exitSelf);
	waitUpdate.NextProcess.AddTail(&selectArea);
	selectArea.NextProcess.AddTail(&selectServer);
	
	selectServer.NextProcess.AddTail(&inputAccount);
	
	selectServer.NextProcess.AddTail(&fullAlert);
	selectServer.NextProcess.AddTail(&delayAlert);
	selectServer.NextProcess.AddTail(&loginAdv);
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
	LogLine ( qq << " ��ʼ��¼ " << area )
	//SendMessage(KeeperHandle, WM_USER + 102, NULL, NULL);
	while (currentProcess->NextProcess.GetCount() > 0){
		AutoProcess * oldProcess = currentProcess;
		int timeout = 0;
		while (!currentProcess->IsReady()){
			timeout++;
			if (timeout >= currentProcess->readyTimeout){
				LogLine("����" << timeout << "������Ӧ" << typeid(*currentProcess).name() )
				//ExitProcess(1);
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
		LogLine ("��ʼ����" << typeid(*currentProcess).name() )
		currentProcess->Run();currentProcess->tryTimes=0;
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
						LogLine("��ʼ����" << typeid(*currentProcess).name())
						currentProcess->startTime = time(NULL);
						next->Run();
						break;
					}
					else{
						LogLine("����ʹ�ò���ʧ��" << typeid(*next).name() )
					}
				};
			}
			if (currentProcess == oldProcess && currentProcess->CheckTimeout()){
				LogLine( "��⵽��ʱ" << typeid(*currentProcess).name() )
				currentProcess->tryTimes++;
				if (currentProcess->tryTimes > 2){
					LogLine("���Զ��ʧ��" << typeid(*currentProcess).name())
					//ExitProcess(1);
				}
				if (currentProcess->IsReady()){
					LogLine("���¿�ʼ����" << typeid(*currentProcess).name())
					currentProcess->startTime = time(NULL);
					currentProcess->ReRun();
				}
				else{
					currentProcess->startTime = time(NULL);
					LogLine("�ȴ�" )
				}	
			}
			Sleep(1000);
		}
		currentProcess = currentProcess->NextProcess.GetHead();
	}
	LoginLog::LogFile.close();
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
		errStr.Format("����Ϸ����ʧ�ܣ������[%d]", err);
		_dm._cdnf.Bind_ret = false;

		MessageBox(_dm.wnd_hwnd, errStr, "�󶨴����", MB_OK);
	}

	_dm._dm.SetPicPwd("lh7812119");
	_dm._dm.SetDictPwd("lh7812119");

	//dm.SetShowErrorMsg(0);
	//��ȡ���ڴ�С
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

_DNF_END