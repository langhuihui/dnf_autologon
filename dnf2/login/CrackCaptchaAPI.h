//CrackCaptchaAPI.h 
#ifndef CRACK_CAPTCHA_API_H
#define CRACK_CAPTCHA_API_H

#define CRACKCAPTCHA_API WINAPI

//************************************
//			error code
//************************************
//success code
#define ERR_CC_SUCCESS					(0)
	//parameter eror
#define ERR_CC_SOFTWARE_NAME_ERR		(-1)
#define ERR_CC_SOFTWARE_ID_ERR			(-2)
#define ERR_CC_FILE_URL_ERR				(-3)
#define ERR_CC_COOKIE_ERR				(-4)
#define ERR_CC_REFERER_ERR				(-5)
#define ERR_CC_VCODE_LEN_ERR			(-6)
#define ERR_CC_VCODE_TYPE_ID_ERR		(-7)
#define ERR_CC_POINTER_ERROR			(-8)
#define ERR_CC_TIMEOUT_ERR				(-9)
#define ERR_CC_INVALID_SOFTWARE			(-10)
#define ERR_CC_COOKIE_BUFFER_TOO_SMALL	(-11)
#define ERR_CC_PARAMETER_ERROR			(-12)
	//user error
#define ERR_CC_USER_ALREADY_EXIST		(-100)
#define ERR_CC_BALANCE_NOT_ENOUGH		(-101)
#define ERR_CC_USER_NAME_ERR			(-102)
#define ERR_CC_USER_PASSWORD_ERR		(-103)
#define ERR_CC_QQ_NO_ERR				(-104)
#define ERR_CC_EMAIL_ERR				(-105)
#define ERR_CC_TELNO_ERR				(-106)
#define ERR_CC_DYNC_VCODE_SEND_MODE_ERR	(-107)
#define ERR_CC_INVALID_CARDNO			(-108)
#define ERR_CC_DYNC_VCODE_OVERFLOW		(-109)
#define ERR_CC_DYNC_VCODE_TIMEOUT		(-110)
#define ERR_CC_USER_SOFTWARE_NOT_MATCH	(-111)
#define ERR_CC_NEED_DYNC_VCODE			(-112)
	//logic error
#define ERR_CC_NOT_LOGIN				(-201)
#define ERR_CC_ALREADY_LOGIN			(-202)
#define ERR_CC_INVALID_REQUEST_ID		(-203)		//invalid request id, perhaps request is timeout
#define ERR_CC_INVALID_VCODE_ID			(-204)		//invalid captcha id, perhaps request is timeout
#define ERR_CC_NO_RESULT				(-205)
#define ERR_CC_NOT_INIT_PARAM			(-206)
#define ERR_CC_ALREADY_INIT_PARAM		(-207)
#define ERR_CC_SOFTWARE_DISABLED		(-208)
#define ERR_CC_NEED_RELOGIN				(-209)
#define EER_CC_ILLEGAL_USER				(-210)
#define EER_CC_REQUEST_TOO_MUCH			(-211)		//concurrent request is too much

	//system error
#define ERR_CC_CONFIG_ERROR				(-301)
#define ERR_CC_NETWORK_ERROR			(-302)
#define ERR_CC_DOWNLOAD_FILE_ERR		(-303)
#define ERR_CC_CONNECT_SERVER_FAIL		(-304)
#define ERR_CC_MEMORY_OVERFLOW			(-305)
#define ERR_CC_SYSTEM_ERR				(-306)
#define ERR_CC_SERVER_ERR				(-307)
#define ERR_CC_VERSION_ERROR			(-308)
#define ERR_CC_READ_FILE                (-309)


extern "C"
{
//************************************
// Method:    GetOrigError
// FullName:  GetOrigError
// Returns:   int CRACKCAPTCHA_API
//************************************
int CRACKCAPTCHA_API GetOrigError();

//************************************
// Method:    Init
// FullName:  Init
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pszSoftwareName, software name,  maximum 31 characters
// Parameter: const char * pszSoftwareID, 32 hex characters
//************************************
int CRACKCAPTCHA_API Init(const char *pszSoftwareName, const char *pszSoftwareID);

//************************************
// Method:    Uninit
// FullName:  Uninit
// Returns:   int CRACKCAPTCHA_API
//************************************
void CRACKCAPTCHA_API Uninit();

//************************************
// Method:    Login
// FullName:  Login
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: const char * pDyncVerificationCode, NULL, disable dyncmic verification code
// Parameter: char * pszSysAnnouncementURL: buffer length must be greater than 512
// Parameter: char * pszAppAnnouncementURL: buffer length must be greater than 512
//************************************
int CRACKCAPTCHA_API Login(const char *pszUserName, const char *pszUserPassword,
	const char *pDyncVerificationCode, char *pszSysAnnouncementURL, char *pszAppAnnouncementURL);

//************************************
// Method:    Login2
// FullName:  Login2
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
//************************************
int CRACKCAPTCHA_API Login2(const char *pszUserName, const char *pszUserPassword);

//************************************
// Method:    Logoff
// FullName:  Logoff
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
//************************************
int CRACKCAPTCHA_API Logoff();

//************************************
// Method:    Register
// FullName:  Register
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Qualifier:
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: const char * pszQQ
// Parameter: const char * pszTelNo
// Parameter: const char * pszEmail
// Parameter: int nDyncSendMode, dyncmic code send mode, 
//				1 for mobil, 2 for email, 3 for both
//************************************
int CRACKCAPTCHA_API Register(const char *pszUserName, const char *pszUserPassword, 
	const char *pszQQ, const char *pszTelNo, const char *pszEmail, int nDyncSendMode);

//************************************
// Method:    ReadInfo
// FullName:  ReadInfo
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: char * pszUserName, buffer length must be greater than 32
// Parameter: char * pszQQ, buffer length must be greater than 16
// Parameter: char * pszTelNo, buffer length must be greater than 16
// Parameter: char * pszEmail, buffer length must be greater than 48
// Parameter: int * pDyncSendMode, dyncmic code send mode, 
//				1 for mobil, 2 for email, 3 for both
//************************************
int CRACKCAPTCHA_API ReadInfo(char *pszUserName, char *pszQQ, 
	char *pszTelNo, char *pszEmail, int *pDyncSendMode);

//************************************
// Method:    ChangeInfo
// FullName:  ChangeInfo
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pUserOldPassword, maximum 16 characters
// Parameter: const char * pUserNewPassword, maximum 16 characters
// Parameter: const char * pszQQ, maximum 15 characters
// Parameter: const char * pszTelNo, maximum 15 characters
// Parameter: const char * pszEmail, maximum 47 characters
// Parameter: const char * pszDyncVCode, maximum 15 characters
// Parameter: int nDyncSendMode,  dyncmic code send mode, 
//				1 for mobil, 2 for email, 3 for both
//************************************
int CRACKCAPTCHA_API ChangeInfo(const char *pUserOldPassword, const char * pUserNewPassword, 
	const char *pszQQ, const char *pszTelNo, const char *pszEmail, const char *pszDyncVCode, 
	int nDyncSendMode);

//************************************
// Method:    Decode
// FullName:  Decode
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pszFileURL, maximum 511 characters
// Parameter: const char * pszCookie, maximum 4095 characters
// Parameter: const char * pszReferer, maximum 511 characters
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: int bGetPictureByLocalMachine, 1 download picture by local machine, 0 by other machine
// Parameter: unsigned long *pulRequestID, return on success, for GetDecodeResult call
//************************************
int CRACKCAPTCHA_API Decode( const char *pszFileURL, 
	const char *pszCookie, const char *pszReferer,
	unsigned char ucVerificationCodeLen, unsigned short usTimeout,
	unsigned long ulVCodeTypeID, int bDownloadPictureByLocalMachine, 
	unsigned long *pulRequestID);

//************************************
// Method:    DecodeSync
// FullName:  DecodeSync
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
// Parameter: const char * pszFileURL, maximum 511 characters
// Parameter: const char * pszCookie, maximum 4095 characters
// Parameter: const char * pszReferer, maximum 511 characters
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: int bGetPictureByLocalMachine, 1 download picture by local machine, 0 by other machine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
// Parameter: char *pszRetCookie, return cookie, 至少4096字节长度，如果不需要，传NULL
//************************************
int CRACKCAPTCHA_API DecodeSync( const char *pszFileURL, 
	const char *pszCookie, const char *pszReferer,
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText, char *pszRetCookie);

//************************************
// Method:    DecodeBuf
// FullName:  DecodeBuf
// Access:    public 
// Returns:   int CRACKCAPTCHA_API
// Qualifier:
// Parameter: LPVOID pImageData
// Parameter: DWORD dwDataLen
// Parameter: const char * pszExtName
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: unsigned long * pulRequestID
//************************************
int CRACKCAPTCHA_API DecodeBuf( 
	LPCVOID pImageData, DWORD dwDataLen, const char *pszExtName, 
	unsigned char ucVerificationCodeLen, unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	unsigned long *pulRequestID);

//************************************
// Method:    DecodeBufSync
// FullName:  DecodeBufSync
// Access:    public 
// Returns:   int >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
// Qualifier:
// Parameter: LPVOID pImageData
// Parameter: DWORD dwDataLen
// Parameter: const char * pszExtName
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: unsigned long * pulRequestID
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API DecodeBufSync( 
	LPCVOID pImageData, DWORD dwDataLen, const char *pszExtName, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);

//************************************
// Method:    DecodeWnd
// FullName:  DecodeWnd
// Access:    public 
// Returns:   int CRACKCAPTCHA_API
// Qualifier:
// Parameter: const char *pszWndDef:窗口定义字串,格式如下: 
//	 由"\n"分隔的多个子串组成,一个子串表示一级窗口查找的条件.第一级窗口须为顶级窗口
//	 每个子串由3个元素组成:窗口Class名,窗口名,窗口索引.元素之间以逗号(半角)分隔
//	 	窗口类名:如不想通过类名查找,填"ANY_CLASS"
//	 	窗口名:窗口的名字,如没有窗口名,填"ANY_NAME"
//	 	序号:以1开始数字,1表示第1个窗口名和窗口类名符合的窗口,
//	 		如序号不为1,则依次查找符合条件和序号的窗口
//	 窗口级最大为50级
//	 如要查找要查找第一个类名为"TopClass"窗口名不限的第二子窗口(类名和类名都不限),
//	 字串如下:
//	 	TopClass,ANY_NAME,1\nANY_CLASS,ANY_NAME,2
// Parameter: LPCRECT lpRect: 要截取的窗口内容矩形(相对于窗口最左上角),
//	  NULL表示截取整个窗口内容
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: unsigned long * pulRequestID
//************************************
int CRACKCAPTCHA_API DecodeWnd( 
	const char *pszWndDef, LPCRECT lpRect, 
	unsigned char ucVerificationCodeLen, unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	unsigned long *pulRequestID);

//************************************
// Method:    DecodeWndHandle
// FullName:  DecodeWndHandle
// Access:    public 
// Returns:   int CRACKCAPTCHA_API
// Qualifier:
// Parameter: HWND hHandle：窗口句柄
// Parameter: LPCRECT lpRect: 要截取的窗口内容矩形(相对于窗口最左上角),
//	  NULL表示截取整个窗口内容
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: unsigned long * pulRequestID
//************************************
int CRACKCAPTCHA_API DecodeWndHandle( 
	HWND hWnd, LPCRECT lpRect, 
	unsigned char ucVerificationCodeLen, unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	unsigned long *pulRequestID);

//************************************
// Method:    DecodeWndHandleSync
// FullName:  DecodeWndHandleSync
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
// Qualifier:
// Parameter: HWND hHandle，窗口句柄
// Parameter: LPCRECT lpRect: 要截取的窗口内容矩形(相对于窗口最左上角),
//	  NULL表示截取整个窗口内容
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API DecodeWndHandleSync( 
	HWND hHandle, LPCRECT lpRect, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);


//************************************
// Method:    DecodeWndSync
// FullName:  DecodeWndSync
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
// Qualifier:
// Parameter: const char *pszWndDef:窗口定义字串,格式如下: 
//	 由"\n"分隔的多个子串组成,一个子串表示一级窗口查找的条件.第一级窗口须为顶级窗口
//	 每个子串由3个元素组成:窗口Class名,窗口名,窗口索引.元素之间以逗号(半角)分隔
//	 	窗口类名:如不想通过类名查找,填"ANY_CLASS"
//	 	窗口名:窗口的名字,如没有窗口名,填"ANY_NAME"
//	 	序号:以1开始数字,1表示第1个窗口名和窗口类名符合的窗口,
//	 		如序号不为1,则依次查找符合条件和序号的窗口
//	 窗口级最大为50级
//	 如要查找要查找第一个类名为"TopClass"窗口名不限的第二子窗口(类名和类名都不限),
//	 字串如下:
//	 	TopClass,ANY_NAME,1\nANY_CLASS,ANY_NAME,2
// Parameter: LPCRECT lpRect: 要截取的窗口内容矩形(相对于窗口最左上角),
//	  NULL表示截取整个窗口内容
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API DecodeWndSync( 
	const char *pszWndDef, LPCRECT lpRect, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);

//************************************
// Method:    DecodeFile
// FullName:  DecodeFile
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回请求ID（用于调用GetResult）, <0失败
// Qualifier:
// Parameter: const char *pszFilePath,  本机图片文件路径
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
//************************************
int CRACKCAPTCHA_API DecodeFile( 
	const char *pszFilePath, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID);

//************************************
// Method:    DecodeFileSync
// FullName:  DecodeFileSync
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
// Qualifier:
// Parameter: const char *pszFilePath,  本机图片文件路径
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API DecodeFileSync( 
	const char *pszFilePath, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);


//************************************
// Method:    GetResult
// FullName:  GetResult
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: unsigned long ulRequestID
// Parameter: unsigned long ulTimeout, unit is ms
// Parameter: char * pszVCode, its length is specified by ulVCodeBufLen
// Parameter: unsigned long ulVCodeBufLen, must be greater than 40
// Parameter: unsigned long * pulVCodeID, return on success, for ReportDecodeCorrectness
// Parameter: char * pszReturnCookie
// Parameter: unsigned long ulCookieBufferLen
//************************************
int CRACKCAPTCHA_API GetResult(unsigned long ulRequestID, 
	unsigned long ulTimeout, char *pszVCode, unsigned long ulVCodeBufLen, 
	unsigned long *pulVCodeID, char *pszReturnCookie, unsigned long ulCookieBufferLen);

//************************************
// Method:    ReportResult
// FullName:  ReportResult
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: unsigned long nVCodeID, return from GetDecodeResult
// Parameter: int bCorrect, 1 for right,0 for error
//************************************
int CRACKCAPTCHA_API ReportResult(unsigned long ulVCodeID, int bCorrect);

//************************************
// Method:    QueryUserBalance
// FullName:  QueryUserBalance
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: unsigned long * pulBalance, return on success
//************************************
int CRACKCAPTCHA_API QueryBalance(unsigned long *pulBalance);

//************************************
// Method:    Recharge
// FullName:  Recharge
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pszUserName
// Parameter: const char * pszCardNo
// Parameter: unsigned long * pulBalance: return on success
//************************************
int CRACKCAPTCHA_API Recharge(const char *pszUserName, const char *pszCardNo, unsigned long *pulBalance);


//************************************
// Method:    D2File
// FullName:  D2File
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
//		应该停机处理的错误码包括：-1~-199（参数错误、用户错误）、-208（软件禁用）、-210（非法用户）、-301（配置错误、DLL找不到）
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: const char *pszFilePath,  本机图片文件路径
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API D2File( 
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	const char *pszFilePath, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);

//************************************
// Method:    D2FileAsync
// FullName:  D2FileAsync
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回请求ID（用于调用GetResult）, <0失败
//		应该停机处理的错误码包括：-1~-199（参数错误、用户错误）、-208（软件禁用）、-210（非法用户）、-301（配置错误、DLL找不到）
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: const char *pszFilePath,  本机图片文件路径
// Parameter: unsigned char ucVerificationCodeLen
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
//************************************
int CRACKCAPTCHA_API D2FileAsync( 
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	const char *pszFilePath, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID);

//************************************
// Method:    D2Buf
// FullName:  D2Buf
// Access:    public 
// Returns:   int >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
//		应该停机处理的错误码包括：-1~-199（参数错误、用户错误）、-208（软件禁用）、-210（非法用户）、-301（配置错误、DLL找不到）
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: LPVOID pImageData
// Parameter: DWORD dwDataLen
// Parameter: const char * pszExtName
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: unsigned long * pulRequestID
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API D2Buf( 
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	LPCVOID pImageData, DWORD dwDataLen, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);

}

//************************************
// Method:    D2BufAsync
// FullName:  D2BufAsync
// Access:    public 
// Returns:   int CRACKCAPTCHA_API, >0 成功，返回请求ID（用于调用GetResult）, <0失败
//		应该停机处理的错误码包括：-1~-199（参数错误、用户错误）、-208（软件禁用）、-210（非法用户）、-301（配置错误、DLL找不到）
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: LPVOID pImageData
// Parameter: DWORD dwDataLen
// Parameter: const char * pszExtName
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: unsigned long * pulRequestID
//************************************
int CRACKCAPTCHA_API D2BufAsync( 
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	LPCVOID pImageData, DWORD dwDataLen, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID);

//************************************
// Method:    D2Balance
// FullName:  D2Balance
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: unsigned long * pulBalance, return on success
//************************************
int CRACKCAPTCHA_API D2Balance(
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	unsigned long *pulBalance);

//************************************
// Method:    D2Recharge
// FullName:  D2Recharge
// Parameter: const char * pszSoftwareID, 32 hex characters
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Parameter: const char * pszUserName
// Parameter: const char * pszCardNo
// Parameter: unsigned long * pulBalance: return on success
//************************************
int CRACKCAPTCHA_API D2Recharge(
	const char *pszSoftwareId,
	const char *pszUserName, 
	const char *pszCardNo, 
	unsigned long *pulBalance);

//************************************
// Method:    D2WndHandle
// FullName:  D2WndHandle
// Access:    public 
// Returns:   int >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
//		应该停机处理的错误码包括：-1~-199（参数错误、用户错误）、-208（软件禁用）、-210（非法用户）、-301（配置错误、DLL找不到）
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: HWND hHandle，窗口句柄
// Parameter: LPCRECT lpRect: 要截取的窗口内容矩形(相对于窗口最左上角),
//	  NULL表示截取整个窗口内容
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API D2WndHandle( 
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	HWND hHandle, LPCRECT lpRect, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);


//************************************
// Method:    D2Wnd
// FullName:  D2Wnd
// Access:    public 
// Returns:   int >0 成功，返回验证码ID（用于调用ReportResult）, <0失败
//		应该停机处理的错误码包括：-1~-199（参数错误、用户错误）、-208（软件禁用）、-210（非法用户）、-301（配置错误、DLL找不到）
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: const char *pszWndDef:窗口定义字串,格式如下: 
//	 由"\n"分隔的多个子串组成,一个子串表示一级窗口查找的条件.第一级窗口须为顶级窗口
//	 每个子串由3个元素组成:窗口Class名,窗口名,窗口索引.元素之间以逗号(半角)分隔
//	 	窗口类名:如不想通过类名查找,填"ANY_CLASS"
//	 	窗口名:窗口的名字,如没有窗口名,填"ANY_NAME"
//	 	序号:以1开始数字,1表示第1个窗口名和窗口类名符合的窗口,
//	 		如序号不为1,则依次查找符合条件和序号的窗口
//	 窗口级最大为50级
//	 如要查找要查找第一个类名为"TopClass"窗口名不限的第二子窗口(类名和类名都不限),
//	 字串如下:
//	 	TopClass,ANY_NAME,1\nANY_CLASS,ANY_NAME,2
// Parameter: LPCRECT lpRect: 要截取的窗口内容矩形(相对于窗口最左上角),
//	  NULL表示截取整个窗口内容
// Parameter: unsigned short usTimeout, unit is second
// Parameter: unsigned long ulVCodeTypeID
// Parameter: bool bDownloadPictureByLocalMachine
// Parameter: char *pszVCodeText, return captcha text, min 30 characters
//************************************
int CRACKCAPTCHA_API D2Wnd( 
	const char *pszSoftwareId,
	const char *pszUserName,
	const char *pszUserPassword,
	const char *pszWndDef, LPCRECT lpRect, 
	unsigned short usTimeout,
	unsigned long ulVCodeTypeID, 
	char *pszVCodeText);

//************************************
// Method:    D2Register
// FullName:  D2Register
// Returns:   int CRACKCAPTCHA_API, 0 for success, others for failure
// Qualifier:
// Parameter: const char * pszSoftwareID, 32 hex characters
// Parameter: const char * pszUserName, user name, maximum 31 characters
// Parameter: const char * pszUserPassword, maximum 16 characters
// Parameter: const char * pszQQ
// Parameter: const char * pszTelNo
// Parameter: const char * pszEmail
// Parameter: int nDyncSendMode, dyncmic code send mode, 
//				1 for mobil, 2 for email, 3 for both
//************************************
int CRACKCAPTCHA_API D2Register(
	const char *pszSoftwareId,
	const char *pszUserName, const char *pszUserPassword, 
	const char *pszQQ, const char *pszTelNo, const char *pszEmail, int nDyncSendMode);


#endif //CRACK_CAPTCHA_API_H

