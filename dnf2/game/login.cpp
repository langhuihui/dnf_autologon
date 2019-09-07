#include "login.h"

_GAME_BEGIN

Name::Name()
{
}

string Name::gen_new()
{
	string name_num; 
	string name_str; 
	name_str = "abcdefghijklmnopqrstuvwxyz";
	random_shuffle( name_str.begin(), name_str.end(), random() );
	string name = name_str.substr( 0, 5 );
	
	if( 'L' == name[ 0 ] )
		name[ 0 ] = 'A';

	name_num = "1234567890";
	random_shuffle( name_num.begin(), name_num.end(), random() );
	name += name_num.substr( 0, 4 );
	return name;
}

SelectPlayerBase::SelectPlayerBase( World& world )
	:_world( world ),
	_first_pos( 69,87 ),
	_first_pos2( 187,291 ),
	_x_offset( 122 ),
	_y_offset( 210 ),
	_info_start_offset_pos( 5, 161 ),
	_info_end_offset_pos( 113, 199 )
{
}

void SelectPlayerBase::get_frame_info_pos(
			const Pos& frame_pos,
			Pos& info_start_pos,
			Pos& info_end_pos )
{
	frame_pos.corrent_assert();

	info_start_pos.x = frame_pos.x 
		+ _info_start_offset_pos.x;
	info_start_pos.y = frame_pos.y
		+ _info_start_offset_pos.y;

	info_end_pos.x = frame_pos.x
		+ _info_end_offset_pos.x;
	info_end_pos.y = frame_pos.y
		+ _info_end_offset_pos.y;
}


Pos SelectPlayerBase::get_frame_pos( size_t num )
{
	assert( num >= 0
		&& num < 8
		&& "SelectPlayerBase::get_frame_pos:num������Χ" );

	Pos pos = _first_pos;
	
	pos.x += ( num % 4 ) * _x_offset;
	pos.y += ( num / 4 ) * _y_offset;

	return pos;
}

Pos SelectPlayerBase::get_frame_pos2( size_t num )
{
	assert( num >= 0
		&& num < 8
		&& "SelectPlayerBase::get_frame_pos:num������Χ" );

	Pos pos = _first_pos2;
	
	pos.x += ( num % 4 ) * _x_offset;
	pos.y += ( num / 4 ) * _y_offset;

	return pos;
}


int SelectPlayerBase::selected_frame_num()
{
	glog( "SelectPlayerBase::selected_frame_num():��ǰѡ��ĵڼ�����" );
	Pos pos;
	for( size_t i = 0; i < 8; i++ )
	{
		pos = get_frame_pos( i );
		if( 
			_world.dnf.has_pos_color( 
			pos.x, pos.y, "1dffff-120202" )
			)
		{
			glog( i );
			return i;
		}

		pos = get_frame_pos2( i );
		if( 
			_world.dnf.has_pos_color( 
			pos.x, pos.y, "1dffff-200808" )
			)
		{
			glog( i );
			return i;
		}
	}

	/*
	assert( false
		&& "SelectPlayerBase::selected_frame_num():û��ѡ���ɫ��" );
		*/
	trace( "SelectPlayerBase::selected_frame_num():û��ѡ���ɫ��" );

	return -1;
}

int SelectPlayerBase::selected_frame_column()
{
	return selected_frame_num() % 4;
}

bool SelectPlayerBase::is_selected_frame()
{
	if( -1 != selected_frame_num() )
		return true;
	else
		return false;
}

void SelectPlayerBase::set_safe_mouse()
{
	_world.dnf.dm.MoveTo( 760, 40 );
	_world.dnf.dm.LeftClick();
	_world.dnf.dm.LeftClick();
	_world.dnf.dm.LeftClick();
	_world.dnf.dm.LeftClick();
}

void SelectPlayerBase::up()
{
	_world.player.key_press( VK_UP, 20 );
	Sleep( 20 );
}

void SelectPlayerBase::down()
{
	_world.player.key_press( VK_DOWN, 20 );
	Sleep( 20 );
}

void SelectPlayerBase::left()
{
	_world.player.key_press( VK_LEFT, 20 );
	Sleep( 20 );
}

void SelectPlayerBase::right()
{
	_world.player.key_press( VK_RIGHT, 20 );
	Sleep( 20 );
}

bool SelectPlayerBase::is_selected_first_frame()
{
	if( 0 == selected_frame_num() )
		return true;
	else
		return false;
}

bool SelectPlayerBase::is_unchange_info( const string& info )
{
	//trace( _last_info );
	//trace( info );
	glog( "SelectPlayerBase::is_unchange_info:" );
	glog( _last_info );
	glog( info );
	if( info != _last_info )
		return false;
	else
		return true;
}

void SelectPlayerBase::to_first()
{

	set_safe_mouse();

	block_call(
		boost::bind( &SelectPlayerBase::is_selected_first_frame, this ),
		boost::bind( null ),
		boost::bind( &SelectPlayerBase::up, this ) );

	_last_info = get_info( 0 );

	while( true )
	{
		up();
		Sleep( 1 );
		if( is_unchange_info( get_info( 0 ) ) )
			break;

		set_last_info( get_info( 0 ) );
	}

	//block_call_before_action(
	//	boost::bind( &SelectPlayerBase::is_unchange_info, this, get_info( 0 ) ),
	//	boost::bind( &SelectPlayerBase::up, this ),
	//	boost::bind( &SelectPlayerBase::set_last_info, this, get_info( 0 ) ),
	//	60, 100 );


	/*
	for( size_t i = 0; i < 6; i++ )
	{
		up();
		//trace( i );
		//Sleep( 1000 );
	}
	*/
}

string SelectPlayerBase::get_info( size_t num )
{
	block_call(
		boost::bind( &SelectPlayerBase::is_selected_frame, this ),
		boost::bind( null ),
		boost::bind( null ), 60, 5 );

	Pos selected_frame_pos = get_frame_pos( selected_frame_num() );

	Pos info_start_pos, info_end_pos;

	get_frame_info_pos(
			selected_frame_pos,
			info_start_pos,
			info_end_pos
			);

	string str = _world.dnf.dm.Ocr(
			info_start_pos.x,
			info_start_pos.y,
			info_end_pos.x,
			info_end_pos.y,
			"d1b994-050505", 1.0 );

	glog( "SelectPlayerBase::get_info:" );
	glog( str );
//	trace( "get_info" );

	return str;
}

void SelectPlayerBase::set_last_info( const string& info )
{
	_last_info = info;
}

size_t SelectPlayerBase::get_total()
{
	/*
	set_safe_mouse();

	if( !is_selected_frame() )
		return 0;

	to_first();

	size_t line = 0;
	size_t column = 0;

	while( true )
	{
		down();
		column = selected_frame_column();
		if( 0 == column )
		{
			if( is_unchange_info( get_info(
				selected_frame_num() ) ) ) 
				break;
			else
			{
				line++;
				set_last_info( get_info(
					selected_frame_num() ) );
			}
		}
		else
			break;
	}

	return line * 4 + column + 1;
	*/

	size_t total;

	while( true )
	{
		total = _get_total();

		//	���������1
		if( 1 == total )
		{
			//	���û�ҵ��ڶ�����
			if( !check_second() )
				break;

		}
		else
			break;
	}

	return total;
}

size_t SelectPlayerBase::_get_total()
{
	//Sleep( 2000 );

	//	��ô���glog����ȡ��
	//	��Ϊ��������Ż�����
	//	���³��������߼�����
	glog( "SelectPlayerBase::_get_total():start", 1 );
	set_safe_mouse();

	if( !is_selected_frame() )
	{
		glog( "SelectPlayerBase::_get_total():!is_selected_frame" );
		return 0;
	}

	to_first();

	size_t line = 0;
	size_t column = 0;

	while( true )
	{
		//	����
		glog( "SelectPlayerBase::_get_total():����" );
		down();
		
		//	ѡ�����
		glog( "SelectPlayerBase::_get_total():ѡ�����:" );
		column = selected_frame_column();
		glog( column );

		if( 0 == column )
		{
			if( is_unchange_info( get_info(
				selected_frame_num() ) ) ) 
				break;
			else
			{
				line++;
				set_last_info( get_info(
					selected_frame_num() ) );
			}
		}
		else
			break;
	}

	//glog( "SelectPlayerBase::_get_total():" );
	//glog( line * 4 + column + 1 );

	return line * 4 + column + 1;
}

bool SelectPlayerBase::check_second()
{
	string str = _world.dnf.dm.Ocr(
		194,251,305,270, "d1b994-050505", 1.0 );

	if( str.find( "L" ) != string::npos )
		return true;
	else
		return false;
}

void SelectPlayerBase::select( size_t num )
{
	assert( num >= 0
		&& "SelectPlayerBase::select:num������Χ" );

	to_first();

	size_t line = num / 4;
	size_t column = num % 4;

	//trace( "----------" );
	//trace( "num" );
	//trace( num );
	//trace( "line" );
	//trace( line );
	//trace( "column" );
	//trace( column );

	for( size_t i = 0; i < line; i++ )
	{
		//trace( "down" );
		down();
		//assert( false );
		while( true )
			if( is_unchange_info( get_info(
				selected_frame_num() ) ) )
			{
				//trace( "down2" );
				down();
			}
			else
				break;
	}

	while( true )
	{
		if( selected_frame_column() == column )
			break;

		right();
	}
}

/////////////////////////////////////////////
SelectPlayer::SelectPlayer( World& world )
	:_base( world ),
	_file_name( "data\\login_info.txt" ),
	_total( 0 ),
	_last_num( 0 )
{
}

void SelectPlayer::save()
{
	ofstream ofile( _file_name.c_str() );
	if( !ofile )
		throw std::exception( "SelectPlayer::save():Can not open file" );

	ofile << _total << endl;
	ofile << _last_num << endl;
}

void SelectPlayer::load()
{
	glog( "SelectPlayer::load():start" );

	ifstream ifile( _file_name.c_str() );
	if( !ifile )
		throw std::exception( "SelectPlayer::load():Can not open file" );

	string str;
	getline( ifile, str, '\n' );
	istringstream iss( str );
	iss >> _total;
	getline( ifile, str, '\n' );
	iss.clear();
	iss.str( "" );
	iss.str( str );
	iss >> _last_num;

	glog( "_total" );
	glog( _total );
	glog( "_last_num:" );
	glog( _last_num );
	glog( "SelectPlayer::load():end" );
}

void SelectPlayer::set_total()
{
	_total = _base.get_total();
}

size_t SelectPlayer::get_total()
{
	return _total;
}

size_t SelectPlayer::get_last_num()
{
	return _last_num;
}

void SelectPlayer::select( size_t num )
{
	_base.select( num );
	_last_num = num;
}

void SelectPlayer::to_first()
{
	_base.to_first();
	_last_num = 0;
}

bool SelectPlayer::to_next()
{
	if( is_last() )
		return false;
	else
	{
		_last_num++;
		_base.select( _last_num );

		glog( "SelectPlayer::to_next():_base.select" );
		glog( _last_num );

		return true;
	}
}

bool SelectPlayer::to_current()
{
	_base.select( _last_num );
	return true;
}

bool SelectPlayer::is_last()
{
	if( _last_num + 1 >= _total )
		return true;
	else
		return false;
}

void SelectPlayer::clear()
{
	_total = 0;
	_last_num = 0;
}


///////////////////////////////////////////////////////////
Login::Login( World& world_, const string& qq_ )
:_world( world_ ),
_select_player( world_ ),
_ch11( 421,217 ),
_ch12( 612,213 ),
_ch13( 238,257 ),
_ch16( 218,296 ),
_game_start( 399, 556 ),
_create( 178, 547 ),
_dizaozhe( 741,120 ),// 136, 151 ),
_ensure_name( 371, 571 ),
_yes( 353,319 ),
_ensure_exist( 400, 327 ),
_ensure_success( 400,320 ),
_is_new_character( false ),
_login_mark( qq_ )
{
}

void Login::run()
{
	_world.dm.unbind_wnd();
	_world.dm.set_wnd_name( "���³�����ʿ" );
	block_call( boost::bind( &Dm::find_wnd, &_world.dm ), 120 );
	//_world.dm.open_console();
	_world.dm.for_dnf_game_wnd();

	block_call( boost::bind( &Login::is_in_ui, this ), 300 );	

	_DNF::Sync::set_start_watch();
	//_world.attach_ime.set_en();

	select_ch();
	//Sleep( 1000 );
	if( !has_character() )
		create();
	else
	{
		Pos pos = get_character();
		if( !pos.is_init() )
			create();
	}

	select_character();
	join();
}

bool Login::is_in_ui()
{
	string str = _world.dnf.dm.Ocr( 579,534,672,560, "ddc593-050505", 1.0 );
	if( str.find( "����" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::is_in_character()
{
	//handle_recommend();
	string str = _world.dnf.dm.Ocr( 499,531,591,565, "ddc593-050505", 1.0 );
	if( str.find( "ѡ��Ƶ��" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::_select_ch()
{
	//_world.player.LB( _random_ch() );
	//Pos pos = _random_ch();
	//_world.dnf.dm.MoveTo( pos.x, pos.y );
	//_world.dnf.dm.LeftDoubleClick();
	Pos pos = _random_ch();
	Sleep( 500 );
	if( is_in_character() )
		return true;
	else
	{
		_world.dnf.dm.MoveTo( pos.x, pos.y );
		_world.dnf.dm.LeftDoubleClick();
		Sleep( 500 );
		handle_recommend();
		//trace( "double click" );
		return false;
	}
}

void Login::select_ch()
{
	block_call( boost::bind( &Login::_select_ch, this ) );
}


bool Login::has_character()
{
	//_world.update_player_win_pos();
	//if( !_world.player.win_pos.is_invaild() )
	string str = _world.dnf.dm.Ocr( 212,533,304,559, "8c8c8c-050505", 1.0 );
	if( str.find( "ɾ��" ) == string::npos )
		return true;
	else
		return false;
}
///////////////////////////////////
bool Login::is_in_create()
{
	return !is_in_ui();
}
	
bool Login::is_selected_dizaozhe()
{
	//if( _world.dnf.has_pic( "dizaozhe.bmp",696,198,786,231 ) )
	if( _world.dnf.has_pic( "dizaozhe1.bmp",19,461,118,543 ) )
		return true;
	else
		return false;
}
		
bool Login::is_start_input_name()
{
	string str =  _world.dnf.dm.Ocr( 340,559,406,583, "ddc593-050505", 1.0 );
	if( str.find( "ȷ��" ) != string::npos )
		return true;
	else
		return false;
}
bool Login::is_input_name()
{
	string str =  _world.dnf.dm.Ocr( 304,535,461,554, "ffffff-000000", 1.0 );
	if( str.find( _name ) != string::npos )
		return true;
	else
		return false;
	
}

bool Login::is_ensure_name()
{
	string str =  _world.dnf.dm.Ocr( 308,277,493,303, "ffffff-000000", 1.0 );
	if( str.find( "ʹ��" ) != string::npos )
		return true;
	else
		return false;
}


bool Login::is_ensure_exist()
{
	string str =  _world.dnf.dm.Ocr( 358,272,457,299, "ffffff-000000", 1.0 );
	if( str.find( "�Ѵ���" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::is_create_end()
{
	return is_in_character();
}

bool Login::is_ensure_success()
{
	string str =  _world.dnf.dm.Ocr( 257,261,479,326, "ffffff-000000", 1.0 );
	glog( "Login::is_ensure_success():str" );
	glog( str );
	if( str.find( "�ɹ�" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::is_create_limit()
{
	string str = _world.dnf.dm.Ocr( 315,276,478,305, "ffffff-000000", 1.0 );
	if( str.find( "���ɴ���" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::is_ensure_create_limit()
{
	return !is_create_limit();
}


bool Login::in_create()
{
	_world.player.LB( _create );
	if( is_in_create() )
		return true;
	else
		return false;
}
		
bool Login::select_dizaozhe()
{
	_world.player.LB( _dizaozhe );
	if( is_selected_dizaozhe() )
	{
		trace( "is_selected_dizaozhe" );
		return true;
	}
	else
		return false;
}
		
bool Login::start_input_name()
{
	_world.player.LB( Pos(388,557) );
	if( is_start_input_name() )
		return true;
	else
		return false;
}
bool Login::input_name()
{
	_name = Name::gen_new();
	_world.dnf.dm.KeyPressStr( _name.c_str(), 20 );

	if( is_input_name() )
		return true;
	else
		return false;
}

bool Login::ensure_name()
{
	//_world.player.LB( _ensure_name );
	//iSleep( 1000 );
	if( is_ensure_name() )
		return true;
	else
		return false;
}

bool Login::ensure_success()
{
	_world.player.LB( _ensure_success );
	if( !is_ensure_success() )
		return true;
	else
		return true;
}

bool Login::handle_success()
{
	if( is_ensure_success() )
	{
		//block_call( boost::bind(
		//	&Login::ensure_success,
		//	this ) );

		return true;
	}
	else
		return false;
}

bool Login::handle_exist()
{
	_world.player.LB( _ensure_exist );
	if( !is_ensure_exist() )
	{
		block_call( boost::bind( &Login::start_input_name, this ) );
		block_call( boost::bind( &Login::input_name, this ) );
		_world.player.LB( _ensure_name );
		block_call( boost::bind( &Login::ensure_name, this ) );
		block_call( boost::bind( &Login::create_end, this ) );

		return true;
	}
	else
		return false;
}

bool Login::create_end()
{
	_world.player.LB( _yes );
	if( is_create_end() )
		return true;
	else if( is_ensure_exist() )
	{
		block_call(boost::bind( &Login::handle_exist ,this ) );
		return true;
	}
	else
		return false;
}

void Login::create()
{
	block_call( boost::bind( &Login::in_create, this ) );
	block_call( boost::bind( &Login::select_dizaozhe, this ) );
	block_call( boost::bind( &Login::start_input_name, this ) );
	block_call( boost::bind( &Login::input_name, this ) );
	_world.player.LB( _ensure_name );
	block_call( boost::bind( &Login::ensure_name, this ) );
	block_call( boost::bind( &Login::create_end, this ) );
	//block_call( boost::bind( &Login::handle_success, this ) );
	block_call( boost::bind( &Login::ensure_success, this ) );
	_is_new_character = true;
}

///////////////////////////////////

Pos Login::get_character()
{
	string str;
	str = _world.dnf.dm.Ocr(80,251,180,292,"d1b994-020202", 1.0);
	if( str.find( "������" ) != string::npos )
		return Pos( 130,272 );
	str = _world.dnf.dm.Ocr(195,253,304,287,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 251,273 );
	str = _world.dnf.dm.Ocr(324,252,423,289,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 373,272 );
	str = _world.dnf.dm.Ocr(445,253,546,292,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 495,273 );

	str = _world.dnf.dm.Ocr(80,463,179,500,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 125,480 );
	str = _world.dnf.dm.Ocr(200,463,305,505,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 247,481 );
	str = _world.dnf.dm.Ocr(324,463,426,505,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 370,480 );
	str = _world.dnf.dm.Ocr(445,463,550,503,"d1b994-050505", 1.0 );
	if( str.find( "������" ) != string::npos )
		return Pos( 494,477 );
	
	return Pos();
}

bool Login::is_selected( Pos pos )
{
	//if( )
	//	return true;
	//else
		return false;
}

void Login::select_character()
{
	//Pos pos = get_character();

	//block_call( boost::bind(
	//	&Login::is_selected, 
	//	this, pos ) );
}

bool Login::is_joined()
{
	if( _world.is_in_big_map() )
		return true;
	else
		return false;
}

bool Login::_join( Pos pos )
{
	//_world.player.LB( pos );
	_world.dnf.dm.MoveTo( pos.x, pos.y );
	_world.dnf.dm.LeftDoubleClick();
	if( is_joined() )
		return true;
	else
		return false;
}
bool Login::is_loading_ui(Pos pos)
{
	_world.dnf.dm.MoveTo( pos.x, pos.y );
	_world.dnf.dm.LeftDoubleClick();
	//_world.player.LB( pos );
	string str = _world.dnf.dm.Ocr(
		320,567,480,596, "ffff00-050505", 1.0 );

	if( str.find( "�����ڽ�" ) != string::npos )
		return true;
	else
		return false;
}
bool Login::is_loading_ui()
{
	string str = _world.dnf.dm.Ocr(
		320,567,480,596, "ffff00-050505", 1.0 );

	if( str.find( "�����ڽ�" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::is_movie()
{
	if( _world.dnf.has_pos_color( 159,560, "000000-000000" )
		&& _world.dnf.has_pos_color( 599,559, "000000-000000" )
		&& _world.dnf.has_pos_color( 143,33, "000000-000000" )
		&& _world.dnf.has_pos_color( 607,30, "000000-000000" ) )
		return true;
	else
		return false;
}

bool Login::start_stop_movie()
{
	_world.player.key_press( VK_ESCAPE );
	Sleep( 2000 );
	string str = _world.dnf.dm.Ocr(
		309,274,471,298, "ffffff-000000", 1.0 );
	//trace( str );
	if( str.find( "ֹ���Ÿ�" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::end_stop_movie()
{
	_world.player.LB( Pos( 367,326 ) );
	Sleep( 2000 );
	string str = _world.dnf.dm.Ocr(
		309,274,471,298, "ffffff-000000", 1.0 );

	//trace( str );
	if( str.find( "ֹ���Ÿ�" ) == string::npos )
		return true;
	else
		return false;
}

bool Login::to_town_ensure()
{
	_world.player.LB( Pos( 517,424 ) );
	Sleep( 100 );
	string str = _world.dnf.dm.Ocr(
		342,271,432,297, "ffffff-000000", 1.0 );
	//trace( str );
	if( str.find( "���س���" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::to_town()
{
	_world.player.LB( Pos( 371,328 ) );
	Sleep( 100 );
	if( _world.is_in_big_map() )
		return true;
	else
		return false;
}

void Login::join_first()
{
	Pos pos;

	while( true )
	{
		pos = get_character();
		if( !pos.is_init() )
			pos = get_character();
		else
			break;
	}

	block_call( boost::bind( &Login::is_loading_ui, this, pos ) );
	block_call( boost::bind( &Login::is_movie, this ) );
	block_call( boost::bind( &Login::start_stop_movie, this ) );
	block_call( boost::bind( &Login::end_stop_movie, this ) );
	_world.open_esc();
	block_call( boost::bind( &Login::to_town_ensure, this ) );
	block_call( boost::bind( &Login::to_town, this ) );
}

void Login::join()
{
	if( !_is_new_character )
	{
		Pos pos = get_character();
		block_call( boost::bind(
			&Login::_join, this, pos ), 120 );
	}
	else
	{
		join_first();
	}
}

Pos Login::_random_ch()
{
	size_t num = random() % 4;
	if( 0 == num )
		return _ch11;
	else if( 1 == num )
		return _ch12;
	else if( 2 == num )
		return _ch13;
	else if( 3 == num )
		return _ch16;
}

void Login::run2()
{
	//cout << "xx00" << endl;

	output( "����󶨴���" );
	_world.dm.unbind_wnd();

	//cout <<  "xx01" << endl;
	output( "������Ϸ���ھ��" );
	_world.dm.set_wnd_name( "���³�����ʿ" );
	//cout <<  "xx02" << endl;
	block_call( boost::bind( &Dm::find_wnd, &_world.dm ), 120 );
	//cout <<  "xx03" << endl;
	//_world.dm.open_console();
	output( "����Ϸ����" );
	_world.dm.for_dnf_game_wnd();
	//cout << "xx04" << endl;

	block_call( boost::bind( &Login::is_in_ui, this ), 300 );	
	//cout << "xx05" << endl;

	output( "���������߳�" );
	_DNF::Sync::set_start_watch();
	//cout << "xx06" << endl;
	//_world.attach_ime.set_en();

	output( "�ָ�����̨" );
	_world.dm.restore_console();

	output( "����ѡ��Ƶ��" );
	select_ch();

	//handle_recommend();

	output( "��ʼѡ���ɫ" );

	if( is_set_login() && !is_out_date() )
	{
		//cout << "run2:01" << endl;

		output( "���ǵ�һ�ε�¼" );
		output( "�����¼�Ľ�ɫѡ����Ϣ" );

		_select_player.load();
		output( "ѡ���ɫ" );
		_select_player.select( _select_player.get_last_num() );
		glog( "_select_player.get_last_num()" );
		glog( _select_player.get_last_num() );
		Sleep( 1500 );
		output( "������Ϸ" );
		join2();
		output( "�������" );
	}
	else
	{
		//cout << "run2:02" << endl;

		//	��ת���޸�
		/*
		if( !is_can_not_create() 
			&& !is_stop_create() )
			while( true )
				if( !create2() )
					break;
		*/
		/*
		if( !is_stop_create() )
			if( !is_can_not_create() )
				while( true )
					if( !create2() )
						break;
						*/

		output( "��һ�ν�����Ϸ" );

		if( !is_stop_create() )
			if( !is_can_not_create() )
			{
				output( "������ɫ1" );
				create2();
			}

		//	���Sleep���ڷ�ֹ.����
		//	is_stop_create�л�ȡ
		//	���������Ĳ���ȷ��
		
		//Sleep( 1000 );
		//_select_player.set_total();

		if( !is_stop_create() )
			if( !is_can_not_create() )
			{
				output( "������ɫ2" );
				create2();
			}


		output( "������Ч��" );
		set_new_date();
		output( "���µ�¼��Ϣ" );
		set_login_mark();
		//switch_to_next();
		output( "�����ɫ����" );
		_select_player.set_total();
		output( "�л�����һ����ɫ" );
		switch_to_first();
		output( "����ѡ����Ϣ" );
		_select_player.save();
		Sleep( 1500 );
		output( "������Ϸ" );
		join2();
		output( "�������" );
	}
}

bool Login::create2()
{
	glog( "Login::create2()" );
	block_call( boost::bind( &Login::in_create, this ) );
	block_call( boost::bind( &Login::select_dizaozhe, this ) );
	block_call( boost::bind( &Login::start_input_name, this ) );
	block_call( boost::bind( &Login::input_name, this ) );
	_world.player.LB( _ensure_name );
	block_call( boost::bind( &Login::ensure_name, this ) );


	boost::timer out_timer;
	while( true )
	{
		_world.player.LB( _yes );

		Sleep( 20 );
		if( is_create_end() )
		{
			output( "��ɫ�������" );
			break;
		}
		else if( is_ensure_exist() )
		{
			block_call(boost::bind( &Login::handle_exist ,this ) );
			break;
		}
		else if( is_create_limit() )
		{
			output( "��ɫ�����Ѿ��ﵽ��������(2)" );
			block_call( 
				boost::bind( &Login::is_ensure_create_limit, this ),
				boost::bind( null ),
				boost::bind( &Player::LB, &_world.player, Pos( 396,321 ) ),
				60, 100 );

			block_call(
				boost::bind( &Login::is_in_character, this ),
				boost::bind( null ),
				boost::bind( &Player::LB, &_world.player, Pos( 733,571 ) ),
				60, 100 );

			return false;
		}

		if( out_timer.elapsed() > 60 )
			return false;
		Sleep( 10 );
	}

	//block_call( boost::bind( &Login::create_end, this ) );
	if( block_call( 
			boost::bind( &Login::handle_success, this ),
			boost::bind( null ),
			boost::bind( null ),
			3, 100, false )
			)
	{
		output( "��������ɫ�ɹ�����" );
		if( !block_call( 
			boost::bind( &Login::ensure_success, this ),
			boost::bind( null ),
			boost::bind( null ),
			5, 100, false )
			)
			output( "��������ɫ�ɹ������쳣" );
		else
			output( "��������ɫ�ɹ��������" );

	}

	return true;
}

bool Login::join2()
{

	glog( "Login::join2().start" );

	boost::timer out_timer;

	while( true )
	{
		press_start_game();
		Sleep( 100 );
		if( is_joined() )
			break;

		if( is_loading_ui() )
		{
			output(	"�������ﶯ��" );
			block_call( boost::bind( &Login::is_movie, this ) );
			block_call( boost::bind( &Login::start_stop_movie, this ) );
			block_call( boost::bind( &Login::end_stop_movie, this ) );
			_world.open_esc();
			block_call( boost::bind( &Login::to_town_ensure, this ) );
			block_call( boost::bind( &Login::to_town, this ) );

			glog( "Login::join2().end" );

			break;
		}

		if( out_timer.elapsed() > 60 )
		{
			assert( false
				&& "Login::join2:������Ϸʧ��" );

			glog( "Login::join2().1" );

			break;
		}
	}

	Sleep( 10000 );
	handle_opening_movie();

	if( _world.has_motion() )
	{
		output( "������Ϸ�" );
		_world.close_motion();
	}

	output( "�򿪴��ͼ,�ر�����˵��" );	
	_world.open_big_map();

	_world.open_esc();
	_world.close_esc();
			
	
	glog( "Login::join2()2" );

	return true;
}

bool Login::is_can_not_create()
{
	//	��ת���޸�
	/*
	string str = _world.dnf.dm.Ocr(
		136,536,214,559, 
		"8c8c8c-020202", 1.0 
		);

	if( str.find( "������ɫ" ) != string::npos )
		return true;
	else
		return false;
		*/

	_world.player.LB( _create );
	Sleep( 1000 );
	string str = _world.dnf.dm.Ocr(
		317,268,432,296,
		"ffffff-000000", 1.0 );

	if( str.find( "��ɫ����" ) != string::npos )
	{
		_world.player.key_press( VK_ESCAPE );
		_world.player.key_press( VK_ESCAPE );
		_world.player.key_press( VK_ESCAPE );

		return true;
	}
	else
		return false;
}

int Login::get_last_num()
{
	_select_player.load();
	return _select_player.get_last_num();
}

bool Login::switch_to_next()
{
	/*	
	_world.player.key_press( VK_RIGHT );
	*/


	//if( _last_select_num < _total_charater )
	//{
	//	_last_select_num++;
	//	_select_player.select( _last_select_num );


	//	return true;
	//}
	//else
	output( "��ȡ��ɫ��Ϣ" );
	glog( "Login::switch_to_next():start" );

	_select_player.load();

	output( "ѡ����һ����ɫ" );

	bool ret = _select_player.to_next();

	output( "�����ɫ��Ϣ" );
	_select_player.save();

	glog( "Login::switch_to_next():end" );
	return ret;
}

bool Login::switch_to_current()
{
	output( "��ȡ��ɫ��Ϣ" );
	glog( "Login::switch_to_last_select():start" );
	_select_player.load();
	output( "ѡ��ǰ�Ľ�ɫ" );

	return _select_player.to_current();
}


void Login::switch_to_first()
{
	/*
	for( size_t i = 0; i < 20; i++ )
	{
		_world.player.key_press( VK_LEFT );
		Sleep( 50 );
	}
	*/

	_select_player.to_first();
	glog( "_select_player.to_first();" );
}

void Login::press_start_game()
{
	_world.player.LB( Pos( 445,556 ) );
}

bool Login::_load_login_mark()
{
	ifstream ifile( "data\\login_mark.txt" );
	if( !ifile )
		throw std::exception( "Can not open data\\login_mark.txt" );

	string str;
	getline( ifile, str, '\n' );

	if( _login_mark == str )
		return true;
	else
		return false;
}

bool Login::is_set_login()
{
	return _load_login_mark();
}

void Login::set_login_mark()
{
	ofstream ofile( "data\\login_mark.txt" );
	if( !ofile )
		throw std::exception( "Can not open data\\login_mark.txt" );

	ofile << _login_mark << endl;
}

void Login::unset_login_mark()
{
	ofstream ofile( "data\\login_mark.txt" );
	if( !ofile )
		throw std::exception( "Can not open data\\login_mark.txt" );

	ofile << "0" << endl;
}

bool Login::is_out_date()
{
	ifstream ifile( "data\\last_date.txt" );
	if( !ifile )
		throw std::exception( "can not open data\\last_date.txt" );

	string str; 
	getline( ifile, str, '\n' );
	ifile.close();

	if( str.empty() )
		set_new_date();

	boost::gregorian::date date( 
		boost::gregorian::from_undelimited_string( str ) );

	boost::posix_time::ptime out_time(
		date, boost::posix_time::hours( 6 )
		);
	out_time += boost::gregorian::days( 1 );

	//cout << out_time << endl;

	boost::posix_time::ptime ptime( 
		boost::posix_time::second_clock::local_time() );

	//cout << ptime << endl;

	boost::posix_time::time_duration time_duration(
		ptime - out_time );

	if( !time_duration.is_negative() )
		return true;
	else
		return false;
}

void Login::set_new_date()
{
	ofstream ofile( "data\\last_date.txt" );
	if( !ofile )
		throw std::exception( "can not open data\\last_date.txt" );

	boost::gregorian::date date(
		boost::gregorian::day_clock::local_day() );
	ofile << boost::gregorian::to_iso_string( date ) << endl;
}

bool Login::is_stop_create()
{
	ifstream ifile( "data\\stop_create.txt" );
	if( !ifile )
		throw std::exception( "can not open data\\stop_create.txt" );

	string str;
	getline( ifile, str, '\n' );

	istringstream iss( str );
	size_t num;

	iss >> num;

	_select_player.set_total();

	glog( "num:" );
	glog( num );
	glog( "_select_player.get_total();" );
	glog( _select_player.get_total() );

	if( _select_player.get_total() >= num
		&& num != 0 )
		return true;
	else
		return false;
}

int Login::load_stop_num()
{
	ifstream ifile( "data\\stop_create.txt" );
	if( !ifile )
		throw std::exception( "can not open data\\stop_create.txt" );

	string str;
	getline( ifile, str, '\n' );

	istringstream iss( str );
	size_t num;

	iss >> num;
	return num;
}

bool Login::has_opening_movie_notice()
{
	string str = _world.dnf.dm.Ocr(
		308,266,408,297,
		"ffffff-000000", 1.0 );
	if( str.find( "�رտ���" ) != string::npos )
		return true;
	else
		return false;
}
bool Login::has_no_opening_movie_notice()
{
	return !has_opening_movie_notice();
}

void Login::handle_opening_movie()
{
	if( _world.is_right_low_ui() )
		return;

	output( "������ϷƬͷ����1" );

	block_call( 
		boost::bind( &Login::has_opening_movie_notice, this ),
		boost::bind( null ),
		boost::bind( &Player::key_press, &_world.player, VK_ESCAPE, 10 ),
		20, 100, false );

	output( "������ϷƬͷ����2" );
		
	block_call(
		boost::bind( &Login::has_no_opening_movie_notice, this ),
		boost::bind( null ),
		boost::bind( &Player::LB, &_world.player, Pos( 372,327 ) ),
		20, 100, false );
}

bool Login::has_recommend()
{
	string str = _world.dnf.dm.Ocr(
		1,1,799,599,
		"e2cc9a-202020", 1.0 );
	if( str.find( "�Ƽ�" ) != string::npos )
		return true;
	else
		return false;
}

bool Login::has_no_recommend()
{
	return !has_recommend() && has_no_recommend_ensure();
}

void Login::cancel_recommend()
{
	vector<PosStr> pos_str_list;
	_world.dm._cdnf.GetPosStrList(
		pos_str_list,
		1,1,799,599,
		"dec58f-202020", 1.0 );

	Pos cancel_pos;
	for( size_t i = 0; i < pos_str_list.size(); i++ )
	{
		if( pos_str_list[ i ].str.find( "ȡ��" )
			!= string::npos )
		{
			cancel_pos.x = pos_str_list[ i ].pos.x;
			cancel_pos.y = pos_str_list[ i ].pos.y + 3;
		}
	}

	//ostringstream os;
	//os << "cancel_pos:" 
	//	<< cancel_pos.x << ","
	//	<< cancel_pos.y;

	//glog( os.str() );

	if( cancel_pos.is_init()
		&& !cancel_pos.is_invaild() )
	{
		_world.player.LB( cancel_pos );
		_world.player.LB( cancel_pos );
	}

	Sleep( 300 );
	handle_recommend_ensure();
}

bool Login::has_recommend_ensure()
{
	string str = _world.dnf.dm.Ocr(
		1,1,799,599,
		"ffffff-000000", 1.0 );
	if( str.find( "����" ) != string::npos )
		return true;
	else
		return false;
}
bool Login::has_no_recommend_ensure()
{
	return !has_recommend_ensure();
}

void Login::cancel_recommend_ensure()
{
	vector<PosStr> pos_str_list;
	_world.dm._cdnf.GetPosStrList(
		pos_str_list,
		1,1,799,599,
		"ffffff-020202", 1.0 );

	Pos ensure_cancel_pos;
	for( size_t i = 0; i < pos_str_list.size(); i++ )
	{
		if( pos_str_list[ i ].str.find( "����" )
			!= string::npos )
		{
			ensure_cancel_pos.x = pos_str_list[ i ].pos.x;
			ensure_cancel_pos.y = pos_str_list[ i ].pos.y + 97;
		}
	}

	//ostringstream os;
	//os << "ensure_cancel_pos:" 
	//	<< ensure_cancel_pos.x << ","
	//	<< ensure_cancel_pos.y;

	//glog( os.str() );

	if( ensure_cancel_pos.is_init()
		&& !ensure_cancel_pos.is_invaild() )
	{
		_world.player.LB( ensure_cancel_pos );
		_world.player.LB( ensure_cancel_pos );
	}
	
}

void Login::handle_recommend_ensure()
{
	if( block_call(
		boost::bind( &Login::has_recommend_ensure, this ),
		boost::bind( null ),
		boost::bind( null ),
		10, 200, false )
		)
	{
		output( "�����Ƽ���ȷ����ť" );
		if( !block_call(
			boost::bind( &Login::has_no_recommend_ensure, this ),
			boost::bind( null ),
			boost::bind( &Login::cancel_recommend_ensure, this ),
			20, 200, false ) )
			output( "�ر��Ƽ���ȷ����ť�쳣" );
	}
	else
		output( "�����Ƽ���ȷ����ť�쳣" );
}

void Login::handle_recommend()
{
	if( block_call( 
		boost::bind( &Login::has_recommend, this ),
		boost::bind( null ),
		boost::bind( null ),
		1, 200, false ) )
	{
		output( "�����Ƽ���" );
		if( !block_call(
			boost::bind( &Login::has_no_recommend, this ),
			boost::bind( null ),
			boost::bind( &Login::cancel_recommend, this ),
			40, 200, false ) )
			output( "�����Ƽ����쳣" );
		else
			glog( "�Ƽ��˴������" );
	}

}


bool Login::is_last_charater()
{
	_select_player.load();
	if( _select_player.is_last() )
		return true;
	else
		return false;

	return false;
}

_GAME_END