/*++

Module Name:

    driver.c

Abstract:

    This file contains the driver entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "driver.tmh"

//#define IOCTL_PROTECT_CONTROL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define NT_DEVICE_NAME      L"\\Device\\ProtectProcess"
#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#endif
UCHAR *PsGetProcessImageFileName(PEPROCESS EProcess);
pid = 0;
PVOID obHandle;//定义一个void*类型的变量，它将会作为ObRegisterCallbacks函数的第2个参数。 

static VOID EvtDriverUnload(WDFDRIVER Driver)
{
	DbgPrint(("unload driver\n"));
	DbgPrint(("Doesn't need to clean up the devices, since we only have control device here\n"));
	ObUnRegisterCallbacks(obHandle);
}/* EvtDriverUnload */

VOID EvtDeviceFileCreate(__in WDFDEVICE Device, __in WDFREQUEST Request, __in WDFFILEOBJECT FileObject)
{
	DbgPrint(("EvtDeviceFileCreate"));

	WdfRequestComplete(Request, STATUS_SUCCESS);
}

VOID EvtFileClose(__in  WDFFILEOBJECT FileObject)
{
	DbgPrint(("EvtFileClose"));
}
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT  DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry specifies the other entry
    points in the function driver, such as EvtDevice and DriverUnload.

Parameters Description:

    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverEntry must initialize members of DriverObject before it
    returns to the caller. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.

    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
	NTSTATUS        ntStatus = STATUS_SUCCESS;


	DbgPrint("驱动程序加载...\n");
	
    WDF_DRIVER_CONFIG config;
    NTSTATUS status;
	WDF_OBJECT_ATTRIBUTES object_attribs;
	UNICODE_STRING ustring;
	//驱动对象相关  
	WDFDRIVER drv = NULL;//wdf framework 驱动对象  

	//设备对象相关  
	PWDFDEVICE_INIT device_init = NULL;
	
	WDF_FILEOBJECT_CONFIG f_cfg;
	
	PDEVICE_OBJECT dev = NULL;
    //
    // Initialize WPP Tracing
    //
  WPP_INIT_TRACING( DriverObject, RegistryPath );

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");

	
    WDF_DRIVER_CONFIG_INIT(&config,NULL);
	config.DriverInitFlags = WdfDriverInitNonPnpDriver;  //指定非pnp驱动  
	config.DriverPoolTag = (ULONG)'PEPU';
	config.EvtDriverUnload = EvtDriverUnload;  //指定卸载函数  

    status = WdfDriverCreate(DriverObject,
                             RegistryPath,
							 WDF_NO_OBJECT_ATTRIBUTES,
                             &config,
                             WDF_NO_HANDLE
                             );

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDriverCreate failed %!STATUS!", status);
        WPP_CLEANUP(DriverObject);
        return status;
    }

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");
	 /* 
	device_init = WdfControlDeviceInitAllocate(drv, &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RW_RES_R);
	if (device_init == NULL)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		goto DriverEntry_Complete;
	}

	//创建设备的名字，内核模式下，名字类似: L"\\Device\\MyWDF_Device"  
	RtlInitUnicodeString(&ustring, NT_DEVICE_NAME);

	//将设备名字存入device_init中  
	status = WdfDeviceInitAssignName(device_init, &ustring);

	if (!NT_SUCCESS(status))
	{
		goto DriverEntry_Complete;
	}

	DbgPrint(("Device name Unicode string: %wZ (this name can only be used by other kernel mode code, like other drivers)\n", &ustring));

	//配置FILEOBJECT配置文件，设置FILECREATE,FILECLOSE回调。  
	WDF_FILEOBJECT_CONFIG_INIT(&f_cfg, EvtDeviceFileCreate, EvtFileClose, NULL);
	
	//将FILEOBJECT的设置存入device_init中  
	WdfDeviceInitSetFileObjectConfig(device_init, &f_cfg, WDF_NO_OBJECT_ATTRIBUTES);

	ProcessProtectorCreateDevice(device_init);
	*/
	PLDR_DATA ldr;

	// 绕过MmVerifyCallbackFunction。
	ldr = (PLDR_DATA)DriverObject->DriverSection;
	ldr->Flags |= 0x20;
	ntStatus = ProtectProcess(NULL);
	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("无法创建驱动设备.%ld\n", ntStatus);
		return ntStatus;
	}
	DbgPrint("驱动程序加载完毕.\n");
DriverEntry_Complete:
	return ntStatus;
}

NTSTATUS ProtectProcess(BOOLEAN Enable)
{

	OB_CALLBACK_REGISTRATION obReg;
	OB_OPERATION_REGISTRATION opReg;

	memset(&obReg, 0, sizeof(obReg));
	obReg.Version = ObGetFilterVersion();
	obReg.OperationRegistrationCount = 1;
	obReg.RegistrationContext = NULL;
	RtlInitUnicodeString(&obReg.Altitude, L"321000");

	memset(&opReg, 0, sizeof(opReg)); //初始化结构体变量

	//下面 请注意这个结构体的成员字段的设置
	opReg.ObjectType = PsProcessType;
	opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;

	opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall; //在这里注册一个回调函数指针

	obReg.OperationRegistration = &opReg; //注意这一条语句
	
	return ObRegisterCallbacks(&obReg, &obHandle); //在这里注册回调函数
}


OB_PREOP_CALLBACK_STATUS
preCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
	HANDLE _pid = PsGetProcessId((PEPROCESS)pOperationInformation->Object);
	UCHAR* pname = PsGetProcessImageFileName((PEPROCESS)pOperationInformation->Object);
	char szProcName[16] = { 0 };
	UNREFERENCED_PARAMETER(RegistrationContext);
	//DbgPrint("侦测到操作：%s %ld.\n", pname, pOperationInformation->Operation);
	/*strcpy(szProcName, GetProcessNameByProcessId(pid));
	if (!_stricmp(szProcName, "calc.exe"))*/
	if (!_stricmp(pname, "dnf2.exe") || !_stricmp(pname,"DProtector.exe") || !_stricmp(pname,"dnf2_no_login.exe"))
	/*DbgPrint("操作...%ld %ld", _pid, pid);*/
	/*if (_pid == pid)*/
	{
#define HASDESIRED(x) if((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & x) == x)pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~x;
		DbgPrint("操作...%ld,%ld\n", _pid, pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess);
		
		HASDESIRED(PROCESS_SUSPEND_RESUME)
		HASDESIRED(PROCESS_QUERY_INFORMATION)
		HASDESIRED(THREAD_QUERY_INFORMATION)
		
		/*if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE)
			{
				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
			}
			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_OPERATION) == PROCESS_VM_OPERATION)
			{
				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_OPERATION;
			}
			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_READ) == PROCESS_VM_READ)
			{
				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_READ;
			}
			if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_VM_WRITE) == PROCESS_VM_WRITE)
			{
				pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_VM_WRITE;
			}

		}*/
	}
	return OB_PREOP_SUCCESS;
}
