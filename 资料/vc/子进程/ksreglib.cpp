#include "stdafx.h"
#include "ksreglib.h"
//
CString v_inipath;
_int32  v_softcode;         //公共变量，软件编号
CString v_softkey;          //公共变量，软件密钥
_int32  v_softversion;      //公共变量，当前软件版本
CString v_bdinfo;           //公共变量，用户绑定信息
_int32  v_isipcmode;
//
CString  signData;
_int32 hwndfun;
bool myISRunning = false;
//
const CHAR USER32DLL[] = __TEXT("USER32.dll");     //可以改名，但必须放到你程序目录 或 系统文件夹system32目录
const CHAR KERNEL32DLL[] = __TEXT("Kernel32.dll");   //可以改名，但必须放到你程序目录 或 系统文件夹system32目录
const CHAR KSREGDLL[] = __TEXT("kssPlugin.dll");   //可以改名，但必须放到你程序目录 或 系统文件夹system32目录
const int MaxStringBufferSize = 1024; //字符缓冲区最大大小 -- 不够的话，就加大！没事别改它
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

//这个函数最好是自己处理一下
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
    MessageBoxTimeout(0, (LPSTR)(LPCTSTR)v_text, __TEXT("系统提示"), 64, 0, 10000);
}

//把秒换算成*天*小时*分*秒的格式
CString mattime(_int32 cday)
{
	CString uday;
	uday.Format(__TEXT("%d天%d小时%d分%d秒"), (_int32)(cday / 86400), (_int32)(cday % 86400 / 3600), (_int32)(cday % 86400 % 3600 / 60), (cday % 86400 % 3600 % 60));
    return uday;
}

//扣点函数，调用advapi接口实现扣点功能。成功返回剩余的点数。失败返回-1
_int32 kpoints(_int32 v_points, CString *v_errinfo) //注意这里是CString指针
{
    _int32 c;
    CString tresult;

	CString s;
	s.Format(__TEXT("v_points,%d"), v_points);
    tresult = advapi((LPCTSTR)s);
    c = c_int(tresult);
    if(c == 0)    //返回值不是整数或为0就说明扣点失败
    {
        if(iserrno(tresult))
            *v_errinfo = ks_GetMsg((LPSTR)(LPCTSTR)tresult);
        else
            *v_errinfo = tresult;
		return -1;
    }
    *v_errinfo = __TEXT("扣点成功");
    return c;
}

//转成整数，转换失败返回0
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

//读ini文件  节点名 键名
CString r_ini(CString iarea, CString iKeyName)
{
    CString s;
	LPSTR p = s.GetBuffer(1024);
	memset(p, 0, 1024);
    GetPrivateProfileString_((LPSTR)(LPCTSTR)iarea, (LPSTR)(LPCTSTR)iKeyName, __TEXT(""), (LPSTR)(LPCTSTR)s, 1024, (LPSTR)(LPCTSTR)v_inipath);
	s.ReleaseBuffer();
	s.Replace(__TEXT("\x0"), __TEXT("")); //替换空白字符为空串
	s.TrimLeft();
	s.TrimRight();
    return s;
}

//写ini文件 节点名 键名 键值
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
			msg.Format(__TEXT("找不到动态连接库%s里的函数%s"), USER32DLL, aFunName);
		if(aDll == hinKSREGDLL)
			msg.Format(__TEXT("找不到动态连接库%s里的函数%s"), KSREGDLL, aFunName);
		if(aDll == hinKERNEL32DLL)
			msg.Format(__TEXT("找不到动态连接库%s里的函数%s"), KERNEL32DLL, aFunName);
		MsgBox(msg);
		myExitProcess();
		return NULL;
	}
	return p;
}

void ksLoadDLLs() //加载两个相关的DLL
{
	CString msg;
    //加载USER32.DLL
	hinUSER32DLL = LoadLibrary(USER32DLL);
	if( hinUSER32DLL == NULL)
	{
		msg.Format(__TEXT("找不到动态连接库：%s"), USER32DLL);
		MsgBox(msg);
		myExitProcess();
	}
	//取得函数地址
	MessageBoxTimeout = (PMessageBoxTimeout)ksSeekFunction(hinUSER32DLL, __TEXT("MessageBoxTimeoutA"));
    //加载KERNEL32.DLL
	hinKERNEL32DLL = LoadLibrary(KERNEL32DLL);
	if( hinKERNEL32DLL == NULL)
	{
		msg.Format(__TEXT("找不到动态连接库：%s"), KERNEL32DLL);
		MsgBox(msg);
		myExitProcess();
	}
	//取得函数地址
	GetPrivateProfileString_ = (PGetPrivateProfileString_)ksSeekFunction(hinKERNEL32DLL, __TEXT("GetPrivateProfileStringA"));
	WritePrivateProfileString_ = (PWritePrivateProfileString_)ksSeekFunction(hinKERNEL32DLL, __TEXT("WritePrivateProfileStringA"));
	//加载KSDLL.DLL
	hinKSREGDLL = LoadLibrary(KSREGDLL);
	if( hinKSREGDLL == NULL)
	{
		msg.Format(__TEXT("找不到动态连接库：%s"), KSREGDLL);
		MsgBox(msg);
		myExitProcess();
	}
	//取得函数地址
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

void ksFreeDLLs() //卸载两个相关的DLL
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