#include <game.h>

#pragma comment( lib, "game.lib" )

#define _DllExport __declspec(dllimport)

using namespace _DNF;

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	Dm dm;
	dm.open_console();

	AttachIme attach_ime;

	World world( dm, attach_ime );

	dm.for_dnf_game_wnd();

	std::system( "pause" );
	dm.active_wnd();

	_GAME::Game game( world, "0" );
	game.game();

	game.run();

	return 0;
}