#ifndef __ROOM_H__
#define __ROOM_H__


#include <cassert>
#include <string>
#include <vector>
using namespace std;


#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/graph/adj_list_serialize.hpp>

#include <boost/graph/graphviz.hpp>
using namespace boost;

//#include "base.h"
//#include "archive_map.h"
//#include "map_base.h"
#include "graph.h"
//#include "world.h"

#include <sync.h>
#include <world.h>
#pragma comment( lib, "world.lib" )

#ifdef MAP_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

//
//namespace dnf{
//	namespace map{
//		namespace room{

_ROOM_BEGIN


			class Node;
			class Edge;
			class NodeMgr;

			class FPPoint;
			class RoomFingerPrint;
		
			class Door;
			class DoorMgr;
			class DestroyBlock;

			class Room;

_ROOM_END

//		}	//	room
//	}	//	map
//}	//	dnf


namespace boost {
	namespace serialization {

		using namespace _ROOM;

		template<class Archive>
		void serialize(Archive & ar, _ROOM::Node& node, const unsigned int version)
		{
			ar & node.pos;
			ar & node.type;
		}

		template<class Archive>
		void serialize(Archive & ar, Edge& edge, const unsigned int version)
		{
			ar & edge.weight;
		}

		template<class Archive>
		void serialize(Archive & ar, NodeMgr& node_mgr, const unsigned int version)
		{
			ar & node_mgr._g;
			ar & node_mgr._hash_code;
			ar & node_mgr._is_changed;
		}

		template<class Archive>
		void serialize(Archive & ar, FPPoint& ffpoint, const unsigned int version)
		{
			ar & ffpoint.id;
			ar & ffpoint.pos;
		}

		template<class Archive>
		void serialize(Archive & ar, RoomFingerPrint& rfp, const unsigned int version)
		{
			ar & rfp._1057_33_list;
			ar & rfp._comp_scale;
		}

		template<class Archive>
		void serialize(Archive & ar, _ROOM::Door& door, const unsigned int version)
		{
			ar & door.pos;
			ar & door.dir;
		}

		template<class Archive>
		void serialize(Archive & ar, DoorMgr& door_mgr, const unsigned int version)
		{
			ar & door_mgr._door_list;
		}

		template<class Archive>
		void serialize(Archive & ar, DestroyBlock& destroy_block, const unsigned int version)
		{
			ar & destroy_block._545_block_list;
			ar & destroy_block._1057_block_list;
			ar & destroy_block._33_block_list;
			ar & destroy_block._is_set;
		}

		template<class Archive>
		void serialize(Archive & ar, _ROOM::Room& room, const unsigned int version)
		{
			ar & room._rfp;
			ar & room._file_name;
			ar & room._door_mgr;
			ar & room._destroy_block;
			ar & room._node_mgr;
		}

	}	//	namespace serialization;
}	//	namespace boost;


//namespace dnf{
//	namespace map{
//		namespace room{

_ROOM_BEGIN
			using namespace _DNF;
			/////////////////////////////////////////////////////
			//	记录点类
			class _DllExport Node{
			public:
				Node();

				bool operator==( const Node& node ) const;
				bool operator!=( const Node& node ) const;

				void set_door_type();
				void set_record_type();

			public:
				Pos pos;

				//	类型标识
				//	0		表示默认类型
				//	1000	正常的记录点
				//	2000	门
				enum NodeType{
					unused = 0,
					record = 1000,

					door = 2000
				};

				NodeType type;
			};

			class _DllExport Edge{
			public:
				Edge();
				Edge( const Node& source, const Node& target );

				void set_weight( const Node& source, const Node& target );

				void corrent_assert() const;

				bool operator==( const Edge& edge ) const;
				bool operator!=( const Edge& edge ) const;

			public:
				float weight;
			};


			class _DllExport NodeMgr :
				public ShortestGraph<
				vecS,
				vecS,
				directedS,
				Node,
				Edge
				>{
			public:
				template<class Archive>
				friend void boost::serialization::serialize(
					Archive & ar,
					NodeMgr& node_mgr,
					const unsigned int version);
			public:
				NodeMgr();
				
				bool has_record_node();

				Node nearest_record_node( const Node& node );
				Node nearest_record_node( const Pos& pos );

				//	返回的是路径
				vector<Pos> nearest_record_left_pos( const Pos& pos );
				vector<Pos> nearest_record_right_pos( const Pos& pos );

				void update_door_edge();

				void save_dot( const string& file_name ) const;
			};


			//////////////////////////////////////////////////////
			//	RFP需要的内部类型
			class _DllExport FPPoint{
			public:
				FPPoint();

				bool operator==( const FPPoint& fppoint ) const;
				bool operator!=( const FPPoint& fppoint ) const;

			public:
				size_t id;
				Pos pos;
			};

			//////////////////////////////////////////////////////
			//	房间指纹类，用于识别房间
			//	重要的任务：对比房间是否相等
			class _DllExport RoomFingerPrint{
			public:
				friend _DllExport float comp_sim( RoomFingerPrint& rfp_first, RoomFingerPrint& rfp_second );
				template<class Archive>
				friend void boost::serialization::serialize(
					Archive & ar,
					RoomFingerPrint& rfp,
					const unsigned int version);
			public:

				RoomFingerPrint();

				void clear();
				bool empty() const;
				size_t size() const;
				void set( vector<Target>& target_list );
				bool has_fppoint( const FPPoint& fppoint ) const;

				FPPoint& operator[]( size_t index ); 

				////	如果两者节点多
				////	那么碰撞的概率越高
				////	所以需要对比多个点
				////	如果两者的节点多
				////	那么碰撞的概率就低
				////	所以需要对比点个数就少
				////	因此合理的方法是
				////	按比例比较
				//bool operator==( const RoomFingerPrint& rfp ) const;
				//bool operator!=( const RoomFingerPrint& rfp ) const;
				//	使用多个节点同时比较
				//	取最高相似度的节点来
				//	识别节点
				//	这个方法有效的
				//	解决。指纹节点发生
				//	细微变化的导致不能识别的
				//	情况



				//	正确性断言函数
				//	断定_1057_33_list是
				//	非空的，并且坐标中不含有
				//	未初始化坐标的
				//	并且每个坐标都是一个
				//	个pos::corrent_assert()为真的
				//	坐标。
				void corrent_assert() const;

				float get_comp_scale();
				void set_comp_scale( float comp_scale );

			private:
				vector<FPPoint> _1057_33_list;
				//	比较节点的比例,默认为0.3
				float _comp_scale;
			};
			typedef RoomFingerPrint RFP;

			//////////////////////////////////////////////////////
			//	求两个RFP的相似度
			//_DllExport float comp_sim( RoomFingerPrint& rfp_first, RoomFingerPrint& rfp_second );
			//_DllExport float comp_sim( const RoomFingerPrint& rfp1, const RoomFingerPrint& rfp2 );


			/*
			//	内部使用
			//	RFP测试工具
			//	1，33是否是会减少或者增加
			//	2，是否存在没有33或者没1057的房间
			//	3，使用RFP测试一个地图有多少相同的地图
			class __RFPTest{
			public:
				__RFPTest( World& world );

				//	1，33是否是会变化
				void test1_change_33();

				//	2，是否存在没有33或者没1057的房间
				void test2_no_33_or_1057();

				//	3，使用RFP测试一个地图有多少相同的地图
				void test3_same_room();


				bool _is_same_room();

				void _display_33_list();

				void _display_target_group();

			private:

				World& _world;
				Pos _last_map_pos;
				vector<Pos> _map_pos_list;
				vector<Pos> _33_list;

			};
			*/

			///////////////////////////////////////
			//	门的数据结构
			//	含门的方向

			class _DllExport Door{
			public:
				Door();
				void corrent_assert() const;

			public:
				Pos pos;
				Dir dir;
			};


			class _DllExport DoorMgr{
			public:
				template<class Archive>
				friend void boost::serialization::serialize(
					Archive & ar,
					DoorMgr& door_mgr,
					const unsigned int version);
			public:
				DoorMgr();

			private:
				void add( const Door& door );

			public:
				void clear();
				bool empty() const;
				size_t size() const;

				bool has_dir( const Dir dir ) const;

				Pos get_pos( const Dir dir ) const;
				void del_dir( const Dir dir );

				void update( const Door& door );
			private:
				vector<Door> _door_list;
			};

			/////////////////////////////////////////////
			//	DestoryableBlock
			//	可摧毁的障碍物
			//	set_start进图的一瞬间target_group
			//	set_end结束的一瞬间的target_group的状态
			//	保留减少的部分，就是可摧毁的障碍物。
			//	----------------------------------------
			//	6/21日更新:
			//	大转移更新:
			//		
			//		大转移后,测试了几个地下城,没有发现545,
			//	大多数障碍物变成了33, 在障碍物被摧毁后,
			//	在273之前的33会消失,并且有可能在273之后出现
			//	新的33,作为被摧毁后的障碍物的标志.
			//
			//		那么现在可以以此作为障碍物记录的手段,但是
			//	因为有可能以后的地图中会出现不只有33的情况,
			//	
			//		所以,就要在545的记录中加入断言.
			//	
			//		另外也有可能在新的地图中出现未知的新的id
			//	这种情况是否该加入断言.
			//	
			//		其实也是可以的.加入断言后.会提前得到这种
			//	情况,以免未知的id导致未知的错误.
			//	
			//	::::
			//	
			//	修改内容:
			//	1.加入新的内部数据:
			//		vector<Pos> _33_block_list;
			//	2.修改成员函数:
			//		void clear();
			//			:clear函数加入对_33_block_list的清理
			//
			//		void set_start( const vector<Target>& target_group );
			//			:1.set_start加入对进入地图的一瞬间记录
			//			:中,加入对_33_block_list的记录
            //			:
            //			:2.加入对未知id的输出,这里不写断言,避免
            //			:对记录做不必要的影响
			//		
			//		void set_end( const vector<Target>& target_group );
			//			:set_end加入对33的处理,33在这里处理的
            //			:的方式和545和1057不同,545和1057是在set_start
            //			:时记录的_545_block_list和_1057_block_list中
            //			:删除掉还剩下的545和1057
            //			:
            //			:而传入的target_group中的33有可能是已经
            //			:被摧毁的,但是他却在273后面,所以这里
            //			:要把273后面的33去掉.然后再用剩下的33去
            //			:对比set_start时设置的33.如果相同就删掉
            //			:这样没有删掉的就是被摧毁的33.
		    //		vector<Pos> near_pos( 
		    //			const Pos& pos, 
			//  		vector<Target>& target_group,
		    //			float distance = 150
		    //		    );
            //		    :1.加入33的返回.
            //		    :
            //		    :2.同时加入对is_set的检查.
            //		    :因为这个函数只有在is_set调用以后才能调用
			//  	vector<Pos> near_win_pos(
			//  		const Pos& pos,
			//  		vector<Target>& target_group,
			//  		float distace = 150
			//  		);
            //  		:1.加入对33的返回
            //  		:
            //  		:2.同时加入对is_set的检查.
            //  		:因为这个函数值有is_set调用以后才能被调用
            //
            //	3.加入新的成员函数
            //      bool has_pos_33( const Pos& pos ) const:	
            //          :用于在删除33或者在查找33时做断言的检查
            //      void del_33( const Pos& pos );
            //          :删除33
            //
            //  4.DestroyBlock的序列化中加入_33_block_list
            //-------------------------------------------------
            //
			class _DllExport DestroyBlock{
			public:
				template<class Archive>
				friend void boost::serialization::serialize(
					Archive & ar,
					DestroyBlock& destroy_block,
					const unsigned int version);
			public:
				DestroyBlock();

				void clear();

				void set_start( const vector<Target>& target_group );
				//	target_group中不存在怪物(529)
				//	用于判断是否调用set_end

				//	is_end这个接口暂时放弃使用
				//	因为如果boss房间怪永远不会消失
				//	应该使用屏幕是否黑掉来做判断
				//	如果屏幕黑掉.一般为离开房间
				//	所以更为合理.
				bool is_end( const vector<Target>& target_group );
				void set_end( const vector<Target>& target_group );

				bool has_pos_545( const Pos& pos ) const;
				bool has_pos_1057( const Pos& pos ) const;
                bool has_pos_33( const Pos& pos ) const;

				void del_545( const Pos& pos );
				void del_1057( const Pos& pos );
                void del_33( const Pos& pos );

				//	这个接口不太实用
				//	因为障碍物被打掉之后
				//	依然会去去返回这个障碍物
				//	这样是没有意义的.
				Pos nearest_pos( const Pos& pos ) const;
				vector<Pos> near_pos( 
					const Pos& pos, 
					vector<Target>& target_group,
					float distance = 150
					);

				//	前置条件必须已经设置win_pos
				vector<Pos> near_win_pos(
					const Pos& pos,
					vector<Target>& target_group,
					float distace = 150
					);

				bool is_set();

			private:
				vector<Pos> _545_block_list;
				vector<Pos> _1057_block_list;
				vector<Pos> _33_block_list;

				bool _is_set;
			};

			//	-------------------------

			class Store;
			class Input;
			class Record;


			//	--------------------------
			class _DllExport Room{
			public:
				friend class Store;
				friend class Input;
				friend class Record;

				template<class Archive>
				friend void boost::serialization::serialize(
					Archive & ar,
					Room& room,
					const unsigned int version);
			public:
				Room();
				string get_filename( const string& _ext = ".room" );
				//	设置主文件名，不含后缀。
				void set_filename( const string& file_name );

				//	返回文件名的前缀
				//	作为id
				string get_id_str();
				
				void save( const string& file_name = "" );
				void load( const string& file_name = "" );

				bool is_changed();
				void set_changed();
				void set_unchanged();
			//private:
				//	房间特征值
				RFP _rfp;
				//	文件名, _file_name中
				string _file_name;
				DoorMgr _door_mgr;
				DestroyBlock _destroy_block;
				NodeMgr _node_mgr;
				bool _is_changed;
			};

_ROOM_END

//		}	//	room
//	}	//	map
//}	//	dnf
//

#endif
