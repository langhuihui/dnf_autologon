#ifndef __ATTACK_H__
#define __ATTACK_H__

//#include "log.h"
//#include "world.h"
//#include "random.h"
#include "dungeon_sync.h"



#include <set>
using namespace std;

#include <boost/timer.hpp>

#include <world.h>
#include <random.h>
#include <config.h>

#pragma comment( lib, "map.lib" )

#ifdef DUNGEON_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif


//namespace dnf{
//	namespace attack{

//_DUNGEON_BEGIN
//		class Sync;
//_DUNGEON_END

_ATTACK_BEGIN
		//using dnf::dungeon::Sync;

		using namespace _DNF;

		//using _DUNGEON::Sync;

		class _DllExport FlyEnemy{
		public:
			FlyEnemy();

			bool operator<( const FlyEnemy& fly_enemy ) const ;

		public:
			Pos map_pos;
			boost::timer t;
		};

		class _DllExport FlyEnemyMgr{
		public:
			FlyEnemyMgr();

			bool is_ignore( const Pos& enemy_map_pos );

			void check();

			void add( const Pos& enemy_map_pos );
		public:
			set<FlyEnemy> fly_enemy_set;
		};

		class _DllExport BossMode{
		public:
			BossMode();

			void set_active( bool active );

			void check( vector<Target>& target_group );

			bool is_boss();

			Target* boss_ptr();

			// 是否需要使用r
			bool can_use_skill_r();

		public:
			boost::timer t;
			bool _is_like_boss;
			bool _is_boss;
			bool _is_active;
			Target* _boss_ptr;


			boost::timer r_timer;
			bool is_start_r_timer;

		};


		//	专门用来杀boss的模块
		//	从地图中找到boss.并且用类似
		//	boss模式的方式将其固定和杀死.
		//	作用是优先击杀boss.限制其移动.
		class _DllExport BossKiller{
		public:
			BossKiller( World& _world );

		public:

			//	查找boss的位置.找到的值
			//	被放到_pos里面
			//	如果没有找到吧_pos设置为-1
			//
			//	同时根据是否找到boss返回
			//	true或者false;
			bool search();

			void set_pos_offset( int offset_x, int offset_y );


			//	检查是否设置offset
			bool is_set_offset();

			//	指定已经设置了offset
			void set_offset();
			//	重置设定offsets

			void reset_offset();

		private:

			//	search使用的_str_buf缓存
			//	和_iss,_iss_pos;
			string _str_buf;
			istringstream _iss;
			istringstream _iss_pos;

		public:
			void attack();

		private:
			//	attack使用的r技能的计数器
			//	控制变量和函数
			bool _can_use_skill_r();
			void _use_skill_r();
			void _use_skill_q();
			bool _is_start_r_timer;
			boost::timer _r_timer;

			////////////////////
		private:
			
			//	保存boss的位置
			//	如果没有找到为-1
			Pos _pos;

			//	_hit_pos是点击的位置
			//	比pos高40
			Pos _hit_pos;

			//	是否被设置
			bool _is_set_offset;

			//	boss坐标偏移
			int _offset_x;
			int _offset_y;

			World& _world;
		};

		class _DllExport HideBossSkill{
			static const int _boss_skill_id = 61440;
			static const size_t _determine_range_x = 130;
			static const size_t _determine_range_y = 130;

			typedef int index_t;
			static const size_t _invail_index = 0xffffffff;
		public:
			//	躲避boss的技能
			//	前置条件:
			//	_world中的各项数据
			//	已经更新
			HideBossSkill( World& world_, float step_time_ = 0.3f );

			//	更新boss技能在_world.target_group中的index;
			void update_skill_index();

			//	检查boss是否释放技能
			//	检查是否存在61440
			//	前置条件:已经调用
			//	update_skill_index
			bool has_boss_skill();
			
			//	是否需要躲避
			//	首先检查师傅需要存在
			//	boss技能.然后根据现有的条件
			//	来确定是否需要躲避boss技能
			//	会设置_in_range_skill_index
			bool is_need_hide();
			
			enum MoveDir{
				None	= 0,
				Up		= 1,
				Down	= 2,
				Left	= 3,
				Right	= 4

			};

			//	通过最近的计算技能的位置
			//	来判断移动的方向.

			MoveDir set_move_dir();

			void hide_boss_skill( MoveDir move_dir );

			void hide_boss_skill();


		private:

			//	检查是否在判断范围内
			bool _is_in_determine_range( 
				const Pos& boss_skill_pos );

			//	从_in_range_skill_index中
			//	查找并返回最近的skill的index
			index_t _nearest_skill_index();

		public:

			//	启动线程.使用非阻塞模式
			void start();
			bool is_start();
			void end();

			void hide_boss_skill_thread_func();

			void set_step_time( float step_time );

		private:
		
			//	是否开始标志
			//	用于终止线程函数中的
			//	主循环
			bool _is_start;

			//	超时计时器
			//	超时后退出线程循环
			boost::timer _out_timer;

			//	每步移动的时间间隔;
			float _step_time;

		private:

			//	放置61440在_world.target_group中的index;
			vector<index_t> _skill_index;
			//	放置在范围中的61440index
			vector<index_t> _in_range_skill_index;

			World& _world;
		};

		class _DllExport AttackTest{
		public:
			AttackTest( World& world_ );

			void corrent_assert();
		public:

			void operator()();

			bool is_skip_cloned_enemy();

			void left_hit();
			void left_hit2();
			//	击飞的高度
			size_t _hit_height_limit;

			void kill_boss();

			void attack_boss();

			void attack_block();

			void pick_up();

			bool skill_e();

			void skill_q();

			bool e( Pos& pos );

			bool is_in_e_range( const Pos& center, const Pos& pos );

			bool is_right_player_win_pos( const Pos& win_pos );

			bool is_in_window( const Pos& win_pos );

			void handle_special_case();

		private:
			World& _world;
			vector<Target*> enemy_ptr_vec;

			FlyEnemyMgr fly_enemy_mgr;
			BossMode boss_mode;
			BossKiller _boss_killer;
			HideBossSkill _hide_boss_skill;

			Logger log;

			size_t _check_score_cnt;
			bool _is_show_stop_attack;

			boost::timer _check_npc_timer;
			bool _start_not_found_map_pos;

			//	处理数据异常
		public:
			bool _handle_data_exception();

			void handle_data_exception();
			bool has_data_exception();
			
		public:
			//	处理标记被挡住
			void handle_block_sign();

		private:
			bool _start_timing;
			boost::timer _block_sign_timer;

			//	q技能的攻击单独分离出来
			//	不带z坐标
		public:

			void q_hit();
			
		private:
			bool _is_start_q;

		public:
			size_t near_enemies_index( size_t num = 3 );
		private:
			vector<int> _near_enemies_index;

		};

		typedef AttackTest Attack;

_ATTACK_END

//	}	//	attack
//}	//	dnf
//

#endif