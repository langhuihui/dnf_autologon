#ifndef __ARCHIVE_MAP_H__
#define __ARCHIVE_MAP_H__

#include <fstream>
using namespace std;

//////////////////////////////////////////
//	文件格式
//	原始数据.dat
//	障碍物坐标数据
//	原始数据+_block.data
//	序列化的图结构.graph
//	graphviz结构.dot

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/bitset.hpp>

#include <boost/graph/adj_list_serialize.hpp>

using namespace boost;

#include <base.h>
#pragma comment( lib, "base.lib" )

#ifdef WORLD_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

_DNF_BEGIN

class Pos;
class Road;
class RoomID;
class Room;
class Door;
class MiniMap;

class NodeColor;
class Node;
struct Distance;
class RoomNode;

_DNF_END


namespace boost {
	namespace serialization {

		using namespace _DNF;

		template<class Archive>
		void serialize(Archive & ar, Pos& pos, const unsigned int version)
		{
			ar & pos.x;
			ar & pos.y;
		}

		template<class Archive>
		void serialize(Archive & ar, Road& road, const unsigned int version)
		{
			ar & road.dir;
			ar & road.weight;
		}

		template<class Archive>
		void serialize(Archive & ar, RoomID& room_id, const unsigned int version)
		{
			ar & room_id._block_pos;
		}

		//template<class Archive>
		//void serialize(Archive & ar, ::Room& room, const unsigned int version)
		//{
		//	ar & room.pos;
		//	ar & room.room_id;
		//}

		//template<class Archive>
		//void serialize(Archive & ar, ::Door& door, const unsigned int version)
		//{
		//	ar & door.start;
		//	ar & door.end;
		//	ar & door.weight;
		//}

		/*
		template<class Archive>
		void serialize(Archive & ar, MiniMap& mini_map, const unsigned int version)
		{
			ar & mini_map._g;
		}
		*/

		
		template<class Archive>
		void serialize(Archive & ar, NodeColor& node_color, const unsigned int version)
		{
			ar & node_color.color;
		}
		//
		//template<class Archive>ii
		//void serialize(Archive & ar, ::Node& node, const unsigned int version)
		//{
		//	ar & node.pos;
		//	ar & node.node_color;
		//}
		
		template<class Archive>
		void serialize(Archive & ar, Distance& distance, const unsigned int version)
		{
			ar & distance.value;
		}



	} // namespace serialization
} // namespace boost;




#endif