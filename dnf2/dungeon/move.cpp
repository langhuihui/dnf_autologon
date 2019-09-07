#include "move.h"



//namespace dnf{
//	namespace move{
_MOVE_BEGIN

		///////////////////////////
		//	�����������
		Load::Load( World& world_ )
			:_world( world_ ),
			_exist_map_list_ptr( 0 ),
			_exist_room_list_ptr( 0 ),
			_map_dir( "map" ),
			_room_ext( ".room" ),
			_room_cfg_ext( ".rcfg" ),
			_map_ext( ".map" ),
			_map_cfg_ext( ".mcfg" )
		{}

		void Load::set_exist_map_list_ptr( vector<Map>& exist_map_list )
		{
			assert( &exist_map_list
				&& "Load::set_exist_map_list_ptr:exist_map_list����ȷ" );

			_exist_map_list_ptr = &exist_map_list;
		}
		void Load::set_exist_room_list_ptr( vector<Room>& exist_room_list )
		{
			assert( &exist_room_list
				&& "Load::set_exist_room_list_ptr:exist_room_list����ȷ" );

			_exist_room_list_ptr = &exist_room_list;
		}

		void Load::set_map_name_dir()
		{
			assert( bfs::exists( _map_dir ) 
				&& "Load::set_map_name_dir:_map_dir������" );

			vector<string> map_name_list;

			_get_map_name_list( map_name_list );

			string current_map_name = _world.dnf.GetMapName();

			//assert( !current_map_name.empty()
			//	&& "Load::set_map_name_dir::current_map_nameΪ��,\
			//	   �޷��Ӵ���ʶ���ͼ������" ); 

			size_t max_size = 0;
			size_t best_index = -1;

			for( size_t i = 0; i < map_name_list.size(); i++ )
				if( current_map_name.find( map_name_list[ i ] )
					!= string::npos )
					if( map_name_list[ i ].size() > max_size ) 
					{
						max_size = map_name_list[ i ].size();
						best_index = i;
					}

			assert( -1 != best_index
				&& "Load::set_map_name_dir:�Ҳ�����Ч�ĵ�ͼĿ¼" );

			_map_name_dir = 
				_map_dir + "\\" 
				+ map_name_list[ best_index ];


			assert( _map_name_dir.size() > ( _map_dir.size() + 1 )
				&& "Load::set_map_name_dir:_map_name_dir���ô���" );
		}

		bool Load::load_exist_room()
		{
			if( !bfs::exists( _map_name_dir ) )
				return false;

			if( bfs::is_empty( _map_name_dir ) )
				return false;

			vector<string> file_list;


			//_auto_file.set_work_dir( _map_name_dir );
			//_auto_file.load_file_list( file_list, _room_ext );
			_get_room_name_list( file_list, _room_ext );

			assert( 0 != _exist_room_list_ptr
				&& "Load::load_room:_exist_room_list_ptr�쳣" );

			_exist_room_list_ptr->clear();

			assert( _exist_room_list_ptr->empty()
				&& "Load::load_room:_exist_room_list_ptr��Ϊ��" );


			for( size_t i = 0; i < file_list.size(); i++ )
			{
				//	���ﲻ�ܰѳ�ʼ�����ں���
				//	����room��������Լ�
				//	���ǵ����ڵ㡣
				Room room;

				room.load( file_list[ i ] );	
				_exist_room_list_ptr->push_back( room );
			}

			vector<string> cfg_file_list;
			_get_room_name_list( cfg_file_list, _room_cfg_ext );

			for( size_t i = 0; i < cfg_file_list.size(); i++ )
			{
				//cout << "alskdjflajsdlfjalsdjflksajdf;sjf;dsfjas;ldjf" << endl;
				//cout << cfg_file_list[ i ] << endl;
				Special::get_cfg().load( cfg_file_list[ i ] );
			}

			/*
			trace( cfg_file_list.size() );
			trace( cfg_file_list[ 0 ] );
			trace( "���������ļ�" );
			_SPECIAL::Config& ref_cfg = Special::get_cfg();
			trace( ref_cfg.has_case( _SPECIAL::CaseName::TeLeiZeGai ) );
			trace( ref_cfg.get_case_filename(
				ref_cfg.get_case_index( 
					_SPECIAL::CaseName::TeLeiZeGai
					) ) );

			assert( false );

					*/

			if( _exist_room_list_ptr->empty() )
				return false;
			else
				return true;
		}

		bool Load::load_exist_map()
		{

			if( !bfs::exists( _map_name_dir ) )
				return false;

			if( bfs::is_empty( _map_name_dir ) )
				return false;

			//trace( _map_name_dir );

			vector<string> file_list;

			_get_map_graph_name_list( file_list, _map_ext );

			assert( 0 != _exist_map_list_ptr
				&& "Load::load_exist_map:_exist_map_list_ptr�쳣" );

			//assert( _exist_map_list_ptr->empty()
			//	&& "Load::load_exist_map:_exist_map_list_ptr��Ϊ��" );

			_exist_map_list_ptr->clear();


			for( size_t i = 0; i < file_list.size(); i++ )
			{
				//	���ﲻ�ܰѳ�ʼ�����ں���
				//	����map��������Լ�
				//	���ǵ����ڵ㡣
				Map map;
				map.load( file_list[ i ] );	
				_exist_map_list_ptr->push_back( map );
			}

			vector<string> cfg_file_list;

			_get_map_graph_name_list( cfg_file_list, _map_cfg_ext );

			for( size_t i = 0; i < cfg_file_list.size(); i++ )
				Special::get_cfg().load( cfg_file_list[ i ] );

			//trace( cfg_file_list.size() );
			//trace( cfg_file_list[ 0 ] );
			//trace( "���������ļ�" );
			//_SPECIAL::Config& ref_cfg = Special::get_cfg();
			//trace( ref_cfg.has_case( _SPECIAL::CaseName::YingXiongZhong ) );
			//trace( ref_cfg.get_case_filename(
			//	ref_cfg.get_case_index( 
			//		_SPECIAL::CaseName::TeLeiZeGai
			//		) ) );


			//assert( false );

			if( _exist_map_list_ptr->empty() )
				return false;
			else
				return true;
		}


		bool Load::_get_map_name_list( vector<string>& map_name_list )
		{
			map_name_list.clear();

			if( bfs::is_empty( _map_dir ) )
			{
				assert( 0
					&& "Load::_get_map_name_list,��ͼ�����ڵ�Ŀ¼Ϊ��" );
				return false;
			}

			bfs::directory_iterator it( _map_dir );
			bfs::directory_iterator it_end;
			for( ; it != it_end; it++ )
				map_name_list.push_back(
				it->path().filename().string() );

			assert( !map_name_list.empty()
				&& "Load::_get_map_name_list:map_name_listδ�ܻ�ȡ����ͼ��" );

			return true;
		}

		bool Load::_get_room_name_list( vector<string>& file_list, const string& file_ext )
		{
			assert( !file_ext.empty()
				&& "Load::_get_room_name_list:_file_extΪ��" );

			file_list.clear();

			if( bfs::is_empty( _map_name_dir ) )
				return false;

			bfs::directory_iterator it( _map_name_dir );
			bfs::directory_iterator it_end;
			for( ; it != it_end; it++ )
				if( it->path().extension() == file_ext )
				{

#ifdef _DEBUG		
					size_t pos = it->path().filename().string().find( '.' );

					assert( pos > 0
						&& pos != string::npos
						&& "Load::_get_room_name_list:cut�쳣" );

					string file_main_name = 
						it->path().filename().string().substr( 0, pos );

					assert( 
						check_only_letter( "0123456789",
						file_main_name )
						&& !file_main_name.empty()
						&& "Load::_get_room_name_list:�ļ����д��ڷǷ���ʽ���ļ�" );
#endif

					file_list.push_back( it->path().string() );
				}


				if( file_list.empty() )
					return false;

				return true;
		}

		bool Load::_get_map_graph_name_list( vector<string>& file_list, const string& file_ext, bool no_path )
		{
			assert( !file_ext.empty()
				&& "Load::_get_map_graph_name_list:_file_extΪ��" );

			file_list.clear();

			if( bfs::is_empty( _map_name_dir ) )
				return false;

			bfs::directory_iterator it( _map_name_dir );
			bfs::directory_iterator it_end;
			for( ; it != it_end; it++ )
				if( it->path().extension() == file_ext )
				{

#ifdef _DEBUG		
					size_t pos = it->path().filename().string().find( '.' );

					assert( pos > 0
						&& pos != string::npos
						&& "Load::_get_map_graph_name_list:cut�쳣" );

					string file_main_name = 
						it->path().filename().string().substr( 0, pos );

					assert( 
						check_only_letter( "0123456789abcdef",
						file_main_name )
						&& file_main_name.size() == 8
						&& "Load::_get_map_graph_name_list:�ļ����д��ڷǷ���ʽ���ļ�" );
#endif

					if( no_path )
					{
						file_list.push_back( it->path().filename().string() );
					}
					else
						file_list.push_back( it->path().string() );
				}


				if( file_list.empty() )
					return false;

				return true;
		}


		///////////////////////
		//	Update
		//	�����ƶ���λ��
		Update::Update( World& world_ )
			:_world( world_ ),
			_map_dir( "map" ),
			_time_limit( 2 ),
			_sleep_time( 1000 ),
			_exist_map_list_ptr( 0 ),
			_exist_room_list_ptr( 0 ),
			_next_pos_ptr( 0 ),
			_room_shortest_index( 0 ),
			_is_dungeon_end( false ),
			_is_dungeon_score( false ),
			_is_in_boss_room( false )
		{
		}

		void Update::init()
		{
			_is_dungeon_end = false;
			_is_dungeon_score = false;
			_is_in_boss_room = false;
			//	����sync��attack����ʹ��
			_DUNGEON::Sync::unset_in_boss_room();
			_last_map_pos = Pos();
		}

		void Update::operator()()
		{
			if( is_at_next_pos() )
				set_next_pos();
		}

		void Update::set_exist_map_list_ptr( vector<Map>& exist_map_list )
		{
			assert( &exist_map_list
				&& "Update::set_exist_map_list_ptr:exist_map_list����ȷ" );

			_exist_map_list_ptr = &exist_map_list;
		}
		void Update::set_exist_room_list_ptr( vector<Room>& exist_room_list )
		{
			assert( &exist_room_list
				&& "Update::set_exist_room_list_ptr:exist_room_list����ȷ" );

			_exist_room_list_ptr = &exist_room_list;
		}

		void Update::set_next_pos_ptr( Pos& next_pos )
		{
			assert( &next_pos 
				&& "Update::set_next_pos_ptr:next_pos����ȷ" );

			_next_pos_ptr = &next_pos;
		}


		string Update::_get_map_name()
		{
			assert( bfs::exists( _map_dir ) 
				&& "Store::set_map_name_dir:_map_dir������" );

			vector<string> map_name_list;

			_get_map_name_list( map_name_list );

			string current_map_name = _world.dnf.GetMapName();

			//assert( !current_map_name.empty()
			//	&& "Update::set_map_name_dir::current_map_nameΪ��,\
			//	   �޷��Ӵ���ʶ���ͼ������" ); 

			if( current_map_name.empty() )
				return string();

			size_t max_size = 0;
			size_t best_index = -1;

			for( size_t i = 0; i < map_name_list.size(); i++ )
				if( current_map_name.find( map_name_list[ i ] )
					!= string::npos )
					if( map_name_list[ i ].size() > max_size ) 
					{
						max_size = map_name_list[ i ].size();
						best_index = i;
					}

					//assert( -1 != best_index
					//	&& "Store::set_map_name_dir:�Ҳ�����Ч�ĵ�ͼĿ¼" );

					if( -1 == best_index )
						return string();
					else	
						return map_name_list[ best_index ];
		}

		bool Update::_get_map_name_list( vector<string>& map_name_list )
		{
			map_name_list.clear();

			if( bfs::is_empty( _map_dir ) )
			{
				assert( 0
					&& "Update::_get_map_name_list,��ͼ�����ڵ�Ŀ¼Ϊ��" );
				return false;
			}

			bfs::directory_iterator it( _map_dir );
			bfs::directory_iterator it_end;
			for( ; it != it_end; it++ )
				map_name_list.push_back(
				it->path().filename().string() );

			assert( !map_name_list.empty()
				&& "Update::_get_map_name_list:map_name_listδ�ܻ�ȡ����ͼ��" );

			return true;
		}

		////////

		bool Update::map_name_is_changed()
		{
			string map_name = 
				_get_map_name();

			if( map_name != _map_name )
				return true;
			else
				return false;
		}

		bool Update::is_corrent_map()
		{
			string map_name =
				_get_map_name();

			if( map_name.empty() )
				return false;
			else
				return true;
		}

		void Update::update_map_name()
		{
			_map_name = _get_map_name();
		}

		void Update::set_current_map()
		{

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_map_win_pos_block();
			_world.update_boss_room_win_pos_block();

			_world.map_pos.corrent_assert();
			_world.map_pos.init_assert();
			_world.boss_room_pos.corrent_assert();
			_world.boss_room_pos.init_assert();

			_current_map.clear();

			trace( "_world.map_pos" );
			trace( _world.map_pos.x );
			trace( _world.map_pos.y );
			trace( "_world.boss_room_pos" );
			trace( _world.boss_room_pos.x );
			trace( _world.boss_room_pos.y );

			trace( _exist_map_list_ptr->size() );
			for( size_t i = 0; i < _exist_map_list_ptr->size(); i++ )
			{
				trace( "has map_pos" );
				trace( (*_exist_map_list_ptr)[ i ].has_vertex( _world.map_pos ) );
				trace( "has boss_room_pos" );
				trace( (*_exist_map_list_ptr)[ i ].has_vertex( _world.boss_room_pos ) );
				if( (*_exist_map_list_ptr)[ i ].has_vertex( _world.map_pos )
					&& (*_exist_map_list_ptr)[ i ].has_vertex(
					_world.boss_room_pos ) )
				{
					trace( "_current_map = (*_exist_map_list_ptr)[ i ];" );
					_current_map = (*_exist_map_list_ptr)[ i ];		
					Sync::set_current_map_id( 
						_current_map.hash_code_str() );
				}
			}

			//	Map���淵�ص�file_name����·��
			//	��load�����file_name����·��
			//if( Special::get_cfg().has_case( _current_map.get_file_name( ".mcfg" ) ) )
			//{
			//	string file_name 
			//		= Special::get_cfg().get_case_filename(
			//		Special::get_cfg().get_case_index( 
			//		_current_map.get_file_name( ".mcfg" )
			//			)	
			//		);
			//}

			assert( !_current_map.empty() 
				&& "Update::set_current_map:�Ҳ�����Ӧ�ĵ�ͼ�ṹ" );
		}

		void Update::set_rfp()
		{
			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_target_group_map_pos();
			_current_room._rfp.set(
				_world.target_group );
			

			_current_room._rfp.corrent_assert();
		}

		bool Update::is_in_exist_room_list()
		{

			float best_sim = 0;
			size_t best_sim_index;

			for( size_t i = 0; i < _exist_room_list_ptr->size(); i++ )
			{
				float sim =
					comp_sim( (*_exist_room_list_ptr)[ i ]._rfp, _current_room._rfp );

				if( best_sim < sim )
				{
					best_sim = sim;
					best_sim_index = i;
				}
			}

			trace( "������ƶ�:" );
			trace( best_sim );


			if( best_sim )
			{
				trace( 
					(*_exist_room_list_ptr)[ best_sim_index ]._file_name
					);
			}


			_current_room._rfp.set_comp_scale( 0.7 );

			handle_special_case_YingXiongZhong();

			if( best_sim > _current_room._rfp.get_comp_scale() )
				return true;
			else
				return false;
		}

		void Update::set_best_sim_room()
		{
			assert( is_in_exist_room_list()
				&& "Update::set_best_sim_room:exist_list�����ڶ�Ӧ��room" );

			float best_sim = 0;
			size_t best_sim_index;

			for( size_t i = 0; i < _exist_room_list_ptr->size(); i++ )
			{
				float sim =
					comp_sim( (*_exist_room_list_ptr)[ i ]._rfp, _current_room._rfp );

				if( best_sim < sim )
				{
					best_sim = sim;

					best_sim_index = i;
				}
			}

			trace( "ƥ�䷿���:" );
			trace( 
				(*_exist_room_list_ptr)[ best_sim_index ]._file_name
				);

			output( "�ҵ�����:" );
			output(	(*_exist_room_list_ptr)[ best_sim_index ]._file_name );

			_current_room = (*_exist_room_list_ptr)[ best_sim_index ];
		
			Sync::set_current_room_id(
				_current_room.get_id_str() );
		}

		bool Update::is_right_state()
		{

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );


			//	��ʱʹ��map_win_pos
			//	���ж�,��������ֶ�
			//	�ر�С��ͼ�ᵼ���߼�
			//	����.

			//	��ת�Ƹ���,��Ϊboss��־ʶ��

			//_world.update_map_win_pos( );
			//if( _world.map_pos.is_invaild() )
			_world.update_boss_room_win_pos();
			if( _world.boss_room_pos.is_invaild() )
				return false;
			else
				return true;
		}

		bool Update::is_change_room()
		{

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			//trace( "start is_change_room()" );

			_world.update_map_win_pos();

			//trace( "get_map_win_pos()" );

			if( !_last_map_pos.is_init() )
			{
				if( !_world.map_pos.is_invaild() )
				{
					_last_map_pos = _world.map_pos;
					return true;
				}

				return false;
			}

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
					//trace( "boss room" );
					_is_in_boss_room = true;
					_DUNGEON::Sync::set_in_boss_room();
					return true;
				}
				else
					return false;
			}
			else
			{
				if( _world.map_pos != _last_map_pos )
					return true;
				else
					return false;
			}
		}
		bool Update::update_last_map_pos()
		{

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_map_win_pos();
			_world.map_pos.init_assert();

			if( !_world.map_pos.is_invaild() )
			{
				_last_map_pos = _world.map_pos;
				return true;
			}
			else
				return false;

		}

		void Update::set_map_shortest()
		{
			assert( !_current_map.empty()
				&& "Update::set_map_shortest:_current_mapΪ��" );


			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_map_win_pos();
			_world.update_boss_room_win_pos();

			_world.map_pos.corrent_assert();
			_world.map_pos.init_assert();
			_world.boss_room_pos.corrent_assert();
			_world.boss_room_pos.init_assert();

			_current_map.set_shortest_path( 
				_world.map_pos, _world.boss_room_pos );

			assert( _current_map.get_shortest_path_vd().size() > 1
				&& "Update::set_map_shortest:���·����ȡ����" );
		}

		void Update::set_room_shortest()
		{
			using _ROOM::Node;
			Node nearest_node;
			Dir dir;
			{
				_world.update_map_win_pos_block( 30, 100, false );

				_world.map_pos.init_assert();
				_world.map_pos.corrent_assert();

				boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

				_world.update_player_map_pos();

				nearest_node =
					_current_room._node_mgr.nearest_record_node( 
					_world.player.map_pos );


				dir = _current_map.next_road_dir( 
					_current_map.get_vd( _world.map_pos ) );
			}

			Node door_node;
			door_node.set_door_type();
			if( _current_room._door_mgr.has_dir( dir ) )
				door_node.pos = _current_room._door_mgr.get_pos( dir );
			else
			{
				//assert( false
				//&& "Update::set_room_shortest:�Ҳ�����Ӧ�������" );
				
				trace( "Update::set_room_shortest:�Ҳ�����Ӧ�������" );
				_DNF::Sync::set_return_town();
				while( true )
					Sleep( 10000 );

			}

			_current_room._node_mgr.set_shortest_path(
				nearest_node, door_node );

			using _ROOM::NodeMgr;
			const vector<NodeMgr::vd_t>& node_shortest
				= _current_room._node_mgr.get_shortest_path_vd();

			_room_shortest.clear();

			for( size_t i = 0; i < node_shortest.size(); i++ )
			{
				_room_shortest.push_back( 
					_current_room._node_mgr[ node_shortest[ i ] ].pos
					);
			}
		}

		bool Update::is_at_next_pos()
		{

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_player_map_pos();

			_world.player.map_pos.init_assert();
			_world.player.map_pos.corrent_assert();

			//trace( "at_pos" );
			//trace( _world.player.map_pos.x );
			//trace( _world.player.map_pos.y );

			size_t max_gap = 20;

			glog( "_world.player.map_pos" );
			glog( _world.player.map_pos.x );
			glog( _world.player.map_pos.y );
			glog( "*_next_pos_ptr" );
			glog( _next_pos_ptr->x );
			glog( _next_pos_ptr->y );

			if( 
				abs( _world.player.map_pos.x - _next_pos_ptr->x ) < max_gap
				&& 
				abs( _world.player.map_pos.y - _next_pos_ptr->y ) < max_gap 
				)
				return true;
			else
				return false;
		}

		void Update::set_next_pos()
		{
			assert( _room_shortest.size() > _room_shortest_index 
				&& "Update::set_next_pos:_room_shortest_indexԽ��" );

			////	��check_enemy���Ƶ��ظ������ڴ�ģ��Ľӿ�
			////	����dm�ӿڵĺ����ķ������ûᵼ�����ܵ��½�
			////	������ʱ�������Ƕ�move���̵�ѭ�����ʱ��
			////	������.��Sleep(100).�����ⲻ��һ������Ľ��
			////	����
			//check_enemy();

			//if( _has_enemy )
			//{
			//	

			//}
			//else
			//{

			if( !_is_set_room_shortest )
			{
				set_room_shortest();
				_is_set_room_shortest = true;
			}

			if( _room_shortest_index < _room_shortest.size() )
			{
				*_next_pos_ptr = _room_shortest[ _room_shortest_index ];
				_room_shortest_index++;
			}
			//}
		}

		void Update::unset_room_shortest()
		{
			_room_shortest_index = 0;
			_is_set_room_shortest = false;
		}

		void Update::set_enemy()
		{
			_has_enemy = true;
		}

		void Update::check_enemy()
		{
			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_target_group_map_pos();

			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id )
				{
					_has_enemy = true;
					return;
				}

				_has_enemy = false;
		}

		void Update::run()
		{
			output( "��ʼ���" );
			clear_room();
			output( "������" );

			if( !is_last_room() )
			{
				output( "�뿪����" );
				out_room();
			}
		}

		bool Update::is_last_room()
		{
			Pos last_room_pos =
				_current_map[ _current_map.get_shortest_path_vd().back() ];

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_map_win_pos();

			if( last_room_pos != _world.map_pos )
				return false;
			else
				return true;
		}

		void Update::moveto_next_pos()
		{

			_out_timer.restart();
			_sec_out_timer.restart();

			while( true )
			{
				//	���û��ֹͣ�ƶ���
				//	�ƶ�,�������ó�ʱ��ʱ��
				//	��������ʱ.

				//_world.moveto( *_next_pos_ptr );

				if( !_DNF::Sync::is_stop_move() )
					_world.moveto( *_next_pos_ptr );
				else
				{
					glog( "moveto_next_pos():_DNF::Sync::is_stop_move" );
					_out_timer.restart();
					_sec_out_timer.restart();
				}

				//trace( "end_world_moveto" );

				if( is_at_next_pos() )
				{
					//trace( "at_next_pos" );
					glog( "moveto_next_pos():is_at_next_pos" );
					break;
				}

				//assert( is_right_state()
				//	&& "Update::moveto_next_pos:��Ϸ״̬����ȷ" );

				if( _out_timer.elapsed() > _time_limit )
				{
					attack_near_block();
					_out_timer.restart();
				}

				assert( _sec_out_timer.elapsed() < 180
					&& "Update::moveto:3���ӻ�û�е���ָ���ĵص�" );

				Sleep(1);
			}

			glog( "moveto_next_pos():move end" );
			//trace( "move end" );
		}

		void Update::moveto_next_pos_and_wait_clear()
		{
			glog( "Update::moveto_next_pos_and_wait_clear():start" );
			_out_timer.restart();
			_sec_out_timer.restart();

			while( true )
			{
				//_world.moveto( *_next_pos_ptr );
				if( !_DNF::Sync::is_stop_move() )
					_world.moveto( *_next_pos_ptr );
				else
				{
					_out_timer.restart();
					_sec_out_timer.restart();
				}

				//trace( "end_world_moveto" );

				if( is_at_next_pos() )
				{
					//trace( "at_next_pos" );
					_out_timer.restart();
					break;
				}

				//assert( is_right_state()
				//	&& "Update::moveto_next_pos_and_wait_clear:��Ϸ״̬����ȷ" );

				if( _out_timer.elapsed() > _time_limit )
				{
					attack_near_block();
					_out_timer.restart();
				}

				assert( _sec_out_timer.elapsed() < 180
					&& "Update::moveto:3���ӻ�û�е���ָ���ĵص�" );

				Sleep(1);
			}

			glog( "Update::moveto_next_pos_and_wait_clear():wait_clear" );

			while( true )
			{
				_out_timer.restart();
				if( is_clear_window() )
					break;
				Sleep( 1 );
			}

			glog( "Update::moveto_next_pos_and_wait_clear():end" );
		}

		void Update::moveto_next_door()
		{
			glog( "Update::moveto_next_door():start" );
			_out_timer.restart();
			_sec_out_timer.restart();

			while( true )
			{
//				_world.moveto( *_next_pos_ptr );

				if( !_DNF::Sync::is_stop_move() )
					_world.moveto( *_next_pos_ptr );
				else
				{
					_out_timer.restart();
					_sec_out_timer.restart();
				}

				//trace( "end_world_moveto" );

				if( is_at_next_pos() )
				{
					//trace( "at_next_pos" );
					break;
				}

				if( is_change_room() )
				{
					//trace( "is_change_room" );
					break;
				}

				if( !is_right_state() )
				{
					//trace( "is_not_right_state" );
					break;
				}

				if( _out_timer.elapsed() > _time_limit )
				{
					attack_near_block();
					_out_timer.restart();

					assert( _sec_out_timer.elapsed() < 180
						&& "Update::moveto:3���ӻ�û�е���ָ���ĵص�" );
				}

				Sleep(1);
			}

			glog( "Update::moveto_next_door():start2" );

			_out_timer.restart();
			while( true )
			{
				if( !is_right_state() )
					break;
				if( is_change_room() )
					break;

				if( _out_timer.elapsed() > _time_limit )
				{
					if( !is_clear_room() )
					{
						clear_room();

						if( !is_last_room() )
							out_room();
					}

//					_out_timer.restart();
				}


				if( _out_timer.elapsed() > _time_limit )
				{
					_world.player.A( 300 );
					_world.player.D( 300 );
					_world.player.W( 300 );
					_world.player.S( 300 );
					_world.player.D( 300 );
					_world.player.A( 300 );
					_world.player.S( 300 );
					_world.player.W( 300 );

					_world.moveto( *_next_pos_ptr );

					_out_timer.restart();
				}
				Sleep(1);
			}

			glog( "Update::moveto_next_door():end" );
		}

		void Update::attack_near_block()
		{
			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_player_map_pos();
			_world.player.map_pos.corrent_assert();
			_world.player.map_pos.init_assert();

			_world.update_player_win_pos();
			
			_world.update_target_group_map_pos();

			vector<Pos> near_block_list;
			vector<Pos> near_block_map_pos_list
				= _current_room._destroy_block.near_pos( 
				_world.player.map_pos, _world.target_group );

			for( size_t i = 0; i < near_block_map_pos_list.size(); i++ )
				near_block_list.push_back(
				_world.get_win_pos_by_player( 
					near_block_map_pos_list[ i ] )
					);

			//Pos block_pos =  _current_room._destroy_block.nearest_pos( 
			//	_world.map_pos );

			//block_pos = _world.get_win_pos_by_player( block_pos );

			for( size_t j = 0; j < 5; j++ )
				for( size_t i = 0; i < near_block_list.size(); i++ )
				{
					_world.player.LB( near_block_list[ i ] );
					Sleep( 5 );
				}
		}

		void Update::clear_room()
		{
			glog( "goto_nearest_pos" );
			goto_nearest_pos();

			_error_545 = false;

			glog( "hide_enemy_restart" );
			hide_enemy_restart();
			while( true )
			{
				glog( "hide_enemy()" );
				hide_enemy();

				if( is_clear_window() )
				{
					goto_nearest_pos();

					while( true )
					{
						if( !has_left_enemy() )
							break;
						glog( "clear_left_window()" );
						clear_left_window();
						Sleep(_sleep_time);
					}

					while( true )
					{
						if( !has_right_enemy() )
							break;
						glog( "clear_right_window" );
						clear_right_window();
						Sleep(_sleep_time);
					}
				}

				if( is_clear_room() )
				{
					glog( "is_clear_room" );
					break;
				}

				Sleep(_sleep_time);
			}
		}

		bool Update::is_clear_room()
		{

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			if( is_dungeon_score()
					|| is_dungeon_end() )
			{
				//trace( "is_clear_room:is_dungeon_score" );
				glog( "Update::is_clear_room():is_dungeon_score||is_dungeon_end" );
				return true;
			}

			_world.update_target_group_map_pos();

			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id
					|| ( 545 == _world.target_group[ i ].id
					&& !_error_545 ) )
					return false;

			return true;
		}

		void Update::goto_nearest_pos()
		{
			{
				boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

				_world.update_player_map_pos();

				*_next_pos_ptr = _current_room._node_mgr.nearest_record_node( 
					_world.player.map_pos ).pos;
			}

			moveto_next_pos();
		}

		bool Update::is_clear_window()
		{
		//	if( _world.is_dungeon_score() )
			if( is_in_boss_room() && _DUNGEON::Sync::is_find_score() )
			{
				_world.update_boss_room_win_pos();
				if( _world.boss_room_pos.is_invaild() )
				{
					trace( "is_find_score" );
					//cout << "is_find_score" << endl;
					_is_dungeon_score = true;
					return true;
				}
			}
			if( is_in_boss_room() && _world.is_dungeon_reward() )
			{
				trace( "_world._is_dungeon_reward()" );
				cout << "is_find_reward" << endl;
				_is_dungeon_score = true;
				return true;
			}
			if(  is_in_boss_room() && _world.is_dungeon_end() )
			{
				_is_dungeon_end = true;
				return true;
			}

			_update_enemy();


			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id
					|| ( 545 == _world.target_group[ i ].id
					&& !_error_545 )
					)
					if( _in_window( _world.target_group[ i ].win_pos ) )
						return false;


			return true;
		}

		void Update::hide_enemy_restart()
		{
			//	ȥ��ע�ʹ򿪶��
			if( special_case_hide_enemy() )
				_hide_timer.restart();
		}

		void Update::hide_enemy()
		{
			if( has_auto_hide_enemy() )
			{
				size_t x_range = 150;
				size_t y_range = 5;
				size_t step_time = 3000;
				get_auto_hide_enemy_agrv(
					x_range,
					y_range,
					step_time
					);

				glog( "�����Զ����" );
				glog( x_range );
				glog( y_range );
				glog( step_time );

				auto_hide_enemy( 
					x_range,
					y_range,
					step_time
					);
			}
			else if( is_in_boss_room() )
				auto_hide_enemy();
			else if( special_case_hide_enemy() )
				if( _hide_timer.elapsed() > 
					special_case_hide_enemy_time()
					)
				{
					if( !_hide_enemy_up )
					{
						_world.dnf.s_down();
						Sleep( 2000 );
						_world.dnf.s_up();
						_hide_enemy_up = true;
					}
					else
					{
						_world.dnf.w_down();
						Sleep( 2000 );
						_world.dnf.w_up();
						_hide_enemy_up = false;
					}
				}
		}

		void Update::auto_hide_enemy(
				size_t x_range,
				size_t y_range,
				size_t step_time
				)
		{
			{
				boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );
				_world.update_player_map_pos();
				_world.player.map_pos.corrent_assert();
				_world.player.map_pos.init_assert();
				_world.update_target_group_map_pos();
			}

			bool is_safe = true;
			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id )
					if( std::abs( _world.player.map_pos.x - _world.target_group[ i ].map_pos.x ) < x_range
						&& std::abs( _world.player.map_pos.y - _world.target_group[ i ].map_pos.y ) < y_range )
						is_safe = false;

			if( !is_safe )
			{
				if( !_hide_enemy_up )
				{
					_world.dnf.s_down();
					Sleep( step_time );
					_world.dnf.s_up();
					_hide_enemy_up = true;
				}
				else
				{
					_world.dnf.w_down();
					Sleep( step_time );
					_world.dnf.w_up();
					_hide_enemy_up = false;
				}
			}
		}

		bool Update::has_left_enemy()
		{
			
			if( is_dungeon_score()
					|| is_dungeon_end() )
			{
				//trace( "has_left_enemy:is_dungeon_score" );
				return false;
			}

			_update_enemy();

			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id
					|| ( 545 == _world.target_group[ i ].id
					&& !_error_545 ) )
					if( _left_window( _world.target_group[ i ].win_pos ) )
						return true;

			return false;
		}

		void Update::clear_left_window()
		{
			goto_nearest_pos();
			glog( "Update::clear_left_window():goto_left_pos" );
			goto_left_pos();


			hide_enemy_restart();
			while( true )
			{
				glog( "hide_enemy_left" );
				hide_enemy();

				if( is_clear_window() )
				{
					glog( "Update::clear_left_window()" );
					break;
				}

				Sleep( _sleep_time );
			}
		}

		bool Update::has_right_enemy()
		{
			if( is_dungeon_score()
					|| is_dungeon_end() )
			{
				//trace( "has_right_enemy:is_dungeon_score" );
				return false;
			}

			_update_enemy();

			for( size_t i = 0; i < _world.target_group.size(); i++ )
				if( 529 == _world.target_group[ i ].id
					|| ( 545 == _world.target_group[ i ].id
					&& !_error_545 ) ) 
					if( _right_window( _world.target_group[ i ].win_pos ) )
						return true;

			return false;
		}

		void Update::clear_right_window()
		{
			goto_nearest_pos();
			glog( "Update::clear_right_window():goto_right_pos" );
			goto_right_pos();

			hide_enemy_restart();
			while( true )
			{
				glog( "hide_enemy_right" );
				hide_enemy();
				if( is_clear_window() )
				{
					glog( "Update::clear_right_window():is_clear_window" );
					break;
				}

				Sleep( _sleep_time );
			}
		}

		void Update::goto_left_pos()
		{
			if( !set_left_path() )
			{
				_error_545 = true;
				return;
			}

			assert( _room_path.size() > 1 
				&& "Update::goto_left_pos:_room_path���ô���,������ȱ�ٶ�Ӧ�ļ�¼��" );

			for( size_t i = 1; i < _room_path.size(); i++ )
			{
				*_next_pos_ptr = _room_path[ i ];

				//moveto_next_pos();
				moveto_next_pos_and_wait_clear();
			}
		}

		bool Update::set_left_path()
		{	

			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_room_path_index = 0;

			_world.update_player_map_pos();

			_world.player.map_pos.corrent_assert();
			_world.player.map_pos.init_assert();

			_room_path = _current_room._node_mgr.nearest_record_left_pos(
				_world.player.map_pos );

			if( _room_path.empty() )
				return false;

			return true;
		}

		void Update::goto_right_pos()
		{
			if( !set_right_path() )
			{
				_error_545 = true;
				return;
			}

			assert( _room_path.size() > 1 
				&& "Update::goto_right_pos:_room_path���ô���,������ȱ�ٶ�Ӧ�ļ�¼��" );

			for( size_t i = 1; i < _room_path.size(); i++ )
			{
				*_next_pos_ptr = _room_path[ i ];

				//moveto_next_pos();
				moveto_next_pos_and_wait_clear();
			}
		}

		bool Update::set_right_path()
		{
			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_room_path_index = 0;

			_world.update_player_map_pos();

			_world.player.map_pos.corrent_assert();
			_world.player.map_pos.init_assert();

			_room_path = _current_room._node_mgr.nearest_record_right_pos(
				_world.player.map_pos );

			if( _room_path.empty() )
				return false;

			return true;
		}

		DestroyBlock& Update::get_current_destroy_block()
		{
			return _current_room._destroy_block;
		}

		void Update::_update_enemy()
		{
			boost::recursive_mutex::scoped_lock lock( _DUNGEON::Sync::get_mutex() );

			_world.update_player_map_pos();
			_world.player.map_pos.corrent_assert();
			_world.player.map_pos.init_assert();

			_world.update_player_win_pos();

			_world.update_target_group_map_pos();
			_world.update_target_group_win_pos();
		}

		bool Update::_left_window( const Pos& pos )
		{
			if( pos.x <= 10 )
				return true;
			else
				return false;
		}

		bool Update::_right_window( const Pos& pos )
		{
			if( pos.x >= 790 )
				return true;
			else
				return false;
		}

		bool Update::_in_window( const Pos& pos )
		{
			if( pos.x > 10
				&& pos.x < 790 )
				return true;
			else
				return false;
		}

		bool Update::is_dungeon_score()
		{
			return _is_dungeon_score;
		}

		bool Update::is_dungeon_end()
		{
			return _is_dungeon_end;
		}	
		bool Update::is_in_boss_room()
		{
			return _is_in_boss_room;
		}

		void Update::out_room()
		{
			glog( "out_room" );
			unset_room_shortest();

			glog( "_world.update_boss_room_win_pos();" );
			_world.update_boss_room_win_pos();//_block( 2, 100, false );


			if( _world.boss_room_pos.is_invaild() )
				_is_dungeon_score = true;
			//_DUNGEON::Sync::set_stop_atk();
			if( _is_dungeon_score
			|| _is_dungeon_end )
				return;

			
			//	��ת�ƺ��޸�Ϊ���ȴ�
			//if( _world.player.get_level() < 3 )
			//		Sleep( 8000 );
			glog( "set_room_shortest();" );

			set_room_shortest();

			for( size_t i = 0; i < _room_shortest.size(); i++ )
			{
				*_next_pos_ptr = _room_shortest[ i ];

				if( _room_shortest.size() - 1 == i )
				{
					glog( "moveto_next_door()" );
					moveto_next_door();
				}
				else
				{
					glog( "moveto_next_pos()" );
					moveto_next_pos();
				}
			}

			//trace( "out_room:end" );
		}

		void Update::handle_special_case()
		{
			using namespace _SPECIAL;
			Config& ref_cfg = Special::get_cfg();
			if( ref_cfg.has_case( CaseName::TeLeiZeGai ) )
			{
				string rcfg_file_id, room_file_id;

				rcfg_file_id = ref_cfg.get_case_filename( 
					ref_cfg.get_case_index( CaseName::TeLeiZeGai )
					);

				room_file_id = _current_room.get_filename();

				size_t pos;
				pos = rcfg_file_id.find_last_of( '\\' );
				rcfg_file_id = rcfg_file_id.substr( pos+1, rcfg_file_id.size() - pos );
				pos = rcfg_file_id.find_last_of( '.' );
				rcfg_file_id = rcfg_file_id.substr( 0, pos );
				
				pos = room_file_id.find_last_of( '\\' );
				room_file_id = room_file_id.substr( pos+1, room_file_id.size() - pos );
				pos = room_file_id.find_last_of( '.' );
				room_file_id = room_file_id.substr( 0, pos );

				//trace( rcfg_file_id );
				//trace( room_file_id );

				if( rcfg_file_id == room_file_id )
				{
					//trace( " rcfg_file_id == room_file_id " );

					CaseInfo case_info;
					case_info.name = CaseName::TeLeiZeGai_InRoom;
					ref_cfg.add_case( case_info );
				}

				if( ref_cfg.has_case( CaseName::TeLeiZeGai_InRoom ) )
					if( rcfg_file_id != room_file_id )
					{
						ref_cfg.del_case( 
							CaseName::TeLeiZeGai_InRoom
							);
						ref_cfg.del_case(
							CaseName::TeLeiZeGai
							);
					}
			}
		}

		void Update::handle_special_case_YingXiongZhong()
		{
			using namespace _SPECIAL;
			Config& ref_cfg = Special::get_cfg();
			if( ref_cfg.has_case( CaseName::YingXiongZhong ) )
				_current_room._rfp.set_comp_scale( 0.1 );
		}

		bool Update::special_case_hide_enemy()
		{
			using namespace _SPECIAL;
			Config& ref_cfg = Special::get_cfg();
			if( ref_cfg.has_case( CaseName::OpenHideEnemy ) )
				return true;
			else
				return false;
		}

		size_t Update::special_case_hide_enemy_time()
		{
			using namespace _SPECIAL;
			Config& ref_cfg = Special::get_cfg();
			assert(  ref_cfg.has_case( CaseName::OpenHideEnemy ) 
				&& "Update::special_case_hide_enemy_time():has_no_case_OpenHideEnemy" );

			CaseData case_data = ref_cfg.get_case_data( 
				ref_cfg.get_case_index(
					CaseName::OpenHideEnemy
					)
				);

			if( case_data.empty() )
			{
				//trace( "Update::special_case_hide_enemy_time():10" );
				return 10;
			}
			else
			{
				//trace( "Update::special_case_hide_enemy_time()" );
				//trace( "case_data[ 0 ]:" );
				//trace( case_data[ 0 ] );
				return case_data[ 0 ];
			}
		}

		bool Update::has_auto_hide_enemy()
		{
			if( Special::has_current_case(
				CaseName::OpenAutoHideEnemy )
				)
				return true;
			else
				return false;
		}
		bool Update::get_auto_hide_enemy_agrv(
				size_t& x_range,
				size_t& y_range,
				size_t& step_time
				)
		{
			_SPECIAL::CaseData case_data = 
				Special::get_current_case_data(
				CaseName::OpenAutoHideEnemy );

			if( case_data.empty() )
				return false;
			else if( 1 == case_data.size() )
			{
				x_range = case_data[ 0 ];
				return true;
			}
			else if( 2 == case_data.size() )
			{
				x_range = case_data[ 0 ];
				y_range = case_data[ 1 ];
				return true;
			}
			else if( 3 == case_data.size() )
			{
				x_range = case_data[ 0 ];
				y_range = case_data[ 1 ];
				step_time = case_data[ 2 ];

				return true;
			}
			else
				return false;
		}


		////	�ָ����ܶ�ʧ
		////	�򿪼�����.��Q,E,R
		////	�ϵ���ԭ����λ����

		//RestoreSkill::RestoreSkill( World& world_ )
		//	:_world( world_ )
		//{
		//}

		//void RestoreSkill::restore()
		//{
		//	output( "�ָ�����" );

		//	_world.open_skill();

		//	restore_one( 
		//		q_pos_in_study,
		//		q_pos_in_set
		//		);

		//	restore_one( 
		//		e_pos_in_study,
		//		e_pos_in_set
		//		);

		//	restore_one( 
		//		r_pos_in_study,
		//		r_pos_in_set
		//		);

		//	_world.close_skill();

		//}

		//void RestoreSkill::restore_one( 
		//		const Pos& study_pos,
		//		const Pos& set_pos
		//		)
		//{
		//	Sleep( 10 );
		//	_world.dnf.dm.MoveTo( 
		//		study_pos.x,
		//		study_pos.y
		//		);
		//	_world.dnf.dm.LeftDown();
		//	Sleep( 10 );
		//	_world.dnf.dm.MoveTo(
		//		study_pos.x + 10,
		//		study_pos.y + 10 
		//		);
		//	Sleep( 10 );

		//	_drag_mouse( 
		//		study_pos,
		//		set_pos 
		//		);

		//	Sleep( 10 );

		//	_world.dnf.dm.MoveTo(
		//		set_pos.x,
		//		set_pos.y
		//		);
		//	_world.dnf.dm.LeftUp();
		//}

		//void RestoreSkill::_drag_mouse(
		//		const Pos& start_pos,
		//		const Pos& end_pos
		//	)
		//{
		//	_world.dnf.dm.MoveTo(
		//		start_pos.x,
		//		start_pos.y
		//		);

		//	//	�������ʼ�ͽ�����ƫ��
		//	int offset = compute_distance( start_pos, end_pos );

		//	//	������Ҫ�ƶ��Ĵ���
		//	//	200Ϊÿ���ƶ��ľ���
		//	size_t move_times = offset / 100;
		//	
		//	if( move_times )
		//	{
		//		Pos offset_pos;
		//		offset_pos.x = end_pos.x - start_pos.x;
		//		offset_pos.y = end_pos.y - start_pos.y;

		//		Pos move_pos;
		//		move_pos.x = offset_pos.x / move_times;
		//		move_pos.y = offset_pos.y / move_times;

		//		for( size_t i = 0; i < move_times; i++ )
		//		{
		//			_world.dnf.dm.MoveTo(
		//				start_pos.x + move_pos.x * i,
		//				start_pos.y + move_pos.y * i
		//				);
		//			Sleep( 10 );
		//		}
		//	}

		//	_world.dnf.dm.MoveTo(
		//		end_pos.x,
		//		end_pos.y
		//		);

		//}
		//
		//Pos RestoreSkill::q_pos_in_study = Pos( 108,157 );
		//Pos RestoreSkill::q_pos_in_set = Pos( 588,521 );
		//Pos RestoreSkill::e_pos_in_study = Pos( 205,154 );
		//Pos RestoreSkill::e_pos_in_set = Pos( 635,513 );
		//Pos RestoreSkill::r_pos_in_study = Pos( 300,156 );
		//Pos RestoreSkill::r_pos_in_set = Pos( 680,518 );


		///////////////////////////
		//	�Զ��ƶ�
		Move::Move( World& world_ )
			:_world( world_ ),
			_load( world_ ),
			_update( world_ ),
			_is_just_into_dungeon( true )//,
			//_restore_skill( world_ )
		{}

		void Move::init()
		{
			_load.set_exist_map_list_ptr( _exist_map_list );
			_load.set_exist_room_list_ptr( _exist_room_list );
			_update.set_exist_map_list_ptr( _exist_map_list );
			_update.set_exist_room_list_ptr( _exist_room_list );
			_update.set_next_pos_ptr( _next_pos );

			_update.init();
			_is_just_into_dungeon = true;

		}
		
		void Move::operator()()
		{
			output( "��ʼ�����³���Ϣ" );
			init();

			_SPECIAL::Special::get_cfg().clear();

			while( true )
			{
				//glog( "move_start" );

				_DUNGEON::Sync::set_stop_atk();

				bool ret_right_state = _update.is_right_state();

				if( ret_right_state
					&& _update.is_corrent_map()
					&& 
					( 
					_update.map_name_is_changed()
					|| _is_just_into_dungeon
					)
					)
				{
					_DUNGEON::Sync::unset_find_score();

					output( "�����ͼ�ṹ" );
					_load.set_map_name_dir();

					_out_timer.restart();
					while( true )
					{
						if( _load.load_exist_map() )
							break;

						if( _out_timer.elapsed() > 10 )
							throw std::exception( "Move::operator:�����ڵ�ͼ�ṹ�ļ�" );

						Sleep(1);
					}

					output( "���뷿��ṹ" );
					_out_timer.restart();
					while( true )
					{
						if( _load.load_exist_room() )
							break;

						if( _out_timer.elapsed() > 10 )
							throw std::exception( "Move::operator:û����ȷ���뷿���ļ�" );

						Sleep(1);
					}

					output( "���õ�ǰ��ͼ" );

					_update.set_current_map();
					output( "���õ�ͼ·��" );
					_update.set_map_shortest();
					output( "���µ�ͼ����" );
					_update.update_map_name();

					
					_is_just_into_dungeon = false;

					_world.dnf.dm.MoveTo( 80, 80 );
					output( "��鸺��" );
					_world.open_equip();
					Sleep( 500 );
					if( _world.is_out_weight() )
					{
						trace( "_DUNGEON::Sync::set_stop_pick_up()" );
						_DUNGEON::Sync::set_stop_pick_up();
					}
					else
					{
						trace( "_DUNGEON::Sync::unset_stop_pick_up()" );
						_DUNGEON::Sync::unset_stop_pick_up();
					}
					_world.close_equip();

					//	�ѻָ����ܸ�Ϊ�ڽ���ͼ֮ǰ�ָ�.
					//_restore_skill.restore();
				}

				if( _update.is_dungeon_score()
					|| _update.is_dungeon_end() )
				{
					output( "���³Ǵ�����" );
					trace( "dungeon_end" );
					//cout << "dungeon end" << endl;
					trace( _update.is_dungeon_score() );
					trace( _update.is_dungeon_end() );
					_DUNGEON::Sync::set_stop_atk();
					break;
				}


				if( ret_right_state
					&& _update.is_change_room() )
				{

					//glog( "move_join_room" );
					output( "�����·���" );
					Message::pluse( 60, 2 );

					trace( "���뷿��" );

					_update.set_rfp();

					trace( "��ȡ��������" );
					output( "�����·�����Ϣ" );
					if( _update.is_in_exist_room_list() )
					{
						_update.set_best_sim_room();

						_update.handle_special_case();

						_DUNGEON::Sync::set_block( 
							_update.get_current_destroy_block() );

						//_update.set_room_shortest();
						//_update.unset_room_shortest();
						//_update.set_next_pos();
					}
					else
					{
						//assert( false
						//	&& "Move::operator()::����δ��¼���·���" );

						trace( "Move::operator()::����δ��¼���·���" );

						_DNF::Sync::set_return_town();

						while( true )
							Sleep( 1000 );
					}

					//if( Special::has_current_case( CaseName::Test ) )
					//{
					//	ostringstream os;
					//	os << "-------------------------" << endl;
					//	os << "�ҵ���ǰ���������:" << 
					//		CaseName::Test << endl;

					//	for( size_t i = 0; i < Special::get_current_case_data(
					//		CaseName::Test ).size(); i++ )
					//		os << "����"<< i << "Ϊ:" << 
					//			Special::get_current_case_data(
					//				CaseName::Test )[ i ] << endl;
					//	os << "-------------------------";
					//	glog( os.str() );
					//}


					_update.update_last_map_pos();

					_DUNGEON::Sync::set_nonstop_atk();
					output( "���·�����Ϣ" );
					_update.run();
					//trace( "run:end" );
				}

				Sleep( 1000 );
				//trace( "loop:end" );

				//glog( "move_end" );
			}
		}


_MOVE_END

	//}	//	namespace move
//}	//	namespace dnf