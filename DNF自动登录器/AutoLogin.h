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
class AutoLogin
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

