

#include <world.h>
#pragma comment( lib, "world.lib" )

#include <attach_ime.h>
#pragma comment( lib, "attach_ime.lib" )


int main()
{
	using namespace _DNF;


	AttachIme attach_ime;

	attach_ime.install();

	//attach_ime.set_ime();

	//attach_ime.set_en();

	//attach_ime.init();

	//Dm dm;
	//World world( dm, attach_ime );
	//
	//while( true )
	//{
	//	world.update_player_map_pos();
	//	cout << world.player.map_pos.x << " "
	//		<< world.player.map_pos.y << endl;

	//	Sleep( 1 );
	//}



//	system( "pause" );

	return 0;
}