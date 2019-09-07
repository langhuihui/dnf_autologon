#include "map_recorder.h"

//namespace dnf{
//	namespace map{

_MAP_BEGIN

		void wait( size_t time )
		{
			boost::this_thread::sleep( boost::posix_time::millisec( time ) );
		}

		/////////////////////////////////
		//	Input
		Input::Input( World& world )
			:_is_start( false ),
			_is_end_record( false ),
			_is_finish( false ),
			_is_restart( false ),
			_world( world )
		{
		}

		void Input::reset()
		{
			_is_start = false;
			_is_end_record = false;
			_is_restart = false;
			_is_finish = false;
		}

		void Input::operator()()
		{
			while( true )
			{
				if( !_is_start )
					start_record();

				wait( 10 );
			}
		}

		void Input::check_map_open()
		{
			while( true )
			{
				_world.update_map_win_pos();
				_world.update_boss_room_win_pos();
				if( 
					(_world.map_pos.x == -1
					&& _world.map_pos.y == -1 )
					|| ( _world.boss_room_pos.x == -1
					&& _world.boss_room_pos.y == -1 )
					)
					MessageBox( 
					0,
					"找不到小地图坐标，可能是被遮挡或者没打开小地图。\n恢复地图后点击确定继续。",
					"警告",
					0
					);
				else
					break;
			}
		}

		void Input::start_record()
		{
			assert( !_is_start
				&& !_is_restart
				&& !_is_end_record
				&& !_is_finish
				&& "Input::start_record:状态错误" );

			cout << "==============================" << endl;
			cout << "输入s开始记录地图结构:" << endl;
			string str;
			cin >> str; 
			if( "s" == str )
			{
				cout << "输入地图名:" << endl;
				cin >> _map_name;

				check_map_open();

				_is_start = true;

				cout << "开始记录..." << endl;
				update_state();
			}
		}

		void Input::end_record()
		{
			assert( _is_start
				&& !_is_end_record
				&& !_is_finish
				&& !_is_restart
				&& "Input::end_record:状态错误" );

			_is_end_record = true;

			cout << "结束记录,等待数据处理..." << endl;

			while( true )
			{
				if( _is_finish )
				{
					cout << "数据处理完成,本次记录完毕." << endl;
					break;
				}

				if( _is_restart )
				{
					cout << "处理中断,重新开始." << endl;
					break;
				}

				wait( 10 );
			}

			reset();
		}

		void Input::update_state()
		{
			assert( _is_start 
				&& !_is_end_record
				&& !_is_finish
				&& !_is_restart
				&& "Input::updata_state:状态错误" );

#ifdef _DEBUG
			size_t debug_cnt = 0;
#endif 

			//	这里的2个pos是用来记录
			//	上次有效的值的
			//	这里这么做.是因为游戏的
			//	过程是map_pos最终会替代掉boss_room_pos
			//	所以到达boss房间后.boss_room_pos不会更新
			//	但是map_pos最终会更新到一个新的值
			//	这个值和boss_room_pos保存的值相等
			//	--------------------------------
			//	6/21(大转移)修改
			//	人物到底boss房间后.boss_room_pos并不会
			//	消失,而map_pos.会消失
			//	这样的话map_pos的值永远不会和boss_room_pos
			//	相等,但是map_pos到boss_room_pos的距离
			//	只有一个房间的距离,并且_world.map_pos为无效
			//	(-1,-1)并且_world.boss_room_pos是有效的.
			//	那么就是能说明人物已经到达了boss房间.
			//	但是这样的话如果遇到npc的光波的话.导致
			//	_world.map_pos无效.
			//	....暂时想不到好的方法
			//	如果出现这种情况,最好重新记录
			Pos map_pos, boss_room_pos;
			while( true )
			{
				_world.update_map_win_pos();
				if( -1 != _world.map_pos.x
					&& -1 != _world.map_pos.y )
					map_pos = _world.map_pos;

				_world.update_boss_room_win_pos();
				if( -1 != _world.boss_room_pos.x
					&& -1 != _world.boss_room_pos.y )
					boss_room_pos = _world.boss_room_pos;

				map_pos.corrent_assert();
				boss_room_pos.corrent_assert();

#ifdef _DEBUG
				if( -1 == _world.map_pos.x
					&& -1 == _world.map_pos.y
					&& -1 == _world.boss_room_pos.x
					&& -1 == _world.boss_room_pos.y )
					debug_cnt++;
				else
					debug_cnt = 0;

				//	20秒内找不到坐标
				if( debug_cnt > 2000 )
				{
					assert( false 
						&& "Input::updata_state:多次找不到坐标，可能是地图没打开" );

					debug_cnt = 0;
				}
#endif 
				if( _is_restart )
					break;

				//if(
				//	map_pos.x
				//	&& map_pos.y
				//	&& boss_room_pos.x
				//	&& boss_room_pos.y
				//	&&
				//	map_pos ==
				//	boss_room_pos )
				//	break;

				if(
					_world.map_pos.is_invaild()
					&& !_world.boss_room_pos.is_invaild()
					&& _world.boss_room_pos.is_init()
					&& abs( map_pos.x - boss_room_pos.x ) < 20 
					&& abs( map_pos.y - boss_room_pos.y ) < 20
					)
					break;

				wait( 100 );
			}

			if( _is_restart )
			{
				reset();
				cout << "记录中断,重新开始." << endl;
				return;
			}
			
			end_record();
		}

		bool Input::is_start()
		{
			return _is_start;
		}
		void Input::set_finish()
		{
			assert( _is_start
				&& !_is_finish
				&& _is_end_record
				&& !_is_restart
				&& "Input::set_finish:状态错误" );

			_is_start = false;
			_is_finish = true;
		}
		bool Input::is_end_record()
		{
			return _is_end_record;
		}
		void Input::restart_record()
		{
			assert( _is_start
				&& !_is_restart
				&& !_is_finish
				&& "Input::restart_record:状态错误" );

			_is_start = false;
			_is_restart = true;
		}

		string Input::get_map_name()
		{
			assert( !_map_name.empty()
				&& "Input::get_map_name:_map_name地图名为空" );

			return _map_name;
		}


		//////////////////////////////////
		//	记录，整理出有用的信息
		//	加入到图中
		Record::Record( World& world )
			:_world( world ),
			_is_end( true ),
			_map_ptr( 0 )
#ifdef	_DEBUG
			,
			_debug_cnt( 0 )
#endif 
		{
		}

		void Record::reset()
		{
			_is_end = true;
			_last_map_pos = Pos();
			_map_ptr = 0;

#ifdef	_DEBUG
			_debug_cnt = 0;
#endif 
		}

		void Record::start( Input& input )
		{
			assert( _is_end 
				&& "Record::start_record:_is_end启动时为假,逻辑异常" );

			_is_end = false;

			while( true )
			{
				update_map_pos();

				if( input.is_end_record() )
				{
					break;
#ifdef	_DEBUG
			_debug_cnt = 0;
#endif 
				}

				wait( 10 );
			}
			
			reset();
		}

		void Record::start()
		{
			assert( _is_end 
				&& "Record::start_record:_is_end_record启动时为假,逻辑异常" );

			_is_end = false;

			while( true )
			{
				update_map_pos();

				if( _is_end )
					break;

				wait( 10 );
			}

			reset();
		}

		void Record::end()
		{
			assert( !_is_end
				&& "Record::end_record:_is_end_record为真,逻辑异常" );

			_is_end = true;
		}
		
		void Record::update_map_pos()
		{
			assert( _map_ptr 
				&& "Record::update_map_pos:_map_ptr为空指针" );


			_world.update_map_win_pos();

#ifdef _DEBUG

			if( -1 == _world.map_pos.x
				&& -1 == _world.map_pos.y )
				_debug_cnt++;
			else
				_debug_cnt = 0;

			//	如果20秒内都无法找到地图坐标
			//	可能是没打开地图
			if( _debug_cnt > 2000 )
			{
				assert( false
				&& "Record::update_map_pos:_debug_cnt超时，可能是地图没打开" );

				_debug_cnt = 0;
			}

#endif
			//	如果更新，并且 _world.map_pos不是没找到
			//	并且 _world.map_pos不是未初始化
			if( _world.map_pos != _last_map_pos
				&& _world.map_pos != Pos()
				&& -1 != _world.map_pos.x
				&& -1 != _world.map_pos.y
				)
			{
				//	加入节点
				if( !_map_ptr->has_vertex( _world.map_pos ) )
				{
					_map_ptr->add_vertex( _world.map_pos );
					cout << "加入节点:" 
						<< _world.map_pos.x << ","
						<< _world.map_pos.y << endl;
				}

				//	如果 _last_map_pos 未初始化
				if( _last_map_pos != Pos() )
				{
					//	断言节点先后两个坐标
					//	在垂直或者水平方向对齐
					//cout << "_world.map_pos:"
					//	<< _world.map_pos.x << ","
					//	<< _world.map_pos.y << endl;
					//
					//cout << "_last_map_pos:" 
					//	<< _last_map_pos.x << ","
					//	<< _last_map_pos.y << endl;
					assert( 
						_world.map_pos.x == _last_map_pos.x
						|| _world.map_pos.y == _last_map_pos.y
						&& "update_map_pos:节点先后两个坐标\
						   在垂直或者水平方向没有对齐" );

					
					assert( _map_ptr->has_vertex( _last_map_pos )
						&& _map_ptr->has_vertex( _world.map_pos )
						&& "update_map_pos:要加入边的节点不存在" );


					//	加入边
					//	因为根据两点坐标可以计算出方向
					//	所以这里没必要记录方向


					Road road;
					road.dir = Dir::no;

					//	同时加入两个方向的边							
					if( !_map_ptr->has_edge( 
							_last_map_pos,
							_world.map_pos,
							road ) )
					{
						_map_ptr->add_edge( 
							_last_map_pos,
							_world.map_pos,
							road );
					
						cout << "加入边:"
							<< _last_map_pos.x << ","
							<< _last_map_pos.y << "->"
							<< _world.map_pos.x << ","
							<< _world.map_pos.y << endl;
					}

					if( !_map_ptr->has_edge( 
							_world.map_pos,
							_last_map_pos,
							road ) )
					{
						_map_ptr->add_edge( 
							_world.map_pos,
							_last_map_pos,
							road );

						cout << "加入边:"
							<< _world.map_pos.x << ","
							<< _world.map_pos.y << "->"
							<< _last_map_pos.x << ","
							<< _last_map_pos.y << endl;
					}
				}


				_last_map_pos = _world.map_pos;
			}

			//	6/21(大转移更新)
			//	因为最后的boss房间不再显示
			//	room_pos而是boss_room_pos
			//	所以一旦_room_pos无效,但是
			//	boss_room_pos有效,并且
			//	_last_map_pos到boss_room_pos
			//	的距离为一个房间的距离
			//	则把boss_room_pos加入到
			//	图中

			_world.update_map_win_pos();
			if( _world.map_pos.is_invaild() )
			{
				_world.update_boss_room_win_pos();
				if( !_world.boss_room_pos.is_invaild() 
					&& _world.boss_room_pos.is_init()
					&& 
					(
						( 
							abs( _world.boss_room_pos.x - _last_map_pos.x ) < 20
							&& _world.boss_room_pos.y == _last_map_pos.y 
						)
						||
						(
							abs( _world.boss_room_pos.y - _last_map_pos.y ) < 20
							&& _world.boss_room_pos.x == _last_map_pos.x
						)
					)
				)
				{

					//	加入节点
					if( !_map_ptr->has_vertex( _world.boss_room_pos ) )
					{
						_map_ptr->add_vertex( _world.boss_room_pos );	
						cout << "加入boss房间节点:" 
							<< _world.boss_room_pos.x << ","
							<< _world.boss_room_pos.y << endl;
					}

					assert( 
						_world.boss_room_pos.x == _last_map_pos.x
						|| _world.boss_room_pos.y == _last_map_pos.y
						&& "update_boss_room_pos:节点先后两个坐标\
						   在垂直或者水平方向没有对齐" );


					assert( _map_ptr->has_vertex( _last_map_pos )
						&& _map_ptr->has_vertex( _world.boss_room_pos )
						&& "update_boss_room_pos:要加入边的节点不存在" );

					Road road;
					road.dir = Dir::no;

					//	同时加入两个方向的边							
					if( !_map_ptr->has_edge( 
						_last_map_pos,
						_world.boss_room_pos,
						road ) )
					{
						_map_ptr->add_edge( 
						_last_map_pos,
						_world.boss_room_pos,
						road );
						cout << "加入边:"
							<< _last_map_pos.x << ","
							<< _last_map_pos.y << "->"
							<< _world.boss_room_pos.x << ","
							<< _world.boss_room_pos.y << endl;
					}

					if( !_map_ptr->has_edge( 
						_world.boss_room_pos,
						_last_map_pos,
						road ) )
					{
						_map_ptr->add_edge( 
						_world.boss_room_pos,
						_last_map_pos,
						road );	
						cout << "加入边:"
							<< _world.boss_room_pos.x << ","
							<< _world.boss_room_pos.y << "->"
							<< _last_map_pos.x << ","
							<< _last_map_pos.y << endl;
					}
				}
			}
		}


		void Record::set_map_ptr( Map& map )
		{
			assert( &map 
				&& "Record::set_map_ptr:map为空指针" );

			assert( map.empty()
				&& "Record::set_map_ptr:map不为空，逻辑错误" );

			_map_ptr = &map;
		}
		
		/////////////////////////////////
		AutoFile::AutoFile(){}

		void AutoFile::set_work_dir( const string& work_dir )
		{
			assert( !work_dir.empty()
				&& "AutoNumFile::set_work_dir: work_dir为空" );

			assert( bfs::is_directory( work_dir )
				&& "AutoNumFile::set_work_dir: work_dir不是一个目录" );

			_work_dir = work_dir;
		}


		bool AutoFile::load_file_list( vector<string>& file_list, const string& file_ext )
		{
			assert( !file_ext.empty()
				&& "AutoNumFile::load_file_list:_file_ext为空" );

			file_list.clear();

			if( bfs::is_empty( _work_dir ) )
				return false;

			bfs::directory_iterator it( _work_dir );
			bfs::directory_iterator it_end;
			for( ; it != it_end; it++ )
				if( it->path().extension() == file_ext )
				{

#ifdef _DEBUG		
					size_t pos = it->path().filename().string().find( '.' );

					assert( pos > 0
						&& pos != string::npos
						&& "AutoNumFile::load_file_list:cut异常" );

					string file_main_name = 
						it->path().filename().string().substr( 0, pos );

					assert( 
						check_only_letter( "0123456789abcdef",
						file_main_name )
						&& file_main_name.size() == 8
						&& "AutoNumFile::load_file_list:文件夹中存在非法格式的文件" );
#endif

					file_list.push_back( it->path().string() );
				}


			if( file_list.empty() )
				return false;

			return true;
		}

		void AutoFile::gen_save_file_name( string& save_file_name, const string& file_ext )
		{
			assert( !file_ext.empty()
				&& "AutoNumFile::gen_save_file_name:_file_ext为空" );

		}

		/////////////////////////////////
		//	Store
		Store::Store()
			:_map_dir( "map" ),
			_map_ext( ".map" ),
			_exist_map_list_ptr( 0 ),
			_save_map_ptr( 0 )
		{
			init();
		}

		void Store::init()
		{
			if( !bfs::exists( _map_dir ) )
				bfs::create_directories( _map_dir );
		}

		void Store::set_map_name_dir( const string& map_name )
		{
			assert( !_map_dir.empty()
				&& " Store::set_map_name_dir:_map_dir名字为空" );

			assert( !map_name.empty()
				&& " Store::set_map_name_dir:地图名字为空" );

			assert( check_no_letter( "-=!@#$%^&*()_+\\\",.;", map_name )
				&& " Store::set_map_name_dir:地图名字中含有不正确的字符" );

			_map_name_dir = _map_dir + "\\" + map_name;

			assert( _map_name_dir.size() > ( map_name.size() + 1 )
				&& " Store::set_map_name_dir:_map_dir名字不正确" );

		}

		void Store::set_exist_map_list_ptr( vector<Map>& exist_map_list )
		{
			assert( &exist_map_list
				&& " Store::set_exist_map_list_ptr:exist_map_list指向0" );
			_exist_map_list_ptr = &exist_map_list;
		}

		void Store::set_save_map_ptr( Map& save_map )
		{
			assert( &save_map
				&& "Store::set_save_map_ptr指向0" );

			_save_map_ptr = &save_map;
		}


		bool Store::load_map( const string& map_name )
		{
			if( "" != map_name )
				set_map_name_dir( map_name );

			if( !bfs::exists( _map_name_dir ) )
				return false;

			if( bfs::is_empty( _map_name_dir ) )
				return false;

			_auto_file.set_work_dir( _map_name_dir );
			_auto_file.load_file_list( _file_list, _map_ext );
			
			assert( 0 != _exist_map_list_ptr
				&& "Store::load_map:_exist_map_list_ptr异常" );

			assert( _exist_map_list_ptr->empty()
				&& "Store::load_map:_exist_map_list_ptr不为空" );

			_exist_map_list_ptr->clear();


			for( size_t i = 0; i < _file_list.size(); i++ )
			{
				//	这里不能把初始化放在后面
				//	否则map不会清空自己
				//	而是递增节点。
				Map map;
				map.load( _file_list[ i ] );	
				_exist_map_list_ptr->push_back( map );
			}

			if( _exist_map_list_ptr->empty() )
				return false;
			else
				return true;
		}

		void Store::save_map( const string& map_name )
		{
			assert( 0 != _save_map_ptr 
				&& "Store::save_map:save_map_ptr为空指针" );

			if( "" != map_name )
				set_map_name_dir( map_name );

			if( !bfs::exists( _map_name_dir ) )
				bfs::create_directory( _map_name_dir );

			string save_dir = _save_map_ptr->get_file_name( _map_ext );
		
			save_dir = _map_name_dir + "\\" + save_dir;
			
			_save_map_ptr->save( save_dir );

			assert( bfs::exists( save_dir )
				&& "Store::save_map:保存文件失败" );

			assert( !bfs::is_empty( save_dir )
				&& "Store::save_map:保存失败，文件为空" );
		}

		void Store::save_map_png( const string& map_name )
		{
			if( "" != map_name )
				set_map_name_dir( map_name );

			if( !bfs::exists( _map_name_dir ) )
				bfs::create_directory( _map_name_dir );

			string save_dot_dir = _save_map_ptr->get_file_name( ".dot" );
			string save_png_dir = _save_map_ptr->get_file_name( ".png" );

			save_dot_dir = _map_name_dir + "\\" + save_dot_dir;
			save_png_dir = _map_name_dir + "\\" + save_png_dir;

			_save_map_ptr->save_dot( save_dot_dir );

			assert( bfs::exists( save_dot_dir )
				&& "Store::save_map_png:保存dot文件失败" );

			assert( !bfs::is_empty( save_dot_dir )
				&& "Store::save_map_png:保存dot文件失败，文件为空" );

			string cmd 
				= "\"\"graphviz-2.36\\release\\bin\\dot.exe\" -Kfdp -n -Tpng \""
				+ save_dot_dir + "\" -o \"" + save_png_dir + "\"\"";

			::system( cmd.c_str() );

			assert( bfs::exists( save_png_dir )
				&& "Store::save_map_png:保存png文件失败" );

			assert( !bfs::is_empty( save_dot_dir )
				&& "Store::save_map_png:保存png文件失败，文件为空" );
		}

		/////////////////////////////////
		Control::Control( World& world )
			:_world( world ),
			_record( _world ),
			_input( world ),
			_input_thread( boost::ref( _input ) )
		{}

		void Control::reset()
		{
			map.clear();
			exist_map.clear();
		}

		void Control::operator()()
		{
			while( true )
			{
				reset();
				
				//	阻塞等待输入地图名开始
				while( true )
				{
					if( _input.is_start() )
						break;
					wait( 10 );
				}

				if( is_new_map() )
				{
					_record.set_map_ptr( map );
					_record.start( _input );

					_store.set_save_map_ptr( map );
					_store.save_map();
					_store.save_map_png();
					_input.set_finish();
				}
				else
				{
					cout << "--地图已经存在,重置地图再试--" << endl;
					_input.restart_record();
				}

				wait( 1 );
			}
		}

		bool Control::is_new_map()
		{
			_store.set_map_name_dir( _input.get_map_name() );
			_store.set_exist_map_list_ptr( exist_map );
			_store.load_map();

			if( exist_map.empty() )
				return true;

			_world.update_map_win_pos();
			_world.map_pos.corrent_assert();
			assert( _world.map_pos != Pos()
				&& "Control::is_new_map:_world.map_pos未初始化" );

			_world.update_boss_room_win_pos();
			_world.boss_room_pos.corrent_assert();
			assert( _world.map_pos != Pos()
				&& "Control::is_new_map:_world.boss_room_pos未初始化" );

			for( size_t i = 0; i < exist_map.size(); i++ )
				if( !exist_map[ i ].has_vertex( _world.map_pos )
					|| !exist_map[ i ].has_vertex( _world.boss_room_pos ) )
					return true;

			return false;
		}


		/////////////////////////////////
		//	MapRecorder
		MapRecorder::MapRecorder( Dm& dm_, AttachIme& attach_ime_ )
			:_world( dm_, attach_ime_ ),
			_control( _world )
		{
			_control();
		}

_MAP_END

//
//	}	//	namespace map
//}	//	namespace dnf