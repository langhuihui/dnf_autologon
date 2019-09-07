#include "message.h"

_DNF_BEGIN

Message::Message()
{
}

HWND Message::get_daemon_hwnd()
{
	if( _daemon_hwnd_from_daemon )
		return _daemon_hwnd_from_daemon;
	else if( _daemon_hwnd )
		return _daemon_hwnd;
	else
		output( "守护程序句柄异常" );
		//throw std::exception( "守护程序句柄异常" );
}

bool Message::set_daemon_hwnd_from_deamon(
	HWND hwnd )
{
	if( hwnd )
	{
		_daemon_hwnd_from_daemon = hwnd;
		return true;
	}
	else
		return false;
}

void Message::unusual()
{
	send( WM_USER + 100 );
}
void Message::complete()
{
	send( WM_USER + 101 );
}
void Message::pluse( size_t limit_time, int restart_exe )
{
	//cout << "Message::pluse():01" << endl;
	//cout << limit_time << "\t" << restart_exe << endl;
	send( WM_USER + 102, limit_time, restart_exe );
	//cout << "Message::pluse():02" << endl;
}
void Message::safemode()
{
	send( WM_USER + 103 );
}
void Message::thirdpart()
{
	send( WM_USER + 104 );
}
void Message::forbidden()
{
	send( WM_USER + 105 );
}
void Message::block()
{
	send( WM_USER + 106 );
}

void Message::restart_without_login()
{
	send( WM_USER + 107 );
}

void Message::switch_character()
{
	send( WM_USER + 108 );
}

void Message::restart_all()
{
	send( WM_USER + 109 );
}

//void Message::pid()
//{
//	send( WM_USER + 107,  );
//}


void Message::send( int msg, int w, int l )
{
	//assert( msg > WM_USER
	//	&& "Message::send:发送消息错误" );

	//cout << "Message::send(: 01" << endl;
	//cout << msg << "\t" << w << "\t" << l << endl;
	if( msg <= WM_USER )
		trace( "Message::send:发送消息错误" );

	//cout << "Message::send(: 02" << endl;
	//cout << _daemon_hwnd << endl;
	//assert( _daemon_hwnd
	//	&& "Message::send:_daemon_hwnd未设置" );
	if( !_daemon_hwnd )
		trace( "Message::send:_daemon_hwnd未设置" );

	//cout << "Message::send(: 03" << endl;
	output( "发送消息..." );
	output( msg );

	glog( "Message::send:.start" );
	glog( msg );
	::SendMessage( get_daemon_hwnd(), msg, w, l );
	glog( ::GetLastError() );
	glog( "GetLastError():" );
	glog( "Message::send:.end" );

	output( "发送完毕" );
	//cout << "Message::send(: 03" << endl;
	//assert( GetLastError()
	//	&& "Message::send:发送消息失败" );
	//if( GetLastError() )
	//{
	//	trace( "Message::send:GetLastError返回失败" );
	//}
	//cout << GetLastError() << endl;
}

string Message::_daemon_wnd_name = "守护程序"; //守护进程的名字
HWND Message::_daemon_hwnd = FindWindow( 
	0, Message::_daemon_wnd_name.c_str() );

HWND Message::_daemon_hwnd_from_daemon = (HWND)0;

_DNF_END