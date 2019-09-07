// HookNtOpenProcessLib.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "mhook.h"
#include <wdm.h>
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

DWORD GetProcessIDByHandle(HANDLE ProcessHandle)
{
	ULONG                         lRet;
	PVOID                         pBuffer;
	PROCESS_BASIC_INFORMATION     *pbi;
	DWORD ProcessID;
	DWORD ret;
	lRet = 0;

	pBuffer = ExAllocatePool(PagedPool, sizeof(PROCESS_BASIC_INFORMATION));
	if (NULL != pZwQueryInformationProcess)
	{

		ret = pZwQueryInformationProcess(ProcessHandle, ProcessBasicInformation,
			pBuffer, sizeof(PROCESS_BASIC_INFORMATION), &lRet);
	}

	pbi = (struct _PROCESS_BASIC_INFORMATION *)pBuffer;
	ProcessID = pbi->UniqueProcessId;
	ExFreePool(pBuffer);
	return ProcessID;

}
//===========================================================
// NtOpenProcess
typedef ULONG(WINAPI *pfnNtOpenProcess)(
	__out PHANDLE ProcessHandle,
	__in ACCESS_MASK AccessMask,
	__in PVOID ObjectAttributes,
	__in PCLIENT_ID ClientId);

pfnNtOpenProcess _NtOpenProcess = (pfnNtOpenProcess)GetProcAddress(GetModuleHandle(L"ntdll"), "NtOpenProcess");
DWORD gProtectProcessID = NULL;
HHOOK glhHook;
HINSTANCE glhInstance;
//===========================================================
//���������Լ���NtOpenProcess
ULONG WINAPI Hook_pfnNtOpenProcess(
	__out PHANDLE ProcessHandle,
	__in ACCESS_MASK AccessMask,
	__in PVOID ObjectAttributes,
	__in PCLIENT_ID ClientId){
	ULONG result = _NtOpenProcess(ProcessHandle, AccessMask, ObjectAttributes, ClientId); 
	DWORD pid = GetProcessIDByHandle(ProcessHandle);
	//ͨ�����̾����ȡPID��Ȼ����֤
	if(gProtectProcessID == pid ){
		return 0xC0000022L;
	}

	return result;
	//===================================
	//�򵥴���ֱ������ProcessHandle����������
	ProcessHandle = NULL;
	return  _NtOpenProcess(ProcessHandle, AccessMask, ObjectAttributes, ClientId);
}

extern "C" __declspec(dllexport)  BOOL InstallHook(DWORD pid)
{
	BOOL bResult = FALSE;
	gProtectProcessID = pid;
	//������Ҫע��X86��X64�´����ǲ�һ����
	return Mhook_SetHook(&(PVOID&)_NtOpenProcess, (PVOID)Hook_pfnNtOpenProcess);
//#ifdef _M_IX86
//	glhHook = SetWindowsHookEx(WH_SHELL, ShellHookProc, glhInstance, 0);
//#elif defined _M_X64
//	glhHook = SetWindowsHookEx(WH_SHELL, ShellHookProc, 0, 0);  //��������Ϊ0�������ǵ�ǰģ���ʵ����� 
//#endif

	if (glhHook != NULL)
	{
		gProtectProcessID = pid;
		bResult = TRUE;
	}

	return bResult;
}
extern "C" __declspec(dllexport)  BOOL UnInstallHook(){
	return Mhook_Unhook(&(PVOID&)_NtOpenProcess);
	//UnhookWindowsHookEx(glhHook);
}