#include "dm.h"

_DNF_BEGIN

Dm::Dm()
	:reg_code( "jerome105039931703737dc93d6fb8e8e3bd8c179fa" ),
	_cdnf( _dm )
{}

Dm::~Dm()
{
	unbind_wnd();
	_dm.ReleaseDispatch();
	::CoUninitialize();
}

void Dm::init()
{
	//reg_dll();
	//init_dm();
	//show_ver();

	//reg();

	//set_wnd_name( "计算器" );
	//find_wnd();

}

long Dm::reg()
{
	return _dm.Reg( (char*)reg_code.c_str(), "" );
}

uint32_t Dm::reg_dll()
{
	return WinExec("regsvr32 jerome10503.dll /s", SW_HIDE);
}

bool Dm::init_dm()
{
	::CoInitialize(NULL);
	return _dm.CreateDispatch(dm_object_name_str);
	//return _dm.CreateDispatch("dm.dmsoft");
}

void Dm::show_ver()
{
	MessageBox(NULL,_dm.Ver(),"",MB_OK);
}

void Dm::set_wnd_name( const string& wnd_name_ )
{
	wnd_name = wnd_name_;
}

bool Dm::find_wnd()
{
	wnd_hwnd = 0;
	//wnd_hwnd = (HWND)_cdnf.dm.DmFindWindow( 0, wnd_name.c_str() );
	wnd_hwnd = ::FindWindowA( 0, wnd_name.c_str() );
	if( wnd_hwnd )
		return true;
	else
		return false;
}

long Dm::bind_wnd()
{
	return _cdnf.BindWinEx( (long)wnd_hwnd );
}

void Dm::for_dnf_game_wnd()
{
#define _NO_SHOW_DM

#ifndef _NO_SHOW_DM
	trace( "开始向系统注册jerome10503.dll" );
#endif
	uint32_t reg_dll_ret = reg_dll();

	//init_dm();
	//show_ver();

#ifndef _NO_SHOW_DM

	trace( "向系统注册jerome10503.dll完成" );

	trace( "开始注册大漠插件..." );
#else
	trace( "OK 1" );

#endif
	long reg_ret = reg();

#ifndef _NO_SHOW_DM
	assert( reg_ret == 1 &&
		"dm插件没有注册成功"
		);
	if( !reg_ret )
		throw exception( "dm插件没有注册成功" );
#else
	assert( reg_ret == 1 &&
		"系统未授权"
		);
	if( !reg_ret )
		throw exception( "系统未授权" );
#endif


#ifndef _NO_SHOW_DM
	trace( "注册大漠插件完成" );
#else
	trace( "OK 2" );
#endif

	//if( reg_ret )
	//	::MessageBox( 0, "dm注册成功", "", 0 );

	set_wnd_name( "地下城与勇士" );

	find_wnd();

	assert( wnd_hwnd &&
		"找不到dnf窗口" );
	if( !wnd_hwnd )
		throw std::exception( "找不到dnf窗口" );


	_cdnf.SetAppPath( _cdnf.MyPath );

	_cdnf.InitializeAddrData();

	_cdnf.ActivateWin( (long)wnd_hwnd );
	Sleep(200);


	long bind_ret = bind_wnd();
	assert( bind_ret 
		&& "无法绑定dnf窗口" );
	if( !bind_ret )
		throw exception( "无法绑定dnf窗口" );

	_cdnf.InitializeDoorData();
}

void Dm::unbind_dnf()
{
	reg_dll();

	long reg_ret = reg();
	assert( reg_ret == 1 &&
		"dm插件没有注册成功"
		);
	if( !reg_ret )
		throw exception("dm插件没有注册成功");

	set_wnd_name( "地下城与勇士" );

	find_wnd();

	assert( wnd_hwnd &&
		"找不到dnf窗口" );
	if( !wnd_hwnd )
		throw exception( "找不到dnf窗口");


	_cdnf.ActivateWin( (long)wnd_hwnd );
	Sleep(200);

	long unbind_ret = unbind_wnd();
	assert( unbind_ret 
		&& "无法解除绑定dnf窗口" );
	if( !unbind_ret )
		throw exception( "无法解除绑定dnf窗口" );
}

long Dm::unbind_wnd()
{
	return _cdnf.UnBindWin();
}

void Dm::_test_key( int sleep_time )
{
	_cdnf.dm.KeyDown( 0x57 );
	Sleep( sleep_time );
	_cdnf.dm.KeyUp( 0x57 );
}

void Dm::_test()
{
	init();

	for_dnf_game_wnd();
}

void Dm::open_console()
{
	//FreeConsole();
	AllocConsole();
	freopen("CON", "r", stdin );
	freopen( "CON", "w", stdout);
	freopen("CON", "w", stderr);

	HWND hwnd = GetConsoleWindow();
	::MoveWindow( hwnd, 0, 600, 800, 380, true );
}

void Dm::restore_console()
{
	HWND hwnd = GetConsoleWindow();
	::ShowWindow( hwnd, SW_RESTORE );
}

void Dm::active_wnd()
{
	_dm.SetWindowState( (long)wnd_hwnd, 12 );
	Sleep( 500 );
}

void Dm::switch_ime()
{
	_dm.KeyDown( 0x11 );
	Sleep( 50 );
	_dm.KeyPress( 0x10 );
	Sleep( 50 );
	_dm.KeyUp( 0x11 );
}

_DNF_END