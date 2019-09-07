#ifndef __TEST_DLL_H__
#define __TEST_DLL_H__

#include <game.h>
#include <login.h>
#include <message.h>
#include <dungeon.h>
#include <game.h>
#pragma comment( lib, "game.lib" )

#ifdef _TEST_DLL_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

using namespace _DNF;

class _DllExport Test{
public:
	Test();
	
	void main();
private:
	Dm _dm;
	AttachIme _attach_ime;
	World _world;
public:

	void test_dll_shell();

	void test_set_window();

	void test_create_character();

	void test_big_map();

	void test_skill();

	void test_select_character();

	void test_update_win_pos();

	void test_low_ui();

	void test_out_weight();

	void test_handle_opening_movie();

	void test_login();

	void test_skill_base();

	void test_win_pos();

	void test_sell();

	void test_select_player();

	void test_join();

	void test_select_ch();

	void test_new_join();
	
	void test_select_player2();

	void test_player_win_pos();

	void test_switch_player();

	void test_select_player3();

	void test_click_mouse();

	void test_open_esc();

	void test_boss_pos();

	void test_boss_pos2();

	void test_quit_game();

	void test_dm_gif();

	void test_ocr();

	void test_find_chair();

	void test_special();
};

#endif