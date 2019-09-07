#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/*
WM_USER == 1024
#define WM_MYMESSAGE_UNUSUAL WM_USER+100						1124
#define WM_MYMESSAGE_COMPLETE WM_USER+101						1125
#define WM_MYMESSAGE_PLUSE WM_USER+102							1126
#define WM_MYMESSAGE_SAFEMODE  WM_USER+103						1127
#define WM_MYMESSAGE_THIRDPART  WM_USER+104						1128
#define WM_MYMESSAGE_FORBIDDEN  WM_USER+105						1129	
#define WM_MYMESSAGE_BLOCK  WM_USER+106							1130
#define WM_MYMESSAGE_RESTARTWITHOUTLOGIN WM_USER+107			1131
#define 角色切换 108											1132
#define 游戏崩溃(无条件重启) 109								1133
*/

#include <base.h>
#pragma comment( lib, "base.lib" )

#include <log.h>
#pragma comment( lib, "common.lib" )

#ifdef WORLD_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

#include <string>
using namespace std;



#include <Windows.h>

_DNF_BEGIN

class _DllExport Message{
	Message();
public:

	static HWND get_daemon_hwnd();
	static bool set_daemon_hwnd_from_deamon(
		HWND hwnd );


	static void unusual();
	static void complete();
	//	restart_exe == 1 启动 dnf2
	//	restart_exe == 2 启动 dnf2_no_login
	static void pluse( size_t limit_time, int restart_exe );
	static void safemode();
	static void thirdpart();
	static void forbidden();
	static void block();
	static void restart_without_login();
	static void switch_character();	
	static void restart_all();

private:
	static void send( int msg, int w = 0, int l = 0 );

private:

	static HWND _daemon_hwnd_from_daemon;
	static HWND _daemon_hwnd;
	static string _daemon_wnd_name;
};

_DNF_END

#endif