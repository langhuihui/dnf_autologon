
#ifndef __LOG_H__
#define __LOG_H__


#include <iostream>
#include <fstream>
using namespace std;


#include <boost/timer.hpp>
#include <boost/thread.hpp>

#include <boost/date_time.hpp>

//#include <base.h>
//#pragma comment( lib, "base.lib" )
#include "common.h"

_DNF_BEGIN

class _DllExport Logger{
public:
	Logger( const string& file_name = "log.log" )
		:log_file( file_name ),_last_time( 0 )
	{

	}


	bool set_file( const string& file_name )
	{
		log_file.close();
		log_file.open( file_name );
		if( !log_file )
			return false;
		
		return true;
	}

	void close()
	{
		log_file.close();
	}

	~Logger()
	{
		log_file.close();
	}

	template <typename Type>
	ostream& operator<<( const Type& t )
	{
#ifdef	_DEBUG
		_current_time = timer.elapsed();
		log_file 
			<< _current_time
			<< "\t"
			<< _current_time - _last_time
			<< "\t"
			<< t;

		_last_time = _current_time;
#endif
		return log_file;
	}

public:
	double _last_time;
	double _current_time;

	ofstream log_file;
	boost::timer timer;
public:

	///////////////////log file///////////////////////////////

	template <typename Type>
	static void log( const Type& t, int level = 0 )
	{

		//#ifdef	_DEBUG
		if( level >= 0 )
		{
			boost::posix_time::ptime current_time( 
				boost::posix_time::second_clock::local_time() );

			//boost::mutex::scoped_lock lock( g_log_mutex );
			g_log_file
				<< current_time 
				<< '\t'
				<< g_timer.elapsed()
				<< "\t"
				<< boost::this_thread::get_id()
				<< "\t"
				<< t
				<< endl;
		}
		//#endif
	}

	static ofstream g_log_file;
	static boost::mutex g_log_mutex;

	///////////////////trace cout/////////////////////////////i
public:

	static bool open_console()
	{
		if( !is_open_console )
		{
			//AllocConsole();
			//freopen("CON", "r", stdin );
			//freopen("CON", "w", stdout);
			//freopen("CON", "w", stderr);
			is_open_console = true;

			return true;
		}
		else
			return false;
	}

	template <typename Type>
	static void trace( const Type& t )
	{
//#if defined(_DEBUG) && _DEBUG
#ifdef	_DEBUG
		boost::mutex::scoped_lock lock( trace_mutex );
		cout 
			<< g_timer.elapsed()
			<< "\t"
			//<< boost::this_thread::get_id()
			//<< "\t"
			<< t
			<< endl;
#endif
	}

	template <typename Type>
	static void display_log( const Type& t, int level = 0 )
	{
		if( level >= 0 )
		{
			boost::posix_time::ptime current_time( 
				boost::posix_time::second_clock::local_time() );

			//cout << "test" << endl;

			boost::mutex::scoped_lock lock( print_mutex );
			cout
				<< current_time 
				<< '\t'
				<< g_timer.elapsed()
				<< "\t"
				<< boost::this_thread::get_id()
				<< "\t"
				<< t
				<< endl;
		}
	}

	static boost::mutex trace_mutex;
	static boost::mutex print_mutex;
	static boost::timer g_timer;
	//static boost::timer print_timer;
	static bool is_open_console;
	//static bool open_console;

public:
	template <typename Type>
	static void output( const Type& t, int level = 0 )
	{
		if( level >= 0 )
		{
			log( t );
			display_log( t );
		}
	}
};

#define trace Logger::trace
#define glog Logger::log
#define output Logger::output

_DNF_END

#endif