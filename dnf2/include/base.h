#ifndef __BASE_H__
#define __BASE_H__

#include <string>
#include <sstream>
#include <vector>
#include <cassert>
using namespace std;

#pragma warning(disable:4273)
#pragma warning(disable:4005)

//	全静态编译
//#define _STATIC_BUILD
#if defined _STATIC_BUILD
#define _DllExport 
#endif

//	全动态编译
#ifndef _STATIC_BUILD
#if defined BASE_EXPORTS//_USRDLL //|| defined _AFXDLL
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif
#endif



#define _DNF_BEGIN						namespace dnf{ 
#define _DNF_END						}
#define _DNF							dnf

#define _GAME_BEGIN		_DNF_BEGIN		namespace game{
#define _GAME_END		_DNF_END		}
#define _GAME			_DNF			::game

#define _MAP_BEGIN		_GAME_BEGIN		namespace map{
#define _MAP_END		_GAME_END		}
#define _MAP			_GAME			::map	

#define _ROOM_BEGIN		_MAP_BEGIN		namespace room{
#define _ROOM_END		_MAP_END		}
#define _ROOM			_MAP			::room

#define _DUNGEON_BEGIN	_GAME_BEGIN		namespace dungeon{
#define _DUNGEON_END	_GAME_END		}
#define _DUNGEON		_GAME			::dungeon

#define _ATTACK_BEGIN	_GAME_BEGIN		namespace attack{
#define _ATTACK_END		_GAME_END		}
#define _ATTACK			_GAME			::attack

#define _MOVE_BEGIN		_GAME_BEGIN		namespace move{
#define _MOVE_END		_GAME_END		}
#define _MOVE			_GAME			::move

#define _SPECIAL_BEGIN	_DNF_BEGIN		namespace special{
#define _SPECIAL_END	_DNF_END		}
#define _SPECIAL		_DNF			::special

#define _WATCH_BEGIN	_DNF_BEGIN		namespace watch{
#define _WATCH_END		_DNF_END		}
#define _WATCH			_DNF			::watch



_DNF_BEGIN

__int64 _DllExport str2int64( const string& str );
int _DllExport str2int( const string& str );

string _DllExport int2str( int i );
string _DllExport __int642str( __int64 i );

//	str中只允许出现在check_letter中出现的字符
bool _DllExport check_only_letter( const string& check_letter, const string& str );

//	str中不允许出现在check_letter中出现的字符
bool _DllExport check_no_letter( const string& check_letter, const string& str );

/*
wstring AnsiToUnicode(const string& buf);

string UnicodeToAnsi(const wstring& buf);

wstring Utf8ToUnicode(const string& buf);

string UnicodeToUtf8(const wstring& buf);

string AnsiToUtf8( const string& buf );

string Utf8ToAnsi( const string& buf );
*/

_DNF_END

#endif