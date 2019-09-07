// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "mhook-lib\mhook.h"

//=========================================================
// struct CLIENT_ID
typedef struct _CLIENT_ID {
	DWORD_PTR UniqueProcess;
	DWORD_PTR UniqueThread;
} CLIENT_ID, *PCLIENT_ID;
typedef struct _PROCESS_BASIC_INFORMATION {
	DWORD ExitStatus;
	ULONG PebBaseAddress;
	ULONG AffinityMask;
	LONG BasePriority;
	ULONG UniqueProcessId;
	ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION, *PPROCESS_BASIC_INFORMATION;
typedef enum _PROCESSINFOCLASS {
	ProcessBasicInformation,
	ProcessQuotaLimits,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,          // Note: this is kernel mode only
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	ProcessDeviceMap,
	ProcessSessionInformation,
	ProcessForegroundInformation,
	ProcessWow64Information,
	ProcessImageFileName,
	ProcessLUIDDeviceMapsEnabled,
	ProcessBreakOnTermination,
	ProcessDebugObjectHandle,
	ProcessDebugFlags,
	ProcessHandleTracing,
	ProcessIoPriority,
	ProcessExecuteFlags,
	ProcessTlsInformation,
	ProcessCookie,
	ProcessImageInformation,
	ProcessCycleTime,
	ProcessPagePriority,
	ProcessInstrumentationCallback,
	ProcessThreadStackAllocation,
	ProcessWorkingSetWatchEx,
	ProcessImageFileNameWin32,
	ProcessImageFileMapping,
	ProcessAffinityUpdateMode,
	ProcessMemoryAllocationMode,
	ProcessGroupInformation,
	ProcessTokenVirtualizationEnabled,
	ProcessOwnerInformation,
	ProcessWindowInformation,
	ProcessHandleInformation,
	ProcessMitigationPolicy,
	ProcessDynamicFunctionTableInformation,
	ProcessHandleCheckingMode,
	ProcessKeepAliveCount,
	ProcessRevokeFileHandles,
	ProcessWorkingSetControl,
	ProcessHandleTable,
	ProcessCheckStackExtentsMode,
	ProcessCommandLineInformation,
	ProcessProtectionInformation,
	MaxProcessInfoClass             // MaxProcessInfoClass should always be the last enum
} PROCESSINFOCLASS;
typedef DWORD(WINAPI *QUERY_INFO_PROCESS) (
	__in HANDLE ProcessHandle,
	__in PROCESSINFOCLASS ProcessInformationClass,
	__out PVOID ProcessInformation,
	__in ULONG ProcessInformationLength,
	__out PULONG ReturnLength
	);

QUERY_INFO_PROCESS pZwQueryInformationProcess = (QUERY_INFO_PROCESS)GetProcAddress(GetModuleHandle(L"ntdll"), "ZwQueryInformationProcess");
//==============================================================
// 变量
HHOOK     glhHook = NULL;
HINSTANCE glhInstance = NULL;
DWORD     gProtectProcessID = 0;
DWORD GetProcessIdByHandle(PHANDLE ProcessHandle)
{
	ULONG                         lRet;
	PVOID                         pBuffer;
	PROCESS_BASIC_INFORMATION     pbi = { NULL };
	DWORD ProcessID;
	DWORD ret;
	lRet = 0;

	//pBuffer = ExAllocatePool(PagedPool, sizeof(PROCESS_BASIC_INFORMATION));
	if (NULL != pZwQueryInformationProcess)
	{

		ret = pZwQueryInformationProcess(*ProcessHandle, ProcessBasicInformation,
			&pbi, sizeof(PROCESS_BASIC_INFORMATION), &lRet);
		cout << "获取进程信息：" << ret << endl;
	}
	else{
		cout << "ZwQueryInformationProcess未获取到" << endl;
	}
	cout << pbi.AffinityMask << " " << pbi.BasePriority << " " << pbi.ExitStatus << " " << pbi.InheritedFromUniqueProcessId << " " << pbi.PebBaseAddress << " " << pbi.UniqueProcessId << endl;
	//pbi = (struct _PROCESS_BASIC_INFORMATION *)pBuffer;
	ProcessID = pbi.UniqueProcessId;
	//ExFreePool(pBuffer);
	return ProcessID;

}
//===========================================================
// NtOpenProcess
typedef ULONG (WINAPI *pfnNtOpenProcess)(
                   __out PHANDLE ProcessHandle,
				   __in ACCESS_MASK AccessMask ,
				   __in PVOID ObjectAttributes,
				   __in PCLIENT_ID ClientId);

pfnNtOpenProcess _NtOpenProcess = (pfnNtOpenProcess)GetProcAddress( GetModuleHandle(L"ntdll"),"NtOpenProcess");

//===========================================================
//定制我们自己的NtOpenProcess
ULONG WINAPI Hook_pfnNtOpenProcess(
                   __out PHANDLE ProcessHandle,
				   __in ACCESS_MASK AccessMask ,
				   __in PVOID ObjectAttributes,
				   __in PCLIENT_ID ClientId){
	
	DWORD pid = GetProcessIdByHandle( ProcessHandle);
	cout <<"计算出的pid："<< pid << endl;
	//通过进程句柄获取PID，然后验证
    if(gProtectProcessID == pid ){
		return 0xC0000022L;
	}
	else{
		return _NtOpenProcess(ProcessHandle, AccessMask, ObjectAttributes, ClientId);
	}
	
	//return result;
	//===================================
	//简单处理，直接设置ProcessHandle，保护所有
	ProcessHandle = NULL;
	return  _NtOpenProcess( ProcessHandle, AccessMask,ObjectAttributes,ClientId);
}



BOOL WINAPI DllMain(
    __in HINSTANCE  hInstance,
    __in DWORD      Reason,
    __in LPVOID     Reserved
    )
{        
    glhInstance = hInstance;
#pragma warning( disable : 4996)
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        Mhook_SetHook((PVOID*)&_NtOpenProcess, Hook_pfnNtOpenProcess);
        break;

    case DLL_PROCESS_DETACH:
        Mhook_Unhook((PVOID*)&_NtOpenProcess);
        break;
    }

    return TRUE;
}


//=================================================================
//继续昨天的通过，通过安装全局的消息钩子
//=================================================================
static LRESULT WINAPI ShellHookProc(int code, WPARAM wParam, LPARAM lParam) 
{
	return ::CallNextHookEx(glhHook, code, wParam, lParam);
}

extern "C" __declspec(dllexport)  BOOL InstallHook(DWORD pid)
{	
	BOOL bResult=FALSE;
	 //这里需要注意X86和X64下处理是不一样的
	#ifdef _M_IX86
		  glhHook = SetWindowsHookEx(WH_SHELL,ShellHookProc,glhInstance, 0);
	#elif defined _M_X64
		   glhHook = SetWindowsHookEx(WH_SHELL,ShellHookProc,0, 0);
	#endif

	if(glhHook!=NULL)
	{
		  gProtectProcessID = pid;
	      bResult=TRUE;
	}	

	return bResult; 
}


extern "C" __declspec(dllexport)  BOOL UninstallHook()
{	
	BOOL bResult=FALSE;
	if(glhHook)
	{
		bResult= UnhookWindowsHookEx(glhHook);
		if(bResult)
		{
			glhHook=NULL;
		}
	}
	return bResult;
}
