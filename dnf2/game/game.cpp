#include "game.h"


_GAME_BEGIN

	StudySkill::StudySkill( World& world_ )
	:_world( world_ ),
	_Q( 95 + 2,176 ),
	_E( 189 + 2, 176 ),
	_R( 283 + 2, 176 ),
	_left_Q( 72 + 2, 243 ),
	_right_Q( 120 + 2, 243 ),
	_left_E( 166 + 2, 243 ),
	_right_E( 213 + 2, 243 ),
	_left_R( 260 + 2, 243 ),
	_m_offset( 56, -27 ),//( 75, -4 ),
	_c_offset( 59, -1 ),
	_offset_x1( -12 ),
	_offset_y1( -38 ),
	_offset_x2( 76 ),
	_offset_y2( 7 ),
	_is_set_level( false ),
	_last_set_level( 0 )
	{

	}

	void StudySkill::Q()
	{
		study( _Q );
	}

	void StudySkill::E()
	{
		study( _E );
	}

	void StudySkill::R1()
	{
		study1( _R );
	}

	void StudySkill::right_Q()
	{
		study( _right_Q );
	}
	void StudySkill::left_Q()
	{
		study( _left_Q );
	}
	void StudySkill::right_E()
	{
		study( _right_E );
	}
	void StudySkill::left_E()
	{
		study( _left_E );
	}
	void StudySkill::left_R1()
	{
		study1( _left_R );
	}

	void StudySkill::open()
	{
		_world.open_skill();
		while( true )
		{
			Sleep( 100 );
			if( _world.has_skill() )
				return;
			else
				_world.open_skill();
		}
	}

	void StudySkill::close()
	{
		_world.close_skill();
	}

	bool StudySkill::_has_black_study()
	{
		string str = _world.dnf.dm.Ocr( 221,539,295,568, "8c8c8c-000000", 1.0 );
	
		if( str.find( "学习" ) != string::npos )
			return true;
		else
			return false;
		//	用像素的识别更快,更准
		/*
		if( _world.dnf.has_pos_color( 251, 557, "8c8c8c-000000" ) )
			return true;
		else
			return false;
			*/
	}

	bool StudySkill::_has_ensure()
	{
		string str = _world.dnf.dm.Ocr( 301,271,393,300, "ffffff-000000", 1.0 );
		if( str.find( "是否确定" ) != string::npos )
			return true;
		else
			return false;
	}

	bool StudySkill::_ensure()
	{
		_world.player.LB( Pos( 350,328 ) );
		//if( _has_black_study() )
		if( !_has_ensure() )
			return true;
		
		return false;
	}

	bool StudySkill::_ensure_study()
	{
		_world.player.LB( Pos( 264 + 20 ,555 ) );
		//Sleep( 500 );
		//_world.dnf.dm.RightClick();
		if( _has_ensure() ||
			_has_black_study() )
			return true;
		
		return false;
	}

	void StudySkill::ensure_study()
	{
		block_call( boost::bind( &StudySkill::_ensure_study, this ), 20 );
		block_call( boost::bind( &StudySkill::_ensure, this ), 20 );
	}


	bool StudySkill::_study( const Pos& pos )
	{

		//if( _world.dnf.has_pic_m( 
		//	pos.x + _offset_x1,
		//	pos.y + _offset_y1,
		//	pos.x + _offset_x2,
		//	pos.y + _offset_y2 )
		//	|| 
		//	_world.dnf.has_pic_no_m(
		//	pos.x + _offset_x1,
		//	pos.y + _offset_y1,
		//	pos.x + _offset_x2,
		//	pos.y + _offset_y2 )
		//	)
		if(
			_world.dnf.has_color_c(
			pos.x + _c_offset.x,
			pos.y + _c_offset.y 
			)
			//_world.dnf.has_color_m(
			//pos.x + _m_offset.x,
			//pos.y + _m_offset.y
			//)
			//||
			//_world.dnf.has_color_no_m(
			//pos.x + _m_offset.x,
			//pos.y + _m_offset.y
			//)
			)
		{

			return true;
		}
		else
		{	
			//_world.player.RB( pos );
			_world.player.LB( pos );
			return false;
		}
	}

	bool StudySkill::_study_close_m( const Pos& pos )
	{
		_world.player.LB( Pos( 27,155 ) );
		Sleep( 10 );
		if(
			!_world.dnf.has_color_c(
			pos.x + _c_offset.x,
			pos.y + _c_offset.y
			)
			)
		{
			//trace( "has_no_c" );
			//trace( pos.x );
			//trace( pos.y );
			return true;
		}
		
		return false;
	}

	bool StudySkill::_study_end( const Pos& pos )
	{

		//	大转移修改
		//_world.player.LB( Pos( pos.x + _m_offset.x, pos.y + _m_offset.y ) );
		_world.player.LB( Pos( pos.x, pos.y ) );

		//if( _world.dnf.has_pic_no_m(
		//	pos.x + _offset_x1,
		//	pos.y + _offset_y1,
		//	pos.x + _offset_x2,
		//	pos.y + _offset_y2 )
		//	)
		if(
			//_world.dnf.has_color_no_m(jjj
			!_world.dnf.has_color_m(
			pos.x + _m_offset.x,
			pos.y + _m_offset.y
			)
			)
		{
			//trace( "has_no_m" );
			//trace( pos.x );
			//trace( pos.y );j
			return true;
		}
		
		return false;
	}

	void StudySkill::study( const Pos& pos  )
	{
		//	大转移修改
		block_call( boost::bind( &StudySkill::_study, this, pos ), 20 );

		block_call( boost::bind( &StudySkill::_study_end, this, pos), 20 );
		block_call( boost::bind( &StudySkill::_study_close_m, this, pos), 20 );
	}

	void StudySkill::study1( const Pos& pos )
	{
		block_call( boost::bind( &StudySkill::_study, this, pos ), 20 );

		block_call( boost::bind( &StudySkill::_study_close_m, this, pos), 20 );
	}

	void StudySkill::level_10()
	{
		open();
		Q();
		E();
		left_Q();
		right_E();
		right_Q();

		ensure_study();
		close();
	}
	
	void StudySkill::level_15()
	{
		level_10();
	}

	void StudySkill::level_20()
	{
		open();

		if( !has_r() )
		{
			R1();
			left_R1();
		}

		Q();
		E();
		left_Q();
		right_E();
		right_Q();

		ensure_study();
		close();
	}

	void StudySkill::set( size_t level )
	{
		if( 10 == level
			&& !_is_set_level )
		{
			level_10();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 11 == level
			&& !_is_set_level )
		{
			level_10();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 15 == level
			&& !_is_set_level )
		{
			level_15();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 20 == level
			&& !_is_set_level )
		{
			level_20();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 25 == level
			&& !_is_set_level )
		{
			level_20();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 27 == level
			&& !_is_set_level )
		{
			level_20();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 29 == level
			&& !_is_set_level )
		{
			level_20();
			_is_set_level = true;
			_last_set_level = level;
		}

		if( 30 == level
			&& !_is_set_level )
		{
			level_20();
			_is_set_level = true;
			_last_set_level = level;
		}
		
		if(  level > 30
			&& level != _last_set_level )
		{
			level_20();
			_is_set_level = true;
			_last_set_level = level;
		}


		if( level > _last_set_level )
		{
			_is_set_level = false;
		}
	}

	bool StudySkill::has_r()
	{
		return _world.dnf.has_pos_color(
			675,497,"6e1ee6-030303" );
	}


	//	恢复技能丢失
	//	打开技能栏.把Q,E,R
	//	拖到他原来的位置上

	RestoreSkill::RestoreSkill( World& world_ )
		:_world( world_ )
	{
	}

	void RestoreSkill::restore()
	{
		output( "恢复技能" );

		_world.open_skill();

		//
		//restore_one( 
		//	q_pos_in_study,
		//	q_pos_in_set
		//	);

		//restore_one( 
		//	e_pos_in_study,
		//	e_pos_in_set
		//	);

		//restore_one( 
		//	r_pos_in_study,
		//	r_pos_in_set
		//	);

		//	以下所有的restore都要改为
		//	阻塞的
		if( !is_set_q() )
			if( !block_call(
				boost::bind( &RestoreSkill::restore_q, this ),
				boost::bind( null ),
				boost::bind( null ),
				30, 10, false )
				)
				output( "恢复技能Q异常" );

		if( is_study_e() 
			&& !is_set_e() )
			if( !block_call(
				boost::bind( &RestoreSkill::restore_e, this ),
				boost::bind( null ),
				boost::bind( null ),
				30, 10, false )
				)
				output( "恢复技能E异常" );


		if( is_study_r() 
			&& !is_set_r() )
			if( !block_call(
				boost::bind( &RestoreSkill::restore_r, this ),
				boost::bind( null ),
				boost::bind( null ),
				30, 10, false )
				)
				output( "恢复技能R异常" );

		_world.close_skill();

	}

	void RestoreSkill::restore_one( 
			const Pos& study_pos,
			const Pos& set_pos
			)
	{
		Sleep( 10 );
		_world.dnf.dm.MoveTo( 
			study_pos.x,
			study_pos.y
			);
		_world.dnf.dm.LeftDown();
		Sleep( 10 );
		_world.dnf.dm.MoveTo(
			study_pos.x + 10,
			study_pos.y + 10 
			);
		Sleep( 10 );

		_drag_mouse( 
			study_pos,
			set_pos 
			);

		Sleep( 10 );

		_world.dnf.dm.MoveTo(
			set_pos.x,
			set_pos.y
			);
		_world.dnf.dm.LeftUp();
	}

	void RestoreSkill::_drag_mouse(
			const Pos& start_pos,
			const Pos& end_pos
		)
	{
		_world.dnf.dm.MoveTo(
			start_pos.x,
			start_pos.y
			);

		//	计算出起始和结束的偏移
		int offset = compute_distance( start_pos, end_pos );

		//	计算需要移动的次数
		//	200为每次移动的距离
		size_t move_times = offset / 100;
		
		if( move_times )
		{
			Pos offset_pos;
			offset_pos.x = end_pos.x - start_pos.x;
			offset_pos.y = end_pos.y - start_pos.y;

			Pos move_pos;
			move_pos.x = offset_pos.x / move_times;
			move_pos.y = offset_pos.y / move_times;

			for( size_t i = 0; i < move_times; i++ )
			{
				_world.dnf.dm.MoveTo(
					start_pos.x + move_pos.x * i,
					start_pos.y + move_pos.y * i
					);
				Sleep( 10 );
			}
		}

		_world.dnf.dm.MoveTo(
			end_pos.x,
			end_pos.y
			);

	}

	bool RestoreSkill::is_set_q()
	{
		Pos pos;
		_world.dnf.GetPicPos(
			pos.x, pos.y,
			"check_q.bmp",
			"030303",
			check_q_start_pos.x,
			check_q_start_pos.y,
			check_q_end_pos.x,
			check_q_end_pos.y
			);
		if( pos.is_init()
			&& !pos.is_invaild() )
			return true;
		else
			return false;
	}
	bool RestoreSkill::is_set_e()
	{
		Pos pos;
		_world.dnf.GetPicPos(
			pos.x, pos.y,
			"check_e.bmp",
			"030303",
			check_e_start_pos.x,
			check_e_start_pos.y,
			check_e_end_pos.x,
			check_e_end_pos.y
			);
		if( pos.is_init()
			&& !pos.is_invaild() )
			return true;
		else
			return false;
	}
	bool RestoreSkill::is_set_r()
	{
		Pos pos;
		_world.dnf.GetPicPos(
			pos.x, pos.y,
			"check_r.bmp",
			"030303",
			check_r_start_pos.x,
			check_r_start_pos.y,
			check_r_end_pos.x,
			check_r_end_pos.y
			);
		if( pos.is_init()
			&& !pos.is_invaild() )
			return true;
		else
			return false;
	}

	bool RestoreSkill::is_study_e()
	{
		Pos pos;
		_world.dnf.GetPicPos(
			pos.x, pos.y,
			"check_no_e.bmp",
			"030303",
			check_no_e_start_pos.x,
			check_no_e_start_pos.y,
			check_no_e_end_pos.x,
			check_no_e_end_pos.y
			);
		if( pos.is_init()
			&& !pos.is_invaild() )
			return false;
		else
			return true;
	}
	bool RestoreSkill::is_study_r()
	{
		Pos pos;
		_world.dnf.GetPicPos(
			pos.x, pos.y,
			"check_no_r.bmp",
			"030303",
			check_no_r_start_pos.x,
			check_no_r_start_pos.y,
			check_no_r_end_pos.x,
			check_no_r_end_pos.y
			);
		if( pos.is_init()
			&& !pos.is_invaild() )
			return false;
		else
			return true;
	}


	bool RestoreSkill::restore_q()
	{
		restore_one( 
			q_pos_in_study,
			q_pos_in_set
			);
		set_safe_mouse();
		if( is_set_q() )
			return true;
		else
			return false;
	}
	bool RestoreSkill::restore_e()
	{
		restore_one( 
			e_pos_in_study,
			e_pos_in_set
			);
		set_safe_mouse();
		if( is_set_e() )
			return true;
		else
			return false;
	}
	bool RestoreSkill::restore_r()
	{
		restore_one( 
			r_pos_in_study,
			r_pos_in_set
			);
		set_safe_mouse();
		if( is_set_r() )
			return true;
		else
			return false;
	}
			

	void RestoreSkill::set_safe_mouse()
	{
		_world.dnf.dm.MoveTo( 550, 90 );
		Sleep( 50 );
	}


	
	Pos RestoreSkill::q_pos_in_study = Pos( 108,157 );
	Pos RestoreSkill::q_pos_in_set = Pos( 588,521 );
	Pos RestoreSkill::e_pos_in_study = Pos( 205,154 );
	Pos RestoreSkill::e_pos_in_set = Pos( 635,513 );
	Pos RestoreSkill::r_pos_in_study = Pos( 300,156 );
	Pos RestoreSkill::r_pos_in_set = Pos( 680,518 );

	Pos RestoreSkill::check_q_start_pos = Pos( 565,489 );
	Pos RestoreSkill::check_q_end_pos = Pos( 614,539 );
	Pos RestoreSkill::check_e_start_pos = Pos( 610,481 );
	Pos RestoreSkill::check_e_end_pos = Pos( 660,542 );
	Pos RestoreSkill::check_r_start_pos = Pos( 655,488 );
	Pos RestoreSkill::check_r_end_pos = Pos( 706,544 );

	Pos RestoreSkill::check_no_e_start_pos = Pos( 182,130 );
	Pos RestoreSkill::check_no_e_end_pos = Pos( 232,186 );
	Pos RestoreSkill::check_no_r_start_pos = Pos( 270,130 );
	Pos RestoreSkill::check_no_r_end_pos = Pos( 327,184 );

	//////////////////////////////////////

	Town::Town( World& world_ )
	:_world( world_ ),
	//_dungeon( world_ ),
	_last_level( 0 ),
	_study_skill( world_ ),
	_restore_skill( world_ )
	{}

	void Town::run()
	{
		//while( true )
		//{
		Message::pluse( 80, 2  );


		Sync::set_start_watch();

		if( _world.has_motion() )
		{
			output( "处理活动窗口" );
			_world.close_motion();
		}

		output( "打开大地图,关闭任务说明" );	
		_world.open_big_map();

		_world.open_esc();
		_world.close_esc();

			if( _world.has_esc() )
			{
				output( "处理其他窗口" );
				_world.close_esc();
			}
				
			if( _world.has_notice() )
			{
				output( "处理公告信息" );
				_world.close_notice();
			}


			output( "设置等级" );
			_world.player.set_level();
			size_t level = _world.player.get_level();
			

			//if( level != _last_level
			//	|| _world.is_in_big_map() )
			//{
			
				output( "进入大地图" );
				_world.to_big_map();

//				if( 10 == level )
//					_study_skill.level_10();

				block_call( 
					_DNF::Sync::is_nonstop_all, 
					0x1000000, 100 );

				output( "学习技能" );
				_study_skill.set( level );

				output( "恢复技能" );
				_restore_skill.restore();

				output( "进入地下城" );

				to_dungeon( level );

				//if( level < 3 )
				//{
				//	_world.to_luo_lan();
				//	if( level < 2 )
				//		_world.select_dungeon( "洛兰" );
				//	else if( level < 3 )
				//		_world.select_dungeon( "洛兰深处" );
				//}
				//else if( level < 16 )
				//{
				//	_world.to_luo_lan_zhi_sen();

				//	if( level < 5 )
				//		_world.select_dungeon( "幽暗密林深处" );
				//	else if( level < 7 )
				//		_world.select_dungeon( "雷鸣废墟" );
				//	else if( level < 10 )
				//		_world.select_dungeon( "猛毒雷鸣废墟" );
				//	else if( level < 12 )
				//		_world.select_dungeon( "格拉卡" );
				//	else if( level < 16 )
				//		_world.select_dungeon( "烈焰格拉卡" );
				//}
				//else if( level < 27 )
				//{
				//	_world.to_tian_kong_zhi_cheng();
				//	if( level < 24 )
				//		_world.select_dungeon( "龙人之塔" );
				//	else if( level < 27 )
				//		_world.select_dungeon( "石巨人塔" );
				//}
				//else if( level < 32 )
				//{
				//	_world.to_tian_wei_ju_shou();
				//	if( level < 32 )
				//		_world.select_dungeon( "神殿外围" );
				//}
				//else
				//	assert( false
				//	&& "Town::run:level超出可选地图范围" );

			//}

			//else if( level == _last_level
			//	&& !_world.is_in_big_map() )
			//{
			//	_world.player.key_press( VK_F10 );
			//	_world.player.key_press( VK_F10 );
			//}



			//_dungeon.run();

			////	翻牌子
			//_out_timer.restart();
			//trace( "_out_timer.restart()" );
			//while( true )
			//{
			//	if( _world.is_dungeon_reward() )
			//	{
			//		size_t rand = random();
			//		trace( rand );
			//		rand = rand % 4;
			//		trace( rand );
			//		size_t vk_key_1_4 = 0x31 + rand;

			//		while( true )
			//		{
			//			trace( vk_key_1_4 );
			//			_world.player.key_press( vk_key_1_4 );
			//			Sleep( 300 );

			//			if( !_world.is_dungeon_reward() )
			//				break;
			//		}

			//		break;
			//	}

			//	if( _out_timer.elapsed() > 10 )
			//	{
			//		trace( "_out_timer.elapsed" );

			//		assert( false
			//			&& "Town::run:_world.is_dungeon_reword超时" );

			//		break;
			//	}

			//	Sleep( 100 );
			//}

			//trace( "dungeon_end" );

			////	等待结束
			//_out_timer.restart();
			//trace( "_out_timer.restart()" );
			//while( true )
			//{
			//	if( _world.is_dungeon_end() )
			//		break;

			//	if( _world.is_in_big_map() )
			//		break;

			//	if( _out_timer.elapsed() > 15 )
			//	{
			//		trace( "_out_timer.elapsed" );
			//		assert( false
			//			&& "Town::run::_world.is_dungeon_end超时" );

			//		break;
			//	}

			//	Sleep( 200 );
			//}

		//}
	}

	/*
	void Town::to_dungeon( size_t level )
	{

		if( level < 3 )
		{
			_world.to_luo_lan();
			if( level < 2 )
				_world.select_dungeon( "洛兰" );
			else if( level < 3 )
				_world.select_dungeon( "洛兰深处" );
		}
		else if( level < 16 )
		{
			_world.to_luo_lan_zhi_sen();

			if( level < 5 )
				_world.select_dungeon( "幽暗密林深处" );
			else if( level < 7 )
				_world.select_dungeon( "雷鸣废墟" );
			else if( level < 10 )
				_world.select_dungeon( "猛毒雷鸣废墟" );
			else if( level < 12 )
				_world.select_dungeon( "格拉卡" );
			else if( level < 16 )
				_world.select_dungeon( "烈焰格拉卡" );
		}
		else if( level < 27 )
		{
			_world.to_tian_kong_zhi_cheng();
			if( level < 24 )
				_world.select_dungeon( "龙人之塔" );
			else if( level < 27 )
				_world.select_dungeon( "石巨人塔" );
		}
		else if( level < 85 )
		{
			_world.to_tian_wei_ju_shou();
			if( level < 85 )
				_world.select_dungeon( "神殿外围" );
		}
		else
			assert( false
			&& "Town::run:level超出可选地图范围" );
	}
	*/

	void Town::to_dungeon( size_t level )
	{
		if( level < 17 )
		{
			_world.to_ya_nuo_fa_sen_lin();
			if( level < 5 )
				_world.select_dungeon( "炽晶森林" );
			else if( level < 8 )
				_world.select_dungeon( "冰晶森林" );
			else if( level < 11 )
				_world.select_dungeon( "水晶矿脉" );
			else if( level < 14 )
				_world.select_dungeon( "被污染的水晶矿脉" );
			else if( level < 16 )
				_world.select_dungeon( "幽冥监狱" );
			else if( level < 18 )
				_world.select_dungeon( "幽冥监狱下层" );
			//else if( level < 20 )
				//_world.select_dungeon( "遗忘之森" );

		}
		else if( level < 33 )
		{
			_world.to_e_yun_zhi_cheng();
			if( level < 22 )
				_world.select_dungeon( "蘑菇庄园" );
			else if( level < 25 )
				_world.select_dungeon( "蚁后的巢穴" );
			//else if( level < 26 )
				//_world.select_dungeon( "腐烂之地" );
			else if( level < 28 )
				_world.select_dungeon( "赫顿玛尔旧街区" );
			else if( level < 33 )
				_world.select_dungeon( "绝望的棋局" );
			//	无boss标记,不可用
			/*
			if( level < 30 )
				_world.select_dungeon( "堕落的村庄" );
				*/
		}
		else if( level <37 )
		{
			_world.to_piao_liu_dong_xue();
			//if( level <33 )
				//_world.select_dungeon( "英雄冢", 1 );
			 if( level <37)
				_world.select_dungeon( "黄金矿洞");
		}
		else if( level < 47 )
		{
			_world.to_ni_liu_pu_bu();
			if( level < 39 )
				_world.select_dungeon( "鲨鱼栖息地",1 );
			else if( level < 43 )
				_world.select_dungeon( "人鱼的国度" );
			else if( level < 47 )
				_world.select_dungeon( "树精繁殖地" );
		}
	}

	Game::Game( World& world_, const string& qq_ )
	:_world( world_ ),
		_town( world_ ),
		_dungeon( world_ ),
		_login( world_, qq_ ),
		_watch( world_ ),
		_watch_thread( boost::ref( _watch ) )
	{
	}


	void Game::game()
	{
		//set_ime();

		while( true )
		{
			//trace( "Game::game()" );
			Sleep( 300 );

			if( _world.has_motion() )
			{
				output( "处理游戏活动" );
				_world.close_motion();
			}

			_world.open_esc();
			_world.close_esc();

			output( "检查虚弱" );

			if( _world.is_weak() )
			{
				_world.open_esc();
				_world.close_esc();

				Pos pos = _world.weak_pos();
				if( !pos.is_invaild() )
				{
					output( "恢复虚弱" );

					block_call(
						boost::bind( &World::has_restore_notice, &_world ),
						boost::bind( null ),
						boost::bind( &Player::LB, &_world.player, pos ),
						20, 200, false );

					output( "关闭虚弱警告" );

					_world.close_restore_notice();
				}
			}

			//if( _world.has_weaked_notice() )
			//{
			//	_world.close_weaked_notice();

			//	block_call(
			//		boost::bind( &World::has_restore_notice, &_world ),
			//		boost::bind( null ),
			//		boost::bind( null ),
			//		20, 100, false );

			//	_world.close_restore_notice();
			//}

			//block_call( 
			//	boost::bind( &Game::handle_weak, this ),
			//	boost::bind( null ),
			//	boost::bind( null ),
			//	500, 100 );

			Sleep( 1000 );


			while( true )
			{
				output( "检查疲劳" );
				if( !_world.has_energy() )
				{
					trace( "!_world.has_energy() " ); 

					//	临时附加条件.打完3个角色就退出
					//	0,1,2 一共3个角色
					if( _login.get_last_num() >= 2 )
					{
						output( "角色已超过3个" );
						output( "进入退出程序" );
						complete();
						output( "退出完成" );
					}

					if( _login.is_last_charater() )
					{
						output( "所有角色刷完" );
						output( "进入退出程序" );
						complete();
						output( "退出完成" );
					}
					else
					{
						output( "开始切换角色" );
						glog( "Game::game():switch_next().start" );
						switch_next();
						glog( "Game::game():switch_next().end" );
						Message::switch_character();
						output( "切换角色完成" );
					}
				}
				else
					break;

				Sleep( 120 * 1000 );
			}


			Sleep( 200 );
			output( "处理城镇信息" );
			_town.run();
			output( "城镇信息处理完毕" );
			output( "开始处理地下城信息" );
			_dungeon.run();
			output( "离开地下城" );
		}
	}
	

	void Game::run()
	{
		//cout << "Game::run():01" << endl;

		output( "开始游戏内部登录" );

		_login.run2();
		Sleep( 1000 );

		output( "游戏内部登录完成" );
		
		//_world.attach_ime.set_en();

		if( !Sync::is_start_watch() )
			Sync::set_start_watch();


		output( "进入游戏" );
		game();
	}

	void Game::run_no_login()
	{
		if( !Sync::is_start_watch() )
		{
			output( "启动监视线程" );
			Sync::set_start_watch();
		}

		output( "初始化当前状态" );
		if( _login.is_in_character() )
		{
			output( "在角色选择界面" );
			//_login.switch_to_next();
			_login.switch_to_current();
			_login.join2();
		}
		else if( !_world.is_in_big_map() )
		{
			_world.open_esc();
			_world.close_esc();

			if( _world.has_equip() )
			{
				output( "关闭装备栏" );
				_world.close_equip();
			}


			_world.update_boss_room_win_pos();
			_world.update_map_win_pos();
			if( !_world.boss_room_pos.is_invaild()
				&& _world.map_pos.is_invaild() )
			{
				output( "人物在boss房间,返回城镇" );
				_world.to_big_map();
			}
			else 
			{
				if( load_no_login_times() > 4 ) //_login.load_stop_num() )
				{
					output( "超过心跳超时次数,返回城镇" );
					set_no_login_times( 0 );
					_world.to_big_map();
				}
				else
				{
					output( "设置心跳超时次数" );
					set_no_login_times( load_no_login_times() + 1 );
					Message::pluse( 80, 2 );
					_dungeon.run();

					output( "重置心跳超时次数" );
					set_no_login_times( 0 );
				}
			}
		}

		if( _world.is_in_big_map() )
		{

			Sleep( 1000 );
			game();
		}
	}

	size_t Game::load_no_login_times()
	{
		ifstream ifile( "data\\no_login_start_times.txt" );
		if( !ifile )
			throw std::exception( "can not open data\\no_login_start_times.txt" );

		string str;
		getline( ifile, str, '\n' );
		assert( !str.empty()
			&& "Game::load_no_login_times:str为空" );
		size_t times = -1;
		istringstream iss( str );
		iss >> times;
		assert( -1 != times
			&& "Game::load_no_login_times:未初始化" );

		return times;
	}
	void Game::set_no_login_times( size_t times )
	{
		assert( times >= 0
			&& "Game::set_no_login_times:times不正确" );
		ofstream ofile( "data\\no_login_start_times.txt" );
		if( !ofile )
			throw std::exception( "can not open data\\no_login_start_times.txt" );

		ofile << times << endl;
	}

	void Game::quit()
	{
		glog( "Game::quit().start" );
		glog( "Message:complete().start" );
		_DNF::Message::complete();
		glog( "Message:complete().end" );
		_world.quit_game2();
		glog( "Game::quit().end" );
	}

	void Game::set_ime()
	{
		//_world.attach_ime.set_ime();
		//_world.dm.active_wnd();
		for( size_t i = 0; i < 10; i++ )
			_world.dm.switch_ime();
	}

	void Game::switch_next()
	{
		glog( "Game::switch_next():start" );
		_world.return_select_character();
		_login.switch_to_next();
		_login.join2();
		glog( "Game::switch_next():end" );
	}

	void Game::complete()
	{
		glog( "Game::complete().start" );
		//_world.return_select_character();
		_login.unset_login_mark();
		//_login.switch_to_first();
		//_login.join2();
		quit();
		glog( "Game::complete().end" );
	}

	bool Game::handle_weak()
	{
		if( _world.is_weak() )
		{
			Message::pluse( 80, 2 );
			_world.player.set_level();
			size_t level = _world.player.get_level();

			if( level < 18 )
				return true;
			
			_town.to_dungeon( level );

			_world.to_big_map();
			
			block_call(
				boost::bind( &World::has_weaked_notice, &_world ),
				boost::bind( null ),
				boost::bind( null ),
				10, 200, false );

			_world.close_weaked_notice();

			block_call(
				boost::bind( &World::has_restore_notice, &_world ),
				boost::bind( null ),
				boost::bind( null ),
				10, 200, false );

			_world.close_restore_notice();

			if( !_world.is_weak() )
				return true;
			else
				return false;
		}
		else
			return true;
	}

_GAME_END