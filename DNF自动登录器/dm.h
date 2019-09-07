#ifndef __DM_H__
#define __DM_H__


#include "Cdmsoft.h"
#include "Dnf.h"

#include <windows.h>

#include <cassert>
#include <string>
#include <cstdint>
using namespace std;



class Dm{
public:
	Dm();
	~Dm();

	void init();

	long reg();

	uint32_t reg_dll();

	bool init_dm();

	void show_ver();

	void set_wnd_name( const string& wnd_name_ );

	void find_wnd();

	long bind_wnd();
	long unbind_wnd();

	void for_dnf_game_wnd();
	void unbind_dnf();

	void _test();
	void _test_key( int sleep_time );

public:
	Cdmsoft _dm;

	CDnf _cdnf;

	string reg_code;

	string wnd_name;

	HWND wnd_hwnd;
	

};



#endif