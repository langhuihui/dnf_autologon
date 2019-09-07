#include "stdafx.h"
#include "ksreglib.h"
//
CString v_inipath;
_int32  v_softcode;         //����������������
CString v_softkey;          //���������������Կ
_int32  v_softversion;      //������������ǰ����汾
CString v_bdinfo;           //�����������û�����Ϣ
_int32  v_isipcmode;
//
CString  signData;
_int32 hwndfun;
bool myISRunning = false;
//
const CHAR USER32DLL[] = __TEXT("USER32.dll");     //���Ը�����������ŵ������Ŀ¼ �� ϵͳ�ļ���system32Ŀ¼
const CHAR KERNEL32DLL[] = __TEXT("Kernel32.dll");   //���Ը�����������ŵ������Ŀ¼ �� ϵͳ�ļ���system32Ŀ¼
const CHAR KSREGDLL[] = __TEXT("kssPlugin.dll");   //���Ը�����������ŵ������Ŀ¼ �� ϵͳ�ļ���system32Ŀ¼
const int MaxStringBufferSize = 1024; //�ַ�����������С -- �����Ļ����ͼӴ�û�±����
//
HINSTANCE hinUSER32DLL = NULL;
HINSTANCE hinKERNEL32DLL = NULL;
HINSTANCE hinKSREGDLL = NULL;
//
PMessageBoxTimeout MessageBoxTimeout;
PGetPrivateProfileString_ GetPrivateProfileString_;
PWritePrivateProfileString_ WritePrivateProfileString_;
//
pks_setLicense ks_setLicense;
pks_setExtVal ks_setExtVal;
pks_setSoftVal ks_setSoftVal;
pks_setUserVal ks_setUserVal;
pks_setUpVal ks_setUpVal;
pks_IdCheck ks_IdCheck;
pks_advapifree ks_advapifree;
pks_advapi ks_advapi;
pks_GetData ks_GetData;
pks_GetMsg ks_GetMsg;
pks_unbind ks_unbind;
pks_reguser ks_reguser;
pks_prepaid ks_prepaid;
pks_edit ks_edit;
pks_viewinfo ks_viewinfo;
pks_exit ks_exit;
pIPC_Start IPC_Start;
pIPC_Connect IPC_Connect;
pIPC_IdCheck IPC_IdCheck;
pIPC_GetData IPC_GetData;
pIPC_advapi IPC_advapi;
pIPC_advapifree IPC_advapifree;
pdecode_rsa decode_rsa;
pdecode_RC4Base64 decode_RC4Base64;
//
bool iserrno(CString s_txt)
{
	if((s_txt.GetLength() == 6) && (s_txt.Left(3) == __TEXT("eno")))
		return true;
	else
		return false;
}

//�������������Լ�����һ��
void chkPass(_int32 ctype)
{
	srand((unsigned)time(NULL));
	_int32 i = (int)(49 * rand() / (RAND_MAX + 1.0));
	CString v_msgstr = KS_SERVERCLIENTFLAG_IdCheck((LPSTR)(LPCTSTR)(signData.Mid(i * 8, 8)), ctype);
    if(iserrno(v_msgstr))
    {
          MsgBox(ks_GetMsg((LPSTR)(LPCTSTR)v_msgstr));
		  myExitProcess();
    }
	//
    if((signData.Mid(i * 8 + 400, 8)) != v_msgstr)
    {
		v_msgstr += __TEXT(",");
		v_msgstr +=
		v_msgstr += signData.Mid(i * 8 + 400, 8);
        w_ini(__TEXT("signdata"), __TEXT("err"), (LPCTSTR)v_msgstr);
		myExitProcess();
    }
}

CString advapi(CString v_text)
{
    CString result = KS_SERVERCLIENTFLAG_advapi((LPSTR)(LPCTSTR)v_text);
    if(iserrno(result))            MsgBox(ks_GetMsg((LPSTR)(LPSTR)(LPCTSTR)result));
    if(result.Left(3) == __TEXT("err"))    MsgBox(result);
	return result;
}

void MsgBox(CString v_text)
{
    MessageBoxTimeout(0, (LPSTR)(LPCTSTR)v_text, __TEXT("ϵͳ��ʾ"), 64, 0, 10000);
}

//���뻻���*��*Сʱ*��*��ĸ�ʽ
CString mattime(_int32 cday)
{
	CString uday;
	uday.Format(__TEXT("%d��%dСʱ%d��%d��"), (_int32)(cday / 86400), (_int32)(cday % 86400 / 3600), (_int32)(cday % 86400 % 3600 / 60), (cday % 86400 % 3600 % 60));
    return uday;
}

//�۵㺯��������advapi�ӿ�ʵ�ֿ۵㹦�ܡ��ɹ�����ʣ��ĵ�����ʧ�ܷ���-1
_int32 kpoints(_int32 v_points, CString *v_errinfo) //ע��������CStringָ��
{
    _int32 c;
    CString tresult;

	CString s;
	s.Format(__TEXT("v_points,%d"), v_points);
    tresult = advapi((LPCTSTR)s);
    c = c_int(tresult);
    if(c == 0)    //����ֵ����������Ϊ0��˵���۵�ʧ��
    {
        if(iserrno(tresult))
            *v_errinfo = ks_GetMsg((LPSTR)(LPCTSTR)tresult);
        else
            *v_errinfo = tresult;
		return -1;
    }
    *v_errinfo = __TEXT("�۵�ɹ�");
    return c;
}

//ת��������ת��ʧ�ܷ���0
_int32 c_int(CString v_str)
{
    CString s = __TEXT("0123456789");
	for(_int32 i = 0; i < v_str.GetLength(); ++i)
    {
        if(s.Find(v_str.Mid(i, 1)) == -1)
        {
            return 0;
        }
    }
	return _ttoi((LPCTSTR)v_str);
}

//��ini�ļ�  �ڵ��� ����
CString r_ini(CString iarea, CString iKeyName)
{
    CString s;
	LPSTR p = s.GetBuffer(1024);
	memset(p, 0, 1024);
    GetPrivateProfileString_((LPSTR)(LPCTSTR)iarea, (LPSTR)(LPCTSTR)iKeyName, __TEXT(""), (LPSTR)(LPCTSTR)s, 1024, (LPSTR)(LPCTSTR)v_inipath);
	s.ReleaseBuffer();
	s.Replace(__TEXT("\x0"), __TEXT("")); //�滻�հ��ַ�Ϊ�մ�
	s.TrimLeft();
	s.TrimRight();
    return s;
}

//дini�ļ� �ڵ��� ���� ��ֵ
void w_ini(CString iarea, CString iKeyName, CString iKeyValue)
{
    WritePrivateProfileString_((LPSTR)(LPCTSTR)iarea, (LPSTR)(LPCTSTR)iKeyName, (LPSTR)(LPCTSTR)iKeyValue, (LPSTR)(LPCTSTR)v_inipath);
}


void myExitProcess()
{
	if(myISRunning) ks_exit();
	ExitProcess(0);
}

void * ksSeekFunction(HINSTANCE aDll, LPSTR aFunName)
{
	void *p = (void*)GetProcAddress(aDll, aFunName);
	if(p == NULL)
	{
		CString msg;
		if(aDll == hinUSER32DLL)
			msg.Format(__TEXT("�Ҳ�����̬���ӿ�%s��ĺ���%s"), USER32DLL, aFunName);
		if(aDll == hinKSREGDLL)
			msg.Format(__TEXT("�Ҳ�����̬���ӿ�%s��ĺ���%s"), KSREGDLL, aFunName);
		if(aDll == hinKERNEL32DLL)
			msg.Format(__TEXT("�Ҳ�����̬���ӿ�%s��ĺ���%s"), KERNEL32DLL, aFunName);
		MsgBox(msg);
		myExitProcess();
		return NULL;
	}
	return p;
}

void ksLoadDLLs() //����������ص�DLL
{
	CString msg;
    //����USER32.DLL
	hinUSER32DLL = LoadLibrary(USER32DLL);
	if( hinUSER32DLL == NULL)
	{
		msg.Format(__TEXT("�Ҳ�����̬���ӿ⣺%s"), USER32DLL);
		MsgBox(msg);
		myExitProcess();
	}
	//ȡ�ú�����ַ
	MessageBoxTimeout = (PMessageBoxTimeout)ksSeekFunction(hinUSER32DLL, __TEXT("MessageBoxTimeoutA"));
    //����KERNEL32.DLL
	hinKERNEL32DLL = LoadLibrary(KERNEL32DLL);
	if( hinKERNEL32DLL == NULL)
	{
		msg.Format(__TEXT("�Ҳ�����̬���ӿ⣺%s"), KERNEL32DLL);
		MsgBox(msg);
		myExitProcess();
	}
	//ȡ�ú�����ַ
	GetPrivateProfileString_ = (PGetPrivateProfileString_)ksSeekFunction(hinKERNEL32DLL, __TEXT("GetPrivateProfileStringA"));
	WritePrivateProfileString_ = (PWritePrivateProfileString_)ksSeekFunction(hinKERNEL32DLL, __TEXT("WritePrivateProfileStringA"));
	//����KSDLL.DLL
	hinKSREGDLL = LoadLibrary(KSREGDLL);
	if( hinKSREGDLL == NULL)
	{
		msg.Format(__TEXT("�Ҳ�����̬���ӿ⣺%s"), KSREGDLL);
		MsgBox(msg);
		myExitProcess();
	}
	//ȡ�ú�����ַ
	ks_setLicense = (pks_setLicense)ksSeekFunction(hinKSREGDLL, __TEXT("ks_setLicense"));
	ks_setExtVal = (pks_setExtVal)ksSeekFunction(hinKSREGDLL, __TEXT("ks_setExtVal"));
	ks_setSoftVal = (pks_setSoftVal)ksSeekFunction(hinKSREGDLL, __TEXT("ks_setSoftVal"));
	ks_setUserVal = (pks_setUserVal)ksSeekFunction(hinKSREGDLL, __TEXT("ks_setUserVal"));
	ks_setUpVal = (pks_setUpVal)ksSeekFunction(hinKSREGDLL, __TEXT("ks_setUpVal"));
	ks_IdCheck = (pks_IdCheck)ksSeekFunction(hinKSREGDLL, __TEXT("ks_IdCheck"));
	ks_advapifree = (pks_advapifree)ksSeekFunction(hinKSREGDLL, __TEXT("ks_advapifree"));
	ks_advapi = (pks_advapi)ksSeekFunction(hinKSREGDLL, __TEXT("ks_advapi"));
	ks_GetData = (pks_GetData)ksSeekFunction(hinKSREGDLL, __TEXT("ks_GetData"));
	ks_GetMsg = (pks_GetMsg)ksSeekFunction(hinKSREGDLL, __TEXT("ks_GetMsg"));
	ks_unbind = (pks_unbind)ksSeekFunction(hinKSREGDLL, __TEXT("ks_unbind"));
	ks_reguser = (pks_reguser)ksSeekFunction(hinKSREGDLL, __TEXT("ks_reguser"));
	ks_prepaid = (pks_prepaid)ksSeekFunction(hinKSREGDLL, __TEXT("ks_prepaid"));
	ks_edit = (pks_edit)ksSeekFunction(hinKSREGDLL, __TEXT("ks_edit"));
	ks_exit = (pks_exit)ksSeekFunction(hinKSREGDLL, __TEXT("ks_exit"));
	ks_viewinfo = (pks_viewinfo)ksSeekFunction(hinKSREGDLL, __TEXT("ks_viewinfo"));
	IPC_Start = (pIPC_Start)ksSeekFunction(hinKSREGDLL, __TEXT("IPC_Start"));
	IPC_Connect = (pIPC_Connect)ksSeekFunction(hinKSREGDLL, __TEXT("IPC_Connect"));
	IPC_IdCheck = (pIPC_IdCheck)ksSeekFunction(hinKSREGDLL, __TEXT("IPC_IdCheck"));
	IPC_GetData = (pIPC_GetData)ksSeekFunction(hinKSREGDLL, __TEXT("IPC_GetData"));
	IPC_advapi = (pIPC_advapi)ksSeekFunction(hinKSREGDLL, __TEXT("IPC_advapi"));
	IPC_advapifree = (pIPC_advapifree)ksSeekFunction(hinKSREGDLL, __TEXT("IPC_advapifree"));

	decode_rsa = (pdecode_rsa)ksSeekFunction(hinKSREGDLL, __TEXT("decode_rsa"));
	decode_RC4Base64 = (pdecode_RC4Base64)ksSeekFunction(hinKSREGDLL, __TEXT("decode_RC4Base64"));
}

void ksFreeDLLs() //ж��������ص�DLL
{
	FreeLibrary(hinUSER32DLL);
	FreeLibrary(hinKERNEL32DLL);
	FreeLibrary(hinKSREGDLL);
}


CString myGetWindowText(CWnd &aCWnd)
{
	CString lsS;
	aCWnd.GetWindowText(lsS);
	return lsS;
}

CString myGetWindowTextTrim(CWnd &aCWnd)
{
	CString lsS = myGetWindowText(aCWnd);
	lsS.TrimLeft();
	lsS.TrimRight();
	return lsS;
}