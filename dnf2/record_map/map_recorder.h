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

		//	睡眠time毫秒
		void wait( size_t time );

		class Input{
		public:
			Input( World& world );

			//	检查小地图是否打开
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
		//	记录，整理出有用的信息
		//	加入到图中
		class Record{
		public:

			Record( World& world );

			void start( Input& input );
			void start();
			
			//	这个接口还需改进
			//	他需要外部的调用
			//	而且必须是其他线程
			//	的调用，因为start
			//	函数是阻塞的，他需要
			//	end函数来解除阻塞
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
		//	Store需要的模块
		//	前置条件是：文件所在的目录
		//	已经存在。
		//	作用是读取已经存在的符合条件的文件
		//	已经生成需要存储的文件的名。
		class AutoFile{
		public:

			AutoFile();

			void set_work_dir( const string& work_dir );

			bool load_file_list( vector<string>& file_list, const string& file_ext );

			void gen_save_file_name( string& save_file_name, const string& file_ext );

		private:

			string _work_dir;
		};

		//	Store负责载入和保存相关的文件
		//	
		class Store{
		public:
			Store();

			void init();

			void set_map_name_dir( const string& map_name );

			void set_exist_map_list_ptr( vector<Map>& exist_map_list );
			void set_save_map_ptr( Map& save_map );

			//	读取地图，如果不存在地图。
			//	返回false，读取返回true。
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
		//	控制模块
		//	控制整个过程,但是Input是单独的线程
		class Control{
		public:
			Control( World& world );

			//	主过程
			void operator()();

			void reset();

			bool is_new_map();

		private:
			World& _world;
			Record _record;
			Store _store;

			Input _input;

			//	已经存在的地图
			vector<Map> exist_map;
			//	当前的地图
			Map map;

			boost::thread _input_thread;
		};

		//////////////////////////
		//	地图记录器包括：
		//	1,输入模块
		//	Input
		//	2,记录模块
		//	Record
		//	3,控制模块
		//	Control
		//	4,文件存储模块
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