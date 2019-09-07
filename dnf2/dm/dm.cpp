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

	//set_wnd_name( "������" );
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
	trace( "��ʼ��ϵͳע��jerome10503.dll" );
#endif
	uint32_t reg_dll_ret = reg_dll();

	//init_dm();
	//show_ver();

#ifndef _NO_SHOW_DM

	trace( "��ϵͳע��jerome10503.dll���" );

	trace( "��ʼע���Į���..." );
#else
	trace( "OK 1" );

#endif
	long reg_ret = reg();

#ifndef _NO_SHOW_DM
	assert( reg_ret == 1 &&
		"dm���û��ע��ɹ�"
		);
	if( !reg_ret )
		throw exception( "dm���û��ע��ɹ�" );
#else
	assert( reg_ret == 1 &&
		"ϵͳδ��Ȩ"
		);
	if( !reg_ret )
		throw exception( "ϵͳδ��Ȩ" );
#endif


#ifndef _NO_SHOW_DM
	trace( "ע���Į������" );
#else
	trace( "OK 2" );
#endif

	//if( reg_ret )
	//	::MessageBox( 0, "dmע��ɹ�", "", 0 );

	set_wnd_name( "���³�����ʿ" );

	find_wnd();

	assert( wnd_hwnd &&
		"�Ҳ���dnf����" );
	if( !wnd_hwnd )
		throw std::exception( "�Ҳ���dnf����" );


	_cdnf.SetAppPath( _cdnf.MyPath );

	_cdnf.InitializeAddrData();

	_cdnf.ActivateWin( (long)wnd_hwnd );
	Sleep(200);


	long bind_ret = bind_wnd();
	assert( bind_ret 
		&& "�޷���dnf����" );
	if( !bind_ret )
		throw exception( "�޷���dnf����" );

	_cdnf.InitializeDoorData();
}

void Dm::unbind_dnf()
{
	reg_dll();

	long reg_ret = reg();
	assert( reg_ret == 1 &&
		"dm���û��ע��ɹ�"
		);
	if( !reg_ret )
		throw exception("dm���û��ע��ɹ�");

	set_wnd_name( "���³�����ʿ" );

	find_wnd();

	assert( wnd_hwnd &&
		"�Ҳ���dnf����" );
	if( !wnd_hwnd )
		throw exception( "�Ҳ���dnf����");


	_cdnf.ActivateWin( (long)wnd_hwnd );
	Sleep(200);

	long unbind_ret = unbind_wnd();
	assert( unbind_ret 
		&& "�޷������dnf����" );
	if( !unbind_ret )
		throw exception( "�޷������dnf����" );
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