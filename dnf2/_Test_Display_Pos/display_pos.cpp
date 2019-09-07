#include "display_pos.h"




PosDisplayer::PosDisplayer( Dm& dm, AttachIme& attach_ime_  )
	:world( dm, attach_ime_ )
{
	init();
	main();
}

void PosDisplayer::init()
{
	open_console();
}

void PosDisplayer::main()
{

	while( true )
	{
		world.update_all();
		display();
		Sleep( 1000 );
	}
}

void PosDisplayer::open_console()
{
	//::FreeConsole();
	::AllocConsole();
	FILE* pf;                                                   
	::freopen_s(&pf,"CONOUT$","w",stdout);   // 重定向输出
//	::freopen_s(&pf,"CONERR$","w",stdout);   // 重定向输出
	::freopen_s(&pf,"CONIN$","r",stdin);   // 重定向输出

}

void PosDisplayer::display( ostream& os )
{

	::system( "cls" );

	os << "======================" << endl;

	os << "pos:" << world.player.map_pos.x << ","
		<< world.player.map_pos.y << ","
		<< world.player.map_pos.z << endl;

	os << "mouse:" << world.mouse_pos.x << ","
		<< world.mouse_pos.y << endl;

	os << "lv:" << world.player.win_pos.x << "," 
		<< world.player.win_pos.y << endl;

	os << "map:" << world.map_pos.x << ","
		<< world.map_pos.y << endl;

	os << "boss:" << world.boss_room_pos.x << ","
		<< world.boss_room_pos.y << endl;

	os << "======================" << endl;

	os << "Num:" << world.target_group.size() << endl;

	for( size_t i =0 ; i < world.target_group.size(); i++)
	{
		os << i << ":" << world.target_group[ i ].id
			<< "->(" 
			<< world.target_group[ i ].map_pos.x << "," 
			<< world.target_group[ i ].map_pos.y << ","
			<< world.target_group[ i ].map_pos.z
			<< ")|("
			<< world.target_group[ i ].win_pos.x << ","
			<< world.target_group[ i ].win_pos.y << ","
			<< world.target_group[ i ].win_pos.z
			<< ")" << endl;
	}

	os << "======================" << endl;
}