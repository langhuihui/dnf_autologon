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
#include <stdio.h>
#include <stdlib.h>

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
//#pragma alloc_text (PAGE, ProctectDriverEvtDeviceAdd)
//#pragma alloc_text (PAGE, ProctectDriverEvtDriverContextCleanup)
#endif
UCHAR *PsGetProcessImageFileName(PEPROCESS EProcess);
long pid = 0;
PVOID obHandle;//定义一个void*类型的变量，它将会作为ObRegisterCallbacks函数的第2个参数。 

VOID OnUnload(IN PDRIVER_OBJECT DriverObject)
{
	
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrint("driver unloading...\n");
	ObUnRegisterCallbacks(obHandle); //obHandle是上面定义的 PVOID obHandle;
}

NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT  DeviceObject, IN PIRP  Irp)
{//IRP_MJ_DEVICE_CONTROL的响应函数
	NTSTATUS nStatus = STATUS_SUCCESS;
	ULONG IoControlCode = 0;
	PIO_STACK_LOCATION IrpStack = NULL;

	long* inBuf = NULL;
	char* outBuf = NULL;
	ULONG inSize = 0;
	ULONG outSize = 0;
	PCHAR buffer = NULL;
	PMDL mdl = NULL;
	
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;

	IrpStack = IoGetCurrentIrpStackLocation(Irp);
	switch (IrpStack->MajorFunction)
	{
	case IRP_MJ_CREATE:
		DbgPrint("IRP_MJ_CREATE 被调用\n");
		break;
	case IRP_MJ_CLOSE:
		DbgPrint("IRP_MJ_CLOSE 被调用\n");
		break;
	case IRP_MJ_DEVICE_CONTROL:
		DbgPrint("IRP_MJ_DEVICE_CONTROL 被调用\n");
		IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
		switch (IoControlCode)
		{
		case IOCTL_PROTECT_CONTROL:
			inSize = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
			outSize = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
			inBuf = (long*)Irp->AssociatedIrp.SystemBuffer;

			pid = *inBuf;
			DbgPrint("===========================\n");
			DbgPrint("IOCTL_PROTECT_CONTROL 被调用,通讯成功!\n");
			DbgPrint("输入缓冲区大小: %d\n", inSize);
			DbgPrint("输出缓冲区大小: %d\n", outSize);
			DbgPrint("输入缓冲区内容: %ld\n", *inBuf);
			DbgPrint("当前保护进程ID: %ld\n", pid);
			DbgPrint("===========================\n");
			
			strcpy((char *)Irp->UserBuffer, "OK!\n");
			break;
		default:
			break;
		}
		break;
	default:
		DbgPrint("未知请求包被调用\n");
		break;
	}
	nStatus = Irp->IoStatus.Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return nStatus;
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
	
	//驱动入口函数
	NTSTATUS        ntStatus = STATUS_SUCCESS;
	UNICODE_STRING  ntDeviceName;
	UNICODE_STRING  DeviceLinkString;
	PDEVICE_OBJECT  deviceObject = NULL;
	
	DbgPrint("驱动程序加载...\n");
	/*RtlInitUnicodeString(&ntDeviceName, NT_DEVICE_NAME);
	ntStatus = IoCreateDevice(
		DriverObject,
		0,
		&ntDeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		FALSE,
		&deviceObject);

	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("无法创建驱动设备");
		return ntStatus;
	}
	RtlInitUnicodeString(&DeviceLinkString, DOS_DEVICE_NAME);
	ntStatus = IoCreateSymbolicLink(&DeviceLinkString, &ntDeviceName);
	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("IoCreateSymbolicLink faild");
		return ntStatus;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	DriverObject->DriverUnload = OnUnload;

	DbgPrint("驱动程序已经启动\n");*/
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
	return STATUS_SUCCESS;
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
	
	/*strcpy(szProcName, GetProcessNameByProcessId(pid));
	if (!_stricmp(szProcName, "calc.exe"))*/
	if (!_stricmp(pname, "calc.exe"))
	/*DbgPrint("操作...%ld %ld", _pid, pid);
	if (_pid == pid)*/
	{
		if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
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
		}
	}
	return OB_PREOP_SUCCESS;
}
