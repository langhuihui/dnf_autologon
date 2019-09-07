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
	glog( "����dnf2_no_login" );
	Dm dm;
	glog( "��������̨" );
	dm.open_console();

	string cmdline = lpCmdLine;
	istringstream iss( cmdline );

	glog( "������:lpCmdLine:" );
	glog( cmdline );

	//string path;
	long daemon_hwnd = 0;

	//iss >> path;
	iss >> daemon_hwnd;

	if( daemon_hwnd )
	{
		output( "�����ػ����̾��" );
		output( daemon_hwnd );
		Message::set_daemon_hwnd_from_deamon(
			(HWND)daemon_hwnd );
	}
	else
		output( "�ػ����̾���쳣:Ϊ0" );


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

	output( "����Ϸ����" );

	dm.for_dnf_game_wnd();
	glog( "��ʼ��game" );
	_GAME::Game game( world, "0" );

	game.run_no_login();

	return 0;
}
catch( std::exception& e )
{
	::MessageBoxA( 0, e.what(), "dnf2�쳣", 0 );
}
catch( ... )
{
	::MessageBoxA( 0, "�쳣", "dnf2�쳣", 0 );
}