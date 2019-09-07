#pragma once
#include <string>
#include <afxwin.h>
#include "dm.h"
#include <Afxtempl.h>
#include "CrackCaptchaAPI.h"
#include <iostream>
#pragma comment(lib, "CrackCaptchaAPI.lib")
#define _DNF_BEGIN
#define _DNF_END
struct ServerPointInfo{
public:
	//大区坐标
	CPoint AreaPoint;
	//服务器坐标
	CPoint ServerPoint;
};
class ServerListArray :public CStringArray{
public:
	void ServerListArray::operator = (const ServerListArray &rhs)
	{
		RemoveAll();
		Append(rhs);
	}
};
class AutoLogin
{
public:
	AutoLogin(Dm &dm);
	~AutoLogin();
	Dm &_dm;
	void Login(char * qq, char *password, char * area);
	HWND KeeperHandle;
private:
	//服务器对应大区映射
	CMapStringToString _serverMap;
	//大区列表
	CStringArray _areaList;
	//服务器列表
	 CArray<ServerListArray> _serverLists;
	//获取服务器对应的坐标
	ServerPointInfo GetServerPoint(CString serverName);
	void BindDNFLoginWindow();
};

