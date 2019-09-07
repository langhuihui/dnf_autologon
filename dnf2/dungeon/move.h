#ifndef __MOVE_H__
#define __MOVE_H__


#include <vector>
using namespace std;

#include <boost/timer.hpp>
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;
#define BOOST_FILESYSTEM_NO_DEPRECATED

//#include "world.h"
//#include "map.h"
//#include "room.h"
//#include "log.h"
#include "dungeon_sync.h"
//#include "config.h"

#include <sync.h>

#include <map.h>
#include <world.h>
#include <room.h>
#include <config.h>

#pragma comment( lib, "map.lib" )

#ifdef DUNGEON_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

//namespace dnf{
//	namespace move{
		
//_DUNGEON_BEGIN
//		class Sync;
//_DUNGEON_END
//
_MOVE_BEGIN
		
/*
		using dnf::map::Map;
		using dnf::map::room::Room;
		using dnf::map::room::DestroyBlock;
		using dnf::dungeon::Sync;
		using dnf::special::Special;
*/

		using _MAP::Map;
		using _ROOM::Room;
		using _ROOM::DestroyBlock;
		//using _DUNGEON::Sync;
		using _SPECIAL::Special;
		using _SPECIAL::CaseName;

		using namespace _DNF;

		///////////////////////////
		//	载入相关数据
		class _DllExport Load{
		public:
			Load( World& world_ );

			void set_map_name_dir();

			void set_exist_map_list_ptr( vector<Map>& exist_map_list );
			void set_exist_room_list_ptr( vector<Room>& exist_room_list );

			bool load_exist_room();
			bool load_exist_map();

		private:

			bool _get_map_name_list( vector<string>& map_name_list );

			bool _get_room_name_list( 
				vector<string>& room_name_list,
				const string& file_ext
				);

			bool _get_map_graph_name_list(
				vector<string>& graph_name_list,
				const string& file_ext,
				bool no_path = false
				);



		private:
			vector<Map>* _exist_map_list_ptr;
			vector<Room>* _exist_room_list_ptr;

			string _map_dir;
			string _map_name_dir;

			string _room_ext;
			string _room_cfg_ext;
			string _map_ext;
			string _map_cfg_ext;
			
			World& _world;
		};

		///////////////////////
		//	Update
		//	更新移动的位置
		class _DllExport Update{
		public:
			Update::Update( World& world_ );

			void init();

			void operator()();

			void set_exist_map_list_ptr( vector<Map>& exist_map_list );
			void set_exist_room_list_ptr( vector<Room>& exist_room_list );

			void set_next_pos_ptr( Pos& next_pos );

		private:

			string _get_map_name();
			bool _get_map_name_list( vector<string>& map_name_list );	

		public:

			bool is_corrent_map();
			bool map_name_is_changed();
			void update_map_name();

			void set_current_map();

			void set_rfp();	
			bool is_in_exist_room_list();
			void set_best_sim_room();
			
			bool is_right_state();
			bool is_change_room();
			bool update_last_map_pos();

			void set_map_shortest();
			void set_room_shortest();

			bool is_at_next_pos();
			void set_next_pos();
			void unset_room_shortest();

			void set_enemy();
			void check_enemy();



		/////////////////////////////////
		private:
			vector<Map>* _exist_map_list_ptr;
			vector<Room>* _exist_room_list_ptr;

			Pos* _next_pos_ptr;

			/////////////////

			Room _current_room;
			Map _current_map;

			//vector<Dir> _map_shortest;
			vector<Pos> _room_shortest;


			bool _has_enemy;
			bool _is_set_room_shortest;

			size_t _room_shortest_index;

			string _map_dir;
			string _map_name;

			Pos _last_map_pos;
			World& _world; 


		///////////////////////////////
		public:

			//	代替原来的operator()t
			//	与此不同的是,此接口采用
			//	过程化的接口.
			void run();

			bool is_last_room();

			void moveto_next_pos();
			void moveto_next_pos_and_wait_clear();
			void moveto_next_door();
			void attack_near_block();


			void clear_room();
			bool is_clear_room();
			bool is_clear_window();
			void goto_nearest_pos();
			void hide_enemy_restart();
			void hide_enemy();
			void auto_hide_enemy( 
				size_t x_range = 150,
				size_t y_range = 50,
				size_t step_time = 3000
				);

			bool has_left_enemy();
			void clear_left_window();
			void goto_left_pos();
			bool set_left_path();
			bool has_right_enemy();
			void clear_right_window();
			void goto_right_pos();
			bool set_right_path();
			
			DestroyBlock& get_current_destroy_block();
			
		private:
			void _update_enemy();

			bool _left_window( const Pos& pos );
			bool _right_window( const Pos& pos );
			bool _in_window( const Pos& pos );

			bool _error_545;
			boost::timer _hide_timer;
			bool _hide_enemy_up;
			size_t _sleep_time;
		public:

			void out_room();
	
		private:
			vector<Pos> _room_path;
			size_t _room_path_index;
			//Pos _next_pos;

			boost::timer _out_timer;
			size_t _time_limit;
			boost::timer _sec_out_timer;

		public:
			bool is_dungeon_score();
			bool is_dungeon_end();

		private:
			bool _is_dungeon_score;
			bool _is_dungeon_end;

		public:
			bool is_in_boss_room();
		private:
			bool _is_in_boss_room;

		public:

			void handle_special_case();

			void handle_special_case_YingXiongZhong();

			bool special_case_hide_enemy();
			size_t special_case_hide_enemy_time();

			bool has_auto_hide_enemy();
			bool get_auto_hide_enemy_agrv(
				size_t& x_range,
				size_t& y_range,
				size_t& step_time
				);
		};
		//	
		////	恢复技能
		//class _DllExport RestoreSkill{
		//	static Pos q_pos_in_study;
		//	static Pos q_pos_in_set;
		//	static Pos e_pos_in_study;
		//	static Pos e_pos_in_set;
		//	static Pos r_pos_in_study;
		//	static Pos r_pos_in_set;
		//public:
		//	RestoreSkill( World& _world );

		//	void restore();

		//	void restore_one( 
		//		const Pos& study_pos,
		//		const Pos& set_pos
		//		);

		//private:
		//	void _drag_mouse( 
		//		const Pos& start_pos,
		//		const Pos& end_pos
		//		);

		//private:

		//	World& _world;
		//};
	
		
		///////////////////////////
		//	自动移动
		class _DllExport Move{
		public:

			Move::Move( World& world_ );

			void init();

			void operator()();

		private:
			//	恢复技能
			//	改为在进入副本之前恢复
			//RestoreSkill _restore_skill;

		private:
			Load _load;
			Update _update;

			vector<Map> _exist_map_list;
			vector<Room> _exist_room_list;

			Pos _next_pos;

			World& _world;
			boost::timer _out_timer;

			//	设置是否是刚刚进入地下城
			bool _is_just_into_dungeon;
		};
		

_MOVE_END

//
//	}	//	namespace move
//}	//	namespace dnf 
//

#endif