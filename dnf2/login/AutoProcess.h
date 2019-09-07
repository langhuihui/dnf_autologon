#pragma once
_DNF_BEGIN

class LoginLog{
public :
	static ofstream LogFile;
	static LoginLog Instance;
};
#define Log(x) cout<<x;LoginLog::LogFile << x ;
#define LogLine(x) cout << x << endl;LoginLog::LogFile << x << endl;
class AutoProcess
{
public:
	time_t timeout;
	time_t readyTimeout;
	time_t startTime;
	int tryTimes ;
	Dm *dm;
	CList<AutoProcess*, AutoProcess*> CheckProcess;
	CList<AutoProcess*,AutoProcess*> NextProcess;
public:
	virtual bool IsReady() = 0;
	virtual bool IsComplete(){
		return NextProcess.GetHead()->IsReady();
	}
	virtual void Run() = 0;
	virtual void ReRun(){
		Run();
	}
	AutoProcess()
	{
		
	}
	~AutoProcess()
	{
	}
	bool CheckTimeout(){
		if (timeout == -1)return false;
		if (time(NULL) - startTime > timeout){
			return true;
		}
		return false;
	}
protected:
	VARIANT intX, intY;
	bool FindPicture(const char * path){
		return dm->_dm.FindPic(0, 0, dm->_cdnf.m_WinArea.right, dm->_cdnf.m_WinArea.bottom, path, "000000", 0.9, 0, &intX, &intY) != -1;
	}
	void MoveClick(int x, int y){
		dm->_dm.MoveTo(x - 2, y - 2);
		Sleep(100);
		dm->_dm.MoveR(2, 2);
		Sleep(100);
		dm->_dm.LeftClick();
	}
	void MouseLeftClick(int x, int y){
		dm->_dm.MoveTo(x, y);
		dm->_dm.LeftClick();
	}
	void MouseLeftClick(CPoint &p){
		MouseLeftClick(p.x, p.y);
	}
};
//等待自动更新
class WaitUpdate :public AutoProcess{
public:
	bool IsReady(){
		return FindPicture("download.bmp");
	}
	void Run(){
		//do nothing just wait;
	}
};
//选择大区
class SelectArea :public AutoProcess{
public:
	
	bool IsReady(){
		if (FindPicture("connecting.bmp")){
			return false;
		}
		if (FindPicture("reselectbn.bmp")){
			return true;
		}
		return false;
	}
	
	void Run(){
		Sleep(1000);
		MouseLeftClick(307, 536+100);
	}
};
//选择服务器
class SelectServer :public  AutoProcess{
public:
	ServerPointInfo *serverPointInfo;
	bool IsReady(){
		return FindPicture("chooseArea.bmp");
	}
	bool IsComplete(){
		return FindPicture("jxtp.bmp");
	}
	void Run(){
		MouseLeftClick(serverPointInfo->AreaPoint);
		Sleep(1000);
		MouseLeftClick(serverPointInfo->ServerPoint);
		Sleep(1000);
		while (FindPicture("guangdong.bmp")){
			LogLine( "发现选错大区！重新选择")
			MouseLeftClick(serverPointInfo->AreaPoint);
			Sleep(1000);
			MouseLeftClick(serverPointInfo->ServerPoint);
			Sleep(1000);
		}
		MouseLeftClick(982, 527 + 100);
	}
};
//输入帐号
class InputAccount :public  AutoProcess{
public:
	const char* username;
	const char* password;
	bool IsReady(){
		return dm->_dm.GetColor(532, 444+100) == "192036" && !FindPicture("verifyWindow.bmp");
	}
	void Run(){
		Sleep(1000);
		MoveClick(477, 420 + 100);
		Sleep(500);
		MoveClick(477, 420 + 100);
		Sleep(500);
		for (int i = 0; i < 15; i++){
			dm->_dm.KeyPress(46);
			Sleep(20);
			dm->_dm.KeyPress(8);
			Sleep(20);
		}
		dm->_dm.KeyPressStr(username, 20);
		Sleep(500);
		MoveClick(477, 450 + 100);
		Sleep(500);
		MoveClick(477, 450 + 100);
		Sleep(500);
		dm->_dm.KeyPressStr(password, 20);
		Sleep(500);
		MoveClick(750, 430 + 100);
	}
	void ReRun(){
		Sleep(1000);
		MoveClick(477, 420 + 100);
		Sleep(500);
		MoveClick(477, 420 + 100);
		Sleep(500);
		for (int i = 0; i < 15; i++){
			dm->_dm.KeyPress(46);
			Sleep(20);
			dm->_dm.KeyPress(8);
			Sleep(20);
		}
		dm->_dm.KeyPressStr(username, 20);
		Sleep(500);
		MoveClick(477, 450 + 100);
		Sleep(500);
		MoveClick(477, 450 + 100);
		Sleep(500);
		for (int i = 0; i < 15; i++){
			dm->_dm.KeyPress(46);
			Sleep(20);
			dm->_dm.KeyPress(8);
			Sleep(20);
		}
		dm->_dm.KeyPressStr(password, 20);
		Sleep(500);
		MoveClick(750, 430 + 100);
	}
};
class FullAlert :public AutoProcess{
public:
	bool IsReady(){
		return FindPicture("fullAlert.bmp");
	}
	void Run(){
		MouseLeftClick(538, 366 + 90);
	}
};
class DelayAlert :public AutoProcess{
public:
	bool IsReady(){
		return FindPicture("delayAlert.bmp");
	}
	void Run(){
		MouseLeftClick(541 , 365 + 90);
	}
};
class LoginAdv :public AutoProcess{
public:
	bool IsReady(){
		return FindWindow(NULL, "DNF视频播放器")!=NULL;
		//return FindPicture("ad.bmp");
	}
	void Run(){
		SendMessage(FindWindow(NULL, "DNF视频播放器"), WM_CLOSE, 0, 0);
		//MouseLeftClick(897 , 97 +90);
	}
};
//输入验证码
class InputVerify :public  AutoProcess{
public:
	char *username;
	char *password;
	bool IsReady(){
		return FindPicture("verifyWindow.bmp");
		//return dm->_dm.GetColor(550, 316) == "ffffff";
	}
	void Run(){
		char buffer[32];
		dm->_dm.Capture(424, 297+100, 553, 350+100, "v.bmp");
		int ret = D2File("387437d579612e97f9257db36be579ea", username, password, "v.bmp", 30, 57342, buffer);
		LogLine("打码兔识别返回值：" << ret )
		dm->_dm.KeyPressStr(buffer, 20);
		MouseLeftClick(602 , 377 +90);
		Sleep(2000);
	}
};
class InputWrong :public AutoProcess{
public:
	bool IsReady(){
		return FindPicture("accError.bmp") && !FindPicture("verifyWindow.bmp");
	}
	void Run(){
		MouseLeftClick(596 , 364 +90);
	}
};
class Logined :public AutoProcess{
public:
	bool IsReady(){
		return !FindPicture("jxtp.bmp");
	}
	void Run(){
		//just waiting
	}
};
class CompleteProcess :public AutoProcess{
public:
	bool IsReady(){
		return FindWindow(NULL, "地下城与勇士登录程序") == NULL;
	}
	void Run(){

	}
};
class ExitSelfProcess :public AutoProcess{
	
public:
	HWND KeeperHandle;
	bool IsReady(){
		SendMessage(KeeperHandle, WM_USER + 102, NULL, NULL);
		return FindWindow(NULL, "地下城与勇士登录程序") == NULL;
	}
	void Run(){
		ExitProcess(0);
	}
};
_DNF_END