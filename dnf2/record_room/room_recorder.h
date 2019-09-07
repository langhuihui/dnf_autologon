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
			//	��¼����
			class Node{
			public:
				Node();

				bool operator==( const Node& node ) const;
				bool operator!=( const Node& node ) const;

				void set_door_type();
				void set_record_type();

			public:
				Pos pos;

				//	���ͱ�ʶ
				//	0		��ʾĬ������
				//	1000	�����ļ�¼��
				//	2000	��
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
			//	RFP��Ҫ���ڲ�����
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
			//	����ָ���࣬����ʶ�𷿼�
			//	��Ҫ�����񣺶Աȷ����Ƿ����
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

				////	������߽ڵ��
				////	��ô��ײ�ĸ���Խ��
				////	������Ҫ�Աȶ����
				////	������ߵĽڵ��
				////	��ô��ײ�ĸ��ʾ͵�
				////	������Ҫ�Աȵ��������
				////	��˺���ķ�����
				////	�������Ƚ�
				//bool operator==( const RoomFingerPrint& rfp ) const;
				//bool operator!=( const RoomFingerPrint& rfp ) const;
				//	ʹ�ö���ڵ�ͬʱ�Ƚ�
				//	ȡ������ƶȵĽڵ���
				//	ʶ��ڵ�
				//	���������Ч��
				//	�����ָ�ƽڵ㷢��
				//	ϸ΢�仯�ĵ��²���ʶ���
				//	���



				//	��ȷ�Զ��Ժ���
				//	�϶�_1057_33_list��
				//	�ǿյģ����������в�����
				//	δ��ʼ�������
				//	����ÿ�����궼��һ��
				//	��pos::corrent_assert()Ϊ���
				//	���ꡣ
				void corrent_assert() const;

				float get_comp_scale();

			private:
				vector<FPPoint> _1057_33_list;
				//	�ȽϽڵ�ı���,Ĭ��Ϊ0.3
				float _comp_scale;
			};
			typedef RoomFingerPrint RFP;

			//////////////////////////////////////////////////////
			//	������RFP�����ƶ�
			float comp_sim( const RFP& rfp1, const RFP& rfp2 );



			//	�ڲ�ʹ��
			//	RFP���Թ���
			//	1��33�Ƿ��ǻ���ٻ�������
			//	2���Ƿ����û��33����û1057�ķ���
			//	3��ʹ��RFP����һ����ͼ�ж�����ͬ�ĵ�ͼ
			class __RFPTest{
			public:
				__RFPTest( World& world );

				//	1��33�Ƿ��ǻ�仯
				void test1_change_33();

				//	2���Ƿ����û��33����û1057�ķ���
				void test2_no_33_or_1057();

				//	3��ʹ��RFP����һ����ͼ�ж�����ͬ�ĵ�ͼ
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
			//	�ŵ����ݽṹ
			//	���ŵķ���

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
			//	�ɴݻٵ��ϰ���
			//	set_start��ͼ��һ˲��target_group
			//	set_end������һ˲���target_group��״̬
			//	�������ٵĲ��֣����ǿɴݻٵ��ϰ��
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
				//	target_group�в����ڹ���(529)
				//	�����ж��Ƿ����set_end
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
				//	�������ļ�����������׺��
				void set_filename( const string& file_name );
				
				void save( const string& file_name = "" );
				void load( const string& file_name = "" );

				bool is_changed();
				void set_changed();
				void set_unchanged();
			private:
				//	��������ֵ
				RFP _rfp;
				//	�ļ���, _file_name��
				string _file_name;
				DoorMgr _door_mgr;
				DestroyBlock _destroy_block;
				NodeMgr _node_mgr;
				bool _is_changed;
			};
			
			*/

_ROOM_BEGIN

			///////////////////////////////////////////////
			//	Input����ģ��
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
				//	_is_end�ݲ�ʹ��,״̬�Ļ�
				//	��_is_start�ͺ���	
				//bool _is_end;
				bool _is_show_pic;


				Room* _current_room_ptr;

				World& _world;
			};

			//////////////////////"////////////
			//	Store��Ҫ��ģ��
			//	ǰ�������ǣ��ļ����ڵ�Ŀ¼
			//	�Ѿ����ڡ�
			//	�����Ƕ�ȡ�Ѿ����ڵķ����������ļ�
			//	�Ѿ�������Ҫ�洢���ļ�������
			class AutoFile{
			public:
				AutoFile();
				void set_work_dir( const string& work_dir );
				bool load_file_list( vector<string>& file_list, const string& file_ext );
				void gen_save_file_name( string& save_file_name, const string& file_ext );
			private:
				string _work_dir;
			};

			//	�洢ģ��
			//	�����Զ��洢,�Ͷ�ȡ��Ӧ���ļ�
			//	�洢:
			//	��Ҫ���������ļ����
			//	�����µ��ļ���, Ȼ��洢�ļ�
			//	��ȡ:
			//	��Ҫ��ȡ���е��ļ�
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
				//	��Ϊ�ڵ��ͼ�ṹ�Ƿ���ͬһ
				//	Ŀ¼�µ�,����,����ҲҪһ��
				string _map_dir;
				string _map_name_dir;
				string _room_ext;

				vector<Room>* _exist_room_list_ptr;
				Room* _current_room_ptr;
				//	Ϊ���Զ���ȡ��ͼ����
				//	��Ҫworld
				World& _world;
				AutoFile _auto_file;
			};

			///////////////////////////////
			//	��¼ģ��
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

				//	�Ƿ���һ����ȷ��״̬
				//	�Ȳ��Ǻ�����״̬
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
				
				//	�ⲿControl��Ҫ���ϵĵ���
				//	������������¼�¼
				//	��������ˢ�¼�¼
				//	���ڸ����ϰ���
				//	�뿪ʱ����Ҫ���ֵĶ���
				void update( Store& store );

			public:

				void leave();
				void set_door();
				//	 �����ź�����ڵ�֮��ı�
				//	���room���޸������
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

				//	�Ѿ����ڵķ���
				vector<Room> exist_room_list;
				//	��ǰ�ķ���
				Room current_room;

				boost::thread _input_thread;
			};
			
			//////////////////////////////////////////////////////
			//		����	
			//
			//	0.�������еķ���ڵ��ļ���
			//	1.���뷿�䣬��¼��������ֵ:1057,33��
			//	���������ֵ�Ѿ��������Ѽ�¼��������
			//	2.������еĹ֣��Լ���·���ϰ��
			//	3.���밴������¼��¼�㣨��¼֮ǰ�Զ�������ɴݻ��ϰ�������꣩
			//	4.���밴���������¼�������¼��
			//	5.���밴������ʾЧ��ͼ
			//	6.���밴����ɾ����¼��
			//	7.���밴����ɾ����¼��
			//	8.�������е��š��Զ���¼��
			//	9.�������ȥ������boss���䣬���¼������
			//	��ʾ���������Ч��ͼ��
			//	10.���û��boss���䣬��ص���¼�ķ��䣬
			//	���밴����������¼����ʾ���������Ч��ͼ
			//	11.�����¼
			//
			//	:��¼��������ֵ����Ҳ���ڼ�¼�㡣
			//
			//	-----------------------------------------------
			//		�������ݽṹ
			//	
			//	�����ࣺ��������ֵ���ļ������ɴݻٵ��ϰ������꣬
			//	�ȽϽӿڡ�ȷ�������Ƿ���ȡ��ŵ����ꡣ
			//	
			//	��¼���ࣺ��¼�����ӣ�ɾ�������Եȣ��ߵ����ɾ����
			//	�̳���Graph����ʾЧ��ͼƬ,��¼��Ҳ�����Žڵ�
			//	
			//		ģ��
			//
			//	1���洢ģ�飬������������з�������
			//	2������ģ�飬������ƺ͸��ֽڵ�ָ����߳�
			//	2.5, ��¼ģ��
			//	3������ģ�飬�����������̣����߳�
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