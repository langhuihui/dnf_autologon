#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>

#define BUF_SIZE 4096
#define IOCTL_HELLO_CONTROL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
int main(int argc, char* argv[])
{
	char path[BUF_SIZE];
	char base[BUF_SIZE];
	char sername[BUF_SIZE];
	char disname[BUF_SIZE];
	memset(path, 0, BUF_SIZE);
	memset(base, 0, BUF_SIZE);
	memset(sername, 0, BUF_SIZE);
	memset(disname, 0, BUF_SIZE);

	SC_HANDLE rh = NULL;
	SC_HANDLE sh = NULL;
	if (argc == 1)
	{
		printf("use: install/start/uninstall\n");
		exit(0);
	}
	else if (argc == 3 && !strcmp(argv[1],"set")){
		long pid = atol(argv[2]);
		char ret[4096];
		DWORD ReBytes = 0;
		HANDLE hDevice = CreateFile("\\\\.\\ProtectProcess", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			if (2 == GetLastError())
			{
				printf("驱动程序未注册\n");
			}
			else
				printf("CreateFile() GetLastError reports %d\n", GetLastError());
			return false;
		}

		memset(ret, 0, 4096);
		//printf("请输入需要保护的进程PID:");
	
		WriteFile(hDevice, &pid, 4, &ReBytes, NULL);
		//DeviceIoControl(hDevice, IOCTL_HELLO_CONTROL, &pid, sizeof(long), ret, 4096, &ReBytes, NULL);

		printf("Return Value:%d\n", ReBytes);

		CloseHandle(hDevice);
		return 1;
	}
	::GetModuleFileName(0, base, BUF_SIZE);
	int p = strlen(base);
	while (base[p] != '\\'){ p--; }
	strncpy(path, base, p + 1);
	memset(base, 0, BUF_SIZE);
	sprintf(base, "%sInstall.ini", path);
	memset(path, 0, BUF_SIZE);
	::GetPrivateProfileString("Config", "Path", "", path, BUF_SIZE, base);
	::GetPrivateProfileString("Config", "ServiceName", "", sername, BUF_SIZE, base);
	::GetPrivateProfileString("Config", "DisplayName", "", disname, BUF_SIZE, base);

	printf("[*]Service Name:%s\n", sername);
	printf("[*]Display Name:%s\n", disname);
	printf("[*]Driver  Path:%s\n", path);
	sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!sh){
		printf("[-]Error OpenSCManger.\n");
		exit(0);
	}

	if (!strcmp(argv[1], "install"))
	{
		if (!strcmp(path, ""))
		{
			printf("[-]error read Install.ini\n");
			exit(0);
		}

		rh = CreateService(sh, sername, disname,
			SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			//{
			//SERVICE_SYSTEM_START,
			SERVICE_DEMAND_START,
			//}
			SERVICE_ERROR_NORMAL,
			path,
			NULL, NULL, NULL, NULL, NULL);

		if (!rh){
			printf("[-]error CreateService.%ld\n",GetLastError());
			exit(0);
		}
		/*if (!StartService(rh, 1, &argv[2])){
			printf("error OpenService.%ld\n", GetLastError());
			exit(0);
		}*/
		printf("[-]Install Service Complete...\n");
	}
	else if (!strcmp(argv[1], "start"))
	{
		rh = OpenService(sh, sername, SERVICE_ALL_ACCESS);
		if (!rh || !StartService(rh, NULL, NULL)){
			printf("error OpenService.%ld\n", GetLastError());
			exit(0);
		}
		printf("[-]Start Service Complete...\n");
	}
	else if (!strcmp(argv[1], "uninstall"))
	{
		rh = OpenService(sh, sername, SERVICE_ALL_ACCESS);
		if (!rh){
			printf("error OpenService.\n");
			exit(0);
		}
		
		SERVICE_STATUS ss;
		ControlService(rh, SERVICE_CONTROL_STOP, &ss);
		printf("[%ld]Stop Service Complete...%ld\n",GetLastError(),ss.dwCurrentState);
		DeleteService(rh);
		printf("[%ld]Delete Service Complete...\n", GetLastError());
	}
	CloseServiceHandle(rh);
	CloseServiceHandle(sh);
	return 1;
}