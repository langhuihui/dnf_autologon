#include "room_recorder.h"

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
	dm.for_dnf_game_wnd();

	_ROOM::RoomRecorder( dm, attach_ime );

	return 0;
}