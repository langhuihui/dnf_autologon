#include "attack.h"


//namespace dnf{
//	namespace attack{

_ATTACK_BEGIN

		FlyEnemy::FlyEnemy(){}

		bool FlyEnemy::operator<( const FlyEnemy& fly_enemy ) const
		{
			return map_pos < fly_enemy.map_pos;
		}

		////////////////////////////////////////////////////////
		FlyEnemyMgr::FlyEnemyMgr(){}

		void FlyEnemyMgr::add( const  Pos& enemy_map_pos )
		{
			FlyEnemy fly_enemy;
			fly_enemy.map_pos = enemy_map_pos;

			fly_enemy_set.insert( fly_enemy );
		}

		bool FlyEnemyMgr::is_ignore( const Pos& enemy_map_pos )
		{
			if( fly_enemy_set.empty() )
				return false;

			//glog( "========================" );
			//glog( enemy_map_pos.x );
			//glog( enemy_map_pos.y );
			//glog( "------------------------" );
			//for( set<FlyEnemy>::iterator it = fly_enemy_set.begin();
			//	it != fly_enemy_set.end(); it++ )
			//{
			//	glog( "--------------" );
			//	glog( it->map_pos.x );
			//	glog( it->map_pos.y );
			//	glog( it->map_pos.z );
			//	glog( "--------------" );
			//	glog( it->t.elapsed() );
			//	glog( "--------------" );
			//}
			//glog( "========================" );

			for( set<FlyEnemy>::iterator it = fly_enemy_set.begin();
				it != fly_enemy_set.end(); it++ )
				if( it->map_pos == enemy_map_pos 
					&& it->t.elapsed() < 6 )
					return true;

			return false;
		}

		void FlyEnemyMgr::check()
		{
			for( set<FlyEnemy>::iterator it = fly_enemy_set.begin();
				it != fly_enemy_set.end(); )
			{
				/*
#ifdef _DEBUG
				ostringstream oss;

				oss << it->t.elapsed() << "\t"
					<< "("
					<< it->map_pos.x << ","
					<< it->map_pos.y << ") ";

				 trace( oss.str() );
#endif
				 */

				if(	it->t.elapsed() > 8 )
					it = fly_enemy_set.erase( it );
				else
					it++;
			}
		}

		//////////////////////////////////////////////////////

		BossMode::BossMode()
			:_is_like_boss( false ),
			_is_boss( false ),
			_is_active( false ),
			_boss_ptr( 0 ),
			is_start_r_timer( false )
		{}


		void BossMode::check( vector<Target>& target_group )
		{
			size_t enemy_cnt = 0;
			for( size_t i = 0; i < target_group.size(); i++ )
				if( 529 == target_group[ i ].id )
				{
					enemy_cnt++;
					_boss_ptr = &target_group[ i ];
				}

				if( 1 != enemy_cnt )
				{
					_is_like_boss = false;
					_is_boss = false;
					_boss_ptr = 0;

					return;
				}
				else
				{
					if( !_is_like_boss
						&& !_is_boss )
					{
						t.restart();
						_is_like_boss = true;
						return;
					}

					if( _is_like_boss
						&& t.elapsed() > 5 )
					{
						_is_boss = true;
						//trace( "_______________is_boss" );
						return;
					}
				}
		}

		bool BossMode::is_boss()
		{
			return _is_boss;
		}

		Target* BossMode::boss_ptr()
		{
			return _boss_ptr;
		}

		bool BossMode::can_use_skill_r()
		{
			if( !is_start_r_timer )
			{
				is_start_r_timer = true;
				r_timer.restart();
				return true;
			}
			if( is_start_r_timer 
				&& r_timer.elapsed() < 5 )
			{
				return false;
			}
			if( is_start_r_timer
				&& r_timer.elapsed() >= 5 )
			{
				r_timer.restart();
				return true;
			}

			return false;
		}

		////	BossKiller
		///////////////////////////////////////////

		BossKiller::BossKiller( World& world )
			:_world( world ),
			_offset_x( 40 ),
			_offset_y( 100 ),	
			_is_set_offset( false ),
			_is_start_r_timer( false )
		{}

		bool BossKiller::search()
		{
			_str_buf = _world.dnf.dm.OcrEx(
				0,0,799,599,"ff00ff-020202",1.0 );

			if( _str_buf.empty() )
			{
				_pos.x = -1;
				_pos.y = -1;
				return false;
			}

			if( _str_buf.find( "领" ) == string::npos )
			{
				_pos.x = -1;
				_pos.y = -1;
				return false;
			}

			_iss.str( "" );
			_iss.clear();

			_iss.str( _str_buf );

			bool is_find = false;

			for( size_t i = 0; i < 3; i++ ) 
			{
				getline( _iss, _str_buf, '|' ); 

				if( _str_buf.find( "领" ) != string::npos )
				{
					is_find = true;
					break;
				}
			}

			if( !is_find )
			{
				_pos.x = -1;
				_pos.y = -1;
				return false;
			}

			_iss.str( "" );
			_iss.clear();
			_iss.str( _str_buf );

			
			getline( _iss, _str_buf, '$' );

			getline( _iss, _str_buf, '$' );
			_iss_pos.str( "" );
			_iss_pos.clear();
			_iss_pos.str( _str_buf );
			_iss_pos >> _pos.x;
			_pos.x += _offset_x;	//	默认为40
			_hit_pos.x = _pos.x;

			getline( _iss, _str_buf, '$' );
			_iss_pos.str( "" );
			_iss_pos.clear();
			_iss_pos.str( _str_buf );
			_iss_pos >> _pos.y;
			_pos.y += _offset_y;	//	默认为100
			_hit_pos.y = _pos.y - 50;

			_pos.init_assert();
			_pos.invalid_assert();

			return true;
		}

		void BossKiller::set_pos_offset( int offset_x, int offset_y )
		{
			assert( offset_x < 3000
				&& offset_x >= 0
				&& "set_pos_offset:offset_x超出范围" );

			assert( offset_y < 3000
				&& offset_y >= 0
				&& "set_pos_offset:offset_y超出范围" );

			//cout << "BossKiller::set_pos_offset:"
			//	<< offset_x << ',' << offset_y << endl;

			_offset_x = offset_x;
			_offset_y = offset_y;
		}

		bool BossKiller::is_set_offset()
		{
			return _is_set_offset;
		}

		void BossKiller::set_offset()
		{
			_is_set_offset = true;
		}

		void BossKiller::reset_offset()
		{
			cout << "reset_offset" << endl;
			
			_offset_x = 40;
			_offset_y = 100;
			_is_set_offset = false;
		}

		void BossKiller::attack()
		{
			_pos.init_assert();
			_pos.invalid_assert();

			//cout << _pos.x << ','
			//	<< _pos.y << endl;

			//cout << "Attack boss" << endl;
			for( size_t i = 0; i < 2; i++ )
				_world.player.LB( _hit_pos );

			//cout << _world.dnf.is_q_cooldown() << endl;
			//cout << _world.dnf.is_r_use_finish() << endl;
			//cout << _world.dnf.has_r() << endl;
			
			if( /*!_world.dnf.is_cannot_use_r()
				&& !_world.dnf.is_cannot_use_q()
				&& _world.dnf.is_q_cooldown()
				&&*/ !_world.dnf.is_r_use_finish()
				&& _world.dnf.has_r() )
			{
				//cout << "QRQRQRQRQ" << endl;
				if( _can_use_skill_r() )
				{
					//cout << "RRRRRRRRRR" << endl;
					if( _world.dnf.is_start_r() )
					{
						//cout << "R11111111" << endl;
						Sleep( 20 );
						_use_skill_r();
						_use_skill_q();
					}
					else
					{
						//cout << "R222222222" << endl;
						_world.player.R();
						Sleep( 20 );
						_use_skill_r();
						_use_skill_q();
					}
				}



			}

			if( !_world.dnf.is_cannot_use_e() )
				//&& _world.dnf.is_e_cooldown() )
			{
				//cout << "EEEEEEE" << endl;
				if( _world.dnf.is_start_e() )
				{
					_world.player.RB( _pos );
				}
				else if( _world.dnf.is_e_cooldown() )
				{
					_world.player.E();

					_world.player.RB( _pos );
				}
			}


			for( size_t i = 0; i < 2; i++ )
				_world.player.LB( _pos );
		}
		
		bool BossKiller::_can_use_skill_r()
		{
			if( !_is_start_r_timer )
			{
				_is_start_r_timer = true;
				_r_timer.restart();
				return true;
			}
			if( _is_start_r_timer 
				&& _r_timer.elapsed() < 5 )
			{
				return false;
			}
			if( _is_start_r_timer
				&& _r_timer.elapsed() >= 5 )
			{
				_r_timer.restart();
				return true;
			}

			return false;
		}

		void BossKiller::_use_skill_r()
		{

			_world.dnf.dm.MoveTo( _pos.x, _pos.y );
			_world.dnf.dm.LeftDown();
			Sleep( 10 );
			_world.dnf.dm.MoveTo( 
				_pos.x,
				_pos.y + 5
				);
			_world.dnf.dm.LeftUp();

			Sleep( 10 );

			_world.dnf.dm.MoveTo( _pos.x, _pos.y + 50 );
			_world.dnf.dm.LeftDown();
			Sleep( 10 );
			_world.dnf.dm.MoveTo( 
				_pos.x,
				_pos.y + 55
				);
			_world.dnf.dm.LeftUp();

			//Sleep( 10 );

			//_world.dnf.dm.MoveTo( _pos.x, _pos.y + 60 );
			//_world.dnf.dm.LeftDown();
			//Sleep( 10 );
			//_world.dnf.dm.MoveTo( 
			//	_pos.x,
			//	_pos.y + 65
			//	);
			//_world.dnf.dm.LeftUp();
		}	

		void BossKiller::_use_skill_q()
		{
			if( _world.dnf.is_start_q() )
			{
				for( size_t i = 0; i < 10; i++ )
					_world.player.RB( _pos );
			}
			else
			{
				_world.player.Q();

				for( size_t i = 0; i < 10; i++ )
					_world.player.RB( _pos );
			}
		}


		////////////////////////////////////////

		HideBossSkill::HideBossSkill( World& world_, float step_time_ )
			:_world( world_ ),
			_step_time( step_time_ ),
			_is_start( false )
		{
		}

		void HideBossSkill::update_skill_index()
		{
			_skill_index.clear();

			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( _boss_skill_id == _world.target_group[ i ].id )
					_skill_index.push_back( i );
		}

		bool HideBossSkill::has_boss_skill()
		{
			if( _skill_index.empty() )
				return false;
			else
				return true;
		}

		HideBossSkill::MoveDir HideBossSkill::set_move_dir()
		{
			index_t nearest_index = _nearest_skill_index();

			int offset_x = _world.target_group[ nearest_index ].map_pos.x
				- _world.player.map_pos.x;

			int offset_y = _world.target_group[ nearest_index ].map_pos.y
				- _world.player.map_pos.y;
		
			if( std::abs( offset_x ) < std::abs( offset_y ) )
			{
				if( offset_y > 0 )
					return MoveDir::Left;
				else
					return MoveDir::Right;
			}
			else
			{
				if( offset_x > 0 )
					return MoveDir::Up;
				else
					return MoveDir::Down;
			}
		}

		bool HideBossSkill::is_need_hide()
		{
			if( !has_boss_skill() )
				return false;

			_in_range_skill_index.clear();

			for( size_t i = 0; i < _skill_index.size(); i++ )
				if( _is_in_determine_range( 
					_world.target_group[ _skill_index[ i ] ].map_pos )
					)
					_in_range_skill_index.push_back( _skill_index[ i ] );

			if( _in_range_skill_index.empty() )
				return false;
			else
				return true;
		}

		void HideBossSkill::hide_boss_skill( MoveDir move_dir )
		{
			if( MoveDir::None == move_dir )
				return;
			
			if( MoveDir::Up == move_dir )
				_world.player.W( _step_time * 1000 );
			else if( MoveDir::Down == move_dir )
				_world.player.S( _step_time * 1000 );
			else if( MoveDir::Left == move_dir )
			{
				//_world.player.A( 50 );
				_world.player.A( _step_time * 1000 );
			}
			else if( MoveDir::Right == move_dir )
			{
				//_world.player.D( 50 );
				_world.player.D( _step_time * 1000 );
			}
		}

		void HideBossSkill::hide_boss_skill()
		{
			update_skill_index();
			if( is_need_hide() )
				hide_boss_skill( 
					set_move_dir() );
		}

		void HideBossSkill::start()
		{
			boost::thread( 
				boost::bind( &HideBossSkill::hide_boss_skill_thread_func, this ) );

			_is_start = true;
		}

		bool HideBossSkill::is_start()
		{
			return _is_start;
		}

		void HideBossSkill::end()
		{
			_is_start = false;
		}

		void HideBossSkill::hide_boss_skill_thread_func()
		{
			_out_timer.restart();
			while( true )
			{
				if( !_is_start )
					break;

				//	进行下一次移动
				hide_boss_skill();
				
				//	超时后检查是否在地图内
				//	否则结束循环
				if(	_out_timer.elapsed() > 10 )
				{
					if( _world.is_in_big_map() )
					{
						//cout << "超时退出" << endl;
						break;
					}
					else
						_out_timer.restart();
				}

				Sleep( 1 );
			}
		}

		void HideBossSkill::set_step_time( float step_time )
		{
			_step_time = step_time;
		}

		//////

		bool HideBossSkill::_is_in_determine_range( 
				const Pos& boss_skill_pos )
		{
				if( std::abs( 
					boss_skill_pos.x -
					_world.player.map_pos.x 
					) < _determine_range_x 
					&&
					std::abs(
					boss_skill_pos.y -
					_world.player.map_pos.y
					) < _determine_range_y
					)
					return true;
				else
					return false;
		}

		HideBossSkill::index_t HideBossSkill::_nearest_skill_index()
		{
			assert( !_in_range_skill_index.empty()
				&& "HideBossSkill::_nearest_skill_index():为空" );


			int nearest_distance = 10000;
			size_t nearest_index = _invail_index;

			for( size_t i = 0; i < _in_range_skill_index.size(); i++ )
			{
				int distance = compute_distance( 
					_world.player.map_pos,
					_world.target_group[ _in_range_skill_index[ i ] ].map_pos
					);

				if( distance < nearest_distance 
					)
				{
					nearest_distance = distance;
					nearest_index = _in_range_skill_index[ i ];
				}
			}

			assert( _invail_index != nearest_index
				&& "HideBossSkill::_nearest_skill_index():找不到最近的index" );

			return nearest_index;
		}


		///////////////////////////////////////////////////////

		AttackTest::AttackTest( World& world )
			:_world( world ),
			_boss_killer( world ),
			_hide_boss_skill( world ),
			log( "attack.log" ),
			_check_score_cnt( 0 ),
			_start_not_found_map_pos( false ),
			_is_start_q( false ),
			_hit_height_limit( 75 )
		{
			//corrent_assert();
		}

		void AttackTest::corrent_assert()
		{
			_world.update_all();

			_world.corrent_assert();
		}

		void AttackTest::operator()()
		{

			while( true )
			{

				//glog( "attack_start" );
				//trace( _DUNGEON::Sync::is_stop_atk() );
				//trace( _DNF::Sync::is_stop_atk() );
				if( !_DUNGEON::Sync::is_stop_atk() 
					&& !_DNF::Sync::is_stop_atk() )
				{
					_is_show_stop_attack = false;


					{
						handle_special_case();

						//kill_boss();

						boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

						//glog( "attack" );

						log << "==========================" << endl;
						//	更新所有坐标
						//	更新人物地图坐标
						_world.update_player_map_pos();
						log << "start update_player_map_pos" << endl;

						//	更新人物窗口坐标
						_world.update_player_win_pos();
						if( _world.player.win_pos.is_invaild() )
							continue;
						log << "start update_player_win_pos" << endl;
						//	更新目标地图坐标
						_world.update_target_group_map_pos();
						log << "start update_target_group_map_pos" << endl;
						
						if( is_skip_cloned_enemy() )
							continue;

						//	更新目标地图坐标
						_world.update_target_group_win_pos();
						log << "start update_target_group_win_pos" << endl;
						log << "end update" << endl;
					//}

						if( _world.dnf.has_e() )
							while( true )
							{
								//	如果skill_e返回false就退出循环
								//	否则继续
								if( !skill_e() )
									break;

								Sleep( 1 );
							}
						log << "e" << endl;
						

						attack_boss();
						log << "attack_boss" << endl;

						skill_q();
						log << "q" << endl;
							

						if( _is_start_q )
						{
							q_hit();
							log << "q_hit" << endl;
						}
						else
						{
							left_hit2();
							log << "left_hit" << endl;
						}
						

						attack_block();
						log << "attack_block" << endl;

						//log << "==========================" << endl;

						if( !_DNF::Sync::is_safe_mode()
							&& !_DUNGEON::Sync::is_stop_pick_up() )
							pick_up();

						log << "pick_up" << endl;
					}

					//if( _check_score_cnt > 1 )
					//{
					//	_check_score_cnt = 0;

					if( _world.is_dungeon_score() 
						|| _world.is_dungeon_reward() )
					{
						_DUNGEON::Sync::set_find_score();
						//trace( "_DUNGEON::Sync::set_find_score();" );
					}

					log << "check score" << endl;
					//}
					//else
					//	_check_score_cnt++;

					//	大转移修改用boss标记来判断
					if( !_DUNGEON::Sync::is_find_score() )
					{
						//_world.update_map_win_pos();
						_world.update_boss_room_win_pos();

						if( _start_not_found_map_pos )
						{
							trace( "_start_not_found_map_pos" );
							trace( _check_npc_timer.elapsed() );
							if( !_world.boss_room_pos.is_invaild() )
							{
								trace( "_found_boss_room_pos" );
								trace( _check_npc_timer.elapsed() );
								_check_npc_timer.restart();
								_start_not_found_map_pos = false;
							}
							
							if( _check_npc_timer.elapsed() > 6 )
							{
								trace( "_check_npc_timer.elapsed() > 7" );
								trace( _check_npc_timer.elapsed() );
								
								//block_call( 
								//	boost::bind( &World::has_esc, &_world ),
								//	boost::bind( null ),
								//	boost::bind( &Player::key_press, &_world.player, VK_ESCAPE, 50 ),
								//	6, 50, false );
								_world.player.key_press( VK_ESCAPE );

								if( _world.has_esc() )
									_world.close_esc();
							}

							if( _check_npc_timer.elapsed() > 20 )
								assert( false
									&& "AttackTest::operator::npc异常" );
						}
						else if( _world.boss_room_pos.is_invaild() )
						{
							trace( "set_start_not_found_boss_room_pos" );
							trace( _check_npc_timer.elapsed() );
							_check_npc_timer.restart();

							_start_not_found_map_pos = true;
						}
					}

					log << "handle npc" << endl;
					
					handle_data_exception();
					
					log << "handle safe mode" << endl;

					handle_block_sign();

					log << "handle block sign" << endl;

					Sleep( 1 );

					//glog( "stop attack" );
				}
				else
				{
					if( !_is_show_stop_attack )
					{
						trace( "stop attack" );
						_is_show_stop_attack = true;
					}
					Sleep( 1 );
				}
				Sleep( 1 );

				//glog( "attack_end" );
			}
		}

		bool AttackTest::is_skip_cloned_enemy()
		{
			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id 
					&& -10000 == _world.target_group[ i ].map_pos.x 
					//(
					//	_world.target_group[ i ].map_pos.x < -1
					//	|| _world.target_group[ i ].map_pos.x > 3000
					//	|| _world.target_group[ i ].map_pos.y < -1
					//	|| _world.target_group[ i ].map_pos.y > 3000
					//	|| _world.target_group[ i ].map_pos.z < -1
					//	|| _world.target_group[ i ].map_pos.z > 3000
					//)
				)
				return true;

			return false;
		}

		bool AttackTest::skill_e()
		{
			//	如果不能再使用能量，
			//	并且e被激活，则按下E
			//	使e自动恢复
			if( _world.dnf.is_e_use_finish() 
				&& _world.dnf.is_start_e() )
				_world.player.E();

			//	清空怪物指针容器
			enemy_ptr_vec.clear();

			//	设置怪物指针容器
			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id )
					enemy_ptr_vec.push_back( &_world.target_group[ i ] );

			// 如果没有怪物，返回false
			if( enemy_ptr_vec.empty() )
				return false;

			int most_index = 0;
			int most_cnt = 0;
			for( size_t i = 0; i < enemy_ptr_vec.size(); i++ )
			{
				int cnt = 0;

				for( size_t j = 0; j < enemy_ptr_vec.size(); j++ )
				{
					if( is_in_e_range( 
						enemy_ptr_vec[ i ]->map_pos, 
						enemy_ptr_vec[ j ]->map_pos ) )
						cnt++;
				}

				if( cnt > most_cnt )
				{
					most_cnt = cnt;	
					most_index = i;
				}
			}

			if( most_cnt >= 3 )
			{
				Pos& pos = enemy_ptr_vec[ most_index ]->win_pos;
				if( is_in_window( pos ) ) 
				{
					return e( pos );
				}
				else	//	如果能打到的最多怪的施法点不在屏幕内，则返回失败
					return false;
			}	//	如果能打到的最多怪物小于3则返回失败
			else
				return false;
		}

		bool AttackTest::e( Pos& pos )
		{
			//	如果不能使用e则返回失败
			if( _world.dnf.is_cannot_use_e() )
			{
				//trace( "_world.dnf.is_cannot_use_e()" );
				return false;
			}

			//	如果能量条使用完，并且e激活。则关闭e,返回失败
			if( _world.dnf.is_e_use_finish() )
			{
				//trace( "_world.dnf.is_e_use_finish()" );
				if( _world.dnf.is_start_e() )
				{
				//	trace( "_world.dnf.is_start_e()" );
					_world.player.E();
				}

				return false;
			}

			//	如果e没激活。则激活能量条。然后释放技能, 并返回成功
			if( !_world.dnf.is_start_e() )
			{
				//trace( "!_world.dnf.is_start_e()" );
				if( _world.dnf.is_e_cooldown() )
				{
				//	trace( "_world.dnf.is_e_cooldown()" );
					_world.player.E();
					_world.player.RB( pos );
					//Sleep( 2000 );

					return true;
				}

				return false;
			}
			else	//	否则直接释放技能, 并返回成功；
			{
		//		trace( "else !_world.dnf.is_start_e()" );
				_world.player.RB( pos );

				return true;
			}
		}

		bool AttackTest::is_in_e_range( const Pos& center, const Pos& pos )
		{
			center.corrent_assert();
			pos.corrent_assert();
			if( abs( center.x - pos.x ) > 250 
				&& abs( center.y - pos.y ) > 40 )
				return false;

			int offset_y = (float)( 250 - abs( center.x - pos.x ) ) * 0.16 + 1;

			if( abs( center.y - pos.y ) > offset_y )
				return false;

			return true;
		}

		void AttackTest::left_hit()
		{
			fly_enemy_mgr.check();
			if( is_right_player_win_pos( _world.player.win_pos ) )
				for( size_t i = 0; i < _world.target_group.size(); i++ )
					//	如果目标是怪物（529）或者可摧毁物件（545）
					if( 529 == _world.target_group[ i ].id
						|| 545 == _world.target_group[ i ].id )
					{

						Pos& pos = _world.target_group[ i ].win_pos;

						//	如果窗口坐标在窗口中。
						if( is_in_window( pos ) )
						{
							//Sleep( 3 );

							if( _world.target_group[ i ].map_pos.z > _hit_height_limit )
							{
								fly_enemy_mgr.add( _world.target_group[ i ].map_pos );
							}
							else 
								if( !fly_enemy_mgr.is_ignore( _world.target_group[ i ].map_pos ) )
									for( size_t j = 0; j < 1; j++ )
									{
										int r = random() % 5 - 2;
										Pos p = pos;
										p.x += r;
										p.y -= _world.target_group[ i ].map_pos.z;

										//glog( "size:" );
										//glog( _world.target_group.size() );
										//glog( "index:" );
										//glog( i );

										p.y += r;
										p.y -= 18;
										_world.player.LB( p );
										Sleep( 1 );
									}
						}
					}
		}

		void AttackTest::left_hit2()
		{
			fly_enemy_mgr.check();
			if( is_right_player_win_pos( _world.player.win_pos ) )
			{
				size_t size = near_enemies_index( );
				//cout << size << ' ';
				for( size_t i = 0; i < size; i++ )
				{
					size_t ei = _near_enemies_index[ i ];

					Pos& pos = _world.target_group[ ei ].win_pos;

					//	如果窗口坐标在窗口中。
					if( is_in_window( pos ) )
					{
						//Sleep( 3 );

						if( _world.target_group[ ei ].map_pos.z > _hit_height_limit )
						{
							fly_enemy_mgr.add( _world.target_group[ ei ].map_pos );
						}
						else 
							if( !fly_enemy_mgr.is_ignore( _world.target_group[ ei ].map_pos ) )
								for( size_t j = 0; j < 1; j++ )
								{
									int r = random() % 5 - 2;
									Pos p = pos;
									p.x += r;
									p.y -= _world.target_group[ ei ].map_pos.z;

									//glog( "size:" );
									//glog( _world.target_group.size() );
									//glog( "index:" );
									//glog( i );

									p.y += r;
									p.y -= 18;
									_world.player.LB( p );
									Sleep( 1 );
								}
					}
				}

				_near_enemies_index.clear();
			}
		}

		void AttackTest::pick_up()
		{
			if( is_right_player_win_pos( _world.player.win_pos ) )
				for( size_t i = 0; i < _world.target_group.size(); i++ )
					if( 289 == _world.target_group[ i ].id )
					{
						Pos& pos = _world.target_group[ i ].win_pos;

						//	如果窗口坐标在窗口中。
						if( is_in_window( pos ) )
						{
							for( size_t j = 0; j < 3; j++ )
							{
								_world.player.LB( pos );
								Sleep( 1 );
							}
						}
					}
		}

		void AttackTest::skill_q()
		{
			//	清空怪物指针容器h
			enemy_ptr_vec.clear();

			//	设置怪物指针容器
			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id )
					enemy_ptr_vec.push_back( &_world.target_group[ i ] );

			if( enemy_ptr_vec.size() > 1 )
				if( _world.dnf.is_q_cooldown()
					&& !_world.dnf.is_start_q() )
					_world.player.Q();
			
			if( _world.dnf.is_start_q() )
				_is_start_q = true;
			else
				_is_start_q = false;

		}

		void AttackTest::kill_boss()
		{
			if( _DUNGEON::Sync::is_in_boss_room() )
				if( _boss_killer.search() )
					_boss_killer.attack();
		}

		void AttackTest::attack_boss()
		{
			boss_mode.check( _world.target_group );
			if( boss_mode.is_boss() )
			{
				//trace( "is_boss_____________" );

				Target* boss_ptr = boss_mode.boss_ptr();

				//cout << boss_ptr->win_pos.x << ','
				//	<< boss_ptr->win_pos.y << endl;

				if( is_in_window( boss_ptr->win_pos ) )
				{
					//trace( "is_in_window__________" );
					//cout << "is_in_window" << endl;

					//	如果Q和R冷却，并且能使用
					if( /*!_world.dnf.is_cannot_use_r()
						&& !_world.dnf.is_cannot_use_q()
						&&*/ _world.dnf.is_q_cooldown()
						&& !_world.dnf.is_r_use_finish()
						&& _world.dnf.has_r() )
					{
						//trace( "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq" );

						if( boss_mode.can_use_skill_r() )
						{
							if( _world.dnf.is_start_r() )
								_world.player.LB( boss_ptr->win_pos );
							else
							{
								_world.player.R();
								_world.player.LB( boss_ptr->win_pos );
							}
						}

						if( _world.dnf.is_start_q() )
						{
							//cout << "q" << endl;
							for( size_t i = 0; i < 10; i++ )
								_world.player.RB( boss_ptr->win_pos );
						}
						else
						{
							//cout << "q" << endl;
							_world.player.Q();

							for( size_t i = 0; i < 10; i++ )
								_world.player.RB( boss_ptr->win_pos );
						}
					}

					if( !_world.dnf.is_cannot_use_e() )
						//&& _world.dnf.is_e_cooldown() )
					{
						//trace( "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" );

						if( _world.dnf.is_start_e() )
						{
							//cout << "e" << endl;
							_world.player.RB( boss_ptr->win_pos );
							//trace( "e111111111111111111111" );
							//Sleep( 20 );
							//_world.player.RB( boss_ptr->win_pos );
							//trace( "e222222222222222222222" );
						}
						else if( _world.dnf.is_e_cooldown() )
						{
							//cout << "e" << endl;
							_world.player.E();
							//trace( "EEEEEEEEEEEEEEEEEEEEE" );

							_world.player.RB( boss_ptr->win_pos );
							//trace( "e111111111111111111111" );
							//Sleep( 20 );
							//_world.player.RB( boss_ptr->win_pos );
							//trace( "e222222222222222222222" );
						}
					}
				}
			}
		}

		void AttackTest::attack_block()
		{
			if( _DUNGEON::Sync::is_set_block() )
			{
				vector<Pos> block
					= _DUNGEON::Sync::get_destroy_block().near_win_pos(
					_world.player.map_pos, _world.target_group );

				for( size_t i = 0; i < block.size(); i++ )
				{
					_world.player.LB( block[ i ] );
					Sleep( 1 );
				}
			}
		}

		bool AttackTest::is_right_player_win_pos( const Pos& win_pos )
		{
			if( win_pos.x != -1 &&
				win_pos.y != -1 )
				return true;
			else
				return false;
		}

		bool AttackTest::is_in_window( const Pos& pos )
		{
			if( pos.x > 5 
				&& pos.x < 795
				&& pos.y > 5
				&& pos.y < 595 )
				return true;
			else
				return false;
		}

		bool AttackTest::has_data_exception()
		{
			string str = _world.dnf.dm.Ocr( 394,247,496,272, "ffffff-000000", 1.0 );
			if( str.find( "安全模式" ) != string::npos )
			{
				Sync::set_safe_mode();

				return true;
				//_world.player.key_press( VK_ESCAPE );
			}
			else
				return false;
		}

		void AttackTest::handle_data_exception()
		{
			if( has_data_exception() )
				block_call( boost::bind( &AttackTest::_handle_data_exception, this ), 20 );
		}

		bool AttackTest::_handle_data_exception()
		{
			_world.player.key_press( VK_ESCAPE );
			if( !has_data_exception() )
				return true;
			else
				return false;
		}

		void AttackTest::handle_block_sign()
		{
			if( _start_timing )
			{
				if( !_world.player.win_pos.is_invaild() )
				{
					_block_sign_timer.restart();
					_start_timing = false;
				}
				else if( _block_sign_timer.elapsed() > 3 )
				{
					_world.player.W( 2000 );
					_world.player.S( 2000 );
					//_block_sign_timer.restart();
				}

				if( _block_sign_timer.elapsed() > 30 )
					assert( false
					&&	"标记被挡住,异常" );
			}

			if( _world.player.win_pos.is_invaild() )
			{
				_block_sign_timer.restart();
				
				_start_timing = true;;
			}
		}

		void AttackTest::q_hit()
		{
			if( is_right_player_win_pos( _world.player.win_pos ) )
				for( size_t i = 0; i < _world.target_group.size(); i++ )
					//	如果目标是怪物（529）或者可摧毁物件（545）
					if( 529 == _world.target_group[ i ].id
						|| 545 == _world.target_group[ i ].id )
					{
						Pos& pos = _world.target_group[ i ].win_pos;

						//	如果窗口坐标在窗口中。
						if( is_in_window( pos ) )
						{
							//Sleep( 3 );
							//if( 0 == _world.target_group[ i ].map_pos.z )
								for( size_t j = 0; j < 1; j++ )
								{
									int r = random() % 5 - 2;
									pos.x += r;
									pos.y += r;
									pos.y -= 15;
									_world.player.LB( pos );
									Sleep( 1 );
								}
						}
					}
		}

		void AttackTest::handle_special_case()
		{
			using namespace _SPECIAL;
			Config& ref_cfg = Special::get_cfg();
			if( ref_cfg.has_case( CaseName::TeLeiZeGai_InRoom ) )
			{
				//trace( " ref_cfg.has_case( CaseName::TeLeiZeGai_InRoom ) " ); 
				Pos pos; 
				while( true )
				{
					_world.dnf._get_sign( pos.x, pos.y, "TeLeiZeGai.bmp" );
					if( pos.is_invaild() )
						break;
					else
					{
						//int r = random() % 5 - 2;
						pos.y += 110;
						//pos.x += r;
						Pos pos1 = pos;
						pos1.x += -10;
						Pos pos2 = pos;
						pos2.x += -5;
						for( size_t i = 0; i < 2; i++ )
						{
							if( 1 == i % 2 )
								_world.player.LB( pos1 );
							else
								_world.player.LB( pos2 );
							Sleep( 1 );
						}

						if( _world.dnf.is_e_cooldown() )
						{
							_world.player.E();
							_world.player.RB( pos );
							Sleep( 1 );
							_world.player.RB( pos );
							Sleep( 1 );
							_world.player.RB( pos );
							Sleep( 1 );
							_world.player.RB( pos );
						}
					}
				}
			}

			//	躲避boss技能
			if( ref_cfg.has_case( CaseName::OpenHideBossSkill ) )
			{
				if( _DUNGEON::Sync::is_in_boss_room()
					&& !_hide_boss_skill.is_start() )
				{
					CaseData case_data = 
						ref_cfg.get_case_data( ref_cfg.get_case_index( CaseName::OpenHideBossSkill ) );

					if( !case_data.empty() )
					{
						//cout << "set_step_time( " 
						//	<< case_data[ 0 ] << " )" << endl;
						_hide_boss_skill.set_step_time( case_data[ 0 ] );
					}

					//cout << "_hide_boss_skill.start()" << endl;
					_hide_boss_skill.start();
				}
				
				if( !_DUNGEON::Sync::is_in_boss_room()
					&& _hide_boss_skill.is_start()
					)
				{
					//cout << "_hide_boss_skill.end();" << endl;
					_hide_boss_skill.end();
				}
			}


			if( _DUNGEON::Sync::is_in_boss_room() )
			{
				if( !_boss_killer.is_set_offset() )
				{
					if( ref_cfg.has_case( CaseName::SetBossOffset ) )
					{
						CaseData case_data = 
							ref_cfg.get_case_data( 
							ref_cfg.get_case_index( CaseName::SetBossOffset )
							);

						if( case_data.size() == 2 )
						{
							_boss_killer.set_pos_offset( 
								case_data[ 0 ], case_data[ 1 ] );

							_boss_killer.set_offset();
						}
					}
					else if( _boss_killer.is_set_offset()
						)
					{
						_boss_killer.reset_offset();
					}
				}
			}
			else if( _boss_killer.is_set_offset()
				)
			{
				//	不在boss房间重置偏移
				_boss_killer.reset_offset();
			}

			if( ref_cfg.has_case( CaseName::OpenBossKiller ) )
			{
				kill_boss();
			}
			
			if( Special::has_current_case( CaseName::SetHitHeightLimit ) )
			{
				CaseData case_data = 
				Special::get_current_case_data( CaseName::SetHitHeightLimit );
				if( !case_data.empty() )
					_hit_height_limit = case_data[ 0 ];
			}
			else
				_hit_height_limit = 75;
		}

		size_t AttackTest::near_enemies_index( size_t num )
		{
			class Cmp{
			public:
				Cmp( World& world_ )
					:_world( world_ )
				{}
				bool operator()( const int& left, const int& right )
				{
					return compute_distance( 
						_world.target_group[ left ].map_pos,
						_world.player.map_pos ) <
						compute_distance(
						_world.target_group[ right ].map_pos,
						_world.player.map_pos );
				}
				World& _world;
			};

			_near_enemies_index.clear();
		
			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id
					|| 545 == _world.target_group[ i ].id )
					_near_enemies_index.push_back( i );

			if( _near_enemies_index.size() <= num )
				return _near_enemies_index.size();

			std::partial_sort( _near_enemies_index.begin(),
				_near_enemies_index.begin() + num,
				_near_enemies_index.end(), 
				Cmp( _world ) 
				);

			//for( size_t i = 0; i < num; i++ )
			//{
			//	size_t index = _near_enemies_index[ i ];
			//	cout << index << ":["
			//	<< _world.target_group[ index ].id  << "]:"
			//	<< _world.target_group[ index ].win_pos.x << ","
			//	<< _world.target_group[ index ].win_pos.y << " ";
			//}
			//
			//cout << endl;

			return num;
		}


_ATTACK_END

//	}	//	attack
//}	//	dnf