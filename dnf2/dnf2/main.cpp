


#include "main.h"

Main::Main( const std::string& qq, 
		const std::string& pwd, 
		const std::string& area,
		HWND daemon_hwnd 
		)
	:_qq( qq ), _pwd( pwd ), _area( area ),
	_daemon_hwnd( daemon_hwnd ),
	_login( _dm ),
	_world( _dm, _attach_ime ),
	_game( _world , qq )
{
}

void Main::run()
{
//	_attach_ime.set_en();

	glog( "设置守护进程句柄" );
	if( !Message::set_daemon_hwnd_from_deamon( 
		_daemon_hwnd )
		)
	{
		glog( "设置守护进程句柄异常" );
	}

	glog( "打开控制台" );

	_dm.open_console();

	_attach_ime.set_ime();

	//cout << "===============================Main::run():01" << endl;

	_login.KeeperHandle = Message::get_daemon_hwnd();

	glog( "设置置登录模块守护进程句柄" );
	glog( _login.KeeperHandle );

	//cout << "===============================Main::run():02" << endl;
	cout.clear();

	output( "进入登录程序" );

	_login.Login( (char*)_qq.c_str(), ( char* )_pwd.c_str(), (char*)_area.c_str() );

	output( "登录程序完成" );

	//cout << "===============================Main::run():03" << endl;

	_DNF::Message::pluse( 300, 1 ) ;

	//cout << "===============================Main::run():04" << endl;

	output( "进入游戏界面" );
	 
	_game.run();
}
	
int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
try
{
	//assert( false );

	string cmdline = lpCmdLine;
	istringstream iss( cmdline );

	glog( "命令行:lpCmdLine:" );
	glog( cmdline );

	//string path;
	long daemon_hwnd;
	string qq;
	string pwd;
	string area;

	//iss >> path;
	iss >> daemon_hwnd;
	iss >> qq;
	iss >> pwd;
	iss >> area;

	glog( "收到启动消息" );
	glog( qq );
	glog( area );

	Main main( qq, pwd, area, (HWND)daemon_hwnd );

	main.run();

	return 0;
}
catch( std::exception& e )
{
	MessageBox( 0, e.what(), "dnf2异常", 0 );
}
