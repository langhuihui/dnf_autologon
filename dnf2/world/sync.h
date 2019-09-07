#ifndef __SYNC_H__
#define __SYNC_H__


#include <base.h>
#pragma comment( lib, "base.lib" )

#ifdef WORLD_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif 

_DNF_BEGIN

	//	 主要存放Watch,需要更新的状态
	
	class _DllExport Sync{

		Sync();
	public:
		static void clear();

		static void set_game();
		static void unset_game();
		static bool game();
		static void set_town();
		static void unset_town();
		static bool town();
		static void set_dungeon();
		static void unset_dungeon();
		static bool dungeon();
		static void set_score();
		static void unset_score();
		static bool score();

	private:
		//static bool _login;
		static bool _game;
		static bool _town;
		static bool _dungeon;

		//static bool _select;
		//static bool _boss_room;
		static bool _score;
		//static bool _reword;
		//static bool _end;

	public:

		static void set_start_watch();
		static void set_end_watch();

		static bool is_start_watch();

	private:
		static bool _start_watch;

		//////
	public:
		static void set_stop_atk();
		static void set_nonstop_atk();
		static bool is_stop_atk();

	private:
		static bool _is_stop_atk;
		
		//////
	public:
		static void set_stop_move();
		static void set_nonstop_move();
		static bool is_stop_move();
	private:

		static bool _is_stop_move;

	public:
		static void set_stop_all();
		static void set_nonstop_all();
		static bool is_stop_all();
		static bool is_nonstop_all();


		///////设置安全模式
	public:
		static void set_safe_mode();
		static bool is_safe_mode();
	private:
		static bool _is_safe_mode;


	public:
		///////返回城镇
		static bool is_return_town();
		static void set_return_town();
	private:
		static bool _is_return_town;

	public:
		//	用来存储
		//	当前的
		//	地图的id
		//	和房间的id
		//	id对应的是文件的前缀
		//	用来配合config处理
		//	特殊情况
		static void set_current_map_id( const string& map_id );
		static string get_current_map_id();
		static string _current_map_id;

		static void set_current_room_id( const string& room_id );
		static string get_current_room_id();
		static string _current_room_id;

	};

_DNF_END

#endif