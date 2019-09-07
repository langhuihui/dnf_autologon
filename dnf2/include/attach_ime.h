#ifndef __ATTACH_IME_H__
#define __ATTACH_IME_H__

#include <Windows.h>

#pragma comment( lib, "imm32.lib" )
#pragma comment( lib, "user32.lib" )

#include <vector>
#include <string>
#include <cassert>
#include <exception>
using namespace std;

#include <boost/array.hpp>
#include <boost/thread.hpp>

#include <base.h>
#pragma comment( lib, "base.lib" )


#ifndef _STATIC_BUILD
#ifdef ATTACH_IME_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif
#endif

//#include "map_base.h"
//#include "world.h"
//
//class Player;
//class World;
//class Target;
//class Obj;
//class Enemy;

_DNF_BEGIN

class _DllExport _Data{
public:
	_Data()
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

class _DllExport AttachIme{
public:
	AttachIme();

	void init();
	
	HKL find_ime_hkl();
	HKL find_en_hkl();
	void install();
	void set_ime();
	void set_en();

	const _Data& update_player_data();
	//void update_player_data( Player& player );
	
	//	target_size返回数组大小
	//	整个函数返回data的引用
	//	这里把数组换成了指针
	const _Data* update_target_data( size_t& target_size );
	//void update_target_data( vector<Target>& TargetVec );

public:

	boost::mutex player_mutex;
	boost::mutex target_mutex;

	long base_addr;//0x1FA6978

	long* CharacterAddrPtr;


	bool* start_set_player_ptr;
	bool* start_set_target_ptr;
	bool* end_set_player_ptr;
	bool* end_set_target_ptr;


	//	0  == 无异常
	//	-1 == 内存读取异常
	//	-2 == target_size > 1000
	int* err_code_ptr;		
	_Data* player_data_ptr;
	typedef _Data (*target_data_ptr_t)[1000];
	target_data_ptr_t target_data_ptr;
	//boost::array<_Data, 1000>* target_data_ptr;
	long* target_size_ptr;


	string ime_path;
	string ime_name;


#ifdef _DEBUG
	size_t err_code_cnt;
#endif 
};

_DNF_END

#endif