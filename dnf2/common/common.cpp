#include "common.h"

_DNF_BEGIN

bool block_call( boost::function<bool()> condition, 
	size_t time_limit, 
	size_t sleep_time )
{
	boost::timer out_timer;

	//time_limit = time_limit * 1000;
	sleep_time = sleep_time * 20;

	while( true )
	{
		if( condition() )
			return true;

		//cout << out_timer.elapsed() << endl;
		if( out_timer.elapsed() > time_limit )
		{
			assert( false
				&& "block_call:timeout" );
			return false;
		}

		boost::this_thread::sleep( boost::posix_time::millisec( sleep_time ) );
	}

	return true;
}


void null()
{
}

bool block_call(
	boost::function<bool()> condition,
	boost::function<void()> before_action,
	boost::function<void()> after_action,
	double limit_time,
	size_t sleep_time,
	bool has_assert
	)
{
	boost::timer out_timer;
	while( true )
	{
		before_action();
		if( condition() )
			return true;
		after_action();
	
		if( out_timer.elapsed() > limit_time )
		{
			if( has_assert )
				assert( false
					&& "block_call:³¬Ê±" );

			return false;
		}

		boost::this_thread::sleep( 
			boost::posix_time::millisec( 
			sleep_time ) );
	}
}


bool block_call_before_action(
	boost::function<bool()> condition,
	boost::function<void()> before_action,
	boost::function<void()> after_action,
	double limit_time,
	size_t sleep_time,
	bool has_assert
	)
{
	boost::timer out_timer;
	while( true )
	{
		before_action();

		boost::this_thread::sleep( 
			boost::posix_time::millisec( 
			sleep_time ) );

		if( condition() )
			return true;

		after_action();
	
		if( out_timer.elapsed() > limit_time )
		{
			if( has_assert )
				assert( false
					&& "block_call:³¬Ê±" );

			return false;
		}
	}
}


//
//bool block_call(
//	bool condition,
//	boost::function<void()> before_action,
//	boost::function<void()> after_action,
//	size_t limit_time,
//	size_t sleep_time
//	)
//{
//	boost::timer out_timer;
//	while( true )
//	{
//		before_action();
//		if( condition )
//			return true;
//		after_action();
//	
//		if( out_timer.elapsed() > limit_time )
//		{
//			assert( false
//				&& "block_call:³¬Ê±" );
//			return false;
//		}
//
//		boost::this_thread::sleep( 
//			boost::posix_time::millisec( 
//			sleep_time ) );
//	}
//}


_DNF_END