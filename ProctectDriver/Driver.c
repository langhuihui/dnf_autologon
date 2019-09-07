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
PVOID obHandle;//����һ��void*���͵ı�������������ΪObRegisterCallbacks�����ĵ�2�������� 

VOID OnUnload(IN PDRIVER_OBJECT DriverObject)
{
	
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrint("driver unloading...\n");
	ObUnRegisterCallbacks(obHandle); //obHandle�����涨��� PVOID obHandle;
}

NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT  DeviceObject, IN PIRP  Irp)
{//IRP_MJ_DEVICE_CONTROL����Ӧ����
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
		DbgPrint("IRP_MJ_CREATE ������\n");
		break;
	case IRP_MJ_CLOSE:
		DbgPrint("IRP_MJ_CLOSE ������\n");
		break;
	case IRP_MJ_DEVICE_CONTROL:
		DbgPrint("IRP_MJ_DEVICE_CONTROL ������\n");
		IoControlCode = IrpStack->Parameters.DeviceIoControl.IoControlCode;
		switch (IoControlCode)
		{
		case IOCTL_PROTECT_CONTROL:
			inSize = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
			outSize = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;
			inBuf = (long*)Irp->AssociatedIrp.SystemBuffer;

			pid = *inBuf;
			DbgPrint("===========================\n");
			DbgPrint("IOCTL_PROTECT_CONTROL ������,ͨѶ�ɹ�!\n");
			DbgPrint("���뻺������С: %d\n", inSize);
			DbgPrint("�����������С: %d\n", outSize);
			DbgPrint("���뻺��������: %ld\n", *inBuf);
			DbgPrint("��ǰ��������ID: %ld\n", pid);
			DbgPrint("===========================\n");
			
			strcpy((char *)Irp->UserBuffer, "OK!\n");
			break;
		default:
			break;
		}
		break;
	default:
		DbgPrint("δ֪�����������\n");
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
	
	//������ں���
	NTSTATUS        ntStatus = STATUS_SUCCESS;
	UNICODE_STRING  ntDeviceName;
	UNICODE_STRING  DeviceLinkString;
	PDEVICE_OBJECT  deviceObject = NULL;
	
	DbgPrint("�����������...\n");
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
		DbgPrint("�޷����������豸");
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

	DbgPrint("���������Ѿ�����\n");*/
	PLDR_DATA ldr;
	
	// �ƹ�MmVerifyCallbackFunction��
	ldr = (PLDR_DATA)DriverObject->DriverSection;
	ldr->Flags |= 0x20;
	ntStatus = ProtectProcess(NULL);
	if (!NT_SUCCESS(ntStatus))
	{
		DbgPrint("�޷����������豸.%ld\n", ntStatus);
		return ntStatus;
	}
	DbgPrint("��������������.\n");
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

	memset(&opReg, 0, sizeof(opReg)); //��ʼ���ṹ�����

	//���� ��ע������ṹ��ĳ�Ա�ֶε�����
	opReg.ObjectType = PsProcessType;
	opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;

	opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall; //������ע��һ���ص�����ָ��

	obReg.OperationRegistration = &opReg; //ע����һ�����

	return ObRegisterCallbacks(&obReg, &obHandle); //������ע��ص�����
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
	/*DbgPrint("����...%ld %ld", _pid, pid);
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
