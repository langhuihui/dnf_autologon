/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#define INITGUID

#include <ntddk.h>
#include <wdf.h>

#include "device.h"
#include "queue.h"
#include "trace.h"

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;

NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT  DeviceObject, IN PIRP  Irp);
VOID OnUnload(IN PDRIVER_OBJECT DriverObject);

#define NT_DEVICE_NAME      L"\\Device\\ProtectProcess"
#define DOS_DEVICE_NAME     L"\\DosDevices\\ProtectProcess"

#define IOCTL_PROTECT_CONTROL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS ProtectProcess(BOOLEAN Enable);
OB_PREOP_CALLBACK_STATUS preCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation);
typedef struct _LDR_DATA                         // 24 elements, 0xE0 bytes (sizeof)
{
	struct _LIST_ENTRY InLoadOrderLinks;                     // 2 elements, 0x10 bytes (sizeof)
	struct _LIST_ENTRY InMemoryOrderLinks;                   // 2 elements, 0x10 bytes (sizeof)
	struct _LIST_ENTRY InInitializationOrderLinks;           // 2 elements, 0x10 bytes (sizeof)
	VOID*        DllBase;
	VOID*        EntryPoint;
	ULONG32      SizeOfImage;
	UINT8        _PADDING0_[0x4];
	struct _UNICODE_STRING FullDllName;                      // 3 elements, 0x10 bytes (sizeof)
	struct _UNICODE_STRING BaseDllName;                      // 3 elements, 0x10 bytes (sizeof)
	ULONG32      Flags;
	UINT16       LoadCount;
	UINT16       TlsIndex;
	union
	{
		struct _LIST_ENTRY HashLinks;
		struct
		{
			VOID*        SectionPointer;
			ULONG32      CheckSum;
			UINT8        _PADDING1_[0x4];
		};
	};

	union
	{
		ULONG32      TimeDateStamp;
		VOID*        LoadedImports;
	};
	struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
	VOID*        PatchInformation;
	struct _LIST_ENTRY ForwarderLinks;
	struct _LIST_ENTRY ServiceTagLinks;
	struct _LIST_ENTRY StaticLinks;
	VOID*        ContextInformation;
	UINT64       OriginalBase;
	union _LARGE_INTEGER LoadTime;
}LDR_DATA, *PLDR_DATA;

#define OWNER_SECURITY_INFORMATION       (0X00000001L)
#define GROUP_SECURITY_INFORMATION       (0X00000002L)
#define DACL_SECURITY_INFORMATION        (0X00000004L)
#define SACL_SECURITY_INFORMATION        (0X00000008L)
#define PROCESS_TERMINATE         (0x0001)  
#define PROCESS_CREATE_THREAD     (0x0002)  
#define PROCESS_SET_SESSIONID     (0x0004)  
#define PROCESS_VM_OPERATION      (0x0008)  
#define PROCESS_VM_READ           (0x0010)  
#define PROCESS_VM_WRITE          (0x0020)  
#define PROCESS_DUP_HANDLE        (0x0040)  
#define PROCESS_CREATE_PROCESS    (0x0080)  
#define PROCESS_SET_QUOTA         (0x0100)  
#define PROCESS_SET_INFORMATION   (0x0200)  
#define PROCESS_QUERY_INFORMATION (0x0400)  
#define PROCESS_ALL_ACCESS        (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | \
                                   0xFFF)