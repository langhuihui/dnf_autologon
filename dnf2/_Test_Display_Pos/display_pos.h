#ifndef __DISPLAY_POS_H__
#define __DISPLAY_POS_H__

#include <world.h>
#pragma comment( lib, "world.lib" )
using namespace _DNF;

#define _DllExport __declspec(dllimport)


#include <iostream>
using namespace std;


class PosDisplayer{
public:
	PosDisplayer( Dm& dm, AttachIme& attach_ime_  );
	void init();
	void main();
	void open_console();
	void display( ostream& os = cout );

public:
	World world;
};


#endif