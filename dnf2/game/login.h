#ifndef __LOGIN_H__
#define __LOGIN_H__

#include <boost/date_time.hpp>
#include <random.h>
#include <sync.h>
#include <world.h>
#pragma comment( lib, "world.lib" )

#include <algorithm>
using namespace std;

#ifdef GAME_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

_GAME_BEGIN

	class _DllExport Name{
	public:
		Name();

		static string gen_new();
	};

	//	����ȷ��ѡ���ɫ
	//	���ڵ�¼���л�
	class _DllExport SelectPlayerBase{
	public:	// base
		SelectPlayerBase( World& world );


		//	������Ż�ȡ0-7����ɫ���ж�λ�õ�����.
		Pos get_frame_pos( size_t num );
		//	��ȡ���½ǵ��ж�λ�õ�����
		Pos get_frame_pos2( size_t num );

		//	����ѡ�еĽ�ɫ����
		int selected_frame_num();

		//	����ѡ�н�ɫ������
		int selected_frame_column();

		//	�Ƿ�ѡ���ɫ
		bool is_selected_frame();

		//	���ݽ�ɫ����ж����귵��
		//	��Ϣ������,����������

		void get_frame_info_pos(
			const Pos& frame_pos,
			Pos& info_start_pos,
			Pos& info_end_pos );

		void set_safe_mouse();

		void up();
		void down();
		void left();
		void right();

	private:

		//	��һ������ж�����.
		//	ȡ���Ͻǵ�����.
		const Pos _first_pos;
		//	�ڶ����ж�����
		//	ȡ���½ǵ�����
		const Pos _first_pos2;

		//	���º����ҿ�ļ��
		const size_t _x_offset;
		const size_t _y_offset;

		const Pos _info_start_offset_pos;
		const Pos _info_end_offset_pos;

		World& _world;

	public:	//high
		bool is_selected_first_frame();
		void to_first();
		
		bool is_unchange_info( const string& info );
		string get_info( size_t num );
		void set_last_info( const string& info );

		size_t get_total();
		size_t _get_total();
		bool check_second();

		void select( size_t num );

	private:
		string _last_info;

	};

	class _DllExport SelectPlayer{
	public:
		SelectPlayer( World& world );
		//////////////////////
		void save();
		void load();

		void set_total();
		size_t get_total();
		size_t get_last_num();

		void select( size_t num );

		void to_first();

		bool to_next();

		bool to_current();

		bool is_last();

		void clear();

	private:
		const string _file_name;
		size_t _total;
		size_t _last_num;

	private:
		SelectPlayerBase _base;
	};

	class _DllExport Login{
	public:
		Login( World& world_, const string& qq_ );

		void run();
		bool is_in_ui();

		////
		bool is_in_character();
		bool _select_ch();
		void select_ch();
		bool has_character();

		////
		bool is_in_create();
		bool is_selected_dizaozhe();
		bool is_start_input_name();
		bool is_input_name();
		bool is_ensure_name();
		bool is_ensure_exist();
		bool is_create_end();
		bool is_ensure_success();
		bool is_create_limit();
		bool is_ensure_create_limit();

		bool in_create();
		bool select_dizaozhe();
		bool start_input_name();
		bool input_name();
		bool ensure_name();
		bool handle_exist();
		bool create_end();
		bool ensure_success();
		bool handle_success();

		void create();
		////
		Pos get_character();
		bool is_selected( Pos pos );
		void select_character();

		/////
		bool is_joined();
		bool _join(Pos pos);

		bool is_loading_ui( Pos pos);
		bool is_loading_ui();
		bool is_movie();
		bool start_stop_movie();
		bool end_stop_movie();
		bool to_town_ensure();
		bool to_town();
		void join_first();
		void join();

	public:
		Pos _random_ch();
		
	private:
		const Pos _ch11;
		const Pos _ch12;
		const Pos _ch13;
		const Pos _ch16;

		const Pos _game_start;
		const Pos _create;
		const Pos _dizaozhe;
		//const Pos _input_name;
		const Pos _ensure_name;
		const Pos _yes;
		const Pos _ensure_exist;
		const Pos _ensure_success;

		bool _is_new_character;
		string _name;
		World& _world;


		//	���л���ɫ�İ汾
		//	�÷�������л���ɫ
		//	�����ʼ��Ϸ���õ�
	public:

		void run2();
		bool join2();
		bool create2();

		bool is_can_not_create();
		
		int get_last_num();

		bool switch_to_next();
		bool switch_to_current();
		void switch_to_first();
		void press_start_game();

		bool _load_login_mark();
		bool is_set_login();
		void set_login_mark();
		void unset_login_mark();

		bool is_out_date();
		void set_new_date();

		bool is_stop_create();
		int load_stop_num();

		//	�����ת�ƺ�����
		//	�Ŀ�������
		bool has_opening_movie_notice();
		bool has_no_opening_movie_notice();
		void handle_opening_movie();

		string _login_mark;

		bool is_last_charater();

		//	������
		bool has_recommend();
		bool has_no_recommend();
		void cancel_recommend();
		bool has_recommend_ensure();
		bool has_no_recommend_ensure();
		void cancel_recommend_ensure();
		void handle_recommend_ensure();
		void handle_recommend();

	private:
		SelectPlayer _select_player;

	};

_GAME_END


#endif