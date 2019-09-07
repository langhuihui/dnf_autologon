#ifndef __MAIN_H__
#define __MAIN_H__

#include <game.h>
#pragma comment( lib, "game.lib" )

#include <AutoLogin.h>
#pragma comment( lib, "login.lib" )

#define _DllExport __declspec(dllimport)
using namespace _DNF;


class Main{
public:
	Main( 
		const std::string& qq, 
		const std::string& pwd, 
		const std::string& area,
		HWND daemon_hwnd
		);

	void run();

private:

	HWND _daemon_hwnd;
	string _qq;
	string _pwd;
	string _area;

	Dm _dm;
	AttachIme _attach_ime;
	World _world;
	AutoLogin _login;
	_GAME::Game _game;
};


#endif