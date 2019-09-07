#include "dungeon_sync.h"

_DUNGEON_BEGIN
	
		Sync::Sync()
		{}

		void Sync::set_stop_atk()
		{
			_is_stop_atk = true;
		}

		void Sync::set_nonstop_atk()
		{
			_is_stop_atk = false;
		}

		bool Sync::is_stop_atk()
		{
			return _is_stop_atk;
		}

		boost::recursive_mutex& Sync::get_mutex()
		{
			return _mutex;
		}

		void Sync::set_block( DestroyBlock& destroy_block )
		{
			assert( &destroy_block
				&& "Sync::set_block:destory_block无效" );

			_destroy_block_ptr = &destroy_block;
			_is_set_block = true;
		}

		void Sync::unset_block()
		{
			_is_set_block = false;
		}

		bool Sync::is_set_block()
		{
			return _is_set_block;
		}

		DestroyBlock& Sync::get_destroy_block()
		{
			assert( _destroy_block_ptr
				&& "Sync::get_destroy_block:_destroy_block_ptr指向空指针" );

			return *_destroy_block_ptr;
		}

		//	初始状态为不攻击
		bool Sync::_is_stop_atk = true;
		boost::recursive_mutex Sync::_mutex;
		bool Sync::_is_set_block = false;
		DestroyBlock* Sync::_destroy_block_ptr = 0;

		//	评分
	
		bool Sync::is_find_score()
		{
			//trace( "is_find_score" );
			//trace( _is_find_score );
			return _is_find_score;
		}

		void Sync::set_find_score()
		{
			//trace( "set_find_score" );	
			_is_find_score = true;	
		}

		void Sync::unset_find_score()
		{
			//trace( "unset_find_score" );	
			_is_find_score = false;
		}

		bool Sync::_is_find_score = false;


		//	拾取
		bool Sync::is_stop_pick_up()
		{
			return _stop_pick_up;
		}

		void Sync::set_stop_pick_up()
		{
			_stop_pick_up = true;
		}

		void Sync::unset_stop_pick_up()
		{
			_stop_pick_up = false;
		}

		bool Sync::_stop_pick_up = false;

		//	是否在boss房间
		bool Sync::is_in_boss_room()
		{
			return _is_in_boss_room;
		}

		void Sync::set_in_boss_room()
		{
			_is_in_boss_room = true;
		}

		void Sync::unset_in_boss_room()
		{
			_is_in_boss_room = false;
		}

		bool Sync::_is_in_boss_room = false;


_DUNGEON_END