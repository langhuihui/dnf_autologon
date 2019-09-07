#ifndef __TEST_KEY_H__
#define __TEST_KEY_H__

#include <world.h>
#include <log.h>
#include <random.h>

#pragma comment( lib, "world.lib" )

#define _DllExport __declspec(dllimport)

using namespace _DNF;

class TestKey{
public:
	TestKey();

	void run();

	void test_key_speed( size_t vk );

	void test_basic();

	void test_move();
	
	void basic_key_test( long vk, size_t sleep_time );
	void basic_test( size_t sleep_time, size_t check_offset );

	bool check_pos( size_t offset );



	//////////////////
	void test_random_move( size_t x1, size_t y1, size_t x2, size_t y2 );


	void test_windows_key();

private:
	Dm _dm;
	AttachIme _attach_ime;
	World _world;

	Pos _last_pos;
};

#endif