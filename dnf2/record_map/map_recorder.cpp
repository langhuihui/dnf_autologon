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
					"�Ҳ���С��ͼ���꣬�����Ǳ��ڵ�����û��С��ͼ��\n�ָ���ͼ����ȷ��������",
					"����",
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
				&& "Input::start_record:״̬����" );

			cout << "==============================" << endl;
			cout << "����s��ʼ��¼��ͼ�ṹ:" << endl;
			string str;
			cin >> str; 
			if( "s" == str )
			{
				cout << "�����ͼ��:" << endl;
				cin >> _map_name;

				check_map_open();

				_is_start = true;

				cout << "��ʼ��¼..." << endl;
				update_state();
			}
		}

		void Input::end_record()
		{
			assert( _is_start
				&& !_is_end_record
				&& !_is_finish
				&& !_is_restart
				&& "Input::end_record:״̬����" );

			_is_end_record = true;

			cout << "������¼,�ȴ����ݴ���..." << endl;

			while( true )
			{
				if( _is_finish )
				{
					cout << "���ݴ������,���μ�¼���." << endl;
					break;
				}

				if( _is_restart )
				{
					cout << "�����ж�,���¿�ʼ." << endl;
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
				&& "Input::updata_state:״̬����" );

#ifdef _DEBUG
			size_t debug_cnt = 0;
#endif 

			//	�����2��pos��������¼
			//	�ϴ���Ч��ֵ��
			//	������ô��.����Ϊ��Ϸ��
			//	������map_pos���ջ������boss_room_pos
			//	���Ե���boss�����.boss_room_pos�������
			//	����map_pos���ջ���µ�һ���µ�ֵ
			//	���ֵ��boss_room_pos�����ֵ���
			//	--------------------------------
			//	6/21(��ת��)�޸�
			//	���ﵽ��boss�����.boss_room_pos������
			//	��ʧ,��map_pos.����ʧ
			//	�����Ļ�map_pos��ֵ��Զ�����boss_room_pos
			//	���,����map_pos��boss_room_pos�ľ���
			//	ֻ��һ������ľ���,����_world.map_posΪ��Ч
			//	(-1,-1)����_world.boss_room_pos����Ч��.
			//	��ô������˵�������Ѿ�������boss����.
			//	���������Ļ��������npc�ĹⲨ�Ļ�.����
			//	_world.map_pos��Ч.
			//	....��ʱ�벻���õķ���
			//	��������������,������¼�¼
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

				//	20�����Ҳ�������
				if( debug_cnt > 2000 )
				{
					assert( false 
						&& "Input::updata_state:����Ҳ������꣬�����ǵ�ͼû��" );

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
				cout << "��¼�ж�,���¿�ʼ." << endl;
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
				&& "Input::set_finish:״̬����" );

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
				&& "Input::restart_record:״̬����" );

			_is_start = false;
			_is_restart = true;
		}

		string Input::get_map_name()
		{
			assert( !_map_name.empty()
				&& "Input::get_map_name:_map_name��ͼ��Ϊ��" );

			return _map_name;
		}


		//////////////////////////////////
		//	��¼����������õ���Ϣ
		//	���뵽ͼ��
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
				&& "Record::start_record:_is_end����ʱΪ��,�߼��쳣" );

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
				&& "Record::start_record:_is_end_record����ʱΪ��,�߼��쳣" );

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
				&& "Record::end_record:_is_end_recordΪ��,�߼��쳣" );

			_is_end = true;
		}
		
		void Record::update_map_pos()
		{
			assert( _map_ptr 
				&& "Record::update_map_pos:_map_ptrΪ��ָ��" );


			_world.update_map_win_pos();

#ifdef _DEBUG

			if( -1 == _world.map_pos.x
				&& -1 == _world.map_pos.y )
				_debug_cnt++;
			else
				_debug_cnt = 0;

			//	���20���ڶ��޷��ҵ���ͼ����
			//	������û�򿪵�ͼ
			if( _debug_cnt > 2000 )
			{
				assert( false
				&& "Record::update_map_pos:_debug_cnt��ʱ�������ǵ�ͼû��" );

				_debug_cnt = 0;
			}

#endif
			//	������£����� _world.map_pos����û�ҵ�
			//	���� _world.map_pos����δ��ʼ��
			if( _world.map_pos != _last_map_pos
				&& _world.map_pos != Pos()
				&& -1 != _world.map_pos.x
				&& -1 != _world.map_pos.y
				)
			{
				//	����ڵ�
				if( !_map_ptr->has_vertex( _world.map_pos ) )
				{
					_map_ptr->add_vertex( _world.map_pos );
					cout << "����ڵ�:" 
						<< _world.map_pos.x << ","
						<< _world.map_pos.y << endl;
				}

				//	��� _last_map_pos δ��ʼ��
				if( _last_map_pos != Pos() )
				{
					//	���Խڵ��Ⱥ���������
					//	�ڴ�ֱ����ˮƽ�������
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
						&& "update_map_pos:�ڵ��Ⱥ���������\
						   �ڴ�ֱ����ˮƽ����û�ж���" );

					
					assert( _map_ptr->has_vertex( _last_map_pos )
						&& _map_ptr->has_vertex( _world.map_pos )
						&& "update_map_pos:Ҫ����ߵĽڵ㲻����" );


					//	�����
					//	��Ϊ��������������Լ��������
					//	��������û��Ҫ��¼����


					Road road;
					road.dir = Dir::no;

					//	ͬʱ������������ı�							
					if( !_map_ptr->has_edge( 
							_last_map_pos,
							_world.map_pos,
							road ) )
					{
						_map_ptr->add_edge( 
							_last_map_pos,
							_world.map_pos,
							road );
					
						cout << "�����:"
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

						cout << "�����:"
							<< _world.map_pos.x << ","
							<< _world.map_pos.y << "->"
							<< _last_map_pos.x << ","
							<< _last_map_pos.y << endl;
					}
				}


				_last_map_pos = _world.map_pos;
			}

			//	6/21(��ת�Ƹ���)
			//	��Ϊ����boss���䲻����ʾ
			//	room_pos����boss_room_pos
			//	����һ��_room_pos��Ч,����
			//	boss_room_pos��Ч,����
			//	_last_map_pos��boss_room_pos
			//	�ľ���Ϊһ������ľ���
			//	���boss_room_pos���뵽
			//	ͼ��

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

					//	����ڵ�
					if( !_map_ptr->has_vertex( _world.boss_room_pos ) )
					{
						_map_ptr->add_vertex( _world.boss_room_pos );	
						cout << "����boss����ڵ�:" 
							<< _world.boss_room_pos.x << ","
							<< _world.boss_room_pos.y << endl;
					}

					assert( 
						_world.boss_room_pos.x == _last_map_pos.x
						|| _world.boss_room_pos.y == _last_map_pos.y
						&& "update_boss_room_pos:�ڵ��Ⱥ���������\
						   �ڴ�ֱ����ˮƽ����û�ж���" );


					assert( _map_ptr->has_vertex( _last_map_pos )
						&& _map_ptr->has_vertex( _world.boss_room_pos )
						&& "update_boss_room_pos:Ҫ����ߵĽڵ㲻����" );

					Road road;
					road.dir = Dir::no;

					//	ͬʱ������������ı�							
					if( !_map_ptr->has_edge( 
						_last_map_pos,
						_world.boss_room_pos,
						road ) )
					{
						_map_ptr->add_edge( 
						_last_map_pos,
						_world.boss_room_pos,
						road );
						cout << "�����:"
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
						cout << "�����:"
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
				&& "Record::set_map_ptr:mapΪ��ָ��" );

			assert( map.empty()
				&& "Record::set_map_ptr:map��Ϊ�գ��߼�����" );

			_map_ptr = &map;
		}
		
		/////////////////////////////////
		AutoFile::AutoFile(){}

		void AutoFile::set_work_dir( const string& work_dir )
		{
			assert( !work_dir.empty()
				&& "AutoNumFile::set_work_dir: work_dirΪ��" );

			assert( bfs::is_directory( work_dir )
				&& "AutoNumFile::set_work_dir: work_dir����һ��Ŀ¼" );

			_work_dir = work_dir;
		}


		bool AutoFile::load_file_list( vector<string>& file_list, const string& file_ext )
		{
			assert( !file_ext.empty()
				&& "AutoNumFile::load_file_list:_file_extΪ��" );

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
						&& "AutoNumFile::load_file_list:cut�쳣" );

					string file_main_name = 
						it->path().filename().string().substr( 0, pos );

					assert( 
						check_only_letter( "0123456789abcdef",
						file_main_name )
						&& file_main_name.size() == 8
						&& "AutoNumFile::load_file_list:�ļ����д��ڷǷ���ʽ���ļ�" );
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
				&& "AutoNumFile::gen_save_file_name:_file_extΪ��" );

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
				&& " Store::set_map_name_dir:_map_dir����Ϊ��" );

			assert( !map_name.empty()
				&& " Store::set_map_name_dir:��ͼ����Ϊ��" );

			assert( check_no_letter( "-=!@#$%^&*()_+\\\",.;", map_name )
				&& " Store::set_map_name_dir:��ͼ�����к��в���ȷ���ַ�" );

			_map_name_dir = _map_dir + "\\" + map_name;

			assert( _map_name_dir.size() > ( map_name.size() + 1 )
				&& " Store::set_map_name_dir:_map_dir���ֲ���ȷ" );

		}

		void Store::set_exist_map_list_ptr( vector<Map>& exist_map_list )
		{
			assert( &exist_map_list
				&& " Store::set_exist_map_list_ptr:exist_map_listָ��0" );
			_exist_map_list_ptr = &exist_map_list;
		}

		void Store::set_save_map_ptr( Map& save_map )
		{
			assert( &save_map
				&& "Store::set_save_map_ptrָ��0" );

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
				&& "Store::load_map:_exist_map_list_ptr�쳣" );

			assert( _exist_map_list_ptr->empty()
				&& "Store::load_map:_exist_map_list_ptr��Ϊ��" );

			_exist_map_list_ptr->clear();


			for( size_t i = 0; i < _file_list.size(); i++ )
			{
				//	���ﲻ�ܰѳ�ʼ�����ں���
				//	����map��������Լ�
				//	���ǵ����ڵ㡣
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
				&& "Store::save_map:save_map_ptrΪ��ָ��" );

			if( "" != map_name )
				set_map_name_dir( map_name );

			if( !bfs::exists( _map_name_dir ) )
				bfs::create_directory( _map_name_dir );

			string save_dir = _save_map_ptr->get_file_name( _map_ext );
		
			save_dir = _map_name_dir + "\\" + save_dir;
			
			_save_map_ptr->save( save_dir );

			assert( bfs::exists( save_dir )
				&& "Store::save_map:�����ļ�ʧ��" );

			assert( !bfs::is_empty( save_dir )
				&& "Store::save_map:����ʧ�ܣ��ļ�Ϊ��" );
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
				&& "Store::save_map_png:����dot�ļ�ʧ��" );

			assert( !bfs::is_empty( save_dot_dir )
				&& "Store::save_map_png:����dot�ļ�ʧ�ܣ��ļ�Ϊ��" );

			string cmd 
				= "\"\"graphviz-2.36\\release\\bin\\dot.exe\" -Kfdp -n -Tpng \""
				+ save_dot_dir + "\" -o \"" + save_png_dir + "\"\"";

			::system( cmd.c_str() );

			assert( bfs::exists( save_png_dir )
				&& "Store::save_map_png:����png�ļ�ʧ��" );

			assert( !bfs::is_empty( save_dot_dir )
				&& "Store::save_map_png:����png�ļ�ʧ�ܣ��ļ�Ϊ��" );
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
				
				//	�����ȴ������ͼ����ʼ
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
					cout << "--��ͼ�Ѿ�����,���õ�ͼ����--" << endl;
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
				&& "Control::is_new_map:_world.map_posδ��ʼ��" );

			_world.update_boss_room_win_pos();
			_world.boss_room_pos.corrent_assert();
			assert( _world.map_pos != Pos()
				&& "Control::is_new_map:_world.boss_room_posδ��ʼ��" );

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