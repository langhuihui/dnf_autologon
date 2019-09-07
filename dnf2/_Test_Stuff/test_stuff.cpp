#include <game.h>
#include <login.h>
#include <message.h>
#include <dungeon.h>

#pragma comment( lib, "game.lib" )

#define _DllExport __declspec(dllimport)

using namespace _DNF;

void test_study_skill()
{
	Dm dm;
	dm.open_console();

	AttachIme attach_ime;

	World world( dm, attach_ime );

	dm.for_dnf_game_wnd();

	_GAME::StudySkill study_skill( world );

	bool b = world.dnf.has_color_m( 172, 172 );

	while( true )
	{
		bool ret = study_skill._has_black_study();
		if( !ret )
			assert( false );
	}

	std::system( "pause" );

}

void test_create_name()
{
	using namespace _GAME;
//	string name = _GAME::Name::gen_new();
	
	Dm dm;
	dm.open_console();
	dm.for_dnf_game_wnd();

	AttachIme attach_ime;

	World world( dm, attach_ime );


	Login login( world, "" );
	while( true )
	{
		trace( login.is_can_not_create() );
		Sleep( 1000 );
	}

	

//	world.open_esc();
//	world.return_select_character();
	
//	Login login( world );
//	login.run();
	/*
	while( true )
	{
		Pos pos = login._random_ch();
	}
	*/
}

void test_quit_game()
{
	Dm dm;
	dm.open_console();
	AttachIme attach_ime;
	World world( dm, attach_ime );
	dm.for_dnf_game_wnd();
	bool ret = world.has_energy();
	world.quit_game();
}

void test_message()
{
	Message::complete();
}

void test_equipment()
{
	Dm dm;
	dm.open_console();
	dm.for_dnf_game_wnd();

	//vector<PosStr> pos_str_list;

	//dm._cdnf.GetPosStrList(
	//	pos_str_list,609,73,725,100,
	//	"e8cda4-050505"
	//	);

	//dm._cdnf.GetPosStrList(
	//	pos_str_list,566,95,675,119,
	//	"ffffff-000000"
	//	);

	//string str = dm._cdnf.dm.FindPicE(
	//	197,28,984,792, "equipment_locate.bmp",
	//	"000000", 1.0, 0 );

	AttachIme attach_ime;

	World world( dm, attach_ime );

	//while( true )
	//{
	//	//bool ret = world.dnf.has_pic(
	//	//	"seller.bmp",
	//	//	1,1,799,599 );
	//	string str = world.dnf.dm.FindPicE(
	//		1,1,799,599, "seller.bmp", "101010", 1.0, 0 );

	//	if( !str.empty() )
	//		trace( str );
	//}

	_DUNGEON::Equip equip( world );


	//equip.replace_and_sell();
	equip.replace_and_sell_equipment();
	//equip.sell_consumable();
	//equip.sell_stuff();

	//equip.replace_and_sell();

	//while( true )
	//{
	//	equip.switch_to_equipment();
	//	Sleep( 1000 );
	//	equip.switch_to_consumable();
	//	Sleep( 1000 );
	//	equip.switch_to_stuff();
	//	Sleep( 1000 );
	//}
	

//	equip.replace();


	//equip.set_chuan_dai();

	//equip.set_equip_pos();

	//vector<PosStr> equiped_str_list;
	//
	//dm._cdnf.GetPosStrList(
	//	equiped_str_list,
	//	equip._equiped_pos1.x,
	//	equip._equiped_pos1.y,
	//	equip._equiped_pos2.x,
	//	equip._equiped_pos2.y,
	//	"ffffff-000000"
	//	);

	//vector<PosStr> new_str_list;

	//dm._cdnf.GetPosStrList(
	//	new_str_list,
	//	equip._new_pos1.x,
	//	equip._new_pos1.y,
	//	equip._new_pos2.x,
	//	equip._new_pos2.y,
	//	"ffffff-000000"
	//	);
}

void test_is_out_weight()
{
	Dm dm;
	dm.open_console();
	dm.for_dnf_game_wnd();
	AttachIme attach_ime;

	World world( dm, attach_ime );
	while( true )
	{
		trace( "test_weight" );
		world.is_out_weight();

		Sleep( 1000 );
	}
}


void test_get_name()
{
	Dm dm;
	dm.open_console();
	dm.for_dnf_game_wnd();
	AttachIme attach_ime;

	World world( dm, attach_ime );

	trace( world.player.get_name() );
	std::system( "pause" );
}

void test_seller()
{
	Dm dm;
	dm.open_console();
	dm.for_dnf_game_wnd();
	AttachIme attach_ime;

	World world( dm, attach_ime );

	_DUNGEON::Equip equip( world );

	while( true )	
	{
		equip.set_npc_pos();
		Sleep( 10 );
	}
}

void test_has_process()
{
	Dm dm;
	dm.open_console();
	AttachIme attach_ime;

	World world( dm, attach_ime );

	while( true )
	{
		trace( world.has_process( "DNF.exe" ) );
		trace( world.has_process( "YY.exe" ) );
	}
}

void test_is_weak()
{
	Dm dm;
	dm.open_console();
	dm.for_dnf_game_wnd();
	AttachIme attach_ime;

	World world( dm, attach_ime );

	while( true )
	{
		Pos pos = world.weak_pos();
		trace( pos.x );
		trace( pos.y );

		Sleep( 1000 );
	}
}

void test_new_date()
{
	using namespace _GAME;
	
	Dm dm;
	dm.open_console();
//	dm.for_dnf_game_wnd();

	AttachIme attach_ime;
	World world( dm, attach_ime );

	Login login( world, "" );
	cout << login.is_out_date() << endl;;
	//login.set_new_date();
	::system( "pause" );
}

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow
)
{
	//test_study_skill();
	//test_create_name();
	//test_quit_game();
	//test_message();
	//test_equipment();
	test_is_out_weight();

	//test_get_name();
	//test_seller();

	//test_has_process();
	//test_is_weak();

	//test_new_date();

	return 0;
}