#include "imesample.h"

//定义输入法要处理的键
BYTE g_bPrecess[] = {
						0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,        // 00-0F VK_BACK
                        0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,        // 10-1F VK_CAPTIAL
                        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-2F VK_SPACE
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-3F VK_0~VK_9
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-4F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-5F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 60-6F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-7F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-8F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-9F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-AF
                        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,        // B0-BF
                        1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-CF
                        0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,        // D0-DF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-EF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0			// F0-FF
};

//*****************************************************************
//	向HIMC发送消息
//	消息数据保存在lpIMC->dwNumMsgBuf中
//	每次发消息前需要分配消息空间
//*****************************************************************
BOOL MyGenerateMessage(HIMC hIMC, UINT msg, WPARAM wParam, LPARAM lParam)
{
	BOOL bRet=FALSE;
    LPINPUTCONTEXT lpIMC= ImmLockIMC(hIMC);
	if(lpIMC == NULL)
		return FALSE;    
    
    if (IsWindow(lpIMC->hWnd))
    {
        LPTRANSMSG lpTransMsg;
        if (!(lpIMC->hMsgBuf = ImmReSizeIMCC(lpIMC->hMsgBuf,(lpIMC->dwNumMsgBuf+1) * sizeof(TRANSMSG))))
            goto error;
		
        if (!(lpTransMsg = (LPTRANSMSG)ImmLockIMCC(lpIMC->hMsgBuf)))
            goto error;
		
        lpTransMsg += (lpIMC->dwNumMsgBuf);
        lpTransMsg->message=msg;
		lpTransMsg->wParam=wParam;
		lpTransMsg->lParam=lParam;
        lpIMC->dwNumMsgBuf++;

        ImmUnlockIMCC(lpIMC->hMsgBuf);
        ImmGenerateMessage(hIMC);//将消息发送到IME，IME再决定是自己处理还是继续发给应用程序
		bRet=TRUE;
    }
error:
	ImmUnlockIMC(hIMC); 
	return bRet;
}

//************************************************************
//	向消息缓冲区填充消息
//	LPDWORD lpdwTransKey:消息缓冲区,由系统提供,第一个双字为可用消息空间数
//	UINT *uNumTranMsgs:当前消息数
//	UINT msg,WPARAM wParam,LPARAM lParam：消息数据
//	remark:该函数只在ImeToAsciiEx中调用
//************************************************************
BOOL MyGenerateMessageToTransKey(LPDWORD lpdwTransKey,
								 UINT *uNumTranMsgs,
								 UINT msg, 
								 WPARAM wParam, 
								 LPARAM lParam) 
{
    LPTRANSMSG lpTransMsg;
	if (((*uNumTranMsgs) + 1) >= (UINT)*lpdwTransKey)
        return FALSE;
	lpTransMsg = (LPTRANSMSG)(lpdwTransKey+1+(*uNumTranMsgs)*3);
	lpTransMsg->message=msg;
	lpTransMsg->wParam=wParam;
	lpTransMsg->lParam=lParam;
	(*uNumTranMsgs)++;
    return TRUE;
}


BOOL WINAPI ImeInquire(LPIMEINFO lpIMEInfo,LPTSTR lpszUIClass,LPCTSTR lpszOption)
{
    lpIMEInfo->dwPrivateDataSize = 0;//系统根据它为INPUTCONTEXT.hPrivate分配空间

    lpIMEInfo->fdwProperty = IME_PROP_KBD_CHAR_FIRST |
#ifdef _UNICODE
                             IME_PROP_UNICODE |
#endif
                             IME_PROP_IGNORE_UPKEYS |
							 IME_PROP_END_UNLOAD ;

    lpIMEInfo->fdwConversionCaps = IME_CMODE_FULLSHAPE |
								IME_CMODE_NATIVE;

    lpIMEInfo->fdwSentenceCaps = IME_SMODE_NONE;
    lpIMEInfo->fdwUICaps = UI_CAP_2700;

	lpIMEInfo->fdwSCSCaps = 0;

    lpIMEInfo->fdwSelectCaps = SELECT_CAP_CONVERSION;

    _tcscpy(lpszUIClass,CLSNAME_UI);

    return TRUE;
}

BOOL WINAPI AboutDialogProc(HWND hWnd ,	UINT message ,WPARAM wParam ,LPARAM lParam );

BOOL WINAPI ImeConfigure(HKL hKL,HWND hWnd, DWORD dwMode, LPVOID lpData)
{
	//DebugLog(1,(DebugLogFile,"ImeConfigure\n"));
    switch (dwMode) {
    case IME_CONFIG_GENERAL:
        DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUT), (HWND)hWnd, (DLGPROC)AboutDialogProc);
        break;
    default:
        return (FALSE);
        break;
    }
    return (TRUE);
}

/**********************************************************************/
/* ClearCompStr()                                                     */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL PASCAL ClearCompStr(
    LPINPUTCONTEXT lpIMC)
{
    HIMCC               hMem;
    LPCOMPOSITIONSTRING lpCompStr;
    DWORD               dwSize;

    if(!lpIMC) {
        return (FALSE);
    }

    dwSize = sizeof(COMPOSITIONSTRING) + (MAX_PRIVATEDATA);

    if (!lpIMC->hCompStr) {
        // it maybe free by other IME, init it
        lpIMC->hCompStr = ImmCreateIMCC(dwSize);
    } else if (hMem = ImmReSizeIMCC(lpIMC->hCompStr, dwSize)) {
        lpIMC->hCompStr = hMem;
    } else {
        ImmDestroyIMCC(lpIMC->hCompStr);
        lpIMC->hCompStr = ImmCreateIMCC(dwSize);
        return (FALSE);
    }

    lpCompStr = (LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
    if (!lpCompStr) {
        ImmDestroyIMCC(lpIMC->hCompStr);
        lpIMC->hCompStr = ImmCreateIMCC(dwSize);
        return (FALSE);
    }
	memset(lpCompStr,0,dwSize);
    lpCompStr->dwSize = dwSize;
	lpCompStr->dwCompStrOffset = sizeof(COMPOSITIONSTRING);
	lpCompStr->dwCompStrLen = 0;
	lpCompStr->dwResultStrOffset = sizeof(COMPOSITIONSTRING);
	lpCompStr->dwResultStrLen = 0;
	
    ImmUnlockIMCC(lpIMC->hCompStr);
    return (TRUE);
}


/**********************************************************************/
/* ImeSelect()                                                        */
/* Return Value:                                                      */
/*      TRUE - successful, FALSE - failure                            */
/**********************************************************************/
BOOL WINAPI ImeSelect(
    HIMC hIMC,
    BOOL fSelect)
{
    LPINPUTCONTEXT lpIMC;
	BOOL bRet=FALSE;
    if (!hIMC) {
        return (FALSE);
    }

    lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
    if (!lpIMC) {
        return (FALSE);
    }
	lpIMC->fOpen=fSelect;
	if(fSelect) bRet=ClearCompStr(lpIMC);
    ImmUnlockIMC(hIMC);
    return bRet;
}

/*
使一个输入上下文激活或者失活，并通知输入法最新的输入上下文，可以在此做一些初始化工作
HIMC hIMC :输入上下文
BOOL fFlag : TRUE if activated, FALSE if deactivated. 
Returns TRUE if successful, FALSE otherwise. 
*/
BOOL WINAPI ImeSetActiveContext(HIMC hIMC,BOOL fFlag)
{
	//通过IME消息来实现窗口状态变化
	MyGenerateMessage(hIMC,WM_IME_NOTIFY,fFlag?IMN_OPENSTATUSWINDOW:IMN_CLOSESTATUSWINDOW,0);
    return TRUE;
}

/*
系统调用这个接口来判断IME是否处理当前键盘输入
HIMC hIMC:输入上下文
UINT uKey:键值
LPARAM lKeyData: unknown
CONST LPBYTE lpbKeyState:键盘状态,包含256键的状态
return : TRUE-IME处理,FALSE-系统处理
系统则调用ImeToAsciiEx，否则直接将键盘消息发到应用程序
*/
BOOL WINAPI ImeProcessKey(HIMC hIMC,UINT uKey,LPARAM lKeyData,CONST LPBYTE lpbKeyState)
{
	LPINPUTCONTEXT lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
	BOOL bRet=g_bPrecess[uKey];
	if(lpIMC)
	{
		LPCOMPOSITIONSTRING lpCompStr=(LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
		if(lpCompStr)
		{
			if(uKey==VK_BACK && lpCompStr->dwCompStrLen==0) 
			{//没有编码输入，告诉IME关闭输入窗口
				MyGenerateMessage(hIMC,WM_IME_ENDCOMPOSITION,0,0);
				bRet=FALSE;
			}
			ImmUnlockIMCC(lpIMC->hCompStr);
		}else
		{
			bRet=FALSE;
		}
		ImmUnlockIMC(hIMC);
	}else
	{
		bRet=FALSE;
	}
	return bRet;
}

/*
应用程序调用这个接口来进行输入上下文的转换，输入法程序在这个接口中转换用户的输入
UINT uVKey:键值,如果在ImeInquire接口中为fdwProperty设置了属性IME_PROP_KBD_CHAR_FIRST,则高字节是输入键值
UINT uScanCode:按键的扫描码，有时两个键有同样的键值，这时需要使用uScanCode来区分
CONST LPBYTE lpbKeyState:键盘状态,包含256键的状态
LPDWORD lpdwTransKey:消息缓冲区，用来保存IME要发给应用程序的消息，第一个双字是缓冲区可以容纳的最大消息条数
UINT fuState:Active menu flag(come from msdn)
HIMC hIMC:输入上下文
return : 返回保存在消息缓冲区lpdwTransKey中的消息个数
*/
UINT WINAPI ImeToAsciiEx (UINT uVKey,UINT uScanCode,CONST LPBYTE lpbKeyState,LPDWORD lpdwTransKey,UINT fuState,HIMC hIMC)
{
	LPINPUTCONTEXT lpIMC = (LPINPUTCONTEXT)ImmLockIMC(hIMC);
	UINT uMsgCount=0;
	if(lpIMC)
	{
		BYTE byInput=HIBYTE(uVKey);
		LPCOMPOSITIONSTRING lpCompStr=(LPCOMPOSITIONSTRING)ImmLockIMCC(lpIMC->hCompStr);
		if(lpCompStr)
		{
			LPBYTE pComp=(LPBYTE)GETLPCOMPSTR(lpCompStr);
			if(lpCompStr->dwCompStrLen==0)
			{//开始编码输入
				//生成开始编码消息以获取光标跟随时输入窗口的坐标
				MyGenerateMessageToTransKey(lpdwTransKey,&uMsgCount,WM_IME_STARTCOMPOSITION,0,0);
			}
			if(byInput==VK_BACK)
			{
				if(lpCompStr->dwCompStrLen>0) lpCompStr->dwCompStrLen--;
			}else
			{
				pComp[lpCompStr->dwCompStrLen++]=byInput;
			}
			
			if(lpCompStr->dwCompStrLen==9)
			{//结束编码输入,通过编码转换获得用户输入，将输入信息发到应用程序
				static char szSample[]="";
				LPTSTR pResult=GETLPRESULTSTR(lpCompStr);
				strcpy(pResult,szSample);
				lpCompStr->dwResultStrLen=strlen(szSample);
				lpCompStr->dwCompStrLen=0;
				//通知应用程序接收数据
				MyGenerateMessageToTransKey(lpdwTransKey,&uMsgCount,WM_IME_COMPOSITION,0,GCS_RESULTSTR|GCS_COMPSTR);
				//告诉IME关闭输入窗口
				MyGenerateMessageToTransKey(lpdwTransKey,&uMsgCount,WM_IME_ENDCOMPOSITION,0,0);
			}else
			{//正在输入编码
				MyGenerateMessageToTransKey(lpdwTransKey,&uMsgCount,WM_IME_COMPOSITION,0,GCS_COMPSTR);
			}
			ImmUnlockIMCC(lpIMC->hCompStr);
		}
		ImmUnlockIMC(hIMC);
	}
    return (uMsgCount);
}

