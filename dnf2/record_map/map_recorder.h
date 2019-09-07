#ifndef __MAP_RECORDER_H__
#define __MAP_RECORDER_H__

#include <base.h>
#include <map.h>
#include <world.h>

#pragma comment( lib, "map.lib" )

#define _DllExport __declspec(dllimport)

#include <cassert>
#include <string>
using namespace std;

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/bind.hpp>

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

#define BOOST_FILESYSTEM_NO_DEPRECATED

//namespace dnf{
//	namespace map{

_MAP_BEGIN

		//	˯��time����
		void wait( size_t time );

		class Input{
		public:
			Input( World& world );

			//	���С��ͼ�Ƿ��
			void check_map_open();

			void operator()();

			void start_record();
			void end_record();
			void update_state();

			string get_map_name();

			bool is_start();
			void set_finish();
			bool is_end_record();
			void restart_record();

			void reset();
			
		private:
			bool _is_start;
			bool _is_end_record;
			bool _is_finish;
			bool _is_restart;
			string _map_name;

			World& _world;
		};

		//////////////////////////////////
		//	��¼����������õ���Ϣ
		//	���뵽ͼ��
		class Record{
		public:

			Record( World& world );

			void start( Input& input );
			void start();
			
			//	����ӿڻ���Ľ�
			//	����Ҫ�ⲿ�ĵ���
			//	���ұ����������߳�
			//	�ĵ��ã���Ϊstart
			//	�����������ģ�����Ҫ
			//	end�������������
			void end();

			void update_map_pos();

			void set_map_ptr( Map& map );

			void reset();

		private:

			bool _is_end;;

			World& _world;
			Pos _last_map_pos;
			Map* _map_ptr;
#ifdef _DEBUG
			size_t _debug_cnt;
#endif
		};




		//////////////////////"////////////
		//	Store��Ҫ��ģ��
		//	ǰ�������ǣ��ļ����ڵ�Ŀ¼
		//	�Ѿ����ڡ�
		//	�����Ƕ�ȡ�Ѿ����ڵķ����������ļ�
		//	�Ѿ�������Ҫ�洢���ļ�������
		class AutoFile{
		public:

			AutoFile();

			void set_work_dir( const string& work_dir );

			bool load_file_list( vector<string>& file_list, const string& file_ext );

			void gen_save_file_name( string& save_file_name, const string& file_ext );

		private:

			string _work_dir;
		};

		//	Store��������ͱ�����ص��ļ�
		//	
		class Store{
		public:
			Store();

			void init();

			void set_map_name_dir( const string& map_name );

			void set_exist_map_list_ptr( vector<Map>& exist_map_list );
			void set_save_map_ptr( Map& save_map );

			//	��ȡ��ͼ����������ڵ�ͼ��
			//	����false����ȡ����true��
			bool load_map( const string& map_name = "" );
			//
			void save_map( const string& map_name = "" );

			void save_map_png( const string& map_name = "" );

			
		private:
			string _map_dir;
			string _map_name_dir;
			string _map_ext;

			AutoFile _auto_file;
			vector<string> _file_list;

			vector<Map>* _exist_map_list_ptr;
			Map* _save_map_ptr;
		};


		
		///////////////////////////////////
		//	����ģ��
		//	������������,����Input�ǵ������߳�
		class Control{
		public:
			Control( World& world );

			//	������
			void operator()();

			void reset();

			bool is_new_map();

		private:
			World& _world;
			Record _record;
			Store _store;

			Input _input;

			//	�Ѿ����ڵĵ�ͼ
			vector<Map> exist_map;
			//	��ǰ�ĵ�ͼ
			Map map;

			boost::thread _input_thread;
		};

		//////////////////////////
		//	��ͼ��¼��������
		//	1,����ģ��
		//	Input
		//	2,��¼ģ��
		//	Record
		//	3,����ģ��
		//	Control
		//	4,�ļ��洢ģ��
		//	Store
		class MapRecorder{
		public:
			MapRecorder( Dm& dm_, AttachIme& attach_ime_ );
		private:
			Control _control;
			World _world;
		};


_MAP_END

//	}
//}
//
#endif