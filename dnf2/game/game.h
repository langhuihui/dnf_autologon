#ifndef __GAME_H__
#define __GAME_H__


//#include "base.h"
//#include "world.h"
#include "login.h"
#include "watch.h"
#include <dungeon.h>

#pragma comment( lib, "dungeon.lib" )

#ifdef GAME_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif


_GAME_BEGIN

	using _DUNGEON::Dungeon;
	using _WATCH::Watch;

	class _DllExport StudySkill{
	public:
		StudySkill( World& world_ );

		void Q();
		void E();
		void R1();
		void right_Q();
		void left_Q();
		void right_E();
		void left_E();
		void left_R1();

		void open();
		void close();

		bool _study( const Pos& pos );
		bool _study_close_m( const Pos& pos );
		bool _study_end( const Pos& pos );
		void study( const Pos& pos );

		//	只加1级
		void study1( const Pos& pos );

		bool _has_black_study();
		bool _has_ensure();
		bool _ensure();
		bool _ensure_study();
		void ensure_study();

		void level_10();
		void level_15();
		void level_20();
		void set( size_t level );

		bool has_r();

	private:

		const Pos _Q;
		const Pos _E;
		const Pos _R;
		const Pos _right_Q;
		const Pos _left_Q;
		const Pos _right_E;
		const Pos _left_E;
		const Pos _left_R;

		const Pos _m_offset;
		const Pos _c_offset;

		const int _offset_x1;
		const int _offset_y1;
		const int _offset_x2;
		const int _offset_y2;

		World& _world;
		bool _is_set_level;
		size_t _last_set_level;
	};


	//	恢复技能
	class _DllExport RestoreSkill{
		static Pos q_pos_in_study;
		static Pos q_pos_in_set;
		static Pos e_pos_in_study;
		static Pos e_pos_in_set;
		static Pos r_pos_in_study;
		static Pos r_pos_in_set;

		static Pos check_q_start_pos;
		static Pos check_q_end_pos;
		static Pos check_e_start_pos;
		static Pos check_e_end_pos;
		static Pos check_r_start_pos;
		static Pos check_r_end_pos;

		static Pos check_no_e_start_pos;
		static Pos check_no_e_end_pos;
		static Pos check_no_r_start_pos;
		static Pos check_no_r_end_pos;

	public:
		RestoreSkill( World& _world );


		void restore();

		void restore_one( 
			const Pos& study_pos,
			const Pos& set_pos
			);

		bool is_set_q();
		bool is_set_e();
		bool is_set_r();

		bool is_study_e();
		bool is_study_r();

		bool restore_q();
		bool restore_e();
		bool restore_r();

		void set_safe_mouse();

	private:
		void _drag_mouse( 
			const Pos& start_pos,
			const Pos& end_pos
			);

	private:

		World& _world;
	};


	//////////////////////////////

	class _DllExport Town{
	public:
		Town( World& world_ );

		void run();
		void to_dungeon( size_t level );

		//void study_skill();

	private:

		//Dungeon _dungeon;
		StudySkill _study_skill;
		RestoreSkill _restore_skill;
		
		size_t _last_level;

		World& _world;
	};


	class _DllExport Game{
	public:
		Game( World& world_, const string& qq_ );

		void game();
		void run();
		void run_no_login();
		size_t load_no_login_times();
		void set_no_login_times( size_t times );

		void quit();
		void set_ime();

		void switch_next();
		void complete();

		bool handle_weak();

	private:
		Login _login;
		Town _town;
		Dungeon _dungeon;
		Watch _watch;

		boost::thread _watch_thread;
		
		World& _world;
	};

_GAME_END


#endif