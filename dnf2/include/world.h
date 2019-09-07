#ifndef __WORLD_H__
#define __WORLD_H__

#include <vector>
#include <cassert>
using namespace std;


//#include "Dnf.h"

#include "map_base.h"
#include "sync.h"
//#include "attach_ime.h"

#include <dm.h>
#pragma comment( lib, "dm.lib" )

#include <attach_ime.h>
#pragma comment( lib, "attach_ime.lib" )
#include "message.h"

#include <process.h>
#include <TlHelp32.h>

#ifdef WORLD_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif
/*
class _Pos{
public:
	_Pos( float x_ = 0.f, float y_ = 0.f, float z_ = 0.f );
	bool operator!=( const _Pos& p ) const;
	bool operator==( const _Pos& p ) const;

public:
	float x;
	float y;
	float z;
};
*/

_DNF_BEGIN

	using namespace _DNF;

class _DllExport Obj{
public:
	Pos map_pos;
	Pos win_pos;
};



class _DllExport Player : public Obj{
public:
	Player( CDnf& dnf_ );
	void key_press( long vk, long keep_time = 50 );

	void Q(); // 0x51
	void E(); // 0x45
	void R(); // 0x52

	void A( int time = 1000 ); // ms 0x41
	void W( int time = 1000 ); // ms 0x57
	void S( int time = 1000 ); // ms 0x53
	void D( int time = 1000 ); // ms 0x44
	void AA( int time = 1000 );
	void DD( int time = 1000 );

	void WA( int time = 1000 );
	void WD( int time = 1000 );
	void SA( int time = 1000 );
	void SD( int time = 1000 );

	void RB( const Pos& pos );
	void LB( const Pos& pos );

	//	设置人物等级
	bool _set_level();
	void set_level();
	size_t get_level();

	//	设置人物名字
	bool _set_name();
	void set_name();
	string get_name();

	bool has_info();
	bool has_no_info();
	void open_info();
	void close_info();

	//	更新人物的金币数量
	void update_money();
	//	获取人物的金币数量
	size_t money();

	bool has_equip();
	bool _open_equip();
	void open_equip();
	bool _close_equip();
	void close_equip();


public:
	CDnf& dnf;

	size_t _level;
	string _name;
	size_t _money;
};

class _DllExport Enemy : public Obj{
public:
};

class _DllExport Target : public Obj{
public:
	int id;
};

/*-------------------------------------
	Auction:	
	拍卖类:
	处理和拍卖相关的所有行为.

		*:应该在鼠标点击和按键层级之上,
	但是由于前期的world的设定仓促,
	导致鼠标点击和键盘放到了Player
	中.
		这并不合理.鼠标和键盘属于外部
	输入,本质上是模拟外部输入.和角色
	相关的动作并不属于同一层级.

	层级分布应该如下:

		  world
			|
	Auction / Player
		|	|	|
	外部输入/基础模板
	-----------------
	以后再做修改.现在直接使用CDnf中
	的dm来使用鼠标和键盘.

--------------------------------------*/
class Auction{
public:
	Auction( CDnf& dnf );
	bool open();
	bool close();
	
	bool init_pos();
	bool input_keyword();
	bool search();
	bool sort();
	bool buy();


	bool check_safe_state();
	bool set_safe_mouse();
	bool update_money();

private:
	CDnf& _cdnf;

	//	卖家名字
	string _seller;
	//	关键字
	string _keyword;
	//	每页拍卖行的数据缓存
	typedef vector<string> PageCache;
	PageCache _page_cache;
	size_t _money;

protected:

	//	辅助open和close函数
	bool _is_open();
	bool _is_close();
	//	图查找拍卖行位置
	bool _set_base_pos();
	bool _set_other_pos_by_base_pos();
	//	去掉遮挡的奖励窗口
	bool _clear_reward_window();

private:
	//	拍卖行基准坐标
	//	为"拍卖行"这3个字
	//	的左上角坐标
	Pos _auction_base_pos;
	//------------------
	//	以下坐标相对于基
	//	准坐标的相对坐标
	//----
	//	输入关键字坐标
	Pos _keyword_input_pos;
	//	搜索按钮坐标
	Pos _search_pos;
	//	金币排序坐标
	Pos _sort_by_money_pos;
	//	向后翻页坐标
	Pos	_next_page_pos;

	//	一口价坐标
	Pos _buy_pos;
};

class _DllExport Move{
public:
	Move( CDnf& dnf_ );

	void operator()(
		const Pos& current_pos,
		const Pos& target_pos );

private:
	void _move( 
		long main_vk,
		size_t main_dis,
		size_t main_speed,
		long second_vk,
		size_t second_dis,
		size_t second_speed
		);

	void _trace_down_begin( long vk );
	void _trace_down_end( long vk );
	void _trace_up_begin( long vk );
	void _trace_up_end( long vk );

	////////调试断言/////////////
#ifdef _DEBUG
	size_t _cnt_a;
	size_t _cnt_d;
	size_t _cnt_w;
	size_t _cnt_s;
#endif

	/////////////////////////////

private:
	CDnf& _dnf;

	int _offset_x;
	int _offset_y;

	long _vk_x;
	long _vk_y;

	size_t _x_dis;
	size_t _y_dis;

	float _x_time;
	float _x_high_speed_time;
	float _y_time;

	bool _is_main_x;
	bool _is_run;

	//	切割的最小距离
	size_t _min_dis;

	static const size_t _x_speed = 125;
	static const size_t _x_high_speed = 250;
	static const size_t _y_speed = 100;

	static const long _D = 0x44;
	static const long _A = 0x41;
	static const long _W = 0x57;
	static const long _S = 0x53;
};

class _DllExport World{
public:
	World( Dm& dm_, AttachIme& attach_ime_ );
	void update_all();

public:
	void _fix_player_win_pos();
	bool update_player_map_pos();
	bool update_player_win_pos();
	bool update_target_group_map_pos();

/*
	bool is_enemy_cloned;
	boost::timer enemy_cloned_timer;
	*/

	bool update_target_group_win_pos();
	bool update_block_pos();
	bool update_mouse_pos();
	
	bool update_map_win_pos();
	//	给树精繁殖这个特殊情况使用的
	Pos _last_map_pos;
	bool update_map_win_pos_block( 
		size_t limit_time = 20, 
		size_t step_time = 100, 
		bool has_assert = true );
	bool update_big_map_win_pos();
	bool update_boss_room_win_pos();
	bool update_boss_room_win_pos_block(
		size_t limit_time = 20,
		size_t step_time = 100,
		bool has_assert = true 
		);

	Pos get_win_pos_by_player( const Pos& map_pos );

	bool _is_inside_window( Pos& pos );

	void corrent_assert();

	void moveto( const Pos& pos );

	////////////////////////
	bool _big_map_move_to( const Pos& pos );
	bool big_map_move_to( const Pos& pos );
	bool _is_open_big_map();
	bool _open_big_map();
	void open_big_map();

	bool to_luo_lan();
	bool to_luo_lan_zhi_sen();
	bool to_tian_kong_zhi_cheng();
	bool to_tian_wei_ju_shou();

	//////以上为大转移之前的地图///////
	
	bool to_ya_nuo_fa_sen_lin();//		1-16
	bool to_e_yun_zhi_cheng();//		17-26
	bool to_piao_liu_dong_xue();//		27-54
	bool to_ni_liu_pu_bu();//			37-45
	bool to_wang_zhe_xia_gu();//		40-70
	bool to_jue_wang_bing_ya();//		46-53	不可用
	bool to_ge_lan_zhi_sen_shan_lu();//	27-70


	bool _to_dungeon( const Pos& pos, Dir dir );
	bool _to_select_dungeon_ui( const Dir& dir );
	bool _is_in_select_dungeon_ui();

	//	选择副本,difficulty是难度
	//	0为easy
	//	1为middle
	//	2为hard
	//	-1为选最难
	//	-1000为错1

	bool _select_dungeon( const string& dungeon_name );
	bool select_dungeon( const string& dungeon_name, int difficulty = -1 );
	int _get_current_difficulty();

	bool is_in_dungeon( const string& dungeon_name );

	bool is_in_big_map();

	bool _to_big_map();
	void to_big_map();
	bool _to_big_map_by_f12();
	void to_big_map_by_f12();

	bool is_dungeon_end();
	bool is_dungeon_reward();
	bool is_dungeon_reward_end();
	bool is_dungeon_score();
	
	bool is_dead();
	bool is_low_hp();
	bool is_low_mp();
	bool is_weak();
	Pos	 weak_pos();
	bool has_weak_notice();
	bool has_no_weak_notice();
	void close_weak_notice();

	bool has_weaked_notice();
	bool has_no_weaked_notice();
	void close_weaked_notice();

	bool has_restore_notice();
	bool has_no_restore_notice();
	void close_restore_notice();


	bool has_unusual();
	bool _handle_unusual();
	void handle_unusual();
	
	bool has_dis_connect();
	bool _handle_dis_connect();
	void handle_dis_connect();

	bool has_safe_mode();
	bool _handle_safe_mode();
	void handle_safe_mode();

	bool has_motion();
	bool _close_motion();
	void close_motion();

	bool has_notice();
	bool _close_notice();
	void close_notice();

	bool has_esc();
	bool _open_esc();
	void open_esc();
	bool _close_esc();
	void close_esc();

	bool has_info();
	bool has_no_info();
	bool _open_info();
	void open_info();
	bool _close_info();
	void close_info();

	bool has_equip();
	bool _open_equip();
	void open_equip();
	bool _close_equip();
	void close_equip();
	bool is_out_weight();
	bool is_full_weight();

	bool has_skill();
	bool _open_skill();
	void open_skill();
	bool _close_skill();
	void close_skill();

	bool quit_for_reward();
	void quit_game();

	bool quit_for_reward2();
	void quit_game2();

	bool has_energy();

	bool is_return_select_character();
	void return_select_character();

	bool has_process( const string& process_name );

	//	这里可以写成通用的
	//	因为其行为是相似的

	bool is_right_low_ui();

	//	has

public:
	typedef vector<Target> TargetVec;
	typedef vector<Pos> BlockVec;

	CDnf& dnf;
	AttachIme& attach_ime;
	Player player;
	TargetVec target_group;
	BlockVec block_pos;
	Pos mouse_pos;
	Pos map_pos;
	Pos big_map_pos;
	Pos boss_room_pos;

	Move move;

	Dm& dm;

#ifdef _DEBUG
	int debug_cnt;
#endif

	//	world作为游戏的世界背景
public:
	
};


_DNF_END

#endif