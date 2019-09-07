#ifndef __COMMON_H__
#define __COMMON_H__

#include <base.h>
#pragma comment( lib, "base.lib" )

#ifndef _STATIC_BUILD
#ifdef COMMON_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif
#endif

#include <boost/timer.hpp>
#include <iostream>
using namespace std;

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>



_DNF_BEGIN
_DllExport bool block_call( boost::function<bool()> condition, 
	size_t time_limit = 60, 
	size_t sleep_time = 10 );

_DllExport void null();

//_DllExport bool block_call(
//	bool condition,
//	boost::function<void()> before_action ,
//	boost::function<void()> after_action,
//	size_t limit_time = 60,
//	size_t sleep_time = 10
//	);

_DllExport bool block_call(
	boost::function<bool()> condition,
	boost::function<void()> before_action ,
	boost::function<void()> after_action ,
	double limit_time = 60,
	size_t sleep_time = 10,
	bool has_assert = true
	);

_DllExport bool block_call_before_action(
	boost::function<bool()> condition,
	boost::function<void()>	before_action,
	boost::function<void()> after_action ,
	double limit_time = 60,
	size_t sleep_time = 10,
	bool has_assert = true
	);

//_DllExport bool block_call(
//	boost::function<bool()> condition,
//	
//	);



_DNF_END


#endif
