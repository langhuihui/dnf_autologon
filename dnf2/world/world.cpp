#include "world.h"

_DNF_BEGIN
////////////////////////////////////////////////

/*
_Pos::_Pos( float x_, float y_, float z_ )
	:x( x_ ),
	y( y_ ),
	z( z_ )
{}

bool _Pos::operator!=( const _Pos& p ) const
{
	if( x != p.x || y != p.y )
		return true;
	else
		return false;
}

bool _Pos::operator==( const _Pos& p ) const
{
	return !operator!=( p );
}
*/



///////////////////////////////////////////////
//	Player
Player::Player( CDnf& dnf_ )
	:dnf( dnf_ ),
	_level( 0 ),
	_money( 0 )
{}

void Player::key_press( long vk, long keep_time )
{
	dnf.dm.KeyDown( vk );
	Sleep( keep_time );
	dnf.dm.KeyUp( vk );
}


void Player::Q() //	0x51
{
	key_press( 0x51, 5 );
}

void Player::E() // 0x45
{
	key_press( 0x45, 5 );
}

void Player::R() // 0x52
{
	key_press( 0x52, 5 );
}


void Player::A( int time ) // ms 0x41
{
	trace( "A11" );
	key_press( 0x41, time );
	trace( "A22" );
}

void Player::W( int time ) // ms 0x57
{
	trace( "W11" );

	key_press( 0x57, time );
	trace( "W22" );
}

void Player::S( int time ) // ms 0x53
{
	trace( "S11" );
	key_press( 0x53, time );
	trace( "S22" );
}

void Player::D( int time ) // 0x44
{
	trace( "D11" );
	key_press( 0x44, time );
	trace( "D22" );
}

void Player::AA( int time )
{
	A( 10 );
	Sleep( 10 );
	A( time );
}
void Player::DD( int time )
{
	D( 10 );
	Sleep( 10 );
	D( time );
}

void Player::RB( const Pos& pos )
{
	//dnf.MouseRightClick( CPoint( pos.x, pos.y ), 0, 0, true );
	dnf.dm.MoveTo( pos.x, pos.y );
	dnf.dm.RightClick();
}

void Player::LB( const Pos& pos )
{
	//dnf.MouseLeftClick( CPoint( pos.x, pos.y ), 0, 0, true );
	dnf.dm.MoveTo( pos.x, pos.y );
	//Sleep( 1 );
	dnf.dm.LeftClick();
}

//	设置人物等级
bool Player::_set_level()
{
	//key_press( 0x4D, 1 );
	//Sleep( 100 );

	vector<PosStr> pos_str_list;
	dnf.GetPosStrList( pos_str_list, 184,234,299,277, "ffffff-000000", 1.0 );

	size_t index = -1;
	for( size_t i = 0; i < pos_str_list.size(); i++ )
		if( pos_str_list[ i ].str.find( "Lv" ) != string::npos )
		{
			index = i;
			break;
		}
	
	/*
	if( 'L' == pos_str_list[ 0 ].str[ 0 ]
		&& 'v' == pos_str_list[ 0 ].str[ 1 ] )
	{
	*/
	if( -1 != index )
	{
		istringstream iss( pos_str_list[ index + 1 ].str );
		iss >> _level;
		return true;
	}
	else
		return false;
}

void Player::set_level()
{
	/*
	key_press( 0x4D, 1 ); //	M: 0x4D

	Sleep( 50 );

	vector<PosStr> pos_str_list;
	dnf.GetPosStrList( pos_str_list, 184,234,299,277, "ffffff-000000", 1.0 );

	if( pos_str_list.empty()
		|| 'L' != pos_str_list[ 0 ].str[ 0 ] 
	|| 'v'!= pos_str_list[ 0 ].str[ 1 ] )
	{
		key_press( 0x4D, 1 ); //	M: 0x4D

		Sleep( 50 );
		dnf.GetPosStrList( pos_str_list, 184,234,299,277, "ffffff-000000", 1.0 );
	}

	if( pos_str_list.empty()
		|| 'L' != pos_str_list[ 0 ].str[ 0 ] 
	|| 'v'!= pos_str_list[ 0 ].str[ 1 ] )
	{
		key_press( VK_ESCAPE, 1 );
		key_press( 0x4D, 1 ); //	M: 0x4D
		Sleep( 50 );

		dnf.GetPosStrList( pos_str_list, 184,234,299,277, "ffffff-000000", 1.0 );
	}

	boost::timer out_timer;
	while( true )
	{
		if( pos_str_list.empty()
			|| 'L' != pos_str_list[ 0 ].str[ 0 ] 
		|| 'v'!= pos_str_list[ 0 ].str[ 1 ] )
		{
			key_press( VK_ESCAPE, 1 );
			key_press( 0x4D, 1 ); //	M: 0x4D
			Sleep( 50 );

			dnf.GetPosStrList( pos_str_list, 184,234,299,277, "ffffff-000000", 1.0 );

		}
		else
			break;

		if( out_timer.elapsed() > 300 )
		{
			assert( false
				&& "Player::set_level:5分钟内无法获取等级" );
		}

		Sleep( 10 );
	}

	
	if( 'L' == pos_str_list[ 0 ].str[ 0 ]
		&& 'v' == pos_str_list[ 0 ].str[ 1 ] )
	{
		istringstream iss( pos_str_list[ 1 ].str );
		iss >> _level;
	}
	*/


	open_info();

	block_call( boost::bind( &Player::_set_level, this ), 20, 100 );

	close_info();
	//key_press( 0x4D, 1 ); //	M: 0x4D
}


size_t Player::get_level()
{
	/*
	assert( _level
		&& "Player::get_name:_level未设置" );
		*/
	if( !_level )
		set_level();

	assert( _level
		&& "Player::get_name:_level未设置" );

	return _level;
}


bool Player::_set_name()
{
	vector<PosStr> pos_str_list;
	//dnf.GetPosStrList( pos_str_list, 184+10,234,299+10,277, "ffffff-000000", 1.0 );
	dnf.GetPosStrList( pos_str_list,0,25,790,548, "ffffff-000000", 1.0 );

	for( size_t i = 0; i < pos_str_list.size(); i++ )
	{ 
		ostringstream oss;
		oss << pos_str_list[ i ].pos.x << ","
			<< pos_str_list[ i ].pos.y << "\t" 
			<< pos_str_list[ i ].str;
		trace( oss.str() );
	}

	size_t index = -1;
	for( size_t i = 0; i < pos_str_list.size(); i++ )
		if( pos_str_list[ i ].str.find( "Lv" ) != string::npos )
		{
			index = i;
			break;
		}
	
	/*
	if( 'L' == pos_str_list[ 0 ].str[ 0 ]
		&& 'v' == pos_str_list[ 0 ].str[ 1 ] )
	{
	*/
	if( -1 != index )
	{
		_name =  pos_str_list[ index + 2 ].str;
		return true;
	}
	else
		return false;
}

void Player::set_name()
{
	open_info();
	block_call( boost::bind( &Player::_set_name, this ), 20, 100 );
	close_info();
}

string Player::get_name()
{
	set_name();

	assert( !_name.empty()
		&& "Player::get_name():未设置" );

	return _name;
}

bool Player::has_info()
{
	string str = dnf.dm.Ocr( 178,107,315,126, 
		"ffffff-000000", 1.0 );

	Sleep( 10 );

	if( str.find( "个人信息" ) != string::npos )
		return true;
	else
		return false;
}

bool Player::has_no_info()
{
	return !has_info();
}

void Player::open_info()
{
	block_call( 
		boost::bind( &Player::has_info, this ),
		boost::bind( null ),
		boost::bind( &Player::key_press, this, 0x4D, 50 ),
		30, 300 );
}

void Player::close_info()
{
	block_call( 
		boost::bind( &Player::has_no_info, this ),
		boost::bind( null ),
		boost::bind( &Player::key_press, this, 0x4D, 50 ),
		30, 300 );
}

void Player::update_money()
{
	open_equip();

	string str = dnf.dm.Ocr(
		634,522,723,554,
		"e6c89b-020202", 1.0
		);

	if( str.find( "金币" ) == string::npos )
	{
		output( "获取金币数量失败" );
		return;
	}

	size_t pos = str.find( ',' );
	str.erase( pos, 1 );
	
	istringstream iss( str );

	iss >> _money;

	close_equip();
}

size_t Player::money()
{
	if( !_money )
		trace( "金币数量为0" );

	return _money;
}


bool Player::has_equip()
{
	//string str = dnf.dm.Ocr( 550,52,618,76, "ffffff-000000", 1.0 );
	string str = dnf.dm.Ocr( 555,40,627,62, "ffffff-000000", 1.0 );
	if( str.find( "装备栏" ) != string::npos )	
		return true;
	else
		return false;
}

bool Player::_open_equip()
{
	key_press( 0x49 ); // I
	Sleep( 10 );

	if( has_equip() )
		return true;

	return false;
}

void Player::open_equip()
{
	block_call( boost::bind( &Player::_open_equip, this ), 20 );
}

bool Player::_close_equip()
{
	if( !has_equip() )
		return true;
	else
	{
		key_press( 0x49 ); // I
		Sleep( 10 );
		return false;
	}
}

void Player::close_equip()
{
	block_call( boost::bind( &Player::_close_equip, this), 20 );
}
////////////////////////

Auction::Auction( CDnf& dnf )
	:_cdnf( dnf ),
	_auction_base_pos( 375, 32 )
{
}

bool Auction::open()
{
	output( "打开拍卖行" );
	set_safe_mouse();
	if( !block_call(
		boost::bind( &Auction::_is_open, this ),
		boost::bind( null ),
		boost::bind( null ),
		20, 200, false )
		)
		output( "打开拍卖行异常" );
}
bool Auction::close()
{
	output( "关闭拍卖行" );
	set_safe_mouse();
	if( !block_call(
		boost::bind( &Auction::_is_close, this ),
		boost::bind( null ),
		boost::bind( null ),
		20, 200, false )
		)
		output( "关闭拍卖行异常" );
}

bool Auction::init_pos()
{
	_set_base_pos();
	_set_other_pos_by_base_pos();
}

bool Auction::input_keyword()
{
	return false;
}

bool Auction::search()
{
	_cdnf.left_click(
		_search_pos.x,
		_search_pos.y
		);

	return false;
}

bool Auction::sort()
{
	return false;
}

bool Auction::buy()
{
	return false;
}


bool Auction::check_safe_state()
{

	return false;
}
bool Auction::set_safe_mouse()
{
	_cdnf.dm.MoveTo( 60, 450 );
	return false;
}
bool Auction::update_money()
{
	return false;
}

bool Auction::_is_open()
{
	set_safe_mouse();
	if( _cdnf.has_ocr_str( 
		"拍卖行",
		307,19,502,88,
		"ffffff-000000" )
		)
		return true;
	else
		return false;
}
bool Auction::_is_close()
{
	if( !_is_open() 
		&& !_cdnf.has_ocr_str(
		"一口价",
		273,489,530,595,
		"ddc593-020202" )
		)
		return true;
	else
		return false;
}
bool Auction::_set_base_pos()
{
	vector<PosStr> pos_str_list;
	_cdnf.GetPosStrList(
		pos_str_list,
		307,19,502,88,
		"ffffff-000000" 
		);

	for( size_t i = 0; i < pos_str_list.size(); i++ )
		if( pos_str_list[ i ].str.find(
			"拍卖行" ) != string::npos )
		{
			Pos pos(
			pos_str_list[ i ].pos.x,
			pos_str_list[ i ].pos.y
			);

			pos.corrent_assert();
			if( !pos.is_init()
				|| pos.is_invaild() )
				output( "拍卖行基础位置获取错误" );
			else
			{
				_auction_base_pos.x = pos.x;
				_auction_base_pos.y = pos.y;
			}
		}
			
	output( "拍卖行基础位置获取错误2" );
}
bool Auction::_set_other_pos_by_base_pos()
{
	//	正常情况下
	//	拍卖行的位置为
	//----------
	//	375,32
	//----------

	//	139,90
	_keyword_input_pos.x = 
		_auction_base_pos.x - 236;
	_keyword_input_pos.y = 
		_auction_base_pos.y + 58;

	//	594,91
	_search_pos.x =
		_auction_base_pos.x + 219;
	_search_pos.y =
		_auction_base_pos.y + 59;

	//	552,115
	_sort_by_money_pos.x =
		_auction_base_pos.x + 177;
	_sort_by_money_pos.y =
		_auction_base_pos.y + 83;

	//	427,513
	_next_page_pos.x =
		_auction_base_pos.x + 52;
	_next_page_pos.y =
		_auction_base_pos.y + 481;

	//	398,548
	_buy_pos.x =
		_auction_base_pos.x + 23;
	_buy_pos.y =
		_auction_base_pos.y + 516;

	_keyword_input_pos.corrent_assert();
	_search_pos.corrent_assert();
	_sort_by_money_pos.corrent_assert();
	_next_page_pos.corrent_assert();
	_buy_pos.corrent_assert();

	return true;

}
bool Auction::_clear_reward_window()
{
	return true;
}

///////////////////////
//	Move

Move::Move( CDnf& dnf_ )
	:_dnf( dnf_ ),
	_min_dis( 20 )
#ifdef _DEBUG
		,
		_cnt_a( 0 ),
		_cnt_d( 0 ),
		_cnt_w( 0 ),
		_cnt_s( 0 )

#endif
{}

void Move::operator()(
	const Pos& current_pos,
	const Pos& target_pos 
	)
{
	if( current_pos == target_pos )
		return;

	_offset_x = target_pos.x - current_pos.x;
	_offset_y = target_pos.y - current_pos.y;
	
	if( _offset_x >= 0 )
		_vk_x = _D;
	if( _offset_x < 0 )
		_vk_x = _A;

	if( _offset_y >= 0 )
		_vk_y = _S;	 
	if( _offset_y < 0 )
		_vk_y = _W;

	assert( !( 0 == _offset_x
		&& 0 == _offset_y ) 
		&& "Move::operator(): _offset_x和_offset_y同时等于0" );
	
	
	_x_dis = abs( _offset_x );
	_y_dis = abs( _offset_y );

	_x_time = (float)_x_dis / (float)_x_speed;
	_x_high_speed_time = (float)_x_dis / (float)_x_high_speed;
	_y_time = (float)_y_dis / (float)_y_speed;

	if( _x_time > _y_time )
		_is_main_x = true;
	else
		_is_main_x = false;

	if( _x_high_speed_time > _y_time )
		_is_run = true;
	else
		_is_run = false;

	
	if( _is_main_x )
	{
		if( _is_run )
			_move( _vk_x, _x_dis, _x_high_speed, _vk_y, _y_dis, _y_speed );
		else
			_move( _vk_x, _x_dis, _x_speed, _vk_y, _y_dis, _y_speed );
	}
	else
	{
		glog( "Move:_move().start" );
		_move( _vk_y, _y_dis, _y_speed, _vk_x, _x_dis, _x_high_speed );
		glog( "Move:_move().end" );
		/*
		size_t total_time = (float)_y_dis / (float)_y_speed * 1000;

		size_t second_loop_times = _x_dis / _min_dis;

		size_t second_step_time = (float)_min_dis / (float)_x_speed * 1000;

		size_t second_last_dis = _x_dis % _min_dis;

		size_t second_last_time = (float)second_last_dis / (float)_x_speed * 1000;
		
		size_t main_step_time = total_time / second_loop_times + 1;

		_dnf.dm.KeyDown( _y_dis );
		
		for( size_t i = 0; i < second_loop_times; i++ )
		{
			_dnf.dm.KeyDown( _x_dis );
			Sleep( second_step_time );
			_dnf.dm.KeyUp( _x_dis );
			Sleep( main_step_time - second_step_time );
		}
		_dnf.dm.KeyDown( _x_dis );
		Sleep( second_last_time );
		_dnf.dm.KeyUp( _x_dis );
		Sleep( main_step_time - second_last_time );

		_dnf.dm.KeyUp( _y_dis );
*/

	}
}

void Move::_move(
		long main_vk,
		size_t main_dis,
		size_t main_speed,
		long second_vk,
		size_t second_dis,
		size_t second_speed

		)
{
	size_t total_time = (float)main_dis / (float)main_speed * 1000;
	
	size_t second_loop_times = second_dis / _min_dis;

	size_t second_step_time = (float)_min_dis / (float)second_speed * 1000;

	size_t second_last_dis = second_dis % _min_dis;

	size_t second_last_time = (float)second_last_dis / (float)second_speed * 1000;

	size_t main_step_time = total_time;
	
	if( second_loop_times )
		main_step_time = total_time / ( second_loop_times + 1 );


	//trace( "LockInput(3)" );
	//_dnf.dm.LockInput( 3 );

	_trace_down_begin( main_vk );
	//_dnf.dm.KeyDown( main_vk );
	_dnf.key_down( main_vk );
	_trace_down_end( main_vk );

	if( _is_run && _is_main_x )
	{
		size_t delay = 20;
		Sleep( delay );
		//trace( delay );

		_trace_up_begin( main_vk );
		//_dnf.dm.KeyUp( main_vk );
		_dnf.key_up( main_vk );
		_trace_up_end( main_vk );

		size_t delay2 = 20;
		Sleep( delay2 );
		//trace( delay2 );

		_trace_down_begin( main_vk );
		//_dnf.dm.KeyDown( main_vk );
		_dnf.key_down( main_vk );
		_trace_down_end( main_vk );
	}

	Sleep( 20 );

	for( size_t i = 0; i < second_loop_times; i++ )
	{
		_trace_down_begin( second_vk );
		//_dnf.dm.KeyDown( second_vk );
		_dnf.key_down( second_vk );
		_trace_down_end( second_vk );

		Sleep( second_step_time );
		_trace_up_begin( second_vk );
		//_dnf.dm.KeyUp( second_vk );
		_dnf.key_up( second_vk );
		_trace_up_end( second_vk );
		
		if( main_step_time <= second_step_time )
		{
			//cout << "***" 
			//	<< main_step_time - second_step_time;
			//cout << "(" << main_step_time
			//	<< ","
			//	<< second_step_time
			//	<< ")";

			//cout << endl;
			//cout << main_vk << endl;
			//cout << main_dis << endl;
			//cout << main_speed << endl;
			//cout << second_vk << endl;
			//cout << second_dis << endl;
			//cout << second_speed << endl;
			//cout << "--------------" << endl;
			//cout << total_time << endl;
			//cout << second_loop_ti,mmmes << endl;
			//cout << second_step_time << endl;
			//cout << second_last_dis << endl;
			//cout << second_last_time << endl;
			//cout << main_step_time << endl;
		}
		else
			Sleep( main_step_time - second_step_time );

	}
	_trace_down_begin( second_vk );
	//_dnf.dm.KeyDown( second_vk );
	_dnf.key_down( second_vk );
	_trace_down_end( second_vk );
	Sleep( second_last_time );

	_trace_up_begin( second_vk );
	//_dnf.dm.KeyUp( second_vk );
	_dnf.key_up( second_vk );
	_trace_up_end( second_vk );

	if( main_step_time <= second_step_time )
		Sleep( total_time - second_last_time - second_loop_times * second_step_time );
	else
		Sleep( main_step_time - second_last_time );

	_trace_up_begin( main_vk );
//	_dnf.dm.KeyUp( main_vk );
	_dnf.key_up( main_vk );
	_trace_up_end( main_vk );

	//if( main_step_time <= second_step_time
	//	&& second_loop_times )
	//{
	//	cout << endl;
	//	cout << main_vk << endl;
	//	cout << main_dis << endl;
	//	cout << main_speed << endl;
	//	cout << second_vk << endl;
	//	cout << second_dis << endl;
	//	cout << second_speed << endl;
	//	cout << "--------------" << endl;
	//	cout << total_time << endl;
	//	cout << second_loop_times << endl;
	//	cout << second_step_time << endl;
	//	cout << second_last_dis << endl;
	//	cout << second_last_time << endl;
	//	cout << main_step_time << endl;
	//}

	//trace( "LockInput( 0 )" );
	//_dnf.dm.LockInput( 0 );

}

void Move::_trace_down_begin( long vk )
{
	/*
#ifdef _DEBUG
	if( _D == vk )
	{
		_cnt_d++;
		_cnt_a = 0;
		_cnt_w = 0;
		_cnt_s = 0;
		if( _cnt_d > 100 )
		{
			assert( false
			&& "按键D连续100次" );
			_cnt_d = 0;
		}

		trace( "D1a" );
	}
	if( _A == vk )
	{
		_cnt_a++;
		_cnt_d = 0;
		_cnt_w = 0;
		_cnt_s = 0;
		if( _cnt_a > 100 )
		{
			assert( false
			&& "按键A连续100次" );
			_cnt_a = 0;
		}

		trace( "A1a" );
	}
	if( _W == vk )
	{
		_cnt_w++;
		_cnt_d = 0;
		_cnt_a = 0;
		_cnt_s = 0;
		if( _cnt_w > 100 )
		{
			assert( false
			&& "按键W连续100次" );
			_cnt_w = 0;
		}
		trace( "W1a" );
	}
	if( _S == vk )
	{
		_cnt_s++;
		_cnt_d = 0;
		_cnt_a = 0;
		_cnt_w = 0;
		if( _cnt_s > 100 )
		{
			assert( false
			&& "按键W连续100次" );
			_cnt_s = 0;
		}

		trace( "S1a" );
	}

#endif
	*/
}

void Move::_trace_down_end( long vk )
{
	/*
#ifdef _DEBUG
	if( _D == vk )
		trace( "D1b" );
	if( _A == vk )
		trace( "A1b" );
	if( _W == vk )
		trace( "W1b" );
	if( _S == vk )
		trace( "S1b" );
#endif
		*/
}

void Move::_trace_up_begin( long vk )
{
	/*
#ifdef _DEBUG
	if( _D == vk )
		trace( "D2a" );
	if( _A == vk )
		trace( "A2a" );
	if( _W == vk )
		trace( "W2a" );
	if( _S == vk )
		trace( "S2a" );
#endif
		*/
}

void Move::_trace_up_end( long vk )
{
	/*
#ifdef _DEBUG
	if( _D == vk )
		trace( "D2b" );
	if( _A == vk )
		trace( "A2b" );
	if( _W == vk )
		trace( "W2b" );
	if( _S == vk )
		trace( "S2b" );
#endif
	*/
}

//////////////////////////////////////////////////////
//	World;

World::World( Dm& dm_, AttachIme& attach_ime_ )
	:dm( dm_ ),
	dnf( dm_._cdnf ),
	attach_ime( attach_ime_ ),
	player( dm_._cdnf ),
	move( dm_._cdnf )
	//,is_enemy_cloned( false )
#ifdef _DEBUG
	,debug_cnt( 0 )
#endif
{
}

void World::update_all()
{
	update_player_map_pos();
	update_player_win_pos();
	update_target_group_map_pos();
	update_target_group_win_pos();
	update_mouse_pos();
	update_block_pos();
	update_map_win_pos();
	update_boss_room_win_pos();
}

void World::_fix_player_win_pos()
{
	player.win_pos.x += 26;
	player.win_pos.y += 85;
}

bool World::update_player_map_pos()
{
#ifdef _USE_DM_MEM
	dnf.GetPlayerPoint( player.map_pos.x, player.map_pos.y, player.map_pos.z );
#else
	//attach_ime.update_player_data( player );

	const _Data& player_data = attach_ime.update_player_data();
	player.map_pos.x = player_data.x;
	player.map_pos.y = player_data.y;
	player.map_pos.z = player_data.z;

	player.map_pos.corrent_assert();

#endif
	return true;
}


bool World::update_player_win_pos()
{
//	if( player.map_pos == Pos() )
//		return false;
	player.map_pos.corrent_assert();

	dnf.GetWinPlayerPoint2( player.win_pos.x, player.win_pos.y );

#ifdef _DEBUG
	if( -1 == player.win_pos.x &&
		-1 == player.win_pos.y )
		debug_cnt++;
	else
		debug_cnt = 0;

	assert( debug_cnt < 1000 
		&& "连续1000次无法找到得到player.win_pos" );
#endif 
	//_fix_player_win_pos();

	return true;
}

bool World::update_target_group_map_pos()
{
#ifdef	_USE_DM_MEM
	//target_group.clear();

	while(!target_group.empty())
		target_group.pop_back();

	int nTargetNum = dnf.GetTargerNum();
	//	此处可以优化效率,拷贝构造

	/*
	assert( !nTargetNum
	&& "nTargetNum的个数为0" );

	if( !nTargetNum )
	exception( "nTargetNum == 0" );
	*/

	for( size_t i=0 ; i< (size_t)nTargetNum; i++)
	{
		Target target;
		if( dnf.GetTargetInfor(i, target.id, target.map_pos.x, target.map_pos.y, target.map_pos.z ) )
			//if( target.id != 33 &&
			//	target.id != 273 &&
			//	target.id != 1057 )
			target_group.push_back( target );
	}
#else
	//attach_ime.update_target_data( target_group );

	target_group.clear();


	size_t target_size;
	const _Data* target_data_ptr = attach_ime.update_target_data( target_size );

	target_group.resize( target_size );

	for( size_t i = 0; i < target_size; i++ )
	{
		//	处理33的特殊情况
		if( 33 == target_data_ptr[ i ].id 
			&&
				(
					target_data_ptr[ i ].x < 0 
					|| target_data_ptr[ i ].x > 3000 
					|| target_data_ptr[ i ].y < 0 
					|| target_data_ptr[ i ].y > 3000 
					|| target_data_ptr[ i ].z < 0 
					|| target_data_ptr[ i ].z > 3000 
				)
			)
		{
			target_group[ i ].id = 33;
			target_group[ i ].map_pos.x = 0;
			target_group[ i ].map_pos.y = 0;
			target_group[ i ].map_pos.z = 0;
		}
		//	处理1057的的特殊情况
		else if( 1057 == target_data_ptr[ i ].id 
			&& -34 == target_data_ptr[ i ].x
			)
		{
			target_group[ i ].id = 1057;
			target_group[ i ].map_pos.x = 34;
			target_group[ i ].map_pos.y = target_data_ptr[ i ].y;
			target_group[ i ].map_pos.z = target_data_ptr[ i ].z;
		}
		else
		{
			/*
			if( 529 == target_data_ptr[ i ].id 
			&& 
				(
					target_data_ptr[ i ].x < 0
					|| target_data_ptr[ i ].x > 3000
					|| target_data_ptr[ i ].y < 0
					|| target_data_ptr[ i ].y > 3000
					|| target_data_ptr[ i ].z < 0
					|| target_data_ptr[ i ].z > 3000
				)
			)
			{
				if( !is_enemy_cloned )
				{
					enemy_cloned_timer.restart();
					is_enemy_cloned = true;
				}
				else
					if( enemy_cloned_timer.elapsed() > 15 )
						assert( false
						&& "enemy_cloned_timer超时" );
			}			
			else if( 529 == target_data_ptr[ i ].id )
			{
				kk
			}
			*/

			target_group[ i ].id = target_data_ptr[ i ].id;
			target_group[ i ].map_pos.x = target_data_ptr[ i ].x;
			target_group[ i ].map_pos.y = target_data_ptr[ i ].y;
			target_group[ i ].map_pos.z = target_data_ptr[ i ].z;
		}

	}

#ifdef _DEBUG
			for( size_t i = 0; i < target_group.size(); i++ )
			{
				if( 529 == target_group[ i ].id 
					&& -10000 == target_group[ i ].map_pos.x )
					continue;
				if( 1057 == target_group[ i ].id
					&& -34 == target_group[ i ].map_pos.x )
					continue;

				target_group[ i ].map_pos.corrent_assert();
				assert( target_group[ i ].id > -1 
					&& "update_target_data->id设置异常" );
			}
#endif


#endif

#ifdef _DEBUG
	assert( !target_group.empty()
		&& "update_target_group为空" );

	for( size_t i = 0; i < target_group.size(); i++ )
	{
		if( 529 == target_group[ i ].id 
			&& target_group[ i ].map_pos.x == -10000 )
			continue;
		target_group[ i ].map_pos.corrent_assert();
	}
#endif 

	return true;
}

bool World::update_target_group_win_pos()
{
	int x_offset = 0;
	int y_offset = 0;

	for( size_t i = 0; i < target_group.size(); i++ )
	{
		x_offset = player.map_pos.x - target_group[ i ].map_pos.x;
		target_group[ i ].win_pos.x = player.win_pos.x - x_offset;

		y_offset = player.map_pos.y - target_group[ i ].map_pos.y;
		target_group[ i ].win_pos.y = player.win_pos.y - y_offset;
	}

	return true;
}

bool World::update_block_pos()
{
	//block_pos.clear();
	while(!block_pos.empty())
		block_pos.pop_back();

	assert( !target_group.empty()
		&& "target_group的个数为0" );

	//if( target_group.empty() )
		//exception( "target_group的个数为0" );

	if( target_group.empty() )
		return false;

	Pos pos;
	for( size_t i = 0; i < target_group.size(); i++ )
		if( target_group[ i ].id == 1057
			|| target_group[ i ].id == 33 )
		{
			pos.x = target_group[ i ].map_pos.x;
			pos.y = target_group[ i ].map_pos.y;
			block_pos.push_back( pos );
		}

	assert( !block_pos.empty()
		&& "block的个数为0" );

	//if( block_pos.empty() )
		//throw exception( "block_pos.size() == 0" );

	return true;
}

bool World::update_mouse_pos()
{
	long x, y;

	if( dnf.GetMousePos( x, y ) )
	{
		mouse_pos.x = x;
		mouse_pos.y = y;

		return true;
	}
	else
		return false;
}

bool World::_is_inside_window( Pos& pos )
{
	if( pos.x > 1 && pos.x < 800 &&
		pos.y > 1 && pos.y < 600 )
		return false;
	else
		return true;
}


bool World::update_map_win_pos()
{
	dnf.GetMapPoint( map_pos.x, map_pos.y );


	//if( map_pos.is_invaild() )
	//{
	//cout << "map_is_invaild()" << endl;
	string str = dnf.GetMapName();
	if( str.find( "树精繁殖" ) != string::npos )
	{
		//cout << "找到树精繁殖" << endl;
		Pos chair_pos;
		dnf.GetChairPoint( chair_pos.x, chair_pos.y );

		if( !map_pos.is_invaild() )
		{
			//cout << "!map_is_invaild()" << endl;
			//cout << "_last_map_pos:" 
			//	<< _last_map_pos.x << ","
			//	<< _last_map_pos.y << endl;

			//cout << "char_pos:" 
			//	<< chair_pos.x << ","
			//	<< chair_pos.y << endl;

			_last_map_pos = map_pos;
		}
		else 
		{
			Sleep( 500 );
			update_boss_room_win_pos();
			if( !chair_pos.is_invaild()
				&& !boss_room_pos.is_invaild() )
			{
				//cout << "_last_map_pos:" 
				//	<< _last_map_pos.x << ","
				//	<< _last_map_pos.y << endl;

				//cout << "char_pos:" 
				//	<< chair_pos.x << ","
				//	<< chair_pos.y << endl;

				//cout << "distance"
				//	<< std::abs( _last_map_pos.x - chair_pos.x )
				//	<< std::abs( _last_map_pos.y - chair_pos.y ) << endl;

				if( _last_map_pos.is_init()
					&&
					(
					( std::abs( _last_map_pos.x - chair_pos.x ) < 20 
					&& _last_map_pos.y == chair_pos.y 
					&& compute_distance( _last_map_pos, chair_pos ) < 20
					)
					||
					( std::abs( _last_map_pos.y - chair_pos.y ) < 20 
					&& _last_map_pos.x == chair_pos.x )
					&& compute_distance( _last_map_pos, chair_pos ) < 20
					)
					)
				{
					//cout << "set" << endl;
					map_pos = chair_pos;
				}
			}
		}
	}
	//}

	return true;
}

bool World::update_map_win_pos_block( size_t limit_time, size_t step_time, bool has_assert )
{
	//dnf.GetMapPoint( map_pos.x, map_pos.y );
	update_map_win_pos();
	if( !map_pos.is_invaild() )
		return true;

	boost::timer out_timer;
	while( true )
	{
		//dnf.GetMapPoint( map_pos.x, map_pos.y );
		update_map_win_pos();
		if( !map_pos.is_invaild() )
		{
			trace( "==================World::update_map_win_pos_block:return_true" );
			return true;
		}

		trace( "World::update_map_win_pos_block:map_pos.is_invaild()" );

		if( out_timer.elapsed() > limit_time )
		{
			if( has_assert )
			assert( false 
				&& "World::update_map_win_pos_block:获取失败" );

			trace( "World::update_map_win_pos_block:out_time" );

			return false;
		}

		Sleep( step_time );
	}

	return false;
}

bool World::update_boss_room_win_pos()
{
	dnf.GetBossRoomPoint( boss_room_pos.x, boss_room_pos.y );

	if( -1 == boss_room_pos.x 
		&& -1 == boss_room_pos.y )
		return false;
	// 设置偏移
	boss_room_pos.x += 5;
	boss_room_pos.y += 2;

	return true;
}

bool World::update_boss_room_win_pos_block( size_t limit_time, size_t step_time, bool has_assert )
{
	boost::timer out_timer;

	while( true )
	{
		if( update_boss_room_win_pos() )
			return true;

		if( out_timer.elapsed() > limit_time )
		{
			if( has_assert )
				assert( false 
					&& "World::update_boss_room_win_pos_block:获取失败" );

			trace( "World::update_boss_room_win_pos_block:获取失败" );

			return false;
		}

		Sleep( step_time );
	}

	return false;
}


Pos World::get_win_pos_by_player( const Pos& map_pos )
{
	map_pos.corrent_assert();

	Pos win_pos;
	int x_offset = player.map_pos.x - map_pos.x;
	win_pos.x = player.win_pos.x - x_offset;

	int y_offset = player.map_pos.y - map_pos.y;
	win_pos.y = player.win_pos.y - y_offset;

	return win_pos;
}

void World::corrent_assert()
{
	update_all();
	player.map_pos.corrent_assert();
	assert( player.map_pos.x > 0 
		&& player.map_pos.y > 0
		&& "人物坐标小于0" );

	for( size_t i = 0 ; i < target_group.size(); i++ )
	{
		assert( target_group[ i ].map_pos.x >= 0
			&& "target_group超出范围" );

		assert( target_group[ i ].map_pos.x < 10000
			&& "target_group超出范围" );

		assert( target_group[ i ].map_pos.y >= 0
			&& "target_group超出范围" );

		assert( target_group[ i ].map_pos.y < 10000
			&& "target_group超出范围" );
	}

	assert( mouse_pos.x >= 0
		&& mouse_pos.x < 800
		&& mouse_pos.y >= 0
		&& mouse_pos.y < 600
		&& "mouse_pos超出范围" );

	assert( player.win_pos.x >= -1 
		&& player.win_pos.x < 800
		&& player.win_pos.y >= -1
		&& player.win_pos.y < 600
		&& "playaer.win_pos超出范围" );
}


void World::moveto( const Pos& pos )
{

	pos.corrent_assert();
	pos.init_assert();


	glog( "World::moveto" );
	update_player_map_pos();
	player.map_pos.corrent_assert();
	player.map_pos.init_assert();

	//assert( pos != player.map_pos  
	//	&& "World::moveto:pos为player所在的位置,并不需要移动" );
	

	glog( "World::moveto:start_move" );
	move( player.map_pos, pos );
	glog( "World::moveto:end_move" );
}

bool World::update_big_map_win_pos()
{
	dnf.GetBigMapPoint( big_map_pos.x, big_map_pos.y );
	return true;
}

bool World::_is_open_big_map()
{
	//string big_map_check_str = dnf.dm.Ocr( 458,4,559,28, "ffffff-000000", 1.0 );
	string big_map_check_str = dnf.dm.Ocr( 293,4,389,28, "e6e6e6-030303", 1.0 );
	
	//trace( big_map_check_str );
	//	如果画面顶部找到 "npc功能查询" 字样
	//	那就表示大地图打开了
	if( "NPC功能查询" == big_map_check_str )
		return true;
	else
		return false;
}

bool World::_big_map_move_to( const Pos& pos )
{
	player.RB( pos );
	
	Pos target_pos;
	//	修正鼠标和识别坐标的差距
	target_pos.x = pos.x - 4;
	target_pos.y = pos.y - 12;

	target_pos.corrent_assert();
	target_pos.init_assert();
	update_big_map_win_pos();

	if( abs( target_pos.x - big_map_pos.x ) < 4
		&& abs( target_pos.y - big_map_pos.y ) < 4
		)
		return true;
	else
		return false;
}


//大地图移动
bool World::big_map_move_to( const Pos& pos )		// 167,308
{
	pos.corrent_assert();
	pos.init_assert();


	block_call( boost::bind( &World::_open_big_map, this ), 60 );

	//int k = 0;
	//while( true )
	//{
	//	player.key_press( 0x4E ); //	N 打开大地图
	//	Sleep( 3000 );
	//	for( size_t i = 0; i < 200; i++ )
	//	{
	//		update_big_map_win_pos();
	//		if( !big_map_pos.is_invaild() )
	//			break;
	//		Sleep( 1 );
	//	}
	//	
	//	big_map_pos.corrent_assert();
	//	player.key_press( 0x4E ); //	N 打开大地图
	//	Sleep( 1000 );

	//	k++;
	//}

	//if( !_is_open_big_map() )
	//{
	//	player.key_press( 0x4E ); //	N 打开大地图
	//	Sleep( 100 );
	//}

	for( size_t i = 0; i < 50; i++ )
	{
		update_big_map_win_pos();
		if( !big_map_pos.is_invaild() )
			break;
		Sleep( 10 );
	}
	/*
	if(	big_map_pos.is_invaild() )
	{
		player.key_press( 0x4E ); //	N 打开大地图
		Sleep( 100 );

		for( size_t i = 0; i < 50; i++ )
		{
			update_big_map_win_pos();
			if( !big_map_pos.is_invaild() )
				break;
			Sleep( 10 );
		}
	}
	*/

	//if(	big_map_pos.is_invaild() )
	//{
	//	player.key_press( VK_ESCAPE, 1 );
	//	player.key_press( 0x4E , 1 ); //	N 打开大地图
	//	Sleep( 50 );
	//	update_big_map_win_pos();
	//}


	big_map_pos.corrent_assert();
	big_map_pos.init_assert();

	//if( big_map_pos.is_invaild() )
	//{
	//	player.key_press( 0x4E , 1 ); //	N 打开大地图
	//	return false;
	//}
///*
//	player.RB( pos );
//	player.RB( pos )*/;

	//Pos target_pos;
	////	修正鼠标和识别坐标的差距
	//target_pos.x = pos.x - 4;
	//target_pos.y = pos.y - 12;

	//target_pos.corrent_assert();
	//target_pos.init_assert();

	block_call( boost::bind( &World::_big_map_move_to, this, pos ), 100, 100 );

	//boost::timer out_timer;
	//while( true )
	//{
	//	update_big_map_win_pos();
	//	//if( 163 == big_map_pos.x
	//	//	&& 296 == big_map_pos.y )
	//	if( abs( target_pos.x - big_map_pos.x ) < 4
	//		&& abs( target_pos.y - big_map_pos.y ) < 4
	//		)
	//		break;

	//	if( out_timer.elapsed() > 300 )
	//	{
	//		assert( false
	//			&& "World::big_map_move_to:5分钟仍然不能走到目的地" );

	//		return false;
	//	}

	//	Sleep( 100 );
	//}

	player.key_press( 0x4E ); //	N 打开大地图

	//	Watch::pause
	//	需要
	block_call( 
	Sync::is_nonstop_all, 
	0x1000000, 100 );

	return true;
}

bool World::_open_big_map()
{
	player.key_press( 0x4E );
	Sleep( 100 );

	if( _is_open_big_map() )
		return true;
	else
		return false;
}

void World::open_big_map()
{
	block_call( boost::bind( &World::_open_big_map, this ), 20 );
}

bool World::to_luo_lan()
{
	return _to_dungeon( Pos( 605, 359 ), Dir::right ); 
}

bool World::to_luo_lan_zhi_sen()
{
	return _to_dungeon( Pos( 167, 309 ), Dir::left ); 
}

bool World::to_tian_kong_zhi_cheng()
{
	return _to_dungeon( Pos( 660, 277 ), Dir::right );
}

bool World::to_tian_wei_ju_shou()
{
	return _to_dungeon( Pos( 610, 210 ), Dir::right );
}


	
bool World::to_ya_nuo_fa_sen_lin()//		1-16
{
	return _to_dungeon( Pos( 734, 315 ), Dir::right );
}
bool World::to_e_yun_zhi_cheng()//		17-26
{
	return _to_dungeon( Pos( 86,196 ), Dir::left );
}
bool World::to_piao_liu_dong_xue()//		27-54
{
	return _to_dungeon( Pos( 98, 374 ), Dir::left );
}
bool World::to_ni_liu_pu_bu()//			37-45
{
	return _to_dungeon( Pos( 657,466 ), Dir::right );
}
bool World::to_wang_zhe_xia_gu()
{
	return _to_dungeon( Pos( 287,189 ), Dir::left );
}
bool World::to_jue_wang_bing_ya()
{
	return _to_dungeon( Pos( 693,132 ), Dir::right );
}
bool World::to_ge_lan_zhi_sen_shan_lu()//	27-70
{
	return _to_dungeon( Pos( 205, 435 ), Dir::left );
}

bool World::_to_dungeon( const Pos& pos, Dir dir )
{

	pos.corrent_assert();
	pos.init_assert();
	assert( Dir::no != dir 
		&& "World::_to_dungeon:dir==no" );

	if( big_map_move_to( pos ) )	//	洛兰pos
	{
		if( Dir::left == dir )
			player.A( 1000 );
		if( Dir::right == dir )
			player.D( 1000 );

		/*
		for( size_t i = 0; i < 20; i++ )
		{
			if( _is_in_select_dungeon_ui() )
				return true;

			if( Dir::left == dir )
			{
				player.D( 300 );
				player.A( 1000 );
			}
			if( Dir::right == dir )
			{
				player.A( 300 );
				player.D( 1000 );
			}
			Sleep( 10 );
		}
		*/

		block_call( boost::bind( &World::_to_select_dungeon_ui, this, dir ), 20 );

		return false;
	}
	else
		return false;
}

bool World::_to_select_dungeon_ui( const Dir& dir )
{
	if( Dir::left == dir )
	{
		player.D( 300 );
		player.A( 1000 );
	}
	else if( Dir::right == dir )
	{
		player.A( 300 );
		player.D( 1000 );
	}
	else
	{
		assert( false
			&& "World::_is_in_select_dungeon_ui:错误的dir" );
	}

	if( _is_in_select_dungeon_ui() )
		return true;
	else
		return false;
}

bool World::_is_in_select_dungeon_ui( )
{
	string check_str = dnf.dm.Ocr( 690,538,787,567, "ddc593-000000", 1.0 );
	//trace( check_str );
	if( "返回城镇" == check_str )
		return true;
	else
		return false;
}


bool World::_select_dungeon( const string& dungeon_name )
{
	player.key_press( VK_SPACE );
	Sleep( 100 );

	if( is_in_dungeon( dungeon_name ) )
		return true;
	else
		return false;
}

bool World::select_dungeon( const string& dungeon_name, int difficulty )
{
	assert( !dungeon_name.empty()
		&& "World::select_dungeon:dungeon_name为空" );

	assert( 0 == difficulty
		|| 1 == difficulty
		|| 2 == difficulty 
		|| -1 == difficulty 
		&& "World::select_dungeon:difficuly错误" );

	assert( _is_in_select_dungeon_ui()
		&& "World::select_dungeon:不在选择界面" );

	
	string last_dungeon_name;
	const size_t S = 0x53;
	const size_t W = 0x57;
	size_t vk = S;
	while( true )
	{
		//	大转移修改
		//string current_dungeon_name = dnf.dm.Ocr( 696,475,787,500, "ddc593-000000", 1.0 );
		string current_dungeon_name = dnf.dm.Ocr( 660,475,795,500, "ddc593-000000", 1.0 );
		if( current_dungeon_name
			== dungeon_name.c_str() )
			break;

		if( current_dungeon_name
			== last_dungeon_name )
		{
			if( S == vk  )
				vk = W;
			else if( W == vk )
				vk = S;
		}

		player.key_press( vk, 10 );

		last_dungeon_name = current_dungeon_name;

		Sleep( 50 );
	}
	
	if( -1 == difficulty )
	{
		//bool b1 = dnf.has_pic_easy();
		player.D( 10 );
		Sleep( 50 );
		//bool b2 = dnf.has_pic_mid();
		player.D( 10 );
		Sleep( 50 );
		//bool b3 = dnf.has_pic_hard();
		player.D( 10 );
		Sleep( 50 );
	}

	if( 0 == difficulty )
	{
		for( size_t i = 0; i < 10; i++ )
		{
			player.A( 10 );
			Sleep( 50 );

			if( 0 == _get_current_difficulty() )
				break;
		}
	}

	if( 1 == difficulty )
	{
		for( size_t i = 0; i < 10; i++ )
		{
			int diff = _get_current_difficulty();
			if( 0 == diff )
			{
				player.D( 10 );
				Sleep( 50 );
			}
			if( 2 == diff )
			{
				player.A( 10 );
				Sleep( 50 );
			}

			if( 1 == diff )
				break;
		}
	}

	if( 2 == difficulty )
	{
		for( size_t i = 0; i < 10; i++ )
		{
			player.D( 10 );
			Sleep( 50 );

			if( 2 == _get_current_difficulty() )
				break;
		}
	}
/*
	boost::timer out_timer;
	while( true )
	{
		player.key_press( VK_SPACE );
		Sleep( 100 );

		if( is_in_dungeon( dungeon_name ) )
			return true;

		if( out_timer.elapsed() > 15 )
		{
			assert( false
				&& "World::select_dungeon:选择地下城超时" );

			return false;
		}
	}
	*/

	block_call( boost::bind( &World::_select_dungeon, this, dungeon_name ), 30 );

	return true;
}

int	World::_get_current_difficulty()
{
	if( dnf.has_pic_easy() )
		return 0;
	if( dnf.has_pic_mid() )
		return 1;
	if( dnf.has_pic_hard() )
		return 2;

	return -1000;
}


bool World::is_in_dungeon( const string& dungeon_name )
{
	for( size_t i = 0; i < 20; i++ )
	{
		if( dnf.GetMapName().find( dungeon_name ) != string::npos )
			return true;

		Sleep( 500 );
	}

	return false; 
}

bool World::is_in_big_map()
{
	if( dnf.has_pic_big_map_check() )
		return true;
	else
		return false;
}

bool World::_to_big_map()
{
	if( is_in_big_map() )
		return true;
	else
	{
//		player.key_press( VK_F12 );

		open_esc();
		Sleep( 100 );
		player.LB( Pos( 517,424 ) );

		Sleep( 100 );
		if( has_weak_notice() )
			close_weak_notice();

		return false;
	}
}

void World::to_big_map()
{
	block_call( boost::bind( &World::_to_big_map, this ), 30 );
	//close_esc();

/*
	boost::timer out_timer;

	while( true )
	{
		if( !is_in_big_map() )
			player.key_press( VK_F12 );
		else
			break;

		if( out_timer.elapsed() > 300 )
		{
			assert( false
				&& "World::to_big_map:5分钟也无法走到大地图" );

			break;
		}

		Sleep( 200 );
	}

	*/
}

bool World::_to_big_map_by_f12()
{
	if( is_in_big_map() )
		return true;
	else
	{
		player.key_press( VK_F12 );

		return false;
	}
}

void World::to_big_map_by_f12()
{
	block_call( boost::bind( &World::_to_big_map_by_f12, this ), 30 );
	close_esc();
}

bool World::is_dungeon_end()
{
	return dnf.has_pic_dungeon_end();
	//return !dnf.has_pic_reward();
}

bool World::is_dungeon_reward()
{
	return dnf.has_pic_reward();
}

bool World::is_dungeon_reward_end()
{
	//trace( is_dungeon_reward() );
	return false;//!is_dungeon_reward();
}

bool World::is_dungeon_score()
{
	return dnf.has_pic_score();
}

bool World::is_dead()
{
	string str = dnf.dm.Ocr(
		396,282,484,307,
		"e6c89b-010101", 1.0 );
	if( str.find( "复活" ) != string::npos )
		return true;
	else
		return false;
}

bool World::is_low_hp()
{
	if( !dnf.has_pos_color( 39,543, "a02726-020202" ) )
		return true;
	else
		return false;
}

bool World::is_low_mp()
{
	if( !dnf.has_pos_color( 759,545, "245cc7-020202" ) )
		return true;
	else
		return false;
}

bool World::has_weak_notice()
{
	string str = dnf.dm.Ocr(
		433,275,478,296, "ffffff-000000", 1.0 );
	if( str.find( "虚弱" ) != string::npos )
		return true;
	else
		return false;
}

bool World::has_no_weak_notice()
{
	return !has_weak_notice();
}

void World::close_weak_notice()
{
	block_call(
		boost::bind( &World::is_in_big_map, this ),
		boost::bind( null ),
		boost::bind( &Player::LB, &player, Pos( 372,340 ) ),
		20, 100, false );
}

bool World::is_weak()
{
	//if( is_low_hp()
	//	|| is_low_mp() )
	//	return true;
	//else
	//	return false;

	//	大转移修改
	//if( dnf.has_pic( 
	//	"weak.bmp",264,526,530,556 ) )
	if( dnf.has_pic( 
		"weak.bmp",317,478,528,552 ) )
		return true;
	else
		return false;
}

Pos World::weak_pos()
{
	Pos pos;

	dnf.GetWeakPoint( pos.x, pos.y );

	return pos;
}

bool World::has_weaked_notice()
{
	string str = dnf.dm.Ocr(
		370,289,471,316,
		"ff3333-020202", 1.0 
		);

	if( str.find( "虚弱" ) != string::npos )
		return true;
	else
		return false;
}

bool World::has_no_weaked_notice()
{
	return !has_weaked_notice();
}

void World::close_weaked_notice()
{
	block_call(
		boost::bind( &World::has_no_weaked_notice, this ),
		boost::bind( null ),
		boost::bind( &Player::LB, &player, Pos( 399,346 ) ),
		20, 100, false );
}

bool World::has_restore_notice()
{
	string str = dnf.dm.Ocr(
		364,307,457,329,
		"ffffff-000000", 1.0 );
	if( str.find( "交给索" ) != string::npos )
		return true;
	else
		return false;
}

void World::close_restore_notice()
{
	block_call(
		boost::bind( &World::has_no_restore_notice, this ),
		boost::bind( null ),
		boost::bind( &Player::LB, &player, Pos(371,405) ),
		20, 200, false );
	Sleep( 10 * 1000 );
}

bool World::has_no_restore_notice()
{
	return !has_restore_notice();
}


bool World::has_unusual()
{
	string str = dnf.dm.Ocr( 0,0, 799, 599, "ffffff-000000", 1.0 );
	if( str.find( "数据异常" ) != string::npos )
		return true;
	else
		return false;
}
bool World::_handle_unusual()
{
	player.key_press( VK_ESCAPE );
	if( !has_unusual() )
		return true;
	else
		return false;
}
void World::handle_unusual()
{
	block_call( boost::bind( &World::_handle_unusual, this ) );
}

bool World::has_dis_connect()
{
	string str = dnf.dm.Ocr( 0,0, 799, 599, "ffffff-000000", 1.0 );
	if( str.find( "网络连接中断" ) != string::npos )
		return true;
	else
		return false;
}
bool World::_handle_dis_connect()
{
	player.key_press( VK_ESCAPE );
	if( !has_dis_connect() )
		return true;
	else
		return false;
}
void World::handle_dis_connect()
{
	block_call( boost::bind( &World::_handle_dis_connect, this ) );
}

bool World::has_safe_mode()
{
	string str = dnf.dm.Ocr( 394,247,496,272, "ffffff-000000", 1.0 );
	if( str.find( "安全模式" ) != string::npos )
	{
		return true;
		//_world.player.key_press( VK_ESCAPE );
	}
	else
		return false;
}
bool World::_handle_safe_mode()
{
	player.key_press( VK_ESCAPE );
	if( !has_safe_mode() )
		return true;
	else
		return false;
}
void World::handle_safe_mode()
{
	block_call( boost::bind( &World::_handle_safe_mode, this ) );
}

bool World::has_motion()
{
	string str = dnf.dm.Ocr(347,443,454,469,"ddc593-000000", 1.0);
	if( str.find( "关闭" ) != string::npos )
		return true;
	else
		return false;
}

bool World::_close_motion()
{
	player.LB( Pos( 400, 455 ) );
	if( !has_motion() )
		return true;
	else
		return false;
}

void World::close_motion()
{
	block_call( boost::bind( &World::_close_motion, this ) );
}

bool World::has_notice()
{
	string str = dnf.dm.Ocr( 375,222,417,240, "ffffff-000000", 1.0 );

	if( str.find( "公告" ) != string::npos )
		return true;
	else
		return false;
}

bool World::_close_notice()
{
	if( !has_notice() )
		return true;
	else
	{
		player.key_press( VK_ESCAPE );
	}

	return false;

	/*
	boost::timer out_timer;
	while( true )
	{
		if( !has_notice() )
			break;
		else
			player.key_press( VK_ESCAPE );

		if( out_timer.elapsed() > 15 )
		{
			assert( false 
				&& "World::close_notice:关闭公告超时" );
			break;
		}

		Sleep( 100 );
	}
	*/
}

void World::close_notice()
{
	block_call( boost::bind( &World::_close_notice, this ), 20 );
}

bool World::has_esc()
{
	string str = dnf.dm.Ocr( 350,143,431,168, "ffffff-000000", 1.0 );
	//cout << str << endl;
	Sleep( 100 );
	if( str.find( "游戏菜单" ) != string::npos )
		return true;
	else
		return false;
}
bool World::_open_esc()
{
	player.key_press( VK_ESCAPE );
	Sleep( 100 );
	if( has_esc() )
		return true;
	else
		return false;
}

void World::open_esc()
{
	block_call( boost::bind( &World::_open_esc, this ), 20  );
}

bool World::_close_esc()
{
	if( !has_esc() )
		return true;
	else
		player.key_press( VK_ESCAPE );

	return false;

	//boost::timer out_timer;
	//while( true )
	//{
	//	if( !has_esc() )
	//		break;
	//	else
	//		player.key_press( VK_ESCAPE );

	//	if( out_timer.elapsed() > 15 )
	//	{
	//		assert( false
	//			&& "World::close_esc:关闭esc超时" );
	//	}

	//	Sleep( 100 );
	//}
}

void World::close_esc()
{
	block_call( boost::bind( &World::_close_esc, this ), 20 );
}


bool World::has_info()
{

	string str = dnf.dm.Ocr( 178,107,315,126, 
		"ffffff-000000", 1.0 );
	if( str.find( "个人信息" ) != string::npos )
		return true;
	else
		return false;
}

bool World::has_no_info()
{
	return !has_info();
}

bool World::_open_info()
{
	return true;
}

void World::open_info()
{
	block_call( 
		boost::bind( &World::has_info, this ),
		boost::bind( null ),
		boost::bind( &Player::key_press, &player, 0x4D, 50 ),
		30, 300 );
}

bool World::_close_info()
{
	return true;
}

void World::close_info()
{
	block_call( 
		boost::bind( &World::has_no_info, this ),
		boost::bind( null ),
		boost::bind( &Player::key_press, &player, 0x4D, 50 ),
		30, 300 );
}


bool World::has_equip()
{
	//string str = dnf.dm.Ocr( 550,52,618,76, "ffffff-000000", 1.0 );
	string str = dnf.dm.Ocr( 555,40,627,62, "ffffff-000000", 1.0 );
	if( str.find( "装备栏" ) != string::npos )	
		return true;
	else
		return false;
}

bool World::_open_equip()
{
	player.key_press( 0x49 ); // I
	Sleep( 10 );

	if( has_equip() )
		return true;

	return false;
}

void World::open_equip()
{
	block_call( boost::bind( &World::_open_equip, this ), 20 );
}

bool World::_close_equip()
{
	if( !has_equip() )
		return true;
	else
	{
		player.key_press( 0x49 ); // I
		Sleep( 10 );
		return false;
	}
}

void World::close_equip()
{
	block_call( boost::bind( &World::_close_equip, this), 20 );
}

bool World::is_out_weight()
{
	
	//	大转移修改
	//if( dnf.has_pos_color( 612,473, "873838-050505" ) )
	if( dnf.has_pos_color( 612,486, "883333-050505" ) )
	{
		trace( "超出负重" );
		return true;
	}	
	else
		return false;
}

bool World::is_full_weight()
{
	//	大转移修改
	//if( dnf.has_pos_color( 647,473, "873838-050505" ) )
	if( dnf.has_pos_color( 647,486, "883333-050505" ) )
	{
		trace( "满负重" );
		return true;
	}
	else
		return false;
}

bool World::has_skill()
{
	string str = dnf.dm.Ocr(  511,39,585,64, "ddc593-000000", 1.0 );
	string str2 = dnf.dm.Ocr(  511,39,585,64, "8c8c8c-000000", 1.0 );
	if( str.find( "初始化" ) != string::npos
		|| str2.find( "初始化" ) != string::npos )
		return true;
	else
		return false;
}

bool World::_open_skill()
{
	player.key_press( 0x4B ); // K
	Sleep( 10 );

	if( has_skill() )
		return true;

	return false;
}

void World::open_skill()
{
	block_call( boost::bind( &World::_open_skill, this ), 20 );
}

bool World::_close_skill()
{
	if( !has_skill() )
		return true;
	else
	{
		player.key_press( 0x4B ); // K
		Sleep( 10 );
		return false;
	}
}

void World::close_skill()
{
	block_call( boost::bind( &World::_close_skill, this ), 20 );
}


bool World::quit_for_reward()
{
	player.LB( Pos(367,373 ) );
	string str = dnf.dm.Ocr( 369,186,429,210, "ffffff-000000", 1.0 );
	if( str.find( "公告" ) == string::npos )
		return true;
	else
		return false;
}

void World::quit_game()
{
	open_esc();
	
	player.LB( Pos( 573,427 ) );
	player.LB( Pos( 573,427 ) );
	player.LB( Pos( 573,427 ) );

	string str = dnf.dm.Ocr( 369,186,429,210, "ffffff-000000", 1.0 );
	if( str.find( "公告" ) != string::npos )
		block_call( boost::bind( &World::quit_for_reward, this ) );

	attach_ime.set_en();
	dm.unbind_wnd();
}


bool World::quit_for_reward2()
{
	player.LB( Pos(367,373 ) );
	string str = dnf.dm.Ocr( 357,218,497,253, "e6c89b-000000", 1.0 );
	if( str.find( "领取" ) == string::npos )
		return true;
	else
		return false;
}

void World::quit_game2()
{
	open_esc();
	
	player.LB( Pos( 573,427 ) );
	player.LB( Pos( 573,427 ) );
	player.LB( Pos( 573,427 ) );

	Sleep( 1000 );

	string str = dnf.dm.Ocr( 357,218,497,253, "e6c89b-000000", 1.0 );
	if( str.find( "领取" ) != string::npos )
		block_call( boost::bind( &World::quit_for_reward, this ) );

	attach_ime.set_en();
	dm.unbind_wnd();
}

bool World::has_energy()
{
	if( dnf.has_pos_color( 318,554, "b30026-050505" )
		|| dnf.has_pos_color( 318,554, "008ff1-050505" ) )
		return true;
	else
		return false;
}

bool World::is_return_select_character()
{
	string str = dnf.dm.Ocr( 
		500,531,590,560, "ddc593-000000", 1.0 );

	if( str.find( "选择频道" ) != string::npos )
		return true;
	else
		return false;
}

void World::return_select_character()
{
	open_esc();
	block_call(
		boost::bind( 
			&World::is_return_select_character, this 
			),
		boost::bind( null ),
		boost::bind( &Player::LB, &player, Pos( 398,427 ) ),
		60, 1000 );

}


bool World::has_process( const string& process_name )
{
	
		// 获取当前pid；
	//DWORD pid = GetCurrentProcessId(); 

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	PROCESSENTRY32 pe = { 0 };  
	pe.dwSize = sizeof(pe);


	Process32First(hSnapshot,&pe);

	while( true )
	{
		string pe_szExeFile = pe.szExeFile;

		if( pe_szExeFile == process_name )
				return true;

		if( !Process32Next( hSnapshot, &pe ) )
			break;
	}

	return false;
}

bool World::is_right_low_ui()
{
	bool left = dnf.has_pos_color( 
		6,590, "776742-050505" );

	//trace( left );

	bool right = dnf.has_pos_color(
		796,589, "807048-050505" );

	//trace( right );

	return left || right;
}

_DNF_END