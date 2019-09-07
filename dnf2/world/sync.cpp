#include "sync.h"

_DNF_BEGIN

	Sync::Sync()
{}

void Sync::clear()
{
	//_login = false;
	_game = false;
	_town = false;
	_dungeon = false;

}
void Sync::set_game()
{
	_game = true;
}
void Sync::unset_game()
{
	_game = false;
}
bool Sync::game()
{
	return _game;
}
void Sync::set_town()
{
	_town = true;
}
void Sync::unset_town()
{
	_town = false;
}
bool Sync::town()
{
	return _town;
}
void Sync::set_dungeon()
{
	_dungeon = true;
}
void Sync::unset_dungeon()
{
	_dungeon = false;
}

bool Sync::dungeon()
{
	return _dungeon;
}

void Sync::set_score()
{
	_score = true;
}

void Sync::unset_score()
{
	_score = false;
}

bool Sync::score()
{
	return _score;
}

bool Sync::_game = false;
bool Sync::_town = false;
bool Sync::_dungeon = false;
bool Sync::_score = false;

/////////////////////


void Sync::set_start_watch()
{
	_start_watch = true;
}

void Sync::set_end_watch()
{
	_start_watch = false;
}

bool Sync::is_start_watch()
{
	return _start_watch;
}

bool Sync::_start_watch = false;

////////////

void Sync::set_stop_atk()
{
	_is_stop_atk = true;
}

void Sync::set_nonstop_atk()
{
	_is_stop_atk = false;
}

bool Sync::is_stop_atk()
{
	return _is_stop_atk;
}

bool Sync::_is_stop_atk = false;

//////////

void Sync::set_stop_move()
{
	_is_stop_move = true;
}

void Sync::set_nonstop_move()
{
	_is_stop_move = false;
}

bool Sync::is_stop_move()
{
	return _is_stop_move;
}

bool Sync::_is_stop_move = false;

///////////////////

void Sync::set_stop_all()
{
	_is_stop_move = true;
	_is_stop_atk = true;
}

void Sync::set_nonstop_all()
{
	_is_stop_move = false;
	_is_stop_atk = false;
}

bool Sync::is_stop_all()
{
	if( _is_stop_move
		&& _is_stop_atk )
		return true;
	else
		return false;
}

bool Sync::is_nonstop_all()
{
	return !is_stop_all();
}

/////////////////////

void Sync::set_safe_mode()
{
	_is_safe_mode = true;
}

bool Sync::is_safe_mode()
{
	return _is_safe_mode;
}

bool Sync::_is_safe_mode = false;

////////////////////////////

bool Sync::is_return_town()
{
	return _is_return_town;
}

void Sync::set_return_town()
{
	_is_return_town = true;
}

bool Sync::_is_return_town = false;

/////////////////////////////
void Sync::set_current_map_id( const string& map_id )
{
	_current_map_id = map_id;
}
string Sync::get_current_map_id()
{
	return _current_map_id;
}
string Sync::_current_map_id = string();

void Sync::set_current_room_id( const string& room_id )
{
	_current_room_id = room_id;
}
string Sync::get_current_room_id()
{
	return _current_room_id;
}
string Sync::_current_room_id = string();


_DNF_END