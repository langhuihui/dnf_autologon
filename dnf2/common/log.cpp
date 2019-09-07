#include "log.h"


_DNF_BEGIN


///////////////////log file///////////////////////////////
ofstream Logger::g_log_file = ofstream( "g_log.log" ,ios::app );
boost::mutex Logger::g_log_mutex;

///////////////////trace cout/////////////////////////////
boost::mutex Logger::trace_mutex;
boost::mutex Logger::print_mutex;
boost::timer Logger::g_timer = boost::timer();
//boost::timer Logger::print_timer = boost::timer();

bool Logger::is_open_console = false;

//bool Logger::open_console = Logger::open_console();

_DNF_END