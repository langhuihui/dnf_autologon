#pragma once  
#include "stdafx.h"  
struct sMailInfo //�ʼ���Ϣ  
{
	char*   m_pcUserName;//�û���¼���������  
	char*   m_pcUserPassWord;//�û���¼���������  
	char*   m_pcSenderName;//�û�����ʱ��ʾ������  
	char*   m_pcSender;//�����ߵ������ַ  
	char*   m_pcReceiver;//�����ߵ������ַ  
	char*   m_pcTitle;//�������  
	char*   m_pcBody;//�ʼ��ı�����  
	char*   m_pcIPAddr;//��������IP  
	char*   m_pcIPName;//�����������ƣ�IP�����ƶ�ѡһ������ȡ���ƣ�  
	sMailInfo(){ memset(this, 0, sizeof(sMailInfo)); }
};
class CSendMail
{
public:
	CSendMail(void);
	~CSendMail(void);
public:
	bool SendMail(sMailInfo &smailInfo);//�����ʼ�����Ҫ�ڷ��͵�ʱ���ʼ���ʼ���Ϣ  
	void AddFilePath(char * pcFilePath);//��Ӹ����ľ���·���������б���  
	void DeleteFilePath(char* pcFilePath);//ɾ������·��������еĻ�  
	void DeleteAllPath(void);//ɾ��ȫ��������·��  
	
protected:
	void GetFileName(char* fileName, char* filePath);//�Ӹ�����·���л�ȡ�ļ�����  
	void Char2Base64(char* pBuff64, char* pSrcBuff, int iLen);//��char����ת����Base64����  
	bool CReateSocket(SOCKET &sock);//����socket����  
	bool Logon(SOCKET &sock);//��¼���䣬��Ҫ���з��ʼ�ǰ��׼������  
	//���ļ�·����ȡ��������  
	int GetFileData(char* FilePath);
	bool SendHead(SOCKET &sock);//�����ʼ�ͷ  
	bool SendTextBody(SOCKET &sock);//�����ʼ��ı�����  
	bool SendFileBody(SOCKET &sock);//�����ʼ�����  
	bool SendEnd(SOCKET &sock);//�����ʼ���β  
protected:
	CList<char*, char*> m_pcFilePathList;//��¼����·��  

	char  m_cSendBuff[4096];//���ͻ�����  
	char  m_cReceiveBuff[1024];
	char* m_pcFileBuff;//ָ�򸽼�����  
	sMailInfo m_sMailInfo;
};