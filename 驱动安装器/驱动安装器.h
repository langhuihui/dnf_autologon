// 驱动安装器.h

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
using namespace System;
using namespace System::Runtime::InteropServices;
namespace 驱动安装器 {
#define IOCTL_HELLO_CONTROL CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
	public ref class ProtectInstaller
	{
		SC_HANDLE rh = NULL;
		SC_HANDLE sh = NULL;
		char * name;
		char *ManagedString2UnmanagedAnsiString(String ^ str)
		{
			IntPtr p = Marshal::StringToHGlobalAnsi(str);
			if (p == IntPtr::Zero)
				return NULL;

			const char *pTemp = static_cast<char*>(p.ToPointer());
			if (pTemp == NULL) return NULL;

			char *pOut = new char[strlen(pTemp) + 1];
			strcpy(pOut, pTemp);
			Marshal::FreeHGlobal(p);
			return pOut;
		}
	public:
		ProtectInstaller(){
			sh = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		}
		~ProtectInstaller(){
			CloseServiceHandle(sh);
			CloseServiceHandle(rh);
		}
		bool Start(){
			return StartService(rh, NULL, NULL);
		}
		bool _OpenService(){
			rh = OpenService(sh, this->name, SERVICE_ALL_ACCESS);
			if (!rh){
				return false;
			}
			return true;
		}
		 DWORD Install(String^ name, String^ disname, String^ path){
			this->name = ManagedString2UnmanagedAnsiString(name);
			rh = CreateService(sh, this->name, ManagedString2UnmanagedAnsiString(disname),
				SERVICE_ALL_ACCESS,
				SERVICE_KERNEL_DRIVER,
				//{
				//SERVICE_SYSTEM_START,
				SERVICE_DEMAND_START,
				//}
				SERVICE_ERROR_NORMAL,
				ManagedString2UnmanagedAnsiString(path),
				NULL, NULL, NULL, NULL, NULL);

			if (!rh){
				Console::WriteLine("[-]error CreateService");
				return 1;
				//printf("[-]error CreateService.\n");
				//exit(0);
			}
			rh = OpenService(sh, this->name, SERVICE_ALL_ACCESS);
			if (!rh){
				Console::WriteLine("[-]error OpenService");
				return 2;
				//printf("error OpenService.\n");
				//exit(0);
			}
			Console::WriteLine("[-]Install Service Complete");
			
			return GetLastError();
			//printf("[-]Install Service Complete...\n");
			//CloseServiceHandle(rh);
		}
		 void Stop(){
			 SERVICE_STATUS ss;
			 ControlService(rh, SERVICE_CONTROL_STOP, &ss);
			 printf("[-]Stop Service Complete...\n");
		 }
		 void  Uninstall(){
			Stop();
			DeleteService(rh);
			printf("[-]Delete Service Complete...\n");
		}
		 bool SetPID(long pid){
			 
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
			
			 DeviceIoControl(hDevice, IOCTL_HELLO_CONTROL, &pid, sizeof(long), ret, 4096, &ReBytes, NULL);

			 printf("Return Value:%s\n", ret);

			 CloseHandle(hDevice);
			 return true;
		 }
	};
}
