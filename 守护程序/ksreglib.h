//ksreglib.h

#if !defined(MYFX_ksreglib_H__33234425_E7D0_40CE_A3F0_325A6DF20696__INCLUDED_)
#define MYFX_ksreglib_H__33234425_E7D0_40CE_A3F0_325A6DF20696__INCLUDED_
//
#ifdef KS_REG_SERVER
	#define KS_SERVERCLIENTFLAG_IdCheck ks_IdCheck
	#define KS_SERVERCLIENTFLAG_advapi ks_advapi
#else
	#define KS_SERVERCLIENTFLAG_IdCheck IPC_IdCheck
	#define KS_SERVERCLIENTFLAG_advapi IPC_advapi
#endif
//
extern CString v_inipath;
extern _int32  v_softcode;         //公共变量，软件编号
extern CString v_softkey;          //公共变量，软件密钥
extern _int32  v_softversion;      //公共变量，当前软件版本
extern CString v_bdinfo;           //公共变量，用户绑定信息
extern _int32  v_isipcmode;
//
extern CString  signData;
extern _int32 hwndfun;
//
extern bool myISRunning;
//
extern const CHAR USER32DLL[];     //可以改名，但必须放到你程序目录 或 系统文件夹system32目录
extern const CHAR KERNEL32DLL[];     //可以改名，但必须放到你程序目录 或 系统文件夹system32目录
extern const CHAR KSREGDLL[];   //可以改名，但必须放到你程序目录 或 系统文件夹system32目录
extern const int MaxStringBufferSize; //字符缓冲区最大大小 -- 不够的话，就加大！没事别改它
//
extern HINSTANCE hinUSER32DLL;
extern HINSTANCE hinKERNEL32DLL;
extern HINSTANCE hinKSREGDLL;
//
//USER32.DLL接口
typedef _int32 (WINAPI *PMessageBoxTimeout)(
											_int32 hwnd, 
											LPSTR lpText, 
											LPSTR lpCaption, 
											_int32 wType, 
											_int32 wlange, 
											_int32 dwTimeout);
extern PMessageBoxTimeout MessageBoxTimeout;
//KERNEL32.DLL接口
typedef _int32 (WINAPI *PGetPrivateProfileString_)(
												   LPSTR lpApplicationName, 
												   LPSTR lpKeyName, 
												   LPSTR lpDefault, 
												   LPSTR lpReturnedString, 
												   _int32 nSize, 
												   LPSTR lpFileName);
typedef _int32 (WINAPI *PWritePrivateProfileString_)(
													 LPSTR lpApplicationName, 
													 LPSTR lpKeyName, 
													 LPSTR lpString, 
													 LPSTR lpFileName);
//
extern PGetPrivateProfileString_ GetPrivateProfileString_;
extern PWritePrivateProfileString_ WritePrivateProfileString_;
//
//kssPlugin.DLL接口
typedef void (WINAPI *pks_setLicense)(LPSTR LicenseKey);
typedef void (WINAPI *pks_setExtVal)(
									 _int32 v_language,  
									 LPSTR v_pccode,  
									 LPSTR v_ininame,  
									 _int32 v_contime,  
									 _int32 v_is2svr, 
									 LPSTR v_exit,  
									 LPSTR v_ext7,  
									 LPSTR v_ext8,  
									 LPSTR v_ext9,  
									 LPSTR v_callbackfun);
typedef void (WINAPI *pks_setSoftVal)(
									  _int32 v_softcode,  
									  LPSTR v_softkey);
typedef void (WINAPI *pks_setUserVal)(
									  _int32 v_clientid, 
									  LPSTR v_username,  
									  LPSTR v_password,  
									  LPSTR v_bdinfo);
typedef void (WINAPI *pks_setUpVal)(
									_int32 v_softHwnd,  
									_int32 v_softVersion,  
									_int32 v_checkupdata);
typedef LPSTR (WINAPI *pks_IdCheck)(
									LPSTR v_signA, 
									_int32 v_iscon);
typedef void (WINAPI *pks_advapifree)(LPSTR v_key);
typedef LPSTR (WINAPI *pks_advapi)(LPSTR v_expression);
typedef LPSTR (WINAPI *pks_GetData)(_int32 v_index);
typedef LPSTR (WINAPI *pks_GetMsg)(LPSTR v_index);
typedef LPSTR (WINAPI *pks_unbind)(
								   _int32 v_softcode, 
								   _int32 v_clientid, 
								   LPSTR v_UsernameOrKey, 
								   LPSTR v_userpassword);
typedef LPSTR (WINAPI *pks_reguser)(
									_int32 v_softcode,   
									LPSTR v_username,  
									LPSTR v_userpassword, 
									LPSTR v_superpassword,  
									LPSTR v_keynum,  
									LPSTR v_bdinfo,  
									LPSTR v_parentusername);
typedef LPSTR (WINAPI *pks_prepaid)(
									_int32 v_softcode,  
									LPSTR v_username,  
									LPSTR v_keynum);
typedef LPSTR (WINAPI *pks_edit)(
								 _int32 v_softcode,  
								 LPSTR v_username,  
								 LPSTR v_superpassword,  
								 LPSTR v_newloginpassword, 
								 LPSTR v_bdinfo);
typedef LPSTR (WINAPI *pks_viewinfo)(
									 _int32 v_softcode,   
									 LPSTR v_UsernameOrKey);
typedef LPSTR (WINAPI *pks_exit)();
typedef _int32 (WINAPI *pIPC_Start)(
								  _int32 v_sockPort, 
								  _int32 v_clientNum);
typedef _int32 (WINAPI *pIPC_Connect)(
								  _int32 v_sockPort);
typedef LPSTR (WINAPI *pIPC_IdCheck)(
									 LPSTR v_signA, 
									 _int32 v_iscon);
typedef LPSTR (WINAPI *pIPC_GetData)(_int32 v_index);
typedef LPSTR (WINAPI *pIPC_advapi)(LPSTR Question);
typedef void (WINAPI *pIPC_advapifree)(LPSTR Question);

typedef LPSTR (WINAPI *pdecode_rsa)(
									 LPSTR v_text,
									 LPSTR v_rsapubkey,   
									 LPSTR v_rsamodules);

typedef LPSTR (WINAPI *pdecode_RC4Base64)(
									 LPSTR v_key,
									 LPSTR v_text);
//
extern pks_setLicense ks_setLicense;
extern pks_setExtVal ks_setExtVal;
extern pks_setSoftVal ks_setSoftVal;
extern pks_setUserVal ks_setUserVal;
extern pks_setUpVal ks_setUpVal;
extern pks_IdCheck ks_IdCheck;
extern pks_advapifree ks_advapifree;
extern pks_advapi ks_advapi;
extern pks_GetData ks_GetData;
extern pks_GetMsg ks_GetMsg;
extern pks_unbind ks_unbind;
extern pks_reguser ks_reguser;
extern pks_prepaid ks_prepaid;
extern pks_edit ks_edit;
extern pks_viewinfo ks_viewinfo;
extern pks_exit ks_exit;
extern pIPC_Start IPC_Start;
extern pIPC_Connect IPC_Connect;
extern pIPC_IdCheck IPC_IdCheck;
extern pIPC_GetData IPC_GetData;
extern pIPC_advapi IPC_advapi;
extern pIPC_advapifree IPC_advapifree;

extern pdecode_rsa decode_rsa;
extern pdecode_RC4Base64 decode_RC4Base64;
//
//
bool         iserrno(CString s_txt);
void         chkPass(_int32 ctype);
CString        advapi(CString v_text);
CString        mattime(_int32 cday);
void         MsgBox(CString v_text);
_int32       kpoints(_int32 v_points, CString *v_errinfo);
_int32       c_int(CString v_str);
void         w_ini(CString iarea, CString iKeyName, CString iKeyValue);
CString        r_ini(CString iarea, CString iKeyName);
//CStringArray splite(CString Source, CString ch);
//
void myExitProcess(); //掐进程前判断是否需要ks_exit
void ksLoadDLLs(); //加载两个相关的DLL
void ksFreeDLLs(); //卸载两个相关的DLL
//
CString myGetWindowText(CWnd &aCWnd);
CString myGetWindowTextTrim(CWnd &aCWnd);
//

#endif // !defined(MYFX_ksreglib_H__33234425_E7D0_40CE_A3F0_325A6DF20696__INCLUDED_)



