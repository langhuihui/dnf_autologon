#include "test_key.h"

TestKey::TestKey()
	:_world( _dm, _attach_ime )
{}
	

void TestKey::run()
{
/*
	void A( int time = 1000 ); // ms 0x41
	void W( int time = 1000 ); // ms 0x57
	void S( int time = 1000 ); // ms 0x53
	void D( int time = 1000 ); // ms 0x44
	*/

	_dm.open_console();
	_dm.for_dnf_game_wnd();

//	test_basic();

	test_move();
	//while( true )
	//{
	//	test_key_speed( 0x53 );
	//	std::system( "pause" );
	//}

	//test_windows_key();
	//std::system( "pause" );
}

void TestKey::test_key_speed( size_t vk )
{
	_world.update_player_map_pos();
	Pos start_pos = _world.player.map_pos;

	cout << "开始坐标:" 
		<< start_pos.x << ","
		<< start_pos.y << endl;

	_world.dnf.dm.KeyDown( vk );
	Sleep( 1000 );
	_world.dnf.dm.KeyUp( vk );

	_world.update_player_map_pos();
	Pos end_pos = _world.player.map_pos;

	cout << "结束坐标:"
		<< _world.player.map_pos.x << ","
		<< _world.player.map_pos.y << endl;

	cout << "位移:"
		<< end_pos.x - start_pos.x << ","
		<< end_pos.y - start_pos.y << endl;
}

void TestKey::test_move()
{
	//bool ret = _world.dnf.dm.EnableKeypadSync(
	//	1, 500 );

	//bool ret = _world.dnf.dm.EnableSpeedDx( 1 );

	//if( ret )
	//	trace( "speed dx set success" );

	while( true )
	{
		Pos pos;
		pos.x = random() % 700 + 200;
		pos.y = random() % 160 + 180;

		_world.moveto( pos );
		_world.update_player_map_pos();

		cout << "当前位置:"
			<< _world.player.map_pos.x << ","
			<< _world.player.map_pos.y << endl;

		cout << "目标位置:"
			<< pos.x << ","
			<< pos.y << endl;

		size_t offset_x = 
			abs( _world.player.map_pos.x - pos.x );
		size_t offset_y =
			abs( _world.player.map_pos.y - pos.y );

		cout << "差距:"
			<< offset_x << ","
			<< offset_y << endl;

		if(	offset_x > 50
			|| offset_y > 50 ) 
		{
			//::system( "pause" );
			//_world.dm.unbind_wnd();
			//_world.dm.bind_wnd();
		}
		
		Sleep( 1000 );
	}
}

void TestKey::test_basic()
{
	for( size_t i = 0; i < 10000000; i++ )
	{
		basic_test( 50, 80 );
		if( !( i % 100) )
			trace( i );
	}
}

bool TestKey::check_pos( size_t offset )
{
	_world.update_player_map_pos();

	if( !_last_pos.is_init() )
		_last_pos = _world.player.map_pos;

	if( _world.player.map_pos.x >
		_last_pos.x + offset
		|| 
		_world.player.map_pos.x < 
		_last_pos.x - offset
		|| 
		_world.player.map_pos.y >
		_last_pos.y + offset
		||
		_world.player.map_pos.y <
		_last_pos.y - offset
		)
	{
		assert( false
			&& "移动超出范围" );
		trace( "超出移动范围" );
		return false;
	}

	_last_pos = _world.player.map_pos;

	return true;
}

void TestKey::basic_test( size_t sleep_time, size_t check_offset )
{
	basic_key_test( 0x41, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x44, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x53, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x57, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x44, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x41, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x57, sleep_time );
	check_pos( check_offset );
	basic_key_test( 0x53, sleep_time );
	check_pos( check_offset );
}


void TestKey::basic_key_test( long vk, size_t sleep_time )
{
	_world.dnf.dm.KeyDown( vk );
	Sleep( sleep_time );
	_world.dnf.dm.KeyUp( vk );
}

void TestKey::test_random_move( size_t x1, size_t y1, size_t x2, size_t y2  )
{


}

void TestKey::test_windows_key()
{
	cout << "A" << endl;
	_world.dnf.a_down();
	Sleep( 1000 );
	_world.dnf.a_up();
	_world.dnf.d_down();
	Sleep( 1000 );
	_world.dnf.d_up();
	_world.dnf.s_down();
	Sleep( 1000 );
	_world.dnf.s_up();
	_world.dnf.w_down();
	Sleep( 1000 );
	_world.dnf.w_up();

	//::keybd_event( 0x41,0x1E,0,0);
	//Sleep(1000 );
	//::keybd_event( 0x41,0x1E,KEYEVENTF_KEYUP,0);

}

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	TestKey test_key;
	test_key.run();

	
//	test_key.test_windows_key();

	return 0;
}
