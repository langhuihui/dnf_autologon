#include <sstream>
using namespace std;

#include <AutoLogin.h>
#pragma comment( lib, "login.lib" )
#define _DllExport __declspec(dllimport)
using namespace _DNF;

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	string cmdline = lpCmdLine;
	istringstream iss( cmdline );

	string myself;
	string QQ;
	string Password;
	string Area;

	iss >> myself;
	iss >> QQ;
	iss >> Password;
	iss >> Area;

	Dm dm;
			
	AutoLogin auto_login( dm );
	auto_login.KeeperHandle = (HWND)0;
	auto_login.Login((char*)QQ.c_str(), (char*)Password.c_str(), (char*)Area.c_str());

	return 0;
}
