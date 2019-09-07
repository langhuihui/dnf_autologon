


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

	glog( "�����ػ����̾��" );
	if( !Message::set_daemon_hwnd_from_deamon( 
		_daemon_hwnd )
		)
	{
		glog( "�����ػ����̾���쳣" );
	}

	glog( "�򿪿���̨" );

	_dm.open_console();

	_attach_ime.set_ime();

	//cout << "===============================Main::run():01" << endl;

	_login.KeeperHandle = Message::get_daemon_hwnd();

	glog( "�����õ�¼ģ���ػ����̾��" );
	glog( _login.KeeperHandle );

	//cout << "===============================Main::run():02" << endl;
	cout.clear();

	output( "�����¼����" );

	_login.Login( (char*)_qq.c_str(), ( char* )_pwd.c_str(), (char*)_area.c_str() );

	output( "��¼�������" );

	//cout << "===============================Main::run():03" << endl;

	_DNF::Message::pluse( 300, 1 ) ;

	//cout << "===============================Main::run():04" << endl;

	output( "������Ϸ����" );
	 
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

	glog( "������:lpCmdLine:" );
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

	glog( "�յ�������Ϣ" );
	glog( qq );
	glog( area );

	Main main( qq, pwd, area, (HWND)daemon_hwnd );

	main.run();

	return 0;
}
catch( std::exception& e )
{
	MessageBox( 0, e.what(), "dnf2�쳣", 0 );
}
