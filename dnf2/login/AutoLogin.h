#pragma once
#include <string>
#include <afxwin.h>
#include <dm.h>
#include <Afxtempl.h>
#include "CrackCaptchaAPI.h"
#include <iostream>
#pragma comment(lib, "CrackCaptchaAPI.lib")

#include "login_base.h"

_DNF_BEGIN
	using namespace _DNF;

struct ServerPointInfo{
public:
	//��������
	CPoint AreaPoint;
	//����������
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
class _DllExport AutoLogin
{
public:
	AutoLogin(Dm &dm);
	~AutoLogin();
	Dm &_dm;
	void Login(char * qq, char *password, char * area);
	HWND KeeperHandle;
private:
	//��������Ӧ����ӳ��
	CMapStringToString _serverMap;
	//�����б�
	CStringArray _areaList;
	//�������б�
	 CArray<ServerListArray> _serverLists;
	//��ȡ��������Ӧ������
	ServerPointInfo GetServerPoint(CString serverName);
	void BindDNFLoginWindow();
};

_DNF_END
