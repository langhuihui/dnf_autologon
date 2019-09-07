#ifndef __ROOM_RECORDER_H__
#define __ROOM_RECORDER_H__

#include <cassert>
#include <string>
#include <vector>
using namespace std;

#include <room.h>
#include <base.h>
#include <map_base.h>
#include <world.h>
//#include "map_recorder.h"
#include <graph.h>


#pragma comment( lib, "map.lib" )

#define _DllExport __declspec(dllimport)

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/graph/adj_list_serialize.hpp>

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

#define BOOST_FILESYSTEM_NO_DEPRECATED
/*
namespace dnf{
	namespace map{
		namespace room{

			class Node;
			class Edge;
			class NodeMgr;

			class FPPoint;
			class RoomFingerPrint;
		
			class Door;
			class DoorMgr;
			class DestroyBlock;

			class Room;
		}	//	room
	}	//	map
}	//	dnf

namespace boost {
	namespace serialization {

		using namespace dnf::map::room;

		template<class Archive>
		void serialize(Archive & ar, dnf::map::room::Node& node, const unsigned int version)
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
		void serialize(Archive & ar, dnf::map::room::Door& door, const unsigned int version)
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
			ar & destroy_block._is_set;
		}

		template<class Archive>
		void serialize(Archive & ar, dnf::map::room::Room& room, const unsigned int version)
		{
			ar & room._rfp;
			ar & room._file_name;
			ar & room._door_mgr;
			ar & room._destroy_block;
			ar & room._node_mgr;
		}

	}	//	namespace serialization;
}	//	namespace boost;
*/

//namespace dnf{
//	namespace map{
//		namespace room

/*
			/////////////////////////////////////////////////////
			//	记录点类
			class Node{
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

			class Edge{
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


			class NodeMgr :
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

				void update_door_edge();

				void save_dot( const string& file_name ) const;
			};


			//////////////////////////////////////////////////////
			//	RFP需要的内部类型
			class FPPoint{
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
			class RoomFingerPrint{
			public:
				friend float comp_sim( RoomFingerPrint& rfp1, RoomFingerPrint& rfp2 );
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

			private:
				vector<FPPoint> _1057_33_list;
				//	比较节点的比例,默认为0.3
				float _comp_scale;
			};
			typedef RoomFingerPrint RFP;

			//////////////////////////////////////////////////////
			//	求两个RFP的相似度
			float comp_sim( const RFP& rfp1, const RFP& rfp2 );



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
			///////////////////////////////////////
			//	门的数据结构
			//	含门的方向

			class Door{
			public:
				Door();
				void corrent_assert() const;

			public:
				Pos pos;
				Dir dir;
			};


			class DoorMgr{
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
			class DestroyBlock{
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
				bool is_end( const vector<Target>& target_group );
				void set_end( const vector<Target>& target_group );

				bool has_pos_545( const Pos& pos ) const;
				bool has_pos_1057( const Pos& pos ) const;

				void del_545( const Pos& pos );
				void del_1057( const Pos& pos );

				Pos nearest_pos( const Pos& pos ) const;

				bool is_set();

			private:
				vector<Pos> _545_block_list;
				vector<Pos> _1057_block_list;

				bool _is_set;
			};

			//	-------------------------

			class Store;
			class Input;
			class Record;


			//	--------------------------
			class Room{
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
				
				void save( const string& file_name = "" );
				void load( const string& file_name = "" );

				bool is_changed();
				void set_changed();
				void set_unchanged();
			private:
				//	房间特征值
				RFP _rfp;
				//	文件名, _file_name中
				string _file_name;
				DoorMgr _door_mgr;
				DestroyBlock _destroy_block;
				NodeMgr _node_mgr;
				bool _is_changed;
			};
			
			*/

_ROOM_BEGIN

			///////////////////////////////////////////////
			//	Input输入模块
			class Input{
			public:
				Input( World& world );

				void set_current_room_ptr( Room& room );

				void operator()();

				void start();
				void end();

				void add_node();
				void add_edge( const vector<string>& argv_list );

				void del_node( const vector<string>& argv_list );
				void del_edge( const vector<string>& argv_list );

				bool is_show_pic();
				void set_show_pic_end();

				void help();

			private:

				void _argv_to_edge_vd( 
					const string& argv,
					NodeMgr::vd_t& source_vd,
					NodeMgr::vd_t& target_vd
					);

				void _argv_to_node_vd(
					const string& argv,
					NodeMgr::vd_t& vd
					);

				bool _right_state();

			private:
				bool _is_start;
				//	_is_end暂不使用,状态的话
				//	用_is_start就好了	
				//bool _is_end;
				bool _is_show_pic;


				Room* _current_room_ptr;

				World& _world;
			};

			//////////////////////"////////////
			//	Store需要的模块
			//	前置条件是：文件所在的目录
			//	已经存在。
			//	作用是读取已经存在的符合条件的文件
			//	已经生成需要存储的文件的名。
			class AutoFile{
			public:
				AutoFile();
				void set_work_dir( const string& work_dir );
				bool load_file_list( vector<string>& file_list, const string& file_ext );
				void gen_save_file_name( string& save_file_name, const string& file_ext );
			private:
				string _work_dir;
			};

			//	存储模块
			//	负责自动存储,和读取相应的文件
			//	存储:
			//	需要根据现有文件情况
			//	生成新的文件名, 然后存储文件
			//	读取:
			//	需要读取现有的文件
			class Store{
			public:
				Store( World& world );

				void set_map_name_dir();

				void set_exist_room_list_ptr( vector<Room>& exist_room_list );
				void set_current_room_ptr( Room& current_room );

				bool load_room();
				//
				void save_room();

				void save_room_png();
	
			private:
				bool _get_map_name_list( vector<string>& map_name_list );

			private:
				//	因为节点和图结构是放在同一
				//	目录下的,所以,这里也要一致
				string _map_dir;
				string _map_name_dir;
				string _room_ext;

				vector<Room>* _exist_room_list_ptr;
				Room* _current_room_ptr;
				//	为了自动获取地图名称
				//	需要world
				World& _world;
				AutoFile _auto_file;
			};

			///////////////////////////////
			//	记录模块
			class Record{
			public:
				Record( World &world );

				void set_current_room_ptr( Room& current_room );
				void set_exist_room_list_ptr( 
					vector<Room>& exist_room_list );

			public:

				bool map_name_is_changed();
				bool is_corrent_map();
				void update_map_name();

				//	是否处于一个正确的状态
				//	既不是黑屏的状态
				bool right_state();

				bool is_first_map();
				void unset_first_map();

			private:
				string _get_map_name();
				bool _get_map_name_list( vector<string>& map_name_list );

			public:
				bool is_change_room();
				bool update_last_map_pos();

			public:

				void join();

				bool is_join();
				void set_join();
				void set_unjoin();

				void set_rfp();	
				bool is_in_exist_list();
				Room& best_sim_room();

			public:
				
				//	外部Control需要不断的调用
				//	这个函数来更新记录
				//	这里用来刷新记录
				//	用于更新障碍物
				//	离开时的需要保持的对象
				void update( Store& store );

			public:

				void leave();
				void set_door();
				//	 更新门和最近节点之间的变
				//	如果room被修改则更新
				void update_door_edge();

				bool is_leave();
				void set_leave();
				void set_unleave();
			private:
				Dir _door_dir();

			private:
				string _map_dir;
				string _map_name;

				bool _is_first_map;
				bool _is_join;
				bool _is_leave;

				Pos _last_map_pos;
				Pos _last_player_pos;

				vector<Room>* _exist_room_list_ptr;
				Room* _current_room_ptr;

				World& _world;

#ifdef _DEBUG
				size_t _right_state_cnt;
#endif

			public:
				
				bool is_boss_room();
				void set_boss_room_pos();
				void unset_boss_room();
				void update_boss_room();
				
			private:
				Pos _boss_room_pos;
				bool _is_boss_room;
			};					
			
			class Control{
			public:
				Control( World& world );

				void init();

				void operator()();

			private:
				World& _world;
				Record _record;
				Store _store;
				Input _input;

				//	已经存在的房间
				vector<Room> exist_room_list;
				//	当前的房间
				Room current_room;

				boost::thread _input_thread;
			};
			
			//////////////////////////////////////////////////////
			//		流程	
			//
			//	0.载入现有的房间节点文件。
			//	1.进入房间，记录房间特征值:1057,33，
			//	如果该特征值已经存在则已记录，跳过。
			//	2.打掉所有的怪，以及挡路的障碍物。
			//	3.输入按键，记录记录点（记录之前自动计算出可摧毁障碍物的坐标）
			//	4.输入按键，输入记录点对来记录边
			//	5.输入按键，显示效果图
			//	6.输入按键，删除记录点
			//	7.输入按键，删除记录边
			//	8.穿过所有的门。自动记录门
			//	9.如果穿过去的门是boss房间，则记录结束，
			//	显示含门坐标的效果图。
			//	10.如果没有boss房间，则回到记录的房间，
			//	输入按键，结束记录，显示含门坐标的效果图
			//	11.保存记录
			//
			//	:记录点留属性值。门也属于记录点。
			//
			//	-----------------------------------------------
			//		基本数据结构
			//	
			//	房间类：包含特征值，文件名，可摧毁的障碍物坐标，
			//	比较接口。确定房间是否相等。门的坐标。
			//	
			//	记录点类：记录点的添加，删除，属性等，边的添加删除，
			//	继承至Graph。显示效果图片,记录点也包含门节点
			//	
			//		模块
			//
			//	1，存储模块，保存和载入已有房间数据
			//	2，输入模块，输入控制和各种节点指令，子线程
			//	2.5, 记录模块
			//	3，控制模块，控制整个流程，主线程
			//	

			class RoomRecorder{
			public:
				RoomRecorder( Dm& dm_, AttachIme& attach_ime_ );

			private:
				Control _control;
				World _world;
			};

_ROOM_END
//
//		}	//	namespace room
//	}	//	namespace map
//}	//	namespace dnf
//
//
#endif