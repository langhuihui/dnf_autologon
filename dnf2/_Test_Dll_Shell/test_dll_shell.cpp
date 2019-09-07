#include <test.h>

#pragma comment( lib, "_Test_Dll.lib" )

#define _DllExport __declspec(dllimport)

#include <Windows.h>

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{

	Test test;
	test.main();

	return 0;
}