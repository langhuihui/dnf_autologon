
#include "ntddk.h"
#define NT_DEVICE_NAME      L"\\Device\\ProtectProcess"
#define DOS_DEVICE_NAME     L"\\DosDevices\\ProtectProcess"

#define IOCTL_PROTECT_CONTROL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS DispatchDeviceControl(IN PDEVICE_OBJECT  DeviceObject, IN PIRP  Irp);
VOID OnUnload(IN PDRIVER_OBJECT DriverObject);

#pragma pack(1) //SSDT表的结构
typedef struct ServiceDescriptorEntry {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; //Used only in checked build
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
}ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

__declspec(dllimport) ServiceDescriptorTableEntry_t KeServiceDescriptorTable; //变量名是不能变的，因为是从外部导入
//这个是查询某个函数的地址的一个宏
#define SYSTEMSERVICE(_function)  KeServiceDescriptorTable.ServiceTableBase[*(PULONG)((PUCHAR)_function+1)]

NTSYSAPI NTSTATUS NTAPI ZwOpenProcess(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId OPTIONAL);
typedef NTSTATUS(*ZWOPENPROCESS)(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId OPTIONAL);

ZWOPENPROCESS OldZwOpenProcess;
long pid = 0;

NTSTATUS NewZwOpenProcess(OUT PHANDLE ProcessHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes, IN PCLIENT_ID ClientId OPTIONAL)
{//用来替换的新函数
	NTSTATUS nStatus = STATUS_SUCCESS;
	if ((long)ClientId->UniqueProcess == pid)
	{
		DbgPrint("保护进程 PID:%ld\n", pid);
		return STATUS_ACCESS_DENIED;
	}

	//剩下的交给我们的原函数
	nStatus = OldZwOpenProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
	return STATUS_SUCCESS;
}

VOID OnUnload(IN PDRIVER_OBJECT DriverObject)
{
	//卸载时会调用
	UNICODE_STRING DeviceLinkString;
	PDEVICE_OBJECT DeviceObjectTemp1 = NULL;
	PDEVICE_OBJECT DeviceObjectTemp2 = NULL;
	DbgPrint("驱动程序卸载...\n");
	RtlInitUnicodeString(&DeviceLinkString, DOS_DEVICE_NAME);
	IoDeleteSymbolicLink(&DeviceLinkString);
	if (DriverObject)
	{
		DeviceObjectTemp1 = DriverObject->DeviceObject;
		while (DeviceObjectTemp1)
		{
			DeviceObjectTemp2 = DeviceObjectTemp1;
			DeviceObjectTemp1 = DeviceObjectTemp1->NextDevice;
			IoDeleteDevice(DeviceObjectTemp2);
		}
	}
	DbgPrint("设备已经卸载\n");
	DbgPrint("修复SSDT表\n");
	(SYSTEMSERVICE(ZwOpenProcess)) = OldZwOpenProcess;
	DbgPrint("驱动卸载完毕.\n");
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

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING theRegistryPath)
{
	//驱动入口函数
	NTSTATUS        ntStatus = STATUS_SUCCESS;
	UNICODE_STRING  ntDeviceName;
	UNICODE_STRING  DeviceLinkString;
	PDEVICE_OBJECT  deviceObject = NULL;
	DbgPrint("驱动程序加载...\n");
	RtlInitUnicodeString(&ntDeviceName, NT_DEVICE_NAME);
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
		return ntStatus;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	DriverObject->DriverUnload = OnUnload;

	DbgPrint("驱动程序已经启动\n");
	DbgPrint("修改SSDT表...\n");

	//修改 ZwOpenProcess 函数地址
	OldZwOpenProcess = (ZWOPENPROCESS)(SYSTEMSERVICE(ZwOpenProcess));
	(SYSTEMSERVICE(ZwOpenProcess)) = NewZwOpenProcess;
	DbgPrint("驱动程序加载完毕.\n");
	return STATUS_SUCCESS;
}