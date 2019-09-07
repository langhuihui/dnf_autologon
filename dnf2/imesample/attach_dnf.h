#ifndef __ATTACH_DNF_EX_H__
#define __ATTACH_DNF_EX_H__

#include <Windows.h>
#include <process.h>
#include <TlHelp32.h>


#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include <boost/thread.hpp>
#include <boost/array.hpp>

#include "def_name.h"

#define CharacterAddr		___0001
#define	start_set_player	___0002
#define	start_set_target	___0003
#define	end_set_player		___0004
#define	end_set_target		___0005
#define	err_code			___0006
#define	player_data_arr		___0007
#define	target_data_arr		___0008
#define	target_size			___0009


class Data{
public:
	Data()
		:x( 0 ),
		y( 0 ),
		z( 0 ),
		id( 0 )
	{}

	int x;
	int y;
	int z;
	int id;
};

void set_player_data(ostringstream& oss);
void set_target_data(ostringstream& oss);

void dnf_loop();


void attach_dnf();
bool check_process( const string& process_name );
void show( const string& str );

//-----------------------------------------------
//void get_player_mem( Data& data );
//void get_target_mem( vector<Data>& target_group );
//void show_target_pos();
//void show_player_pos();
//void loop();
//-----------------------------------------------

#define _DllExport __declspec(dllexport)

extern "C" 
{
	_DllExport extern long CharacterAddr;
	_DllExport extern bool start_set_player;
	_DllExport extern bool start_set_target;
	_DllExport extern bool end_set_player;
	_DllExport extern bool end_set_target;
	_DllExport extern int err_code;
	_DllExport extern int player_data_arr[ 4 ];
	_DllExport extern int target_data_arr[ 4000 ];
	_DllExport extern long target_size;
}

#endif