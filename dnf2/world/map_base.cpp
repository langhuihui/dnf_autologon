#include "map_base.h"

_DNF_BEGIN
/////////////////////////////////////////////////
//				Pos
//
Pos::Pos( int x_, int y_, int z_ )
	:x( x_ ),
	y( y_ ),
	z( z_ )
{
	assert( x_ > -10000 && x_ < 10000
		&& "Pos.x参数超出范围" );
	assert( y_ > -10000 && y_ < 10000
		&& "Pos.x参数超出范围" );
	assert( x_ > -10000 && x_ < 10000
		&& "Pos.x参数超出范围" );
}

Pos::Pos( const Pos& pos )
{
	assert( pos.x > -10000 && pos.x < 10000
		&& "Pos.x参数超出范围" );
	assert( pos.y > -10000 && pos.y < 10000
		&& "Pos.x参数超出范围" );
	assert( pos.z > -10000 && pos.z < 10000
		&& "Pos.x参数超出范围" );

	x = pos.x;
	y = pos.y;
	z = pos.z;
}

Pos& Pos::operator=( const Pos& pos )
{
	assert( pos.x > -10000 && pos.x < 10000
		&& "Pos.x参数超出范围" );
	assert( pos.y > -10000 && pos.y < 10000
		&& "Pos.x参数超出范围" );
	assert( pos.z > -10000 && pos.z < 10000
		&& "Pos.x参数超出范围" );

	x = pos.x;
	y = pos.y;
	z = pos.z;

	return *this;
}




bool Pos::operator<( const Pos& pos ) const
{
	//	map使用的operator<
	//	来源于pair的operator<
	return ( x < pos.x ||
		!( pos.x < x ) && y < pos. y );
}


bool Pos::operator==( const Pos& pos ) const
{
	if( x == pos.x &&
		y == pos.y )
		return true;
	else
		return false;
}


bool Pos::operator!=( const Pos& pos ) const
{
	return !operator==( pos ); 
}

void Pos::corrent_assert() const
{
	//assert( x > -10000 && x < 10000
	//	&& "Pos.x参数超出范围" );
	//assert( y > -10000 && y < 10000
	//	&& "Pos.x参数超出范围" );
	//assert( z > -10000 && z < 10000
	//	&& "Pos.x参数超出范围" );

	assert( x >= 0 && x < 3000
		&& "Pos.x参数超出范围" );
	assert( y >= 0 && y < 3000
		&& "Pos.x参数超出范围" );
	assert( z >= -100 && z < 3000
		&& "Pos.x参数超出范围" );
}

void Pos::init_assert() const
{
	assert( is_init()
		&& "Pos::init_assert:pos没有初始化" );
}

void Pos::invalid_assert() const
{
	assert( !is_invaild()
		&& "Pos::invalid_assert:pos无效" );
}

bool Pos::is_invaild() const
{
	if( -1 == x
		|| -1 == y )
		return true;
	else
		return false;
}

bool Pos::is_init() const
{
	if( 0 == x
		&& 0 == y )
		return false;
	else
		return true;
}

void Pos::set_invaild()
{
	x = -1;
	y = -1;
}

void Pos::set_init()
{
	x = 0;
	y = 0;
}


float compute_distance( const Pos& first, const Pos& second )
{
	int x = first.x - second.x;
	int y = first.y - second.y;

	return sqrt( (float)(x*x + y*y) );
}


/*
//////////////////////////////////////////////
//	房间节点特征码类
RoomID::RoomID()
{}

RoomID::RoomID( const vector<Pos>& block_pos )
{
	set( block_pos );
}

void RoomID::set( const vector<Pos>& block_pos )
{
	boost::uuids::detail::sha1 sha1;
	ostringstream oss;
	boost::archive::text_oarchive oa( oss );
	oa << block_pos;
	sha1.process_bytes( oss.str().c_str(), oss.str().size() );

	uint32_t digest_arr[ 5 ];

	sha1.get_digest( digest_arr );

	copy( digest_arr, digest_arr + 5, digest.begin() ); 
}


bool RoomID::operator!=( const RoomID& room_id )
{
	for( size_t i = 0; i < digest.size(); i++ )
		if( digest[ i ] != room_id.digest[ i ] )
			return true;
	return false;
}


bool RoomID::operator==( const RoomID& room_id )
{
	return !operator!=( room_id ); 
}
*/


RoomID::RoomID()
{
	
}

RoomID::RoomID( const vector<Pos>& block_pos )
{
	_assert( block_pos );

	set( block_pos );
}

void RoomID::set( const vector<Pos>& block_pos )
{
	_assert( block_pos );

	_block_pos = block_pos;
}

RoomID::RoomID( const RoomID& room_id )
{
	_assert( room_id._block_pos );

	_block_pos = room_id._block_pos;	
}
RoomID& RoomID::operator=( const RoomID& room_id )
{
	_assert( room_id._block_pos );

	_block_pos = room_id._block_pos;

	return *this;
}

void RoomID::_assert( const vector<Pos>& block_pos )
{
	/*
	assert( !block_pos.empty()
		&& "RoomID::RoomID( const \
		   vector<Pos>& block_pos )\
		   ->block_pos为空" );

#ifdef _DEBUG
	for( size_t i = 0; i < block_pos.size(); i++ )
		if( !block_pos[ i ].x
			&& !block_pos[ i ].y )
			assert( 0 
			&& "RoomID::RoomID( const \
			   vector<Pos>& block_pos )\
			   ->block_pos中有元素的x和y同时为0" );
#endif 
			   */
}

//	如何区分两个房间是否是同一个地图
//	首先，需要对比的地图
//	必然是同一个副本中
//
//	同一个地图中出现两个房间的id相同的几率会很小。
//
//	坐标比较的位置是相同的。
//	
bool RoomID::operator!=( const RoomID& room_id ) const
{
	if( room_id._block_pos.empty() 
		|| _block_pos.empty() )
		return true;

#ifdef _DEBUG
	bool all_empty = true;
	for( size_t i = 0; i < _block_pos.size(); i++ )
	{
		_block_pos[ i ].corrent_assert();
		if( _block_pos[ i ].x != 0
			&& _block_pos[ i ].y != 0 )
			all_empty = false;
	}

	assert( !all_empty
		&& "room_id全为空" );

	all_empty = true;
	for( size_t i = 0; i < room_id._block_pos.size(); i++ )
	{
		_block_pos[ i ].corrent_assert();
		if( room_id._block_pos[ i ].x != 0
			&& room_id._block_pos[ i ].y != 0 )
			all_empty = false;
	}

	assert( !all_empty
		&& "room_id全为空2" );
#endif

	const vector<Pos>* first;
	const vector<Pos>* second;
	if( _block_pos.size() >= room_id._block_pos.size() )
	{
		first = &_block_pos;
		second = &room_id._block_pos;
	}
	else
	{
		first = &room_id._block_pos;
		second = &_block_pos;
	}

	int size = second->size();
	int match_size = 0;
	for( size_t i = 0; i < second->size(); i++ )
	{
		//	如果second里面有一个元素在first元素里面找不到.
		//	那么second就不是first的子集
		if( find( first->begin(), first->end(), (*second)[ i ] ) != first->end() )
			match_size++;
	}
	
	float match_scale = (float)match_size / (float)size;

	if( match_scale < 0.6 )
		return true;
		

	return false;
}


bool RoomID::operator==( const RoomID& room_id ) const
{
	return !operator!=( room_id ); 
}


/////////////////////////////////////////////////
//				Room
//
Room::Room()
{
	init();
}

Room::Room( const Pos& pos_ )
	:pos( pos_ )
{
	init();
}

void Room::init()
{

}

bool Room::operator<( const Room& room ) const
{
	return pos < room.pos;
}

bool Room::operator==( const Room& room ) const
{
	if( pos == room.pos 
		&& room_id == room.room_id )
		return true;
	else
		return false;
}

bool Room::operator!=( const Room& room ) const 
{
	return !operator==( room );
}

bool Room::same_pos_diff_room_id( const Room& room ) const
{
	if( pos == room.pos 
		&& room_id != room.room_id )
		return true;
	else
		return false;
}

bool Room::same_pos( const Room& room ) const 
{
	if( pos == room.pos )
		return true;
	else
		return false;
}

bool Room::set_room_id( const vector<Pos>& block_pos )
{
	if( block_pos.empty() )
		return false;
	room_id.set( block_pos );
	return true;
}




void Room::clear()
{
	pos = Pos();
	room_id = RoomID();
}


//////////////////////////////////////////////////

Road::Road()
	:weight( 1 ),
	dir( Dir::no )
{
}


bool Road::operator==( const Road& road ) const
{
	if( road.dir == dir 
		&& road.weight == road.weight )
		return true;
	else
		return false;
}

bool Road::operator!=( const Road& road ) const
{
	return !operator==( road );
}

_DNF_END