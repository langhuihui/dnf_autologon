#include "dungeon.h"


//namespace dnf{
//	namespace dungeon{

_DUNGEON_BEGIN

		//_DUNGEON::Sync::_DUNGEON::Sync()
		//{}

		//void _DUNGEON::Sync::set_stop_atk()
		//{
		//	_is_stop_atk = true;
		//}
		//void _DUNGEON::Sync::set_nonstop_atk()
		//{
		//	_is_stop_atk = false;
		//}

		//bool _DUNGEON::Sync::is_stop_atk()
		//{
		//	return _is_stop_atk;
		//}

		//boost::recursive_mutex& _DUNGEON::Sync::get_mutex()
		//{
		//	return _mutex;
		//}

		//void _DUNGEON::Sync::set_block( DestroyBlock& destroy_block )
		//{
		//	assert( &destroy_block
		//		&& "_DUNGEON::Sync::set_block:destory_block无效" );

		//	_destroy_block_ptr = &destroy_block;
		//	_is_set_block = true;
		//}

		//void _DUNGEON::Sync::unset_block()
		//{
		//	_is_set_block = false;
		//}

		//bool _DUNGEON::Sync::is_set_block()
		//{
		//	return _is_set_block;
		//}

		//DestroyBlock& _DUNGEON::Sync::get_destroy_block()
		//{
		//	assert( _destroy_block_ptr
		//		&& "_DUNGEON::Sync::get_destroy_block:_destroy_block_ptr指向空指针" );

		//	return *_destroy_block_ptr;
		//}

		////	初始状态为不攻击
		//bool _DUNGEON::Sync::_is_stop_atk = false;
		//boost::recursive_mutex _DUNGEON::Sync::_mutex;
		//bool _DUNGEON::Sync::_is_set_block = false;
		//DestroyBlock* _DUNGEON::Sync::_destroy_block_ptr = 0;

		EquipInfo::EquipInfo()
		:value( 0 )
		{}

		void EquipInfo::reset()
		{
			name.clear();
			value = 0;
		}

		bool EquipInfo::is_init()
		{
			if( name.empty() 
				&& !value )
				return true;
			else
				return false;
		}

		Equip::Equip( World& world_ )
			:_world( world_ ),
			_shop_sell_pos( _shop_center_pos ),
			_is_safe_mode( false ),
			_sell_button( 200, 511 ),//( 265,460 ),
			_sell_button_pos1( 151,496 ),//( 219,449 ),
			_sell_button_pos2( 211,522 )//( 271,471 )
		{}

		bool Equip::has_chuan_dai()
		{
			string str = _world.dnf.dm.FindPicE(
				200,1,799,599, 
				//"equipment_locate.bmp",
				"equipment_locate1.bmp",
			"202020", 1.0, 0 );

			trace( "has_chuan_dai()" );
			trace( str );

			istringstream iss( str );

			string size_str;

			getline( iss, size_str, '|' );
			getline( iss, size_str, '|' );
			istringstream x_iss( size_str );
			x_iss >> _chuan_dai.x;
			getline( iss, size_str, '|' );
			istringstream y_iss( size_str );
			y_iss >> _chuan_dai.y;


			if( _chuan_dai.is_invaild() )
				return false;

			_chuan_dai.corrent_assert();

			return true;
		}
		
		bool Equip::set_chuan_dai()
		{
			return block_call( 
				boost::bind( &Equip::has_chuan_dai, this ),
				boost::bind( null ),
				boost::bind( null ),
				0.1f, 10, false );


				
			//string str = _world.dnf.dm.FindPicE(
			//	200,1,799,599, "equipment_locate.bmp",
			//"000000", 1.0, 0 );

			//istringstream iss( str );

			//string size_str;

			//getline( iss, size_str, '|' );
			//getline( iss, size_str, '|' );
			//istringstream x_iss( size_str );
			//x_iss >> _chuan_dai.x;
			//getline( iss, size_str, '|' );
			//istringstream y_iss( size_str );
			//y_iss >> _chuan_dai.y;

			//if( _chuan_dai.is_invaild() )
			//	return false;

			//_chuan_dai.corrent_assert();

			//return true;
		}

		void Equip::set_equip_pos()
		{
			_chuan_dai.corrent_assert();

			_equiped_pos1.x = _chuan_dai.x - 64;
			_equiped_pos1.y = _chuan_dai.y + 12;
			
			_equiped_pos2.x = _chuan_dai.x + 138;
			if( _equiped_pos2.x > 799 )
				_equiped_pos2.x = 799;

			_equiped_pos2.y = 599;

			_new_pos1.x = _chuan_dai.x - 274;
			_new_pos1.y = _chuan_dai.y + 12;

			_new_pos2.x = _chuan_dai.x - 71;
			_new_pos2.y = 599;

#ifdef _DEBUG
			ostringstream oss;
			oss << "_chuan_dai: " << _chuan_dai.x 
				<< "," << _chuan_dai.y << '\n'
				<< "_equiped_pos1: " << _equiped_pos1.x
				<< "," << _equiped_pos1.y << '\n'
				<< "_equiped_pos2: " << _equiped_pos2.x
				<< "," << _equiped_pos2.y << '\n'
				<< "_new_pos1: " << _new_pos1.x
				<< "," << _new_pos1.y << '\n'
				<< "_new_pos2: " << _new_pos2.x
				<< "," << _new_pos2.y << endl;
			trace( oss.str() );
#endif
		}

		void Equip::_set_equip_info( 
				const vector<PosStr>& str_list,
				EquipInfo& equip_info )
		{


			assert( !str_list.empty()
				&& "Equip::_set_equip_info:str_list.empty" );

			for( size_t i = 0; i < str_list.size(); i++ )
				if( str_list[ i ].str.find( "魔法攻击力" )
					!= string::npos )
				{
					equip_info.name = "魔法攻击力";
					if( ( i+1 ) < str_list.size() )
					{
						istringstream iss(
							str_list[ i+1 ].str 
							);
								
						iss >> equip_info.value;
					}
					break;
				}
				else if( str_list[ i ].str.find( "物理防御力" )
					!= string::npos )
				{
					equip_info.name = "物理防御力";
					if( ( i+1 ) < str_list.size() )
					{
						istringstream iss(
							str_list[ i+1 ].str 
							);
								
						iss >> equip_info.value;
					}
					break;
				}

				trace( equip_info.name );
				trace( equip_info.value );
		}

		void Equip::set_equip_info()
		{
			_equiped_info.reset();
			_new_info.reset();

			vector<PosStr> str_list;

#ifdef _DEBUG
			ostringstream oss;
			oss << "Equip::set_equip_info():_equiped:\n"
				<< _equiped_pos1.x << ','
				<< _equiped_pos1.y << ','
				<< _equiped_pos2.x << ','
				<< _equiped_pos2.y << endl;

			trace( oss.str() );
#endif
			_world.dnf.GetPosStrList(
				str_list,
				_equiped_pos1.x,
				_equiped_pos1.y,
				_equiped_pos2.x,
				_equiped_pos2.y,
				"ffffff-000000"
				);

			_set_equip_info( str_list, _equiped_info );

#ifdef _DEBUG
			ostringstream oss1;
			oss1 << "Equip::set_equip_info():_new:\n"
				<< _new_pos1.x << ','
				<< _new_pos1.y << ','
				<< _new_pos2.x << ','
				<< _new_pos2.y << endl;

			trace( oss1.str() );
#endif

			_world.dnf.GetPosStrList(
				str_list,
				_new_pos1.x,
				_new_pos1.y,
				_new_pos2.x,
				_new_pos2.y,
				"ffffff-000000"
				);

			_set_equip_info( str_list, _new_info );
		}

		bool Equip::is_high_value()
		{
			trace( "Equip::is_high_value()" );
			if( _new_info.name == _equiped_info.name )
				if( _new_info.value > _equiped_info.value )
					return true;

			return false;
		}

		bool Equip::is_low_value()
		{
			if( _new_info.name == _equiped_info.name )
				if( _new_info.value < _equiped_info.value )
					return true;

			return false;
		}

		bool Equip::check_replace_one()
		{
			trace( "Equip::check_replace_one()" );
			if( set_chuan_dai() )
			{
				set_equip_pos();
				set_equip_info();
				if( is_low_value() )
					return true;
				else
					return false;
			}
			else
				return false;
		}


		Pos Equip::get_pos( size_t i )
		{
			assert( i >= 0
				|| i < 32
				&& "Equip::get_pos:i超出范围" );

			Pos start_pos( 488, 277 ); //( 488,288 );
			size_t y = i / 8;	// 30递增
			size_t x = i % 8;	// 30递增

			return Pos( 
				start_pos.x + x * 30,
				start_pos.y + y * 30
				);
		}


		void Equip::replace()
		{
			for( size_t i = 0; i < 32; i++ )
			{
				Pos pos = get_pos( i );

				_world.dnf.dm.MoveTo( pos.x, pos.y );
				Sleep( 100 );

				if( set_chuan_dai() )
				{
					set_equip_pos();
					set_equip_info();
					if( is_high_value() )
						//block_call( 
						//boost::bind( &Equip::is_replaced, this ),
						//boost::bind( &Cdmsoft::RightClick, &_world.dnf.dm ),
						//boost::bind( null )
						//);
						_world.dnf.dm.RightClick();
				}
				else
					_world.dnf.dm.RightClick();

				Sleep( 100 );
			}
		}

		bool Equip::set_npc_pos()
		{


			_npc_timer.restart();	
			while( true )
			{
				string str = _world.dnf.dm.FindPicE(
					1,1,799,599, "seller3.bmp", "404040", 1.0, 0 );

				trace( "Equip::set_npc_pos()" );
				trace( str );
				istringstream iss( str );

				string size_str;

				getline( iss, size_str, '|' );
				getline( iss, size_str, '|' );
				istringstream x_iss( size_str );
				x_iss >> _npc_pos.x;
				getline( iss, size_str, '|' );
				istringstream y_iss( size_str );
				y_iss >> _npc_pos.y;

				if( !_npc_pos.is_invaild() )
				{
					_npc_pos.x += 1;
					_npc_pos.y += 30;

					return true;
				}
				else
				{
					string str = _world.dnf.dm.FindPicE(
						1,1,799,599, "seller1.bmp", "303030", 1.0, 0 );

					istringstream iss( str );

					string size_str;

					getline( iss, size_str, '|' );
					getline( iss, size_str, '|' );
					istringstream x_iss( size_str );
					x_iss >> _npc_pos.x;
					getline( iss, size_str, '|' );
					istringstream y_iss( size_str );
					y_iss >> _npc_pos.y;

					if( !_npc_pos.is_invaild() )
					{
						_npc_pos.x += 24;
						_npc_pos.y += 30;
					}
					return true;
				}

				if( _npc_timer.elapsed() > 5 )
					return false;

				Sleep( 1 );
			}
		}

		bool Equip::has_shop_option()
		{
			_npc_pos.corrent_assert();
			string str = _world.dnf.dm.Ocr(
				_npc_pos.x, _npc_pos.y,
				_npc_pos.x + 98, _npc_pos.y +104,
				"e6c89b-101010", 1.0 );
			if( str.find( "商店" ) != string::npos )
				return true;
			else
				return false;
		}

		bool Equip::has_sell_ensure( const Pos& pos )
		{
			string str = _world.dnf.dm.Ocr(
				pos.x + 13,
				pos.y - 22,
				pos.x + 90,
				pos.y +18,
				"ddc593-050505", 1.0 );
			if( str.find( "取消" ) != string::npos )
				return true;
			else
				return false;
		}

		bool Equip::has_no_sell_ensure( const Pos& pos )
		{
			return !has_sell_ensure( pos );
		}

		bool Equip::has_safe_mode()
		{
			string str = _world.dnf.dm.Ocr(
				399,237,490,265, "ffffff-030303", 1.0 );
			if( str.find( "安全" ) != string::npos )
			{
				_is_safe_mode = true;
				return true;
			}
			else
				return false;
		}
		bool Equip::has_no_safe_mode()
		{
			return !has_safe_mode();
		}

		bool Equip::has_secret_seller()
		{
			_secret_seller_timer.restart();
			while( true )
			{
				string str = _world.dnf.dm.FindPicE(
					1,1,799,599, "jbl.bmp", "404040", 1.0, 0 );

				trace( "Equip::has_secret_seller()" );
				trace( str );
				istringstream iss( str );

				string size_str;

				getline( iss, size_str, '|' );
				getline( iss, size_str, '|' );
				istringstream x_iss( size_str );
				x_iss >> _secret_seller_pos.x;
				getline( iss, size_str, '|' );
				istringstream y_iss( size_str );
				y_iss >> _secret_seller_pos.y;

				if( !_secret_seller_pos.is_invaild() )
				{
					_secret_seller_pos.x += 1;
					_secret_seller_pos.y += 30;

					return true;
				}

				if( _secret_seller_timer.elapsed() > 2 )
					return false;

				Sleep( 1 );
			}
		}

		bool Equip::has_open_secret_shop()
		{
			string str = _world.dnf.dm.Ocr( 
				149,509,206,531, "ddc593-050505", 1.0 );
			trace( "Equip::has_secret_seller()" );
			trace( str );
			if( str.find( "出售" ) != string::npos )
				return true;
			else
				return false;
		}
		bool Equip::has_secret_shop_option()
		{
			_secret_seller_pos.corrent_assert();
			string str = _world.dnf.dm.Ocr(
				_secret_seller_pos.x, _secret_seller_pos.y,
				_secret_seller_pos.x + 98, _secret_seller_pos.y +104,
				"e6c89b-101010", 1.0 );
			if( str.find( "商店" ) != string::npos )
				return true;
			else
				return false;
		}

		void Equip::random_shop_pos()
		{
			_shop_sell_pos.x += random() % 20 - 10;
			_shop_sell_pos.y += random() % 20 - 10;
		}

		bool Equip::is_active_sell()
		{
			trace( "Equip::is_active_sell()" );
			trace( _sell_button_pos1.x );
			trace( _sell_button_pos1.y );
			trace( _sell_button_pos2.x );
			trace( _sell_button_pos2.y );
			
			string str = _world.dnf.dm.Ocr(
				_sell_button_pos1.x,
				_sell_button_pos1.y,
				_sell_button_pos2.x,
				_sell_button_pos2.y,
				 "ffffb8-050505", 1.0 );
				//219,449,271,471, "ffffb8-050505", 1.0 );

			

			if( str.find( "出售" ) != string::npos )
				return true;
			else
				return false;
		}

		bool Equip::is_finger_move_to( const Pos& pos )
		{
			if( _world.dnf.has_pic(
				"finger.bmp",
				pos.x - 10, pos.y - 10,
				pos.x + 50, pos.y + 50,
				"050505" ) 
				)
				return true;
			else
				return false;
		}


		void Equip::sell_one( const Pos& pos )
		{
			trace( "sell_one" );
			/*
			_world.dnf.dm.MoveTo(
				pos.x, pos.y );

			block_call( 
				boost::bind( 
						&Equip::is_finger_move_to,
						this, pos ),
				boost::bind( null ),
				boost::bind( null )
				);


			_world.dnf.dm.LeftDown();

			_world.dnf.dm.MoveTo(
				_shop_sell_pos.x,
				_shop_sell_pos.y 
				);

			block_call( 
				boost::bind( 
						&Equip::is_finger_move_to,
						this, _shop_sell_pos ),
				boost::bind( null ),
				boost::bind( null )
				);

			_world.dnf.dm.LeftUp();
			*/

			block_call(
				boost::bind( &Equip::is_active_sell, this ),
				boost::bind( null ),
				boost::bind(
						&Cdmsoft::MoveTo, &_world.dnf.dm, 
						_sell_button.x,_sell_button.y ),
				0.5, 100, false
				);
			
			trace( "Equip::sell_one:1" );

			_world.dnf.dm.LeftClick();
			Sleep( 50 );
			_world.dnf.dm.MoveTo( pos.x, pos.y );
			Sleep( 50 );
			_world.dnf.dm.LeftClick();
			Sleep( 150 );
			if( !has_sell_ensure( pos ) )
				return;

			trace( "Equip::sell_one:2" );
			block_call(
				boost::bind( &Equip::has_no_sell_ensure, this, pos ),
				boost::bind( null ),
				boost::bind( &Player::LB, &_world.player, pos ),
				 5, 50, false
				);

			trace( "Equip::sell_one:2" );
			if( has_safe_mode() ) 
				block_call(
				boost::bind( &Equip::has_no_safe_mode, this ),
				boost::bind( null ),
				boost::bind( &World::handle_safe_mode, &_world ),
				 10, 50
				);
		}

		void Equip::sell_consumable()
		{
			switch_to_consumable();

			for( size_t i = 0; i < 32; i++ )
			{
				random_shop_pos();
				if( !_is_safe_mode )
					sell_one( get_pos( i ) );
			}
		}

		void Equip::sell_stuff()
		{
			switch_to_stuff();

			for( size_t i = 0; i < 32; i++ )
			{
				random_shop_pos();
				if( !_is_safe_mode )
					sell_one( get_pos( i ) );
			}
		}

		bool Equip::open_shop()
		{
			if( has_secret_seller() )
			{
				_sell_button = Pos( 196,521 );
				_sell_button_pos1 = Pos( 149,509 );
				_sell_button_pos2 = Pos( 206,531 );

				block_call(
					boost::bind( &Equip::has_secret_shop_option, this ),
					boost::bind( null ),
					boost::bind( &Player::LB, &_world.player, 
						Pos( _secret_seller_pos.x, _secret_seller_pos.y )
							),
					 5, 200, false
				 );

				block_call(
					boost::bind( &World::has_equip, &_world ),
					boost::bind( null ),
					boost::bind( &Player::LB, &_world.player,
						Pos( _secret_seller_pos.x + 62, _secret_seller_pos.y + 25 )
							),
					 6, 200, false
					);

				return true;
			}

			if( !set_npc_pos() )
				return false;

			_sell_button = Pos( 200, 511 );// Pos( 265,460 );
			_sell_button_pos1 = Pos( 151,496 );//Pos( 219,449 );
			_sell_button_pos2 = Pos( 211,522 );//Pos( 271,471 );

			if( !block_call(
				boost::bind( &Equip::has_shop_option, this ),
				boost::bind( null ),
				boost::bind( &Player::LB, &_world.player, 
							Pos( _npc_pos.x, _npc_pos.y )
							),
				 5, 100, false
				) )
				return false;


			block_call(
				boost::bind( &World::has_equip, &_world ),
				boost::bind( null ),
				boost::bind( &Player::LB, &_world.player,
							Pos( _npc_pos.x + 59, _npc_pos.y + 55 )
							),
				 6, 100, false
				);

			return true;
		}

		void Equip::replace_and_sell_equipment()
		{
			switch_to_equipment();

			for( size_t i = 0; i < 32; i++ )
			{
				Pos pos = get_pos( i );

 				_world.dnf.dm.MoveTo( pos.x, pos.y );
				Sleep( 100 );

				if( set_chuan_dai() )
				{
					
					set_equip_pos();
					set_equip_info();
					if( is_high_value() )
					{
						//_world.dnf.dm.RightClick();
						trace( "is_high_value()" );	
						block_call(
							boost::bind( &Equip::check_replace_one, this ),
							boost::bind( null ),
							boost::bind( &Cdmsoft::RightClick, &_world.dnf.dm ),
							0.5, 100, false );
					}
					Sleep( 10 );
					if( !_is_safe_mode )
						sell_one( get_pos( i ) );
				}
				else
				{
					_world.dnf.dm.RightClick();
					Sleep(10);
					_world.dnf.dm.RightClick();
					Sleep(10);
				}

				Sleep( 100 );
			}

		}

		void Equip::replace_and_sell()
		{
			open_shop();
			replace_and_sell_equipment();
			if( !_is_safe_mode )
			{
				sell_consumable();
				sell_stuff();
			}
		}

		bool Equip::has_highlight_equipment()
		{
			string str = _world.dnf.dm.Ocr(
				//469,245,616,272, 
				478,239,518,259,
				"ffffb8-050505", 1.0 );
			if( str.find( "装备" ) != string::npos )
				return true;
			else
				return false;
		}
		bool Equip::has_highlight_consumable()
		{
			string str = _world.dnf.dm.Ocr(
				//469,245,616,272,
				524,238,566,255,
				"ffffb8-050505", 1.0 );
			if( str.find( "消耗" ) != string::npos )
				return true;
			else
				return false;
		}
		bool Equip::has_highlight_stuff()
		{
			string str = _world.dnf.dm.Ocr(
				//469,245,616,272,
				568,234,616,259,
				"ffffb8-050505", 1.0 );
			if( str.find( "材料" ) != string::npos )
				return true;
			else
				return false;
		}
		
		void Equip::switch_to_equipment()
		{
			assert( _world.has_equip()
				&& "Equip::switch_to_equipment:没有打开装备栏" );

			block_call(
				boost::bind( &Equip::has_highlight_equipment, this ),
				boost::bind( &Player::LB, &_world.player, Pos( 513,246 ) ),//Pos( 511,260 ) ),
				boost::bind( null )
				);
		}


		void Equip::switch_to_consumable()
		{
			assert( _world.has_equip()
				&& "Equip::switch_to_equipment:没有打开装备栏" );

			block_call(
				boost::bind( &Equip::has_highlight_consumable, this ),
				boost::bind( &Player::LB, &_world.player, Pos( 558,246 ) ),//Pos( 560,260 ) ),
				boost::bind( null )
				);
		}

		void Equip::switch_to_stuff()
		{
			assert( _world.has_equip()
				&& "Equip::switch_to_equipment:没有打开装备栏" );

			block_call(
				boost::bind( &Equip::has_highlight_stuff, this ),
				boost::bind( &Player::LB, &_world.player, Pos( 608,247 ) ), //Pos( 607,260 ) ),
				boost::bind( null )
				);
		}

		const Pos Equip::_shop_center_pos = Pos( 327,331 );

		///////////////////////////////////////////////////////

		Dungeon::Dungeon( World& world )
			:_world( world ),
			_move( world ),
			_attack( world ),
			_equip( world ),
			//_move_thread( boost::ref( _move ) ),
			_attack_thread( boost::ref( _attack ) ),
			_select_reward_num( 0 )
		{}

		void Dungeon::run()
		{
			//_move_thread.join();
			//_attack_thread.join();

			Message::pluse( 70, 2 );

			_move();

			Message::pluse( 70, 2 );

			reward();
			output( "处理错误窗口" );

			clear();
			output( "拾取掉落" );
			pick_up();

			output( "出售装备" );
			sell();

			output( "处理地下城程序" );
			out();
			//_attack_thread.join();


			//	//	翻牌子
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
			//			&& "Main::run:_world.is_dungeon_reword超时" );

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
			//			&& "Main::run::_world.is_dungeon_end超时" );

			//		break;
			//	}

			//	Sleep( 200 );

			//}
		}

		void Dungeon::reward()
		{
			//	翻牌子
			_select_reward_num = 0x31 + ( random() % 4 );

			block_call( boost::bind( &Dungeon::_reward_start, this ), 20 );
			
			////////////////////////////
			/*
			_out_timer.restart();
			trace( "_out_timer.restart()" );
			while( true )
			{
				if( _world.is_dungeon_reward() )
				{
					size_t rand = random();
					trace( rand );
					rand = rand % 4;
					trace( rand );
					size_t vk_key_1_4 = 0x31 + rand;

					while( true )
					{
						trace( vk_key_1_4 );
						_world.player.key_press( vk_key_1_4 );
						Sleep( 300 );

//						if( !_world.is_dungeon_reward() )
						if( _world.is_dungeon_end() )
							break;
					}

					break;
				}

				if( _out_timer.elapsed() > 10 )
				{
					trace( "_out_timer.elapsed" );

					assert( false
						&& "Main::run:_world.is_dungeon_reword超时" );

					break;
				}


				Sleep( 100 );
			}
			*/
		}

		bool Dungeon::_reward_start()
		{
			//trace( "_world.is_dungeon_reward()" );
			//trace( _world.is_dungeon_reward() );
			if( _world.is_dungeon_reward() )
			{
				//block_call( boost::bind( &Dungeon::_reward_end, this ), 20 );

				output( "处理奖励信息" );
				for( size_t i = 0;i < 20; i++ )
				{
					trace( "Sleep( 500 )" );
					Sleep( 500 );
					_world.player.key_press( _select_reward_num );
				}

				return true;
			}
			else
				return false;
		}

		bool Dungeon::_reward_end()
		{
			if( _world.is_dungeon_reward_end() )
				return true;
			else
			{
				_world.player.key_press( _select_reward_num );
				return false;
			}
		}

		void Dungeon::clear()
		{
			_world.open_esc();
			_world.close_esc();
		}

		void Dungeon::pick_up()
		{
			_out_timer.restart();
			while( true )
			{
				//	更新所有坐标
				//	更新人物地图坐标
				_world.update_player_map_pos();

				//	更新人物窗口坐标
				_world.update_player_win_pos();
				//	更新目标地图坐标
				_world.update_target_group_map_pos();

				//	更新目标地图坐标
				_world.update_target_group_win_pos();

				_attack.pick_up();

				if( _out_timer.elapsed() > 1 )
					break;

				Sleep( 10 );
			}
		}

		void Dungeon::sell()
		{
			if( !_DNF::Sync::is_safe_mode() )
			{
				if( !_equip.open_shop() )
					return;

				if( _world.is_out_weight() )
				{
					_equip.replace_and_sell_equipment();
					

					// 修理
					for( size_t i = 0; i < 5; i++ )
					{
						//	大转移
						//_world.player.LB( Pos( 418,461) );
						_world.player.LB( Pos( 356,510 ) );
						Sleep( 100 );
					}

					//	整理	
					output( "整理装备" );
					for( size_t i = 0; i < 5; i++ )
					{
						_world.player.LB( Pos( 684,487 ) );
						Sleep( 100 );
					}
				}


				if( !_world.has_energy() )
				{
					_equip.sell_consumable();

					_equip.sell_stuff();
				}
			}

			_world.close_equip();
		}

		void Dungeon::out()
		{
			block_call( boost::bind( &World::is_dungeon_end, &_world ), 20 );

			/*
			trace( "dungeon_end" );

			//	等待结束
			_out_timer.restart();
			trace( "_out_timer.restart()" );
			while( true )
			{
				if( _world.is_dungeon_end() )
					break;

				if( _world.is_in_big_map() )
					break;

				if( _out_timer.elapsed() > 15 )
				{
					trace( "_out_timer.elapsed" );
					assert( false
						&& "Main::run::_world.is_dungeon_end超时" );

					break;
				}

				Sleep( 200 );
			}
			*/

			_world.to_big_map();
		}

	
_DUNGEON_END
		
//	}	// namespace dungeon
//}	//	namespace dnf;