#include <dungeon.h>

#pragma comment( lib, "dungeon.lib" )

#define _DllExport __declspec(dllimport)

using namespace _DNF;

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    //_CrtSetBreakAlloc(679);
	Dm dm;
	dm.open_console();

	AttachIme attach_ime;

	World world( dm, attach_ime );

	dm.for_dnf_game_wnd();

	std::system( "pause" );
	dm.active_wnd();
	for( size_t i = 0; i < 10; i++ )
		dm.switch_ime();

	_DUNGEON::Dungeon dungeon( world );
	dungeon.run();


	return 0;
}