#ifndef __MAP_BASE_H__
#define __MAP_BASE_H__

#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

#include <base.h>
#pragma comment( lib, "base.lib" )

#include "archive_map.h"

#ifdef WORLD_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

_DNF_BEGIN

/////////////////////////////////////////////////
//			Pos

class _DllExport Pos{
public:
	Pos( int x_ = int(), int y_ = int(), int z_ = int() );
	Pos( const Pos& pos );
	Pos& operator=( const Pos& pos );

	void corrent_assert() const;
	void init_assert() const;
	void invalid_assert() const;

	bool operator<( const Pos& pos ) const;
	bool operator==( const Pos& pos ) const;
	bool operator!=( const Pos& pos ) const;

	bool is_invaild() const;
	bool is_init() const;

	void set_invaild();
	void set_init();

public:
	int x;
	int y;
	int z;
};

_DllExport float compute_distance( const Pos& first, const Pos& second );

/*
//////////////////////////////////////////////
//	����ڵ���������
class RoomID{
public:
	RoomID();
	RoomID( const vector<Pos>& block_pos );

	void set( const vector<Pos>& block_pos );

	bool operator!=( const RoomID& room_id );
	bool operator==( const RoomID& room_id );

public:
	std::array<uint32_t, 5> digest;
};
*/

//////////////////////////////////////////////
//	����ڵ���������
//	�ڲ�ֱ�Ӵ�vector<Pos> block_pos;
//	�Ժ�ıȽϵ�ʱ��,ֻҪblock_pos��ԭ����posֵ
//	�Ӽ�.��ôoperator== ���൱
//	��Ϊ�漰����������ܻ᲻ͬ
//	��ֹ����.
class _DllExport RoomID{
public:
	RoomID();
	RoomID( const vector<Pos>& block_pos );
	RoomID( const RoomID& room_id );
	RoomID& operator=( const RoomID& room_id );

	void _assert( const vector<Pos>& block_pos );

	void set( const vector<Pos>& block_pos );

	bool operator!=( const RoomID& room_id ) const;
	bool operator==( const RoomID& room_id ) const;
public:
	vector<Pos> _block_pos;
	
	/*
private:
	bool operator=( const RoomID& room_id );
	RoomID( const RoomID& room_id );
	*/
};

/////////////////////////////////////////////
//	
//	Room
//	
//	idϵͳ�Ѳ�ʹ��
//	ֱ�� �Ƚ�Pos
//
//	/*
//	�����ʱ�������һ��Ψһ��id; ��1��ʼ
//	�����͸�ֵ������id;
//	
//	��������id��̬������_id_cnt
//	���ᱻ���л�.
//	*/

class Room{
public:
	Room();
	Room( const Pos& pos );

	void init();
	bool operator<( const Room& room ) const;
	bool operator==( const Room& room ) const;
	bool operator!=( const Room& room ) const;
	bool same_pos_diff_room_id( const Room& room ) const;
	bool same_pos( const Room& room ) const;
	bool set_room_id( const vector<Pos>& block_pos ); 

	void clear();

public:
	Pos pos;
	RoomID room_id;
};


_DllExport enum Dir{
	no,
	top,
	right,
	bottom,
	left
};

class _DllExport Road{
public:
	Road();

	Dir dir;
	int weight;

	bool operator==( const Road& road ) const;
	bool operator!=( const Road& road ) const;
};

_DNF_END

#endif