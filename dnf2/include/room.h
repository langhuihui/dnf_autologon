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
			//	��¼����
			class _DllExport Node{
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

				//	���ص���·��
				vector<Pos> nearest_record_left_pos( const Pos& pos );
				vector<Pos> nearest_record_right_pos( const Pos& pos );

				void update_door_edge();

				void save_dot( const string& file_name ) const;
			};


			//////////////////////////////////////////////////////
			//	RFP��Ҫ���ڲ�����
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
			//	����ָ���࣬����ʶ�𷿼�
			//	��Ҫ�����񣺶Աȷ����Ƿ����
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

				////	������߽ڵ��
				////	��ô��ײ�ĸ���Խ��
				////	������Ҫ�Աȶ����
				////	������ߵĽڵ��
				////	��ô��ײ�ĸ��ʾ͵�
				////	������Ҫ�Աȵ��������
				////	��˺����ķ�����
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
				void set_comp_scale( float comp_scale );

			private:
				vector<FPPoint> _1057_33_list;
				//	�ȽϽڵ�ı���,Ĭ��Ϊ0.3
				float _comp_scale;
			};
			typedef RoomFingerPrint RFP;

			//////////////////////////////////////////////////////
			//	������RFP�����ƶ�
			//_DllExport float comp_sim( RoomFingerPrint& rfp_first, RoomFingerPrint& rfp_second );
			//_DllExport float comp_sim( const RoomFingerPrint& rfp1, const RoomFingerPrint& rfp2 );


			/*
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
			*/

			///////////////////////////////////////
			//	�ŵ����ݽṹ
			//	���ŵķ���

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
			//	�ɴݻٵ��ϰ���
			//	set_start��ͼ��һ˲��target_group
			//	set_end������һ˲���target_group��״̬
			//	�������ٵĲ��֣����ǿɴݻٵ��ϰ��
			//	----------------------------------------
			//	6/21�ո���:
			//	��ת�Ƹ���:
			//		
			//		��ת�ƺ�,�����˼������³�,û�з���545,
			//	������ϰ�������33, ���ϰ��ﱻ�ݻٺ�,
			//	��273֮ǰ��33����ʧ,�����п�����273֮�����
			//	�µ�33,��Ϊ���ݻٺ���ϰ���ı�־.
			//
			//		��ô���ڿ����Դ���Ϊ�ϰ����¼���ֶ�,����
			//	��Ϊ�п����Ժ�ĵ�ͼ�л���ֲ�ֻ��33�����,
			//	
			//		����,��Ҫ��545�ļ�¼�м������.
			//	
			//		����Ҳ�п������µĵ�ͼ�г���δ֪���µ�id
			//	��������Ƿ�ü������.
			//	
			//		��ʵҲ�ǿ��Ե�.������Ժ�.����ǰ�õ�����
			//	���,����δ֪��id����δ֪�Ĵ���.
			//	
			//	::::
			//	
			//	�޸�����:
			//	1.�����µ��ڲ�����:
			//		vector<Pos> _33_block_list;
			//	2.�޸ĳ�Ա����:
			//		void clear();
			//			:clear���������_33_block_list������
			//
			//		void set_start( const vector<Target>& target_group );
			//			:1.set_start����Խ����ͼ��һ˲���¼
			//			:��,�����_33_block_list�ļ�¼
            //			:
            //			:2.�����δ֪id�����,���ﲻд����,����
            //			:�Լ�¼������Ҫ��Ӱ��
			//		
			//		void set_end( const vector<Target>& target_group );
			//			:set_end�����33�Ĵ���,33�����ﴦ����
            //			:�ķ�ʽ��545��1057��ͬ,545��1057����set_start
            //			:ʱ��¼��_545_block_list��_1057_block_list��
            //			:ɾ������ʣ�µ�545��1057
            //			:
            //			:�������target_group�е�33�п������Ѿ�
            //			:���ݻٵ�,������ȴ��273����,��������
            //			:Ҫ��273�����33ȥ��.Ȼ������ʣ�µ�33ȥ
            //			:�Ա�set_startʱ���õ�33.�����ͬ��ɾ��
            //			:����û��ɾ���ľ��Ǳ��ݻٵ�33.
		    //		vector<Pos> near_pos( 
		    //			const Pos& pos, 
			//  		vector<Target>& target_group,
		    //			float distance = 150
		    //		    );
            //		    :1.����33�ķ���.
            //		    :
            //		    :2.ͬʱ�����is_set�ļ��.
            //		    :��Ϊ�������ֻ����is_set�����Ժ���ܵ���
			//  	vector<Pos> near_win_pos(
			//  		const Pos& pos,
			//  		vector<Target>& target_group,
			//  		float distace = 150
			//  		);
            //  		:1.�����33�ķ���
            //  		:
            //  		:2.ͬʱ�����is_set�ļ��.
            //  		:��Ϊ�������ֵ��is_set�����Ժ���ܱ�����
            //
            //	3.�����µĳ�Ա����
            //      bool has_pos_33( const Pos& pos ) const:	
            //          :������ɾ��33�����ڲ���33ʱ�����Եļ��
            //      void del_33( const Pos& pos );
            //          :ɾ��33
            //
            //  4.DestroyBlock�����л��м���_33_block_list
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
				//	target_group�в����ڹ���(529)
				//	�����ж��Ƿ����set_end

				//	is_end����ӿ���ʱ����ʹ��
				//	��Ϊ���boss�������Զ������ʧ
				//	Ӧ��ʹ����Ļ�Ƿ�ڵ������ж�
				//	�����Ļ�ڵ�.һ��Ϊ�뿪����
				//	���Ը�Ϊ����.
				bool is_end( const vector<Target>& target_group );
				void set_end( const vector<Target>& target_group );

				bool has_pos_545( const Pos& pos ) const;
				bool has_pos_1057( const Pos& pos ) const;
                bool has_pos_33( const Pos& pos ) const;

				void del_545( const Pos& pos );
				void del_1057( const Pos& pos );
                void del_33( const Pos& pos );

				//	����ӿڲ�̫ʵ��
				//	��Ϊ�ϰ��ﱻ���֮��
				//	��Ȼ��ȥȥ��������ϰ���
				//	������û�������.
				Pos nearest_pos( const Pos& pos ) const;
				vector<Pos> near_pos( 
					const Pos& pos, 
					vector<Target>& target_group,
					float distance = 150
					);

				//	ǰ�����������Ѿ�����win_pos
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
				//	�������ļ�����������׺��
				void set_filename( const string& file_name );

				//	�����ļ�����ǰ׺
				//	��Ϊid
				string get_id_str();
				
				void save( const string& file_name = "" );
				void load( const string& file_name = "" );

				bool is_changed();
				void set_changed();
				void set_unchanged();
			//private:
				//	��������ֵ
				RFP _rfp;
				//	�ļ���, _file_name��
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