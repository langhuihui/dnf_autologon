#ifndef __DUNGEON_H__
#define __DUNGEON_H__

//#include "move.h"
//#include "attack.h"

//#include "room.h"
#include "move.h"
#include "attack.h"

#include <boost/thread.hpp>

#include <room.h>
#include <random.h>
#pragma comment( lib, "map.lib" )

#ifdef DUNGEON_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

//namespace dnf{
//	namespace dungeon{
//_MOVE_BEGIN
//		class Move;
//_MOVE_END
//
//_ATTACK_BEGIN
//		class AttackTest;
//_ATTACK_END

_DUNGEON_BEGIN

//		using dnf::map::room::DestroyBlock;
		using namespace _DNF;
		
		using _MOVE::Move;
		using _ATTACK::AttackTest;
		using _ATTACK::Attack;


		////	用于Attack类和Move类
		////	的线程同步
		////	主要是Move的控制状态
		////	Attack通过状态来调整
		////	自己
		//class Sync{
		//	Sync();
		//public:
		//	static void set_stop_atk();
		//	static void set_nonstop_atk();
		//	static bool is_stop_atk();

		//	static boost::recursive_mutex& get_mutex();

		//	static void set_block( DestroyBlock& destroy_block );
		//	static void unset_block();
		//	static bool is_set_block();
		//	static DestroyBlock& get_destroy_block();
		//	
		//private:
		//	//	停止攻击状态
		//	static bool _is_stop_atk;

		//	//	Move和Attack之间的互斥量
		//	static boost::recursive_mutex _mutex;

		//	//	障碍物
		//	static bool _is_set_block;
		//	static DestroyBlock* _destroy_block_ptr; 
		//};

		class EquipInfo{
		public:

			EquipInfo();

			void reset();
			bool is_init();

			string name;
			size_t value;
		};

		class _DllExport Equip{
		public:
			Equip( World& world_ );


		public:

			bool has_chuan_dai();
			bool set_chuan_dai();

			//	设置装备栏的坐标.
			void set_equip_pos();

			void _set_equip_info( 
				const vector<PosStr>& str_list,
				EquipInfo& equip_info );
			void set_equip_info();

			bool is_high_value();
			bool is_low_value();

			bool check_replace_one();

			Pos get_pos( size_t i );

		public:

			bool is_replaced();

			void replace();

		//private:
			World& _world;
			//	第一个装备的位置;
			Pos _chuan_dai;
			Pos _equiped_pos1;
			Pos _equiped_pos2;
			Pos _new_pos1;
			Pos	_new_pos2;

			EquipInfo _equiped_info;
			EquipInfo _new_info;

			//	售卖相关
		public:

			bool set_npc_pos();

			void replace_sell();

			bool has_shop_option();
			bool has_sell_ensure( const Pos& pos );
			bool has_no_sell_ensure( const Pos& pos );
			bool has_safe_mode();
			bool has_no_safe_mode();

			bool has_secret_seller();
			bool has_open_secret_shop();
			bool has_secret_shop_option();


			void random_shop_pos();

			//	查找手指图案似乎是无效的
			bool is_finger_move_to( const Pos& pos );
			bool is_active_sell();

			void sell_one( const Pos& pos );
			void sell_consumable();
			void sell_stuff();

			bool open_shop();
			void replace_and_sell_equipment();
			void replace_and_sell();

			//	切换标签
		public:
			bool has_highlight_equipment();
			void switch_to_equipment();
			bool has_highlight_consumable();
			void switch_to_consumable();
			bool has_highlight_stuff();
			void switch_to_stuff();

			

		private:
			Pos _npc_pos;

			static const Pos _shop_center_pos;
			Pos _shop_sell_pos;

			Pos _secret_seller_pos;

			Pos _sell_button;
			Pos _sell_button_pos1;
			Pos _sell_button_pos2;

			bool _is_safe_mode;

			boost::timer _npc_timer;
			boost::timer _secret_seller_timer;
		};




		//////////////////////////////////////////////////

		class _DllExport Dungeon{
		public:
			Dungeon( World& world );
			
			//	要求,已经进入地下城
			//	开始打怪,直到boss打死.
			//	run结束
			void run();
			bool _reward_start();
			bool _reward_end();
			void reward();
			void clear();
			void pick_up();
	
	
			void sell();
			void out();


		private:
			Move _move;
			Attack _attack;
			Equip _equip;

			size_t _select_reward_num;

			//boost::thread _move_thread;
			boost::thread _attack_thread;
			World& _world;
			boost::timer _out_timer;
		};
		

		
_DUNGEON_END

//	}	// namespace dungeon
//}	//	namespace dnf;
//

#endif