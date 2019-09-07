#include "test.h"

Test::Test()
	:_world( _dm, _attach_ime )
{
	_dm.open_console();
}


void Test::main()
{
	trace( "test.main()" );

	//test_dll_shell();
	//test_create_character();
	//test_big_map();
	//test_skill();
	//test_select_character();
	//test_update_win_pos();
	//test_set_window();
	//test_low_ui();
	//test_out_weight();
	//test_handle_opening_movie();
	//test_login();
	//test_skill_base();
	//test_win_pos();
	//test_sell();
	//test_select_player();
	//test_join();
	//test_select_ch();
	//test_new_join();
	//test_select_player2();
	//test_player_win_pos();
	//test_switch_player();

	//test_select_player3();
	//test_click_mouse();

	//test_open_esc();
	//test_boss_pos2();
	//test_quit_game();

	//test_dm_gif();

	//test_ocr();
	//test_find_chair();

	//test_special();
	//test_study_skill();
	//test_get_money();

	test_restore_skill();

	std::system( "pause" );
}

void Test::test_dll_shell()
{
	trace( "test_dll_shell()" );
}

void Test::test_set_window()
{
	_dm.for_dnf_game_wnd();
	_world.dnf.dm.DmMoveWindow((long)_dm.wnd_hwnd,0,0);
}

void Test::test_create_character()
{
	using namespace _GAME;
	_dm.for_dnf_game_wnd();
	Login login( _world, "" );
	block_call( boost::bind( &Login::in_create, &login ) );
	block_call( boost::bind( &Login::select_dizaozhe, &login ), 10 );
	block_call( boost::bind( &Login::start_input_name, &login ) );
	block_call( boost::bind( &Login::input_name, &login ) );
	/*
	while( true )
	{
		bool ret = false;
		ret = _world.dnf.has_pic( "dizaozhe1.bmp",
			19,461,118,543 );

		trace( "find dizaozhe1.bmp" );
		trace( ret );
		Sleep( 1000 );
	}
	*/
}


void Test::test_big_map()
{
	_dm.for_dnf_game_wnd();

	//_world.to_ya_nuo_fa_sen_lin();
	//_world.player.key_press( VK_ESCAPE );
	//_world.to_e_yun_zhi_cheng();
	//_world.player.key_press( VK_ESCAPE );
	//_world.to_piao_liu_dong_xue();
	//_world.player.key_press( VK_ESCAPE );
	//_world.to_ni_liu_pu_bu();
	//_world.player.key_press( VK_ESCAPE );
	//_world.to_wang_zhe_xia_gu();
	//_world.player.key_press( VK_ESCAPE );
	//_world.to_jue_wang_bing_ya();
	//_world.player.key_press( VK_ESCAPE );
	//_world.to_ge_lan_zhi_sen_shan_lu();
	//_world.player.key_press( VK_ESCAPE );
	
	//_world.select_dungeon( "冰晶森林" );


	trace( "dnf_hwnd" );
	trace( _world.dm.wnd_hwnd );
	_world.dnf.dm.DmMoveWindow((long)_dm.wnd_hwnd,0,0);
	//_world.open_big_map();

	_DUNGEON::Equip equip( _world );
	equip.replace_and_sell_equipment();

	/*
	while( true )
	{
		//trace( _world.is_in_big_map() );
		_world.update_big_map_win_pos();
		trace( "_world.update_big_map_win_pos();" );
		ostringstream oss;
		oss << _world.big_map_pos.x << ','
			<< _world.big_map_pos.y;
		trace( oss.str() );

		Sleep( 1000 );
	}
	*/


}

void Test::test_skill()
{
	//_dm.for_dnf_game_wnd();
	test_set_window();

	_GAME::StudySkill study_skill( _world );
	study_skill.level_10();

}

void Test::test_select_character()
{
	_dm.for_dnf_game_wnd();
	trace( "dnf_hwnd" );
	trace( _world.dm.wnd_hwnd );
	_world.dnf.dm.DmMoveWindow((long)_dm.wnd_hwnd,0,0);

	_GAME::Login login( _world, "" );
	trace( "next" );
	login.switch_to_next();
	Sleep( 1000 );
	login.switch_to_first();

}

void Test::test_update_win_pos()
{
	_dm.for_dnf_game_wnd();
	
	while( true )
	{
		_world.update_map_win_pos();
		cout << "map_win_pos: " 
			<< _world.map_pos.x << ','
			<< _world.map_pos.y << endl;
		_world.update_boss_room_win_pos();
		cout << "boss_room_pos: "
			<< _world.boss_room_pos.x << ','
			<< _world.boss_room_pos.y << endl;

		Sleep( 1000 );
	}
}

void Test::test_low_ui()
{
	test_set_window();

	while( true )
	{
		trace( _world.is_right_low_ui() );

		Sleep( 1000 );
	}
}

void Test::test_out_weight()
{
	test_set_window();

	while( true )
	{
		trace( _world.is_out_weight() );

		Sleep( 1000 );
	}
}

void Test::test_handle_opening_movie()
{
	test_set_window();

	_GAME::Login login( _world, "" );

	login.handle_opening_movie();
}

void Test::test_login()
{
	_dm.open_console();

	_GAME::Login login( _world, "" );
	login.run2();
}

void Test::test_skill_base()
{
	//_dm.open_console();
	//_dm.for_dnf_game_wnd();
	test_set_window();

	while( true )
	{
		trace( "_world.dnf.is_cannot_use_q()" );
		trace( _world.dnf.is_cannot_use_q() );
		trace( "_world.dnf.is_q_cooldown()" );
		trace( _world.dnf.is_q_cooldown() );
		trace( "_world.dnf.is_start_q()" );
		trace( _world.dnf.is_start_q() );

		Sleep( 1000 );
	}
}

void Test::test_win_pos()
{
	test_set_window();

	trace( "start" );

	for( size_t i = 0; i < 1000; i++ )
	{
		/*
		_world.dnf.has_pic(
			"test_win_pos.bmp",
			0,0,799,299 );

		trace(
		_world.dnf.has_pic(
			"test_win_pos.bmp",
			0,0,99,99 )
		);
		*/


		Sleep( 1000 );
	}

	trace( "end" ); 
}

void Test::test_sell()
{
	test_set_window();

	_DUNGEON::Equip equip( _world );
	equip.replace_and_sell_equipment();
}

void Test::test_select_player()
{
	test_set_window();

	_GAME::SelectPlayer select_player( _world );

	/*
	while( true )
	{
		int selected_frame_num;
		while( true )
		{
			selected_frame_num = select_player.selected_frame_num();
			if( -1 != selected_frame_num )
				break;
			Sleep( 1 );
		}

		Pos selected_frame_pos
			= select_player.get_frame_pos( selected_frame_num );

		Pos info_start_pos, info_end_pos;

		select_player.get_frame_info_pos(
			selected_frame_pos,
			info_start_pos,
			info_end_pos
			);

		trace( "-----------" );
		trace( info_start_pos.x );
		trace( info_start_pos.y );
		trace( info_end_pos.x );
		trace( info_end_pos.y );
		string str = _world.dnf.dm.Ocr(
			info_start_pos.x,
			info_start_pos.y,
			info_end_pos.x,
			info_end_pos.y,
			"d1b994-050505", 1.0 );

		trace( str );

		//Sleep( 1000 );
	}
	*/

	while( true )
	{
		//std::system( "cls" );
		/*
		trace( "--" );
		trace( "--" );
		trace( "--" );
		select_player.to_first();
		trace( "----------------------end--------------------" );
		trace( "----------------------end--------------------" );
		trace( "----------------------end--------------------" );
		*/
		
		select_player.set_total();
		size_t total = select_player.get_total();
		trace( total );

		for( size_t i = 0; i < total; i++ )
		{
			select_player.select( i );
			trace( select_player.get_last_num() );
			trace( select_player.is_last() );
			Sleep( 1000 );
		}
	
		select_player.to_first();

		while( true )
			if( !select_player.to_next() )
				break;
			

		select_player.clear();

		trace( select_player.get_last_num() );
		trace( select_player.get_total() );

		Sleep( 2000 );
	}
}

void Test::test_join()
{
	test_set_window();
	_GAME::Login login( _world, "0" );

	//cout << "join2()" << endl;
	//login.join2();
	_GAME::SelectPlayer sp( _world );
	sp.set_total();
	cout << sp.get_total() << endl;

	ifstream ifile( "data\\stop_create.txt" );
	if( !ifile )
		throw std::exception( "can not open data\\stop_create.txt" );

	string str;
	getline( ifile, str, '\n' );

	istringstream iss( str );
	size_t num;

	iss >> num;
	cout << "num:" << num << endl;

	if( sp.get_total() >= num
		&& num != 0 )
		cout << "True" << endl;
	else
		cout << "False" << endl;
}


void Test::test_select_ch()
{
	test_set_window();
	_GAME::Login login( _world, "0" );

	login.run2();
}

void Test::test_new_join()
{
	test_set_window();

	_GAME::Login login( _world, "0" );

	_GAME::SelectPlayer sp( _world );

	sp.set_total();
	cout << sp.get_total() << endl;

	std::system( "pause" );

	Sleep( 2000 );

	sp.select( 2 );
	Sleep( 1000 );
	login.join2();
}

void Test::test_select_player2()
{
	test_set_window();

	while( true )
	{
		_GAME::SelectPlayer sp( _world );
		sp.set_total();

		cout << "sp:" << sp.get_total() << endl;

		ifstream ifile( "data\\stop_create.txt" );
		if( !ifile )
			throw std::exception( "can not open data\\stop_create.txt" );

		string str;
		getline( ifile, str, '\n' );

		istringstream iss( str );
		size_t num;

		iss >> num;
		cout << "num:" << num << endl;

		Sleep( 2000 );
	}
}

void Test::test_player_win_pos()
{
	test_set_window();

	while( true )
	{
		/*
		cout << 
			_world.dnf.dm.FindPicE(
			0,0,799,599,
			"\
			top_left.bmp|\
			top_left_r.bmp|\
			top_right.bmp|\
			top_right_r.bmp|\
			bottom_left.bmp|\
			bottom_left_r.bmp|\
			bottom_right.bmp|\
			bottom_right_r.bmp|\
			mid_left.bmp|\
			mid_left_r.bmp|\
			mid_right.bmp|\
			mid_right_r.bmp|\
			mid_mid.bmp|\
			mid_mid_r.bmp\
			", 
			"000000", 
			1.0, 
			0
			)
			<< endl;

			*/

		int x, y;

		_world.dnf.GetWinPlayerPoint2( x, y );

		cout << x << "," << y << endl;

		//Sleep( 1 );
	}
}

void Test::test_switch_player()
{
	test_set_window();

	_GAME::Login login( _world, "0" );

	cout << "Test::test_switch_player()" << endl;

	cout << login.get_last_num() << endl;
}

void Test::test_select_player3()
{
	test_set_window();

	_GAME::SelectPlayer sp( _world );

	while( true )
	{
		sp.set_total();
		cout << sp.get_total() << endl;

		//if( !sp.get_total() )
		//	cout << 0 << endl;

		Sleep( 3 );
	}
}

void Test::test_click_mouse()
{
	test_set_window();

	boost::timer t;

	for( size_t i = 0; i < 500; i++ )
	{

		_world.dnf.dm.MoveTo( 400, 400 );
		_world.dnf.dm.LeftClick();

		float time = t.elapsed();

		//if( time > 0.5 )
			cout << time << endl;

		t.restart();
		
		Sleep( 1 );
	}
}

void Test::test_open_esc()
{
	test_set_window();
	_world.open_esc();
}

void Test::test_boss_pos()
{
	test_set_window();
	while( true )
	{
		string str = _world.dnf.dm.OcrEx(
			0,0,799,599,"ff00ff-000000",1.0 );

		cout << str << endl;

		Sleep( 100 );
	}
}

void Test::test_boss_pos2()
{
	test_set_window();

	_ATTACK::BossKiller boss_killer( _world );

	while( true )
	{
		if( boss_killer.search() )
			boss_killer.attack();

		Sleep( 200 );
	}
}

void Test::test_quit_game()
{
	test_set_window();

	_world.quit_game2();
}

void Test::test_dm_gif()
{
	test_set_window();

	/*
	_world.dnf.dm.CaptureGif(
		100,100,400,400,
		"good_job.gif", 100, 1000 );
		*/


	while( true )
	{
		/*
		size_t* addr = (size_t*)_world.dnf.dm.GetScreenData(
			100, 100, 300, 300 );

		cout << hex << addr << endl;

		ofstream ofile( "d:\\img.bmp", ios::binary );
		if( !ofile )
			cout << "Can not open img.bmp" << endl;

		for( size_t i = 0; i < 200 * 200; i++ )
			ofile << hex << addr[ i ] << endl;
			*/


		_world.dnf.dm.Capture(
			100, 100, 400, 400,
			"d:\\img.bmp"
			);

		Sleep( 10 );
	}
}


void Test::test_ocr()
{
	//_world.dm.reg();

	//_world.dm.set_wnd_name( "111.bmp  - 803 x 608" );

	//_world.dm.find_wnd();

	//_world.dm.bind_wnd();

	//_world.dm._cdnf.SetAppPath( 
	//	_world.dm._cdnf.MyPath );


	test_set_window();

	_WATCH::OcrHandler ocr_handler( _world );

	cout << "test_ocr" << endl;

	Pos pos;
	pos.x = 10;
	pos.y = 80;

	//Sleep( 1000 * 20 );

	while( true )
	{
		ocr_handler.start( pos );
		//cout << "2秒后连接" << endl;
		//Sleep( 1000 * 2 );

		//Sleep( 1000 * 5 );

		//cout << "2秒后即将关闭连接" << endl;
		//Sleep( 1000 * 2 );
		//Sleep( 1000 * 10 );
		//ocr_handler.update();
		Sleep( 1000 * 20 );
		ocr_handler.end();
	}

	//_WATCH::Watch watch( _world );

	//cout << watch.is_find_ocr() << endl;;

}

void Test::test_find_chair()
{
	test_set_window();

/*
	while( true )
	{
		string str = _world.dnf.dm.FindPicE(
			500,0,799, 300,
			"chair.bmp",
			"202020",
			1.0, 0
			);

		cout << str << endl;
		string str = _world.dm._cdnf.GetMapName();
		
		if( str.find( "树精" ) == string::npos )
			cout << "error" << str << endl;


		Sleep( 1 );
	}
*/

	while( true )
	{
		std::system( "cls" );

		//string str = _world.dnf.dm.FindPicE(
		//	500,0,799, 300,
		//	"chair.bmp",
		//	"202020",
		//	1.0, 0
		//	);
		//cout << str << endl;

		_world.update_map_win_pos();
		cout << _world.map_pos.x << ","
			<< _world.map_pos.y << endl;
		Sleep( 100 );
	}
}

void Test::test_special()
{

}

void Test::test_study_skill()
{
	test_set_window();

	while( true )
	{
		if( !_world.has_skill() )
			_world.open_skill();
		Sleep( 100 );
		if( !_world.has_skill() )
			cout << "1" << endl;
		Sleep( 500 );
		if( _world.has_skill() )
			_world.close_skill();

		Sleep( 100 );
		if( _world.has_skill() )
			cout << "2" << endl;

	}
}

void Test::test_get_money()
{
	test_set_window();

	while( true )
	{
		_world.player.update_money();
		cout << 
			_world.player.money() << endl;

		Sleep( 1000 );
	}
}

void Test::test_restore_skill()
{
	test_set_window();

	while( true )
	{
		_GAME::RestoreSkill restore_skill( _world );

		restore_skill.restore();

		//Pos pos;

		//_world.dnf.GetPicPos(
		//	pos.x,
		//	pos.y,
		//	"check_no_e.bmp",
		//	"030303" );

		//cout << pos.x << ","
		//	<< pos.y << endl;

		//cout << "q:"
		//	<< restore_skill.is_set_q() << endl;
		//cout << "e:"
		//	<< restore_skill.is_set_e() << endl;
		//cout << "r:"
		//	<< restore_skill.is_set_r() << endl;

		//cout << "study_e:"
		//	<< restore_skill.is_study_e() << endl;
		//cout << "study_r:"
		//	<< restore_skill.is_study_r() << endl;
			

		Sleep( 5 * 1000 );
	}
}