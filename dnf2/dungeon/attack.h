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

			// �Ƿ���Ҫʹ��r
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


		//	ר������ɱboss��ģ��
		//	�ӵ�ͼ���ҵ�boss.����������
		//	bossģʽ�ķ�ʽ����̶���ɱ��.
		//	���������Ȼ�ɱboss.�������ƶ�.
		class _DllExport BossKiller{
		public:
			BossKiller( World& _world );

		public:

			//	����boss��λ��.�ҵ���ֵ
			//	���ŵ�_pos����
			//	���û���ҵ���_pos����Ϊ-1
			//
			//	ͬʱ�����Ƿ��ҵ�boss����
			//	true����false;
			bool search();

			void set_pos_offset( int offset_x, int offset_y );


			//	����Ƿ�����offset
			bool is_set_offset();

			//	ָ���Ѿ�������offset
			void set_offset();
			//	�����趨offsets

			void reset_offset();

		private:

			//	searchʹ�õ�_str_buf����
			//	��_iss,_iss_pos;
			string _str_buf;
			istringstream _iss;
			istringstream _iss_pos;

		public:
			void attack();

		private:
			//	attackʹ�õ�r���ܵļ�����
			//	���Ʊ����ͺ���
			bool _can_use_skill_r();
			void _use_skill_r();
			void _use_skill_q();
			bool _is_start_r_timer;
			boost::timer _r_timer;

			////////////////////
		private:
			
			//	����boss��λ��
			//	���û���ҵ�Ϊ-1
			Pos _pos;

			//	_hit_pos�ǵ����λ��
			//	��pos��40
			Pos _hit_pos;

			//	�Ƿ�����
			bool _is_set_offset;

			//	boss����ƫ��
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
			//	���boss�ļ���
			//	ǰ������:
			//	_world�еĸ�������
			//	�Ѿ�����
			HideBossSkill( World& world_, float step_time_ = 0.3f );

			//	����boss������_world.target_group�е�index;
			void update_skill_index();

			//	���boss�Ƿ��ͷż���
			//	����Ƿ����61440
			//	ǰ������:�Ѿ�����
			//	update_skill_index
			bool has_boss_skill();
			
			//	�Ƿ���Ҫ���
			//	���ȼ��ʦ����Ҫ����
			//	boss����.Ȼ��������е�����
			//	��ȷ���Ƿ���Ҫ���boss����
			//	������_in_range_skill_index
			bool is_need_hide();
			
			enum MoveDir{
				None	= 0,
				Up		= 1,
				Down	= 2,
				Left	= 3,
				Right	= 4

			};

			//	ͨ������ļ��㼼�ܵ�λ��
			//	���ж��ƶ��ķ���.

			MoveDir set_move_dir();

			void hide_boss_skill( MoveDir move_dir );

			void hide_boss_skill();


		private:

			//	����Ƿ����жϷ�Χ��
			bool _is_in_determine_range( 
				const Pos& boss_skill_pos );

			//	��_in_range_skill_index��
			//	���Ҳ����������skill��index
			index_t _nearest_skill_index();

		public:

			//	�����߳�.ʹ�÷�����ģʽ
			void start();
			bool is_start();
			void end();

			void hide_boss_skill_thread_func();

			void set_step_time( float step_time );

		private:
		
			//	�Ƿ�ʼ��־
			//	������ֹ�̺߳����е�
			//	��ѭ��
			bool _is_start;

			//	��ʱ��ʱ��
			//	��ʱ���˳��߳�ѭ��
			boost::timer _out_timer;

			//	ÿ���ƶ���ʱ����;
			float _step_time;

		private:

			//	����61440��_world.target_group�е�index;
			vector<index_t> _skill_index;
			//	�����ڷ�Χ�е�61440index
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
			//	���ɵĸ߶�
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

			//	���������쳣
		public:
			bool _handle_data_exception();

			void handle_data_exception();
			bool has_data_exception();
			
		public:
			//	�����Ǳ���ס
			void handle_block_sign();

		private:
			bool _start_timing;
			boost::timer _block_sign_timer;

			//	q���ܵĹ��������������
			//	����z����
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