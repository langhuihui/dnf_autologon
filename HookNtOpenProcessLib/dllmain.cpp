// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
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
// ����
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
		cout << "��ȡ������Ϣ��" << ret << endl;
	}
	else{
		cout << "ZwQueryInformationProcessδ��ȡ��" << endl;
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
//���������Լ���NtOpenProcess
ULONG WINAPI Hook_pfnNtOpenProcess(
                   __out PHANDLE ProcessHandle,
				   __in ACCESS_MASK AccessMask ,
				   __in PVOID ObjectAttributes,
				   __in PCLIENT_ID ClientId){
	
	DWORD pid = GetProcessIdByHandle( ProcessHandle);
	cout <<"�������pid��"<< pid << endl;
	//ͨ�����̾����ȡPID��Ȼ����֤
    if(gProtectProcessID == pid ){
		return 0xC0000022L;
	}
	else{
		return _NtOpenProcess(ProcessHandle, AccessMask, ObjectAttributes, ClientId);
	}
	
	//return result;
	//===================================
	//�򵥴���ֱ������ProcessHandle����������
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
//���������ͨ����ͨ����װȫ�ֵ���Ϣ����
//=================================================================
static LRESULT WINAPI ShellHookProc(int code, WPARAM wParam, LPARAM lParam) 
{
	return ::CallNextHookEx(glhHook, code, wParam, lParam);
}

extern "C" __declspec(dllexport)  BOOL InstallHook(DWORD pid)
{	
	BOOL bResult=FALSE;
	 //������Ҫע��X86��X64�´����ǲ�һ����
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
