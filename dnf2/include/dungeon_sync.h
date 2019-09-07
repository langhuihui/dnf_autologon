#ifndef __DUNGEON_SYNC_H__
#define __DUNGEON_SYNC_H__

//#include "base.h"
//#include "room.h"
#include <boost/thread.hpp>

#include <sync.h>

#include <room.h>
#pragma comment( lib, "map.lib" )

#ifdef DUNGEON_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

_DUNGEON_BEGIN

		using _ROOM::DestroyBlock;

		//	用于Attack类和Move类
		//	的线程同步
		//	主要是Move的控制状态
		//	Attack通过状态来调整
		//	自己

		class _DllExport Sync{
			Sync();
		public:
			static void set_stop_atk();
			static void set_nonstop_atk();
			static bool is_stop_atk();

			static boost::recursive_mutex& get_mutex();

			static void set_block( DestroyBlock& destroy_block );
			static void unset_block();
			static bool is_set_block();
			static DestroyBlock& get_destroy_block();


			
		private:
			//	停止攻击状态
			static bool _is_stop_atk;

			//	Move和Attack之间的互斥量
			static boost::recursive_mutex _mutex;

			//	障碍物
			static bool _is_set_block;
			static DestroyBlock* _destroy_block_ptr; 
		
		public:
			static bool is_find_score();
			static void set_find_score();
			static void unset_find_score();

		private:
			//	是否发现评分
			//	由attack线程来更新
			//	move线程来读取并做定义
			static bool _is_find_score;
		
		public:
			static bool is_stop_pick_up();
			static void set_stop_pick_up();
			static void unset_stop_pick_up();

		private:

			//	是否拾取
			//	由move线程,识别
			//	由attack线程访问并控制
			static bool _stop_pick_up;

		public:

			static bool is_in_boss_room();
			static void set_in_boss_room();
			static void unset_in_boss_room();

		private:
			//	是否在boss房间
			//	由move线程,识别
			//	由attack线程访问
			static bool _is_in_boss_room;
		};





_DUNGEON_END

#endif