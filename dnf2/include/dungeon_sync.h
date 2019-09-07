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

		//	����Attack���Move��
		//	���߳�ͬ��
		//	��Ҫ��Move�Ŀ���״̬
		//	Attackͨ��״̬������
		//	�Լ�

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
			//	ֹͣ����״̬
			static bool _is_stop_atk;

			//	Move��Attack֮��Ļ�����
			static boost::recursive_mutex _mutex;

			//	�ϰ���
			static bool _is_set_block;
			static DestroyBlock* _destroy_block_ptr; 
		
		public:
			static bool is_find_score();
			static void set_find_score();
			static void unset_find_score();

		private:
			//	�Ƿ�������
			//	��attack�߳�������
			//	move�߳�����ȡ��������
			static bool _is_find_score;
		
		public:
			static bool is_stop_pick_up();
			static void set_stop_pick_up();
			static void unset_stop_pick_up();

		private:

			//	�Ƿ�ʰȡ
			//	��move�߳�,ʶ��
			//	��attack�̷߳��ʲ�����
			static bool _stop_pick_up;

		public:

			static bool is_in_boss_room();
			static void set_in_boss_room();
			static void unset_in_boss_room();

		private:
			//	�Ƿ���boss����
			//	��move�߳�,ʶ��
			//	��attack�̷߳���
			static bool _is_in_boss_room;
		};





_DUNGEON_END

#endif