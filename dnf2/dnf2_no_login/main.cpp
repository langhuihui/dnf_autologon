#include <game.h>

#pragma comment( lib, "game.lib" )

#define _DllExport __declspec(dllimport)

using namespace _DNF;

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
try
{
	glog( "开启dnf2_no_login" );
	Dm dm;
	glog( "开启控制台" );
	dm.open_console();

	string cmdline = lpCmdLine;
	istringstream iss( cmdline );

	glog( "命令行:lpCmdLine:" );
	glog( cmdline );

	//string path;
	long daemon_hwnd = 0;

	//iss >> path;
	iss >> daemon_hwnd;

	if( daemon_hwnd )
	{
		output( "设置守护进程句柄" );
		output( daemon_hwnd );
		Message::set_daemon_hwnd_from_deamon(
			(HWND)daemon_hwnd );
	}
	else
		output( "守护进程句柄异常:为0" );


	AttachIme attach_ime;

	Message::pluse( 60, 2 );

	World world( dm, attach_ime );

	if( !world.has_process( "DNF.exe" ) )
	{
		Message::restart_all();
		glog( "Message::restart_all()" );
		trace( "Message::restart_all()" );
		return -1;
	}

	output( "绑定游戏窗口" );

	dm.for_dnf_game_wnd();
	glog( "初始化game" );
	_GAME::Game game( world, "0" );

	game.run_no_login();

	return 0;
}
catch( std::exception& e )
{
	::MessageBoxA( 0, e.what(), "dnf2异常", 0 );
}
catch( ... )
{
	::MessageBoxA( 0, "异常", "dnf2异常", 0 );
}