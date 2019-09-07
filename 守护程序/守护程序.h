#pragma once
#include "SendMail.h"
#include "resource.h"
using namespace std;
#define WM_MYMESSAGE_UNUSUAL WM_USER+100
#define WM_MYMESSAGE_COMPLETE WM_USER+101
#define WM_MYMESSAGE_PLUSE WM_USER+102
#define WM_MYMESSAGE_SAFEMODE WM_USER+103
#define WM_MYMESSAGE_THIRDPART WM_USER+104
#define WM_MYMESSAGE_FORBIDDEN WM_USER+105
#define WM_MYMESSAGE_BLOCK WM_USER+106
#define WM_MYMESSAGE_RESTARTWITHOUTLOGIN WM_USER+107
#define WM_MYMESSAGE_CHANGEROLE WM_USER+108
#define WM_MYMESSAGE_RESTART WM_USER+109
#define INRANGE(x,z,y) (x<y?(x<=z&&z<y):(x<=z||z<y))
#define GETNOW time_t now; time(&now);tm nowTime;localtime_s(&nowTime, &now);
#define GETTIME mktime(&nowTime)
#define RANDTIME(t) ((rand()%t<<1)-t)*60
//#define REST
/*
出安全模式的次数
出第三方的次数
关禁闭的次数
封号的次数
刷完了几个账号
数据异常
正在使用的列队号码
*/
struct LogInfo{
public:
	int SafeModeCount;
	int ThirdPartCount;
	int ForbiddenCount;
	int BlockCount;
	int CompleteCount;
	int UnusualCount;
	CString CurrentAccount;
	time_t CurrentTime;
	/*bool CheckNextTime(){
		time_t now = time(NULL);
		if (now > CurrentTime){
			CurrentTime = Next6();
			SafeModeCount = 0;
			ThirdPartCount = 0;
			ForbiddenCount = 0;
			BlockCount = 0;
			CompleteCount = 0;
			UnusualCount = 0;
			return true;
		}
		return false;
	}*/
} CurrentLogInfo;
struct NetCardInfo{
public:
	bool Connected;
	char Mac[18];
	NetCardInfo(){}
	NetCardInfo(bool c,char* mac){
		Connected = c;
		memcpy_s(Mac, 17, mac, 17);
		//strcpy_s(Mac,18, mac);
		Mac[17] = 0;
	}
	NetCardInfo(const NetCardInfo& copy)
	{
		Connected = copy.Connected;
		memcpy_s(Mac, 17, copy.Mac, 17);
		//strcpy_s(Mac, 18,copy.Mac);
		Mac[17] = 0;
	}
};
struct AccountInfo{
public:
	CString QQ;
	CString Password;
	CString Area;
	time_t CoolDown;
	AccountInfo(){}
	AccountInfo(CString qq, CString password, CString area) :QQ(qq), Password(password), Area(area){}
	AccountInfo(CString qq, CString password, CString area,time_t cooldown) :QQ(qq), Password(password), Area(area),CoolDown(cooldown){}
	AccountInfo(const AccountInfo &copy){
		QQ = copy.QQ; Password = copy.Password; Area = copy.Area; CoolDown = copy.CoolDown;
	}
};
CArray<AccountInfo> AccountList_Original;
CList<AccountInfo> AccountList;
CList<AccountInfo> UnusualAccountList;
HANDLE AutoProcessHandler;// , GameHandler;
HWND hWnd;
AccountInfo *currentAccount = NULL;
std::string autoLoginPath, gamePath, mainPathWithOutLogin;// , imeSwitch;
DWORD ExitCode = STILL_ACTIVE;
time_t lastPluse;
time_t timeout = 5 * 60;
long restartFlag;
unsigned short roleNum;
time_t restTime;
CArray<CPoint> gameTimeRange;
CString lastRestartMsg = "";
std::ofstream logOfStream("Log.txt", ios::out | std::ios::trunc);
CSendMail sender;
sMailInfo mailInfo;
int currentNetCardIndex;
CStringArray netCardList;
#ifdef REST
time_t NextGameTime(time_t after){
	tm aftertm;
	localtime_s(&aftertm,&after);
	for (int i = 0; i < gameTimeRange.GetCount(); i++){
		CPoint &p = gameTimeRange.GetAt(i);
		if (p.x > aftertm.tm_hour){
			aftertm.tm_hour = p.x;
			aftertm.tm_min = 0;
			aftertm.tm_sec = 0;
			return mktime(&aftertm) + RANDTIME(10);
		}
	}
	aftertm.tm_hour = gameTimeRange.GetAt(0).x;
	aftertm.tm_min = 0;
	aftertm.tm_sec = 0;
	return mktime(&aftertm)+24*60*60 + RANDTIME(10);
}
#endif
time_t Next6(){
	GETNOW
	if (nowTime.tm_hour >= 6){
		now += 24 * 60 * 60;
		localtime_s(&nowTime, &now);
	}
	nowTime.tm_hour = 6;
	nowTime.tm_min = 0;
	nowTime.tm_sec = 0;

	return GETTIME;
}
#ifdef REST
int IsInGameTime(){
	GETNOW
	for (int i = 0; i < gameTimeRange.GetCount(); i++){
		CPoint &p = gameTimeRange.GetAt(i);
		if (INRANGE(p.x, nowTime.tm_hour, p.y))return i;
	}
	return -1;
}
int IsInGameTime(time_t t){
	tm tt; localtime_s(&tt, &t);
		for (int i = 0; i < gameTimeRange.GetCount(); i++){
		CPoint &p = gameTimeRange.GetAt(i);
		if (INRANGE(p.x, tt.tm_hour, p.y))return i;
		}
	return -1;
}

bool IsInRestTime(){
	GETNOW
	return now>restTime&&now < currentAccount->CoolDown;
}
#endif
void Log(const char* log){
	if (!logOfStream.is_open()){
		logOfStream.open("Log.txt", ios::app);
	}
	time_t now = time(NULL);
	tm current_time;
	localtime_s(&current_time, &now);
	CString timeStr = "";
	timeStr.Format("[%d-%02d-%02d %02d:%02d:%02d]",
		current_time.tm_year + 1900,
		current_time.tm_mon + 1,
		current_time.tm_mday,
		current_time.tm_hour,
		current_time.tm_min,
		current_time.tm_sec);
	std::cout << timeStr << log << std::endl;
	logOfStream << timeStr << log << std::endl;
}
void Print(const char* log){
	time_t now = time(NULL);
	tm current_time;
	localtime_s(&current_time, &now);
	CString timeStr = "";
	timeStr.Format("[%d-%02d-%02d %02d:%02d:%02d]",
		current_time.tm_year + 1900,
		current_time.tm_mon + 1,
		current_time.tm_mday,
		current_time.tm_hour,
		current_time.tm_min,
		current_time.tm_sec);
	std::cout << timeStr << log << std::endl;
}
#ifdef REST
void SetRestTime(){
	CString l = "";
	GETNOW
		if (now < restTime)return;
	restTime = 0;
	for (int i = 0; i < gameTimeRange.GetCount(); i++){
		CPoint &p = gameTimeRange.GetAt(i);
		int nowHour = nowTime.tm_hour;
		if (INRANGE(p.x, nowHour, p.y)){
			l.Format("获取所在区间%d %d %d", p.x, nowHour, p.y);
			Log(l.GetBuffer());
			nowTime.tm_hour = p.y;
			nowTime.tm_min = 0;
			nowTime.tm_sec = 0;
			if (p.x > p.y && nowHour >= p.x){//在0点之前，需要加一天
				restTime = GETTIME + 60 * 60 * 24;
			}
			else{
				restTime = GETTIME;
			}
		}
	}
	localtime_s(&nowTime, &restTime);
	l.Format("下面的休息时间%d:%d:%d", nowTime.tm_hour, nowTime.tm_min, nowTime.tm_sec);
	Log(l.GetBuffer());
	if (restTime == 0)Log("设置结束时间错误！当前时间不在游戏时间！");
	else{
		restTime = restTime + RANDTIME(10);
		localtime_s(&nowTime,&restTime);
		l.Format("设置最终休息时间%d:%d:%d", nowTime.tm_hour,nowTime.tm_min,nowTime.tm_sec);
		Log(l.GetBuffer());
		//Log("设置休息时间完成");
	}
}
#endif
void AccountLog(const char *log){
	CString l = "";
	l.Format("%s:%s", currentAccount->QQ, log);
	Log(l.GetBuffer());
}
void ReadConfig(){
	Log("读取配置文件");
	std::ifstream fin("config.txt", std::ios::in);
	char line[1024] = { 0 };
	fin.getline(line, sizeof(line));
	gamePath = line;
	fin.getline(line, sizeof(line));
	autoLoginPath = line;
	fin.getline(line, sizeof(line));
	mainPathWithOutLogin = line;
	fin.getline(line, sizeof(line));
	std::stringstream word(line);
	std::string range;
	while (!word.eof()){
		word >> range;
		CPoint p;
		byte index = range.find('-');
		p.x = atoi(range.substr(0, index).c_str());
		p.y = atoi(range.substr(index + 1).c_str());
		gameTimeRange.Add(p);
	}
	/*fin.getline(line, sizeof(line));
	imeSwitch = line;*/
	fin.getline(line, sizeof(line));
	if (line[0]!='#')ShellExecute(NULL, NULL, "rasdial", line, NULL, NULL);
	fin.close();
}
void ReadAccountList(){
	char  line[512] = { 0 };
	int i = 0;
	std::string qq, password, server, cooldown;// , netCard;
	CStringList blockAccounts;
	Log("读取被封列表");
	std::ifstream finSafe("BlockAccounts.txt", std::ios::in);
	while (finSafe.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		word >> qq;
		blockAccounts.AddTail(CString(qq.c_str()));
	}
	finSafe.close();
	//CStringArray macs;
	Log("读取帐号列表");
	std::ifstream fin("Accounts.txt", std::ios::in);
	while (fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		word >> qq;
		word >> password;
		word >> server;
		//word >> netCard;
		AccountList_Original.Add(AccountInfo(qq.c_str(), password.c_str(), server.c_str()));
		//macs.Add(CString(netCard.c_str()));
	}
	fin.close();
	Log("恢复帐号队列");
	fstream _file;
	_file.open("_Accounts.txt", ios::in);
	if (!_file)
	{
		cout << "_Accounts.txt不存在，创建文件"<<endl;
		_file.open("_Accounts.txt", ios::out);
		for (i = 0; i < AccountList_Original.GetCount(); i++){
			AccountInfo  &ac = AccountList_Original.GetAt(i);
			ac.CoolDown = 0;
			_file << ac.QQ << " " << ac.Password << " " << ac.Area << " 0" << endl;
			AccountInfo newInfo(ac.QQ, ac.Password, ac.Area, 0);
			AccountList.AddTail(newInfo);
			//MacMap[macs.GetAt(i)] = &newInfo;
		}
		_file.close();
	}
	else
	{
		while (_file.getline(line, sizeof(line)))
		{
			std::stringstream word(line);
			word >> qq;
			word >> password;
			word >> server;
			word >> cooldown;
			POSITION block = blockAccounts.Find(qq.c_str());
			if (block != NULL){
				blockAccounts.RemoveAt(block);
			}
			else{
				AccountInfo newInfo(qq.c_str(), password.c_str(), server.c_str(), atol(cooldown.c_str()));
				AccountList.AddTail(newInfo);
			}
		}
		_file.close();
	}
	
}
void SaveUnusualAccountList(){
	ofstream fout("UnusualAccounts.txt", ios::out);
	POSITION pos = UnusualAccountList.GetHeadPosition();
	while (pos != NULL)
	{
		AccountInfo &item = UnusualAccountList.GetNext(pos);
		char timebuffer[64];
		_ltoa_s(item.CoolDown, timebuffer, 64, 10);
		fout << item.QQ << " " << item.Password << " " << item.Area << " " << timebuffer << std::endl;
	}
	fout.close();
}
void SetUnusualAccount(int flag){
	AccountInfo &account = AccountList.GetHead();
	account.CoolDown = time(NULL) + flag * 65 * 60;
	UnusualAccountList.AddTail(account);
	AccountList.RemoveHead();
	SaveUnusualAccountList();
}
bool CheckUnusualAccount(){
	if (UnusualAccountList.GetCount() == 0)return false;
	AccountInfo &account = UnusualAccountList.GetHead();
	if (account.CoolDown > time(NULL)){
		account.CoolDown = 0;
		AccountList.AddHead(account);
		UnusualAccountList.RemoveHead();
		SaveUnusualAccountList();
		return true;
	}
	return false;
}
void SaveCurrentAccountList(){
	Log("保存当前帐号队列");
	std::ofstream fout("_Accounts.txt", std::ios::out);
	POSITION pos = AccountList.GetHeadPosition();
	while (pos != NULL)
	{
		AccountInfo &item = AccountList.GetNext(pos);
		char timebuffer[64];
		_ltoa_s(item.CoolDown, timebuffer, 64, 10);
		fout << item.QQ << " " << item.Password << " " << item.Area << " " << timebuffer << std::endl;
	}
	fout.close();
	Log("保存成功！");
}
void ReadUnusualAccountList(){
	Log("读取异常帐号列表");
	std::ifstream fin("UnusualAccounts.txt", std::ios::in);
	char  line[512] = { 0 };
	std::string qq, password, server,cooldown;
	while (fin.getline(line, sizeof(line)))
	{
		std::stringstream word(line);
		word >> qq;
		word >> password;
		word >> server;
		word >> cooldown;
		AccountList.AddTail(AccountInfo(qq.c_str(), password.c_str(), server.c_str(),atol(cooldown.c_str())));
	}
	fin.close();
}
void LaunchGame(){
	Log("启动游戏");
	/*PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	bool fRet = CreateProcess(gamePath.c_str(), NULL, NULL, FALSE, NULL, CREATE_NEW_PROCESS_GROUP, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);*/
	ofstream tempBat("启动游戏.bat", ios::out | ios::trunc);
	tempBat << "start \"\" \"" << gamePath << "\"" << endl;
	tempBat.close();
	ShellExecute(NULL, "open", "启动游戏.bat", NULL, NULL, SW_HIDE);
	//ShellExecute(NULL, "open", "启动游戏.bat", NULL, NULL, SW_SHOW);
	//ShellExecute(NULL, "open", gamePath.c_str(), NULL, NULL, SW_SHOW);
	//GameHandler = pi.hProcess;
}
void LaunchAutoLogin(){
	roleNum = 0;
	Log("启动自动登录，先执行清理");
	SHELLEXECUTEINFO ShellInfo;
	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL;
	ShellInfo.lpVerb = _T("open");
	ShellInfo.lpFile = "新重启前清理一下.bat";
	ShellInfo.nShow = SW_SHOWNORMAL;
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&ShellInfo);
	WaitForSingleObject(ShellInfo.hProcess, INFINITE);
	if(currentAccount == NULL)
		currentAccount = &AccountList.GetHead();

	InvalidateRect(hWnd, NULL, true);
	/*if (GameHandler != NULL){
		TerminateProcess(GameHandler, 0);
		GameHandler = NULL;
	}*/
	LaunchGame();
	ofstream tempBat("tempBat.bat", ios::out|ios::trunc);
	
	tempBat << "start \"\" \"" << autoLoginPath << "\"" << " "<<dec << hWnd << " " << currentAccount->QQ << " " << currentAccount->Password << " " << currentAccount->Area << endl;
	tempBat.close();
	AutoProcessHandler = ShellExecute(NULL, "open", "tempBat.bat", NULL, NULL, SW_HIDE);
	SaveCurrentAccountList();
	lastPluse = time(NULL);
	return;
	CString commandLine = "";
	commandLine.Format("%ld %s %s %s", (long)hWnd, currentAccount->QQ, currentAccount->Password, currentAccount->Area);
	/*SHELLEXECUTEINFO ShellInfo;
	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);*/
	ShellInfo.hwnd = NULL;
	ShellInfo.lpVerb = "open";
	ShellInfo.lpFile = autoLoginPath.c_str();
	ShellInfo.lpParameters = commandLine.GetBuffer(0);
	ShellInfo.nShow = SW_SHOWNORMAL;
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&ShellInfo);
	AutoProcessHandler = ShellInfo.hProcess;
	SaveCurrentAccountList();
	lastPluse = time(NULL);
	//WaitForSingleObject(ShellInfo.hProcess, INFINITE);

	/*PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;	
	bool fRet = CreateProcess(autoLoginPath.c_str(), commandLine.GetBuffer(0), NULL, FALSE, NULL, NULL, NULL, NULL, &si, &pi);
	commandLine.ReleaseBuffer();
	AutoProcessHandler = pi.hProcess;*/
}
void RestartWithoutLogin(){
	AccountLog("非登录重启");
	/*TerminateProcess(AutoProcessHandler, 0);
	CloseHandle(AutoProcessHandler);*/
	SHELLEXECUTEINFO ShellInfo;
	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL;
	ShellInfo.lpVerb = "open";
	ShellInfo.lpFile = "taskkill";
	ShellInfo.lpParameters = "/F /IM dnf2* /T";
	ShellInfo.nShow = SW_SHOWNORMAL;
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&ShellInfo);
	WaitForSingleObject(ShellInfo.hProcess, INFINITE);
	ofstream tempBat("tempBat.bat", ios::out | ios::trunc);
	tempBat << "start \"\" \"" << mainPathWithOutLogin << "\"" << " "<<dec << hWnd << endl;
	tempBat.close();
	AutoProcessHandler = ShellExecute(NULL, "open", "tempBat.bat", NULL, NULL, SW_HIDE);

	/*SHELLEXECUTEINFO ShellInfo;
	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL;
	ShellInfo.lpVerb = "open";
	ShellInfo.lpFile = mainPathWithOutLogin.c_str();
	ShellInfo.lpParameters = NULL;
	ShellInfo.nShow = SW_SHOWNORMAL;
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&ShellInfo);
	AutoProcessHandler = ShellInfo.hProcess;*/
	lastPluse = time(NULL);
}
void KillAutoLogin(){
	Log("Kill自动登录");
	restartFlag = 1;
	SHELLEXECUTEINFO ShellInfo;
	memset(&ShellInfo, 0, sizeof(ShellInfo));
    ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL;
	ShellInfo.lpVerb = "open";
	ShellInfo.lpFile = "taskkill";
	ShellInfo.lpParameters = "/F /IM dnf* /T";
	ShellInfo.nShow = SW_SHOWNORMAL;
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&ShellInfo);
	WaitForSingleObject(ShellInfo.hProcess, INFINITE);
	/*ShellInfo.lpParameters = "/F /IM dnf2.exe /T";
	ShellExecuteEx(&ShellInfo);
	WaitForSingleObject(ShellInfo.hProcess, INFINITE);*/
	/*TerminateProcess(AutoProcessHandler, 0);
	CloseHandle(AutoProcessHandler);*/
	AutoProcessHandler = NULL;
	/*if (GameHandler != NULL){
		TerminateProcess(GameHandler, 0);
		CloseHandle(GameHandler);
		GameHandler = NULL;
	}*/
}
void SetNextAccount(){
	AccountList.AddTail(AccountList.RemoveHead());
	currentAccount = &AccountList.GetHead();
	
}
void restart(){
	switch (restartFlag){
	case 0:
	case 1:
		KillAutoLogin();
		LaunchAutoLogin();
		break;
	case 2:
		RestartWithoutLogin();
		break;
	}
}
bool tryLaunchAutoLogin(){
	if (AutoProcessHandler != NULL){
		KillAutoLogin();
	}
	
	if (currentAccount->CoolDown > time(NULL)){
		if (currentAccount->CoolDown == INT64_MAX){
			Log("该帐号被手动关闭，需要手动启动，自动启动无效");
		}else{
			Log("该帐号已经刷完。");
		}
		return false;
	}
#ifdef REST
	if (!IsInRestTime()){
#endif
		Log("尝试重新启动");
		LaunchAutoLogin();

		return true;
#ifdef REST
	}
	else{
		Log("在休息时间，重启无效");
	}
#endif
	return false;
}

void SaveBlockAccount(CString account){
	std::ofstream fout("BlockAccounts.txt", std::ios::app);
	fout << account << std::endl;
	fout.close();
}
void SaveSafeModeAccount(CString account){
	std::ofstream fout("SafeModeAccounts.txt", std::ios::app);
	fout << account << std::endl;
	fout.close();
}
CArray<NetCardInfo>& GetNetCardInfo()
{
#define EXECDOSCMD "ipconfig /all" //可以换成你的命令
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;
	CArray<NetCardInfo> netCards;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		return netCards;
	}
	//char command[1024];    //长达1K的命令行，够用了吧
	//strcpy(command, "Cmd.exe /C ");
	//strcat(command, EXECDOSCMD);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWrite;            //把创建进程的标准错误输出重定向到管道输入
	si.hStdOutput = hWrite;           //把创建进程的标准输出重定向到管道输入
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	//关键步骤，CreateProcess函数参数意义请查阅MSDN
	if (!CreateProcess(NULL, "ipconfig /all", NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		CloseHandle(hWrite);
		CloseHandle(hRead);
		return netCards;
	}
	CloseHandle(hWrite);
	Sleep(2000);
	char buffer[65535] = { 0 };          //用4K的空间来存储输出的内容，只要不是显示文件内容，一般情况下是够用了。
	DWORD bytesRead = 0;
	DWORD pos = 0;
	while (true)
	{
		if (ReadFile(hRead, buffer + pos, 4095, &bytesRead, NULL) == NULL)
			break;
		pos += bytesRead;
	}
	fstream netcard;
	netcard.open("netcard.txt", ios::in);
	if (!netcard){
		netcard.open("netcard.txt", ios::out);
	}
	else{
		char line[20];
		while (netcard.getline(line, sizeof(line)))
		{
			line[17] = 0;
			netCardList.Add(CString(line));
		}
		netcard.close();
		netcard.open("netcard.txt", ios::out|ios::app);
	}
	pos = 0; bytesRead = 0;
	char * substr = strstr(buffer, "以太网适配器");
	while (substr != NULL){
		char *substr2 = strstr(substr, "物理地址");
		*(substr2 - 1) = 0;
		substr2 = strstr(substr2, " : ");
		NetCardInfo newCardInfo(strstr(substr, "媒体已断开") == NULL, substr2 + 3);
		netCards.Add(newCardInfo);
		cout << "网卡：" << newCardInfo.Mac << (newCardInfo.Connected ? "已联网" : "") << endl;
		POSITION AccountListPOS = AccountList.GetHeadPosition();
		for (pos = 0; pos < netCardList.GetCount(); pos++){
			if (netCardList.GetAt(pos) == CString(newCardInfo.Mac)){
				currentAccount = &AccountList.GetAt(AccountListPOS);
			}
			AccountList.GetNext(AccountListPOS);
		}
		if (currentAccount == NULL){
			netcard << newCardInfo.Mac << endl;
			currentAccount = &AccountList.GetAt(AccountListPOS);
		}
		//if (newCardInfo.Connected)currentNetCardIndex = pos++;
		substr = strstr(substr2, "以太网适配器");
		break;
	}
	netcard.close();
	CloseHandle(hRead);
	return netCards;
}
