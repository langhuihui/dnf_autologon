#include "room_recorder.h"



//namespace dnf{
//	namespace map{
//		namespace room{

_ROOM_BEGIN
		
			/*
			/////////////////////////////////////////////////////
			//	��¼����
			Node::Node()
				:type( unused )
			{}

			void Node::set_door_type()
			{
				type = door;
			}
			void Node::set_record_type()
			{
				type = record;
			}

			bool Node::operator==( const Node& node ) const
			{
				node.pos.corrent_assert();
				node.pos.init_assert();

				if( node.pos == pos
					&& node.type == type )
					return true;
				else
					return false;
			}
			bool Node::operator!=( const Node& node ) const
			{
				return !operator==( node );
			}

			Edge::Edge()
				:weight( 0.f )
			{}

			Edge::Edge( const Node& source, const Node& target )
				:weight( compute_distance( source.pos, target.pos ) )
			{
				source.pos.corrent_assert();
				source.pos.init_assert();

				target.pos.corrent_assert();
				target.pos.init_assert();

			}

			void Edge::set_weight( const Node& source, const Node& target )
			{
				source.pos.corrent_assert();
				source.pos.init_assert();
				target.pos.corrent_assert();
				target.pos.init_assert();

				weight = compute_distance( source.pos, target.pos );
			}

			void Edge::corrent_assert() const
			{
				assert( weight > 0.f
					&& "weightδ��ʼ��" );
			}

			bool Edge::operator==( const Edge& edge ) const
			{
				edge.corrent_assert();
				if(  edge.weight == weight )
					return true;
				else
					return false;
			}

			bool Edge::operator!=( const Edge& edge ) const
			{
				return !operator==( edge );
			}

			//	NodeMgr
			NodeMgr::NodeMgr()
			{
			}

			bool NodeMgr::has_record_node()
			{
				if( 0 == boost::num_vertices( _g ) )
					return false;

				for( pvi_t pvi = boost::vertices( _g );
					pvi.first != pvi.second; pvi.first++ )
					if( _g[ *pvi.first ].type == Node::record )
						return true;

				return false;
			}

			Node NodeMgr::nearest_record_node( const Node& node )
			{
				assert( has_vertex( node )
					&& "NodeMgr::nearest_record_node:node����NodeMgr��" );

				assert( has_record_node()
					&& "NodeMgr::nearest_record_node:NodeMgr�в�����record��" );

				Node shortest_record_node;
				shortest_record_node.set_record_type();

				float shortest_dis = 0;
				float dis;
				
				for( pvi_t pvi = boost::vertices( _g );
					pvi.first != pvi.second; pvi.first++ )
					if( _g[ *pvi.first ].type == Node::record
						&& _g[ *pvi.first ] != node )
					{
						dis = compute_distance( _g[ *pvi.first ].pos, node.pos );
						
						if( 0 == shortest_dis
							|| dis < shortest_dis )
						{
							shortest_dis = dis;
							shortest_record_node = _g[ *pvi.first ];
						}
					}

				assert( has_vertex( shortest_record_node )
					&& "NodeMgr::nearest_record_node:�Ҳ�������ļ�¼��" );

				return shortest_record_node;
			}

			void NodeMgr::update_door_edge()
			{
				assert( has_record_node()
					&& "NodeMgr::update_door_edge:������Record��,�޷�����" );

				for( pvi_t pvi = boost::vertices( _g );
					pvi.first != pvi.second; pvi.first++ )
					if( _g[ *pvi.first ].type == Node::door )
					{
						boost::clear_vertex( *pvi.first, _g );

						Node node = nearest_record_node( _g[ *pvi.first ] );

						Edge edge;
						edge.set_weight( _g[ *pvi.first ], node );

						add_biedge( _g[ *pvi.first ], node, edge );
					}
			}

			void NodeMgr::save_dot( const string& file_name ) const
			{
				assert( !file_name.empty()
					&& "NodeMgr.save_dot:������ļ���Ϊ��" );

				assert( boost::num_vertices( _g ) 
					&& "NodeMgr.save_dot:_g�ڵ�Ϊ��" );

				ofstream ofs( file_name );

				if( !ofs )
					throw std::exception( "NodeMgr.save_dot:�򲻿������ļ�" );

				struct vertex_writer {

					vertex_writer(
						const graph_t& g
						)
						:_g( g )
					{
						vector<int> pos_x_vec;
						vector<int> pos_y_vec;

						typedef graph_traits<graph_t>::vertex_iterator vi_t;
						pair<vi_t, vi_t> vp;

						for( vp = boost::vertices( _g );
							vp.first != vp.second; vp.first++ )
						{
							pos_x_vec.push_back( _g[ *(vp.first) ].pos.x );
							pos_y_vec.push_back( _g[ *(vp.first) ].pos.y );
						}

						min_pos_x = *min_element( pos_x_vec.begin(), pos_x_vec.end() );
						//min_pos_y = *min_element( pos_y_vec.begin(), pos_y_vec.end() );
						//max_pos_x = *max_element( pos_x_vec.begin(), pos_x_vec.end() );
						max_pos_y = *max_element( pos_y_vec.begin(), pos_y_vec.end() );
					}

					void operator()(std::ostream& out, const vd_t& v ) const 
					{
						Pos pos;
						pos.x = _g[ v ].pos.x - min_pos_x;
						pos.y = max_pos_y - _g[ v ].pos.y;

						float x = (float)pos.x / 64.f;
						float y = (float)pos.y / 64.f;

						//out << "[shape = polygon, sides = 4, label=\""
						out << "[xlabel=\""
							<< _g[ v ].pos.x// - min_pos_x
							<< ','
							<< _g[ v ].pos.y// - min_pos_y
							//<< " ["
							//<< v
							//<< "]"
							<< "\", pos = \""
							<< x
							<< ','
							<< y
							<< "!\"";

						if( Node::record == _g[ v ].type )
							out << ", fillcolor=palegreen";
						if( Node::door == _g[ v ].type )
							out << ", fillcolor=deepskyblue, shape=box";

						out << " ]" ;
					}

					int min_pos_x;
					//int min_pos_y;
					//int max_pos_x;
					int max_pos_y;

					const graph_t& _g;
				};

				struct edge_writer {
					edge_writer(
						const graph_t& g,
						const vector<ed_t>& shortest_path_ed_
						)
						:_g( g ),
						shortest_path_ed( shortest_path_ed_ )
					{}

					void operator()(std::ostream& out, const ed_t& e ) const
					{
						//out << "[ label = \""
						//	<< _g[ e ].weight
						//	<< "\" ";

						if( find( shortest_path_ed.begin(), shortest_path_ed.end(), e ) 
							!= shortest_path_ed.end() )
							//out << ", color = red ";
							out << "[ color = red ]";

						//out	<< "]" ;

					}
					const graph_t& _g;
					const vector<ed_t>& shortest_path_ed;
				};

				struct graph_writer {
					void operator()(std::ostream& out) const {
						//out << "graph [bgcolor=white size=\"20,20\"]" << endl;
						out << "graph [bgcolor=white]" << endl;
						out << "node [shape=circle, style = filled, fillcolor=white,"
							<< "fixedsize=true, width=0.4, height=0.3 ]" << endl;
						//out << "edge [style=dashed]" << endl;
					}
				};

				write_graphviz(
					ofs, 
					_g,  
					vertex_writer( _g ),
					edge_writer( _g, _shortest_path_ed ),
					graph_writer()
					);
			}


			//////////////////////////////////////////////////////
			//	RFP��Ҫ���ڲ�����
			FPPoint::FPPoint()
			{
			}

			bool FPPoint::operator==( const FPPoint& fppoint ) const
			{
				fppoint.pos.corrent_assert();

				if( fppoint.id == id
					&& fppoint.pos == pos )
					return true;
				else
					return false;
			}

			bool FPPoint::operator!=( const FPPoint& fppoint ) const
			{
				return !operator==( fppoint );
			}

			//////////////////////////////////////////////////////
			//	����ָ���࣬����ʶ�𷿼�
			RoomFingerPrint::RoomFingerPrint()
				:_comp_scale( 0.8f )
			{
			}

			void RFP::clear()
			{
				_1057_33_list.clear();
			}

			bool RFP::empty() const
			{
				return _1057_33_list.empty();
			}


			size_t RFP::size() const
			{
				return _1057_33_list.size(); 
			}

			bool RFP::has_fppoint( const FPPoint& fppoint ) const
			{
				fppoint.pos.corrent_assert();

				assert( 33 == fppoint.id 
					|| 1057 == fppoint.id 
					&& "RFP::has_fppoint" );

				if( _1057_33_list.empty() )
					return false;

				for( size_t i = 0; i < _1057_33_list.size(); i++ )
					if( fppoint == _1057_33_list[ i ] )
						return true;

				return false;
			}

			void RFP::set( vector<Target>& target_list )
			{
				assert( !target_list.empty() 
					&& "RFP::set:target_listΪ��" );

#ifdef _DEBUG
				size_t cnt = 0;

				for( size_t i = 0; i < target_list.size(); i++ )
					if( 
						1057 == target_list[ i ].id
						|| 33 == target_list[ i ].id 
						)
					{
						cnt++;


						//assert( Pos() != target_list[ i ].map_pos
							//&& "RFP::set:target_list[ i ].map_posδ��ʼ��" );


						target_list[ i ].map_pos.corrent_assert();
					}

					assert( cnt
						&& "RFP::set:1057��33Ϊ��" );

#endif
					_1057_33_list.clear();

					FPPoint fppoint;

					for( size_t i = 0; i < target_list.size(); i++ )
						if( 
							1057 == target_list[ i ].id
							|| 33 == target_list[ i ].id 
							)
						{
							fppoint.id = target_list[ i ].id;
							fppoint.pos = target_list[ i ].map_pos;

							_1057_33_list.push_back(
								fppoint );
						}

						corrent_assert();
			}

//			bool RFP::operator==( const RoomFingerPrint& rfp ) const
//			{
//				corrent_assert();
//				rfp.corrent_assert();
//
//				size_t comp_equal_cnt = 0;
//
//				size_t comp_size = 0;
//				//	��Ԫ�ؽ��ٵ��Ǹ�
//				//	�������Ƚϡ�
//				if( size() < rfp.size() )
//					comp_size = size();
//				else
//					comp_size = rfp.size();
//
//				for( size_t i = 0; i < size(); i++ )
//					if( rfp.has_fppoint( _1057_33_list[ i ] ) )
//						comp_equal_cnt++;
//
//				if( (float)comp_equal_cnt / (float)comp_size > _comp_scale )
//					return true;
//				else
//					return false;
//
//			}
//
//			bool RFP::operator!=( const RoomFingerPrint& rfp ) const
//			{
//				return !operator==( rfp );
//			}

			FPPoint& RFP::operator[]( size_t index )
			{
				assert( index < size()
					&& "RFP::operator[]:index������Χ" );

				return _1057_33_list[ index ];
			}
//
			void RFP::corrent_assert() const
			{
				assert( !_1057_33_list.empty()
					&& "RFP::set:_1057_33_listΪ�գ�����" );

#ifdef _DEBUG
				for( size_t i = 0; i < _1057_33_list.size(); i++ )
				{
					//assert( Pos() != _1057_33_list[ i ].pos
					//	&& "RFP::set:_1057_33_list[ i ]δ��ʼ��" );

					_1057_33_list[ i ].pos.corrent_assert();
				}
#endif
			}

			float RFP::get_comp_scale()
			{
				return _comp_scale;
			}

			float comp_sim( RFP& rfp_first, RFP& rfp_second )
			{
				rfp_first.corrent_assert();
				rfp_second.corrent_assert();

				float sim = 0.f;

				size_t comp_equal_cnt = 0;
				size_t comp_size = 0;
				int size = 0;

				if( rfp_first.size() < rfp_second.size() )
					comp_size = rfp_first.size();
				else
					comp_size = rfp_second.size();

				for( size_t i = 0; i < rfp_first.size(); i++ )
					if( rfp_second.has_fppoint( rfp_first[ i ] ) )
						comp_equal_cnt++;

				assert( comp_size >= comp_equal_cnt
					&& "comp_sim::comp_size < comp_equal_cnt" );

				sim = (float)comp_equal_cnt / (float)comp_size;

				return sim;
			}

			/////////////////////////////
			//	�ڲ�ʹ��
			//	RFP���Թ���
			__RFPTest::__RFPTest( World& world )
				:_world( world )
			{}

			//	1��33�Ƿ��ǻ���ٻ�������
			void __RFPTest::test1_change_33()
			{
				_world.update_target_group_map_pos();
				_display_target_group();
				_world.update_map_win_pos();
				cout << "--------------------------" << endl;
				cout << _world.map_pos.x << ", " << _world.map_pos.y << endl;

				while( true )
				{
					_world.update_map_win_pos();
					if( !_world.map_pos.is_invaild() )
						if( _is_same_room() )
						{
							_world.update_target_group_map_pos();

							if( _33_list.empty() )
							{
								for( size_t i = 0; i < _world.target_group.size(); i++ )
									if(  33 == _world.target_group[ i ].id 
										&& !_world.target_group[  i ].map_pos.is_invaild() )
										_33_list.push_back(
										_world.target_group[ i ].map_pos );
							}
							else
							{
								bool over_273 = false;
								size_t j = 0;
								for( size_t i = 0; i < _world.target_group.size(); i++ )
								{
									if( 273 == _world.target_group[ i ].id )
										over_273 = true;
									if( 33 == _world.target_group[ i ].id
										&& !_world.target_group[ i ].map_pos.is_invaild()
										&& !over_273 )
									{
										assert( j < _33_list.size()
											&& "__RFPTest::test1_change_33:j������Χ" );
										if( j >= _33_list.size() )
										{
											cout << "_33_list��������" << endl;
											_display_33_list();
											_display_target_group();
											continue;
										}

										if( _33_list[ j ] != _world.target_group[ i ].map_pos )
										{
											cout << "_33_list[ " << j << " ]����ı�" << endl;
											_display_33_list();
											_display_target_group();
										}
										j++;
									}
								}

								if( j < _33_list.size() )
								{
									cout << "_33_list��������" << endl;
									_display_33_list();
									_display_target_group();
								}

								_33_list.clear();
								for( size_t i = 0; i < _world.target_group.size(); i++ )
									if(  33 == _world.target_group[ i ].id 
										&& !_world.target_group[  i ].map_pos.is_invaild() )
										_33_list.push_back(
										_world.target_group[ i ].map_pos );
							}

						}
						else
							_33_list.clear();

					dnf::map::wait( 1 );
				}
			}

			//	2���Ƿ����û��33����û1057�ķ���
			void __RFPTest::test2_no_33_or_1057()
			{

			}

			//	3��ʹ��RFP����һ����ͼ�ж�����ͬ�ĵ�ͼ
			void __RFPTest::test3_same_room()
			{
				//_world.update_map_win_pos();
				//
				//if( !

				//if( !_world.map_pos.is_invaild()
				//	&& _world.map_pos != _last_map_pos )
				//{
			}

			bool __RFPTest::_is_same_room()
			{
				_world.update_map_win_pos();

				if( !_last_map_pos.is_init() 
					&& !_world.map_pos.is_invaild() )
				{
					_last_map_pos = _world.map_pos;
					return true;
				}

				if( _last_map_pos.is_init()
					&& !_world.map_pos.is_invaild()
					&& _last_map_pos != _world.map_pos )
				{
					_last_map_pos = _world.map_pos;
					return false;
				}
				else
					return true;
			}

			void __RFPTest::_display_33_list()
			{
				cout << "------------------------" << endl;
				cout << "_33_list:" << endl;
				for( size_t i = 0; i < _33_list.size(); i++ )
					cout << i << ": (" << _33_list[ i ].x
					<< ", " << _33_list[ i ].y << ")" << endl;
				cout << "------------------------" << endl;
			}

			void __RFPTest::_display_target_group()
			{
				cout << "------------------------" << endl;
				cout << "target_group:" << endl;
				for( size_t i = 0; i < _world.target_group.size(); i++ )
					cout << i << ":\t" << _world.target_group[ i ].id 
					<< "->(" << _world.target_group[ i ].map_pos.x << ", "
					<< _world.target_group[ i ].map_pos.y << ")" << endl;

				cout << "------------------------" << endl;
			}

			///////////////////////////////////////
			//	�ŵ����ݽṹ
			//	���ŵķ���
			Door::Door()
			{}

			void Door::corrent_assert() const
			{
				pos.corrent_assert();
				assert( dir != Dir::no
					&& "Door::corrent_assert::dir == Dir::no" );
			}

			DoorMgr::DoorMgr()
			{
			}

			void DoorMgr::add( const Door& door )
			{
				door.corrent_assert();

				assert( door.pos.is_init()
					&& "DoorMgr::add:door.posδ��ʼ��" );

				assert( !has_dir( door.dir )
					&& "DoorMgr::add:door.dir�Ѿ�����" );

				_door_list.push_back( door );
			}

			void DoorMgr::clear()
			{
				_door_list.clear();
			}

			size_t DoorMgr::size() const
			{
				return _door_list.size();
			}

			bool DoorMgr::empty() const
			{
				return _door_list.empty();
			}
			
			bool DoorMgr::has_dir( const Dir dir ) const
			{
				assert( dir != Dir::no
					&& "DoorMgr::_has_dir:dir == Dir::no" );

				if( _door_list.empty() )
					return false;

				for( size_t i = 0; i < _door_list.size(); i++ )
					if( dir == _door_list[ i ].dir )
						return true;
				
				return false;
			}

			Pos DoorMgr::get_pos( const Dir dir ) const
			{
				assert( has_dir( dir )
					&& "DoorMgr::get_pos:ָ����dir������" );

				for( size_t i = 0; i < _door_list.size(); i++ )
					if( dir == _door_list[ i ].dir )
						return _door_list[ i ].pos;
			}

			void DoorMgr::del_dir( const Dir dir ) 
			{
				assert( has_dir( dir )
					&& "DoorMgr::del_dir:ָ����dir������" );

				for( vector<Door>::iterator it = _door_list.begin();
					it != _door_list.end(); )
					if( dir == it->dir )
						it = _door_list.erase(it);
					else
						it++;
			}

			void DoorMgr::update( const Door& door )
			{
				door.corrent_assert();
				if( has_dir( door.dir ) )
					del_dir( door.dir );

				add( door );
			}

			/////////////////////////////////////////////
			//	DestoryBlock
			//	�ɴݻٵ��ϰ���
			DestroyBlock::DestroyBlock()
				:_is_set( false )
			{
			}

			void DestroyBlock::clear()
			{
				_545_block_list.clear();
				_1057_block_list.clear();
			}

			void DestroyBlock::set_start( const vector<Target>& target_group )
			{
				assert( !target_group.empty() 
					&& "DestroyBlock::set_start:target_groupΪ��" );

				clear();
				_is_set = false;

				for( size_t i = 0; i < target_group.size(); i++ )
				{
					if( 545 == target_group[ i ].id )
					{
						target_group[ i ].map_pos.corrent_assert();
						assert( target_group[ i ].map_pos.is_init()
							&& "DestroyBlock::set_start:target_group[ i ].map_pos(545)δ��ʼ��" );
						_545_block_list.push_back(
							target_group[ i ].map_pos );
					}
					if( 1057 == target_group[ i ].id )
					{
						target_group[ i ].map_pos.corrent_assert();
						assert( target_group[ i ].map_pos.is_init()
							&& "DestroyBlock::set_start:target_group[ i ].map_pos(1057)δ��ʼ��" );
						_1057_block_list.push_back(
							target_group[ i ].map_pos );
					}
				}
			}

			bool DestroyBlock::is_end( const vector<Target>& target_group )
			{
				assert( !target_group.empty() 
					&& "DestroyBlock::is_end:target_groupΪ��" );

				for( size_t i = 0; i < target_group.size(); i++ )
					if( 529 == target_group[ i ].id )
						return false;

				return true;
			}
			
			void DestroyBlock::set_end( const vector<Target>& target_group )
			{
				assert( !target_group.empty() 
					&& "DestroyBlock::set_end:target_groupΪ��" );

				for( size_t i = 0; i < target_group.size(); i++ )
				{
					if( 545 == target_group[ i ].id )
						del_545(target_group[ i ].map_pos );
					if( 1057 == target_group[ i ].id )
						del_1057( target_group[ i ].map_pos );
				}

				_is_set = true;
			}

			
			bool DestroyBlock::has_pos_545( const Pos& pos ) const
			{
				pos.corrent_assert();
				assert( pos.is_init()
					&& "DestroyBlock::has_pos_545:pos��Ч" );

				for( size_t i = 0; i < _545_block_list.size(); i++ )
					if( pos == _545_block_list[ i ] )
						return true;

				return false;
			}

			bool DestroyBlock::has_pos_1057( const Pos& pos ) const
			{
				pos.corrent_assert();
				assert( pos.is_init()
					&& "DestroyBlock::has_pos_1057:pos��Ч" );

				for( size_t i = 0; i < _1057_block_list.size(); i++ )
					if( pos == _1057_block_list[ i ] )
						return true;

				return false;
			}

			void DestroyBlock::del_545( const Pos& pos )
			{
				pos.corrent_assert();
				assert( pos.is_init()
					&& "DestroyBlock::has_pos_545:pos��Ч" );
				assert( has_pos_545( pos )
					&& "DestroyBlock::del_545:Ҫɾ����pos������" );

				for( vector<Pos>::iterator it = _545_block_list.begin();
					it != _545_block_list.end(); )
					if( pos == *it )
						it = _545_block_list.erase(it);
					else
						it++;
			}
			void DestroyBlock::del_1057( const Pos& pos )
			{
				pos.corrent_assert();
				assert( pos.is_init()
					&& "DestroyBlock::has_pos_1057:pos��Ч" );
				assert( has_pos_1057( pos )
					&& "DestroyBlock::del_1057:Ҫɾ����pos������" );

				for( vector<Pos>::iterator it = _1057_block_list.begin();
					it != _1057_block_list.end(); )
					if( pos == *it )
						it = _1057_block_list.erase(it);
					else
						it++;
			}

			Pos DestroyBlock::nearest_pos( const Pos& pos ) const
			{
				assert( _is_set
					&& "DestroyBlock::nearest_pos:�ϰ���δ�������" );

				pos.corrent_assert();
				pos.init_assert();

				float shortest_dis = 0.f;
				size_t shortest_index = 0;
				Pos shortest_pos;
				for( size_t i = 0; i < _1057_block_list.size(); i++ )
				{
					float dis = compute_distance( _1057_block_list[ i ], pos );

					if( 0 == i )
					{
						shortest_dis = dis;
						shortest_index = i;
					}
					else
					{
						if( dis < shortest_dis )
						{
							shortest_dis = dis;
							shortest_index = i;
						}
					}
				}

				shortest_pos = _1057_block_list[ shortest_index ];
				shortest_index = -1;

				for( size_t i = 0; i < _545_block_list.size(); i++ )
				{
					float dis = compute_distance( _545_block_list[ i ], pos );
					if( dis < shortest_dis )
					{
						shortest_dis = dis;
						shortest_index = i;
					}
				}

				if( -1 != shortest_index )
					shortest_pos = _545_block_list[ shortest_index ];

				return shortest_pos;
			}

			bool DestroyBlock::is_set()
			{
				return _is_set;
			}

			//	--------------------------
			Room::Room()
				:_is_changed( false )
			{}

			string Room::get_filename( const string& _ext )
			{
				//assert( !_file_name.empty()
				//	&& "Room::get_filename:_file_nameΪ��" );
				
				if( _file_name.empty() )
					return _ext;

				size_t pos = _file_name.find_last_of( '.' );
				assert( pos != string::npos
					&& "Room::get_filename:_file_name��ʽ����ȷ" );

				return _file_name.substr( 0, pos ) + _ext;
			}

			void Room::set_filename( const string& file_name )
			{
				assert( !file_name.empty()
					&& "Room::set_filename:file_nameΪ��" );

				assert( file_name.find( '.' ) != string::npos
					&& "Room::set_filename:file_name��ʽ����ȷ" );

				_file_name = file_name;
			}

			//	��������
			void Room::save(const string& file_name )
			{
				ofstream ofs;
				if( "" != file_name )
					ofs.open( file_name );	
				else
				{
					assert( !_file_name.empty()
						&& "Room::save:main_file_nameΪ��" );

					ofs.open( _file_name );
				}

				if( !ofs )
					throw std::exception( "Room::save�޷����ļ�." );

				boost::archive::text_oarchive boa( ofs );

				boa & *this;
			}

			void Room::load(const string& file_name )
			{
				ifstream ifs;
				if( "" != file_name )
					ifs.open( file_name );
				else
				{
					assert( !_file_name.empty()
						&& "Room::load:main_file_nameΪ��" );

					ifs.open( _file_name );
				}
				if( !ifs )
					throw std::exception( "Room::load�޷����ļ�." );

				boost::archive::text_iarchive bia( ifs );

				bia & *this;
			}
			bool Room::is_changed()
			{
				return _is_changed;
			}
			void Room::set_changed()
			{
				_is_changed = true;
			}
			void Room::set_unchanged()
			{
				_is_changed = false;
			}
*/
			///////////////////////////////////////////
			//	Input����ģ��
			Input::Input( World& world )
				:_world( world ),
				_current_room_ptr( 0 ),
				_is_start( false ),
				_is_show_pic( false )
			{
			}

			void Input::set_current_room_ptr( Room& room )
			{
				assert( &room
					&& "Input::set_current_room_ptr:roomָ���ָ��" );

				_current_room_ptr = &room;
			}

			void Input::operator()()
			{
				cout << "����:help����?����h��ȡ����" << endl;
				//	�ȴ�������Ϣ���������Ϣѭ��
				while( true )
				{
					cout << ">";
					string cmd;
					getline( cin, cmd, '\n' );


					if( "s" == cmd )
					{
						//_is_start = true;
						//start();
					}

					if( "e" == cmd )
					{
						//_is_start = false;
						//end();
					}

					//	---------add,del---------

					if( "ap" == cmd )
					{
						cout << "��ӽڵ�" << endl;
						add_node();
					}

					vector<string> argv_list;
					if( cmd.find( ' ' )
						!= string::npos )
					{
						string str;
						istringstream iss( cmd );
						while( getline( iss, str, ' ' ) )
							argv_list.push_back( str );

						if( "ae" == argv_list[ 0 ] )
						{
							cout << "��ӱ�:" << endl;
							add_edge( argv_list );
						}

						if( "dp" == argv_list[ 0 ] )
						{
							cout << "ɾ���ڵ�" << endl;
							del_node( argv_list );
						}

						if( "de" == argv_list[ 0 ] )
						{
							cout << "ɾ����" << endl;
							del_edge( argv_list );
						}
					}

					if( "sp" == cmd )
					{
						_is_show_pic = true;
						//	����,�ȴ�
						//	Control����
						while(true)
							if( !_is_show_pic )
								break;
					}
					if( "help" == cmd 
						|| "h" == cmd
						|| "?" == cmd )
					{
						help();
					}
				}
			}

			void Input::start()
			{
				cout << "��ʼ��¼" << endl;
			}

			void Input::end()
			{
				cout << "��ɼ�¼" << endl;
			}

			void Input::add_node()
			{
				_world.update_player_map_pos();

				_world.player.map_pos.corrent_assert();
				_world.player.map_pos.init_assert();

				Node node;
				//node.type = Node::record;
				node.set_record_type();
				node.pos = _world.player.map_pos;

				_current_room_ptr->_node_mgr.add_vertex( 
					node );

				_current_room_ptr->set_changed();
			}

			
			void Input::add_edge( const vector<string>& argv_list )
			{
				assert( argv_list.size() > 1
					&& "Input::add_edge:argv_list������Ŀ����ȷ,С��2" );

				NodeMgr::vd_t source_vd, target_vd;

				for( size_t i = 1; i < argv_list.size(); i++ )
				{
					_argv_to_edge_vd( argv_list[ i ], source_vd, target_vd );

					Edge edge;

					edge.set_weight( 
						_current_room_ptr->_node_mgr[ source_vd ],
						_current_room_ptr->_node_mgr[ target_vd ]
						);

					_current_room_ptr->_node_mgr.add_edge(
						source_vd, target_vd,
						edge );

					_current_room_ptr->_node_mgr.add_edge(
						target_vd, source_vd,
						edge );
				}

				_current_room_ptr->set_changed();
			}

			void Input::del_node( const vector<string>& argv_list )
			{
				assert( argv_list.size() > 1
					&& "Input::del_node:argv_list������Ŀ����ȷ,С��2" );

				NodeMgr::vd_t vd;

				for( size_t i = 1; i < argv_list.size(); i++ )
				{
					_argv_to_node_vd( argv_list[ i ], vd );
					_current_room_ptr->_node_mgr.del_vertex( vd );
				}
				_current_room_ptr->set_changed();
			}

			void Input::del_edge( const vector<string>& argv_list )
			{
				assert( argv_list.size() > 1
					&& "Input::del_edge:argv_list������Ŀ����ȷ,С��2" );

				NodeMgr::vd_t source_vd, target_vd;

				for( size_t i = 1; i < argv_list.size(); i++ )
				{
					_argv_to_edge_vd( argv_list[ i ], 
						source_vd, target_vd );

					_current_room_ptr->_node_mgr.del_biedge(
						source_vd, target_vd );
				}
				_current_room_ptr->set_changed();
			}

			bool Input::_right_state()
			{
				if( !_is_start )
				{
					cout << "�����޷����,����״̬����ȷ,������"
						<< "δ���뿪ʼ״̬���߼�¼�Ѿ�����" << endl;

					return false;
				}
				else
					return true;
			}

			void Input::_argv_to_edge_vd( 
					const string& argv,
					NodeMgr::vd_t& source_vd,
					NodeMgr::vd_t& target_vd
					)
			{
				assert( !argv.empty()
					&& "Input::_argv_to_edge_vd:argvΪ��" );
				assert( check_only_letter( "-,0123456789", argv )
					&& "Input::_argv_to_edge_vd:argv���зǷ��ַ�" );

				istringstream iss( argv );

				string source_str, target_str;
				if( argv.find( ',' ) != string::npos )
				{
					getline( iss, source_str, ',' );
					getline( iss, target_str, ',' );
				}
				else if( argv.find( '-' ) != string::npos )
				{
					getline( iss, source_str, '-' );
					getline( iss, target_str, '-' );
				}
				else
					assert( false
						&& "Input::_argv_to_ed:argv������δ���зָ��" );

				assert( !source_str.empty()
					&& !target_str.empty()
					&& "Input::_argv_to_ed:argv��ʽ����,���³�ʼ��ed����ȫ" );

				istringstream source_stream( source_str );
				source_stream >> source_vd;

				istringstream target_stream( target_str );
				target_stream >> target_vd;

			}

			void Input::_argv_to_node_vd(
					const string& argv,
					NodeMgr::vd_t& vd
					)
			{
				assert( !argv.empty()
					&& "Input::_argv_to_node_vd:argvΪ��" );
				assert( check_only_letter( "0123456789", argv )
					&& "Input::_argv_to_node_vd:argv���зǷ��ַ�" );

				istringstream iss( argv );
				iss >> vd;
			}

			bool Input::is_show_pic()
			{
				return _is_show_pic;
			}
			void Input::set_show_pic_end()
			{
				_is_show_pic = false;
			}

			void Input::help()
			{
				cout << "---------------------------------------------------" << endl;
				cout << "����:" << endl;
				cout << "ap\tadd_point,��Ӽ�¼��" << endl;
				cout << "ae\tadd_edge,��Ӽ�¼��" << endl;
				cout << "\t  ��ae 0-1 5-6 �� ae 0,1 5,6" << endl;
				cout << "dp\tdel_point,ɾ����¼��" << endl;
				cout << "\t  ��dp 0 2 4" << endl;
				cout << "de\tdel_edge,ɾ����¼��" << endl;
				cout << "\t  ��de/ 0-1 2-4 �� ae 0,1 2,4" << endl;
				cout << "sp\tshow_pic,��ʾЧ��ͼƬ" << endl;
				cout << "---------------------------------------------------" << endl;
			}

			//////////////////////"////////////
			//	Store��Ҫ��ģ��
			//	ǰ�������ǣ��ļ����ڵ�Ŀ¼
			//	�Ѿ����ڡ�
			//	�����Ƕ�ȡ�Ѿ����ڵķ����������ļ�
			//	�Ѿ�������Ҫ�洢���ļ�������
			AutoFile::AutoFile(){}

			void AutoFile::set_work_dir( const string& work_dir )
			{
				assert( !work_dir.empty()
					&& "AutoNumFile::set_work_dir: work_dirΪ��" );

				assert( bfs::is_directory( work_dir )
					&& "AutoNumFile::set_work_dir: work_dir����һ��Ŀ¼" );

				_work_dir = work_dir;
			}

			bool AutoFile::load_file_list( vector<string>& file_list, const string& file_ext )
			{
				assert( !file_ext.empty()
					&& "AutoNumFile::load_file_list:_file_extΪ��" );

				file_list.clear();

				if( bfs::is_empty( _work_dir ) )
					return false;

				bfs::directory_iterator it( _work_dir );
				bfs::directory_iterator it_end;
				for( ; it != it_end; it++ )
					if( it->path().extension() == file_ext )
					{

#ifdef _DEBUG		
						size_t pos = it->path().filename().string().find( '.' );

						assert( pos > 0
							&& pos != string::npos
							&& "AutoNumFile::load_file_list:cut�쳣" );

						string file_main_name = 
							it->path().filename().string().substr( 0, pos );

						assert( 
							check_only_letter( "0123456789",
							file_main_name )
							&& !file_main_name.empty()
							&& "AutoNumFile::load_file_list:�ļ����д��ڷǷ���ʽ���ļ�" );
#endif

						file_list.push_back( it->path().string() );
					}


					if( file_list.empty() )
						return false;

					return true;
			}

			void AutoFile::gen_save_file_name( string& save_file_name, const string& file_ext )
			{
				assert( !file_ext.empty()
					&& "AutoFile::gen_save_file_name:_file_extΪ��" );

				save_file_name.clear();

				if( bfs::is_empty( _work_dir ) )
				{
					save_file_name = "0" + file_ext;
					return;
				}

				vector<string> file_list;

				if( !load_file_list( file_list, file_ext ) )
				{
					save_file_name = "0" + file_ext;
					return;
				}
				else
				{
					vector<size_t> file_num_list;

					size_t start_pos = _work_dir.size() + 1;

					for( size_t i = 0; i < file_list.size(); i++ )
					{
						size_t pos = file_list[ i ].find( '.' );
						istringstream iss( 
						 file_list[ i ].substr( start_pos, pos - start_pos ) );
						size_t file_num;
						iss >> file_num;
						file_num_list.push_back( file_num );
					}

					vector<size_t>::iterator it = 
						::max_element( file_num_list.begin(), file_num_list.end() );

					ostringstream oss;
					oss << ++(*it);

					save_file_name = oss.str() + file_ext;

					return;
				}
			}

			/////////////////////////////////////
			//	�洢ģ��
			Store::Store( World& world )
				:_world( world ),
				_map_dir( "map" ),
				_room_ext( ".room" ),
				_exist_room_list_ptr( 0 ),
				_current_room_ptr( 0 )
			{
			}
			void Store::set_exist_room_list_ptr(
				vector<Room>& exist_room_list )
			{
				assert( &exist_room_list
					&& "Store::set_exist_room_list_pt:exsit_room_listָ��0" );

				_exist_room_list_ptr = &exist_room_list;
			}
			void Store::set_current_room_ptr(
				Room& current_room )
			{
				assert( &current_room
					&& "Store::set_current_room_ptr:current_roomָ��0" );

				_current_room_ptr = &current_room;
			}
			 
			void Store::set_map_name_dir()
			{
				assert( bfs::exists( _map_dir ) 
					&& "Store::set_map_name_dir:_map_dir������" );

				vector<string> map_name_list;

				_get_map_name_list( map_name_list );

				string current_map_name = _world.dnf.GetMapName();

				assert( !current_map_name.empty()
					&& "Store::set_map_name_dir::current_map_nameΪ��,\
					   �޷��Ӵ���ʶ���ͼ������" ); 

				size_t max_size = 0;
				size_t best_index = -1;

				for( size_t i = 0; i < map_name_list.size(); i++ )
					if( current_map_name.find( map_name_list[ i ] )
						!= string::npos )
						if( map_name_list[ i ].size() > max_size ) 
						{
							max_size = map_name_list[ i ].size();
							best_index = i;
						}

				assert( -1 != best_index
					&& "Store::set_map_name_dir:�Ҳ�����Ч�ĵ�ͼĿ¼" );

				_map_name_dir = 
					_map_dir + "\\" 
					+ map_name_list[ best_index ];


				assert( _map_name_dir.size() > ( _map_dir.size() + 1 )
					&& "Store::set_map_name_dir:_map_name_dir���ô���" );

				_auto_file.set_work_dir( _map_name_dir );
			}

			bool Store::_get_map_name_list( vector<string>& map_name_list )
			{
				map_name_list.clear();
			
				if( bfs::is_empty( _map_dir ) )
				{
					assert( 0
						&& "Store::_get_map_name_list,��ͼ�����ڵ�Ŀ¼Ϊ��" );
					return false;
				}

				bfs::directory_iterator it( _map_dir );
				bfs::directory_iterator it_end;
				for( ; it != it_end; it++ )
					map_name_list.push_back(
					it->path().filename().string() );

				assert( !map_name_list.empty()
					&& "Store::_get_map_name_list:map_name_listδ�ܻ�ȡ����ͼ��" );

				return true;
			}

			bool Store::load_room()
			{
				if( !bfs::exists( _map_name_dir ) )
					return false;

				if( bfs::is_empty( _map_name_dir ) )
					return false;

				vector<string> file_list;

				_auto_file.set_work_dir( _map_name_dir );
				_auto_file.load_file_list( file_list, _room_ext );

				assert( 0 != _exist_room_list_ptr
					&& "Store::load_room:_exist_room_list_ptr�쳣" );

				_exist_room_list_ptr->clear();

				assert( _exist_room_list_ptr->empty()
					&& "Store::load_room:_exist_room_list_ptr��Ϊ��" );



				for( size_t i = 0; i < file_list.size(); i++ )
				{
					//	���ﲻ�ܰѳ�ʼ�����ں���
					//	����room��������Լ�
					//	���ǵ����ڵ㡣
					Room room;

					room.load( file_list[ i ] );	
					_exist_room_list_ptr->push_back( room );
				}

				if( _exist_room_list_ptr->empty() )
					return false;
				else
					return true;
			}
			//
			void Store::save_room()
			{
				assert( 0 != _current_room_ptr 
					&& "Store::save_map:save_room_ptrΪ��ָ��" );
				string save_dir;

				//	����ļ���Ϊ��
				//	˵���÷���û�ж�Ӧ���ļ�
				//	�����µ��ļ���
				if( ".room" == _current_room_ptr->get_filename() )
				{
					_auto_file.gen_save_file_name( save_dir, ".room" );
					save_dir = _map_name_dir + "\\" + save_dir;
					_current_room_ptr->set_filename( save_dir );
				}
				else
					save_dir = _current_room_ptr->get_filename();

				_current_room_ptr->save( save_dir );

				assert( bfs::exists( save_dir )
					&& "Store::save_room:�����ļ�ʧ��" );

				assert( !bfs::is_empty( save_dir )
					&& "Store::save_room:����ʧ�ܣ��ļ�Ϊ��" );
			}

			//	ǰ���������ܹ���ȡ
			//	filename,���Ѿ��������.
			void Store::save_room_png()
			{

				string save_dot_dir = _current_room_ptr->get_filename( ".dot" );
				assert( save_dot_dir != ".dot" 
					&& "Store::save_room_png:.dot�洢·����ȡ����" );
				string save_png_dir = _current_room_ptr->get_filename( ".png" );
				assert( save_png_dir != ".dot" 
					&& "Store::save_room_png:.png�洢·����ȡ����" );

				//save_dot_dir = _map_name_dir + "\\" + save_dot_dir;
				//save_png_dir = _map_name_dir + "\\" + save_png_dir;

				_current_room_ptr->_node_mgr.save_dot( save_dot_dir );

				assert( bfs::exists( save_dot_dir )
					&& "Store::save_room_png:����dot�ļ�ʧ��" );

				assert( !bfs::is_empty( save_dot_dir )
					&& "Store::save_room_png:����dot�ļ�ʧ�ܣ��ļ�Ϊ��" );

				string cmd 
					= "\"\"graphviz-2.36\\release\\bin\\dot.exe\" -Kfdp -n -Tpng \""
					+ save_dot_dir + "\" -o \"" + save_png_dir + "\"\"";

				::system( cmd.c_str() );

				assert( bfs::exists( save_png_dir )
					&& "Store::save_room_png:����png�ļ�ʧ��" );

				assert( !bfs::is_empty( save_dot_dir )
					&& "Store::save_room_png:����png�ļ�ʧ�ܣ��ļ�Ϊ��" );
			}

			
			///////////////////////////////
			//	��¼ģ��
			Record::Record( World& world )
				:_world( world ),
				_map_dir( "map" ),
				_current_room_ptr( 0 ),
				_exist_room_list_ptr( 0 ),
				_is_join( false ),
				_is_leave( true ),
				_is_first_map( true )
#ifdef _DEBUG
				,_right_state_cnt( 0 )
#endif
			{
			}
			
			void Record::set_current_room_ptr( Room& current_room )
			{
				assert( &current_room
					&& "Record::set_current_room_ptr:current_roomָ���ָ��" );

				_current_room_ptr = &current_room;
			}

			void Record::set_exist_room_list_ptr( 
				vector<Room>& exist_room_list )
			{
				assert( &exist_room_list
					&& "Record::set_exist_room_list_ptr: exist_room_listָ���ָ��" );

				_exist_room_list_ptr = &exist_room_list;
			}

			bool Record::is_join()
			{
				return _is_join;
			}

			void Record::set_join()
			{
				_is_join = true;
			}
			void Record::set_unjoin()
			{
				_is_join = false;
			}


			bool Record::map_name_is_changed()
			{
				string map_name = 
					_get_map_name();

				if( map_name.empty() )
					return false;
				
				if( map_name != _map_name )
					return true;
				else
					return false;
			}

			bool Record::is_corrent_map()
			{
				string map_name =
					_get_map_name();

				if( map_name.empty() )
					return false;
				else
					return true;
			}

			void Record::update_map_name()
			{
				_map_name = _get_map_name();
			}

			bool Record::right_state()
			{
				//	��ʱʹ��map_win_pos
				//	���ж�,��������ֶ�
				//	�ر�С��ͼ�ᵼ���߼�
				//	����.


				//	��ת���޸�,��Ϊboss��־
				//	һֱ������.����������boss��־
				//	��ȷ���Ƿ���������״̬
				_world.update_boss_room_win_pos();
				//_world.update_map_win_pos();


				//if( _world.map_pos.is_invaild() )
				if( _world.boss_room_pos.is_invaild() )
				{
#ifdef _DEBUG
					_right_state_cnt++;
					if( _right_state_cnt > 10000 )
					{
						assert( false
							&& "Record::right_state:20000���Ҳ���С��ͼ����" );

						_right_state_cnt = 0;
					}
#endif
					return false;
				}
				else
				{
#ifdef _DEBUG
					_right_state_cnt = 0;

#endif
					return true;
				}
			}

			bool Record::is_first_map()
			{
				return _is_first_map;
			}

			void Record::unset_first_map()
			{
				_is_first_map = false;
			}

			string Record::_get_map_name()
			{
				assert( bfs::exists( _map_dir ) 
					&& "Store::set_map_name_dir:_map_dir������" );

				vector<string> map_name_list;

				_get_map_name_list( map_name_list );

				string current_map_name = _world.dnf.GetMapName();

				if( current_map_name.empty() )
					return current_map_name;

				assert( !current_map_name.empty()
					&& "Record::set_map_name_dir::current_map_nameΪ��,\
					   �޷��Ӵ���ʶ���ͼ������" ); 

				size_t max_size = 0;
				size_t best_index = -1;

				for( size_t i = 0; i < map_name_list.size(); i++ )
					if( current_map_name.find( map_name_list[ i ] )
						!= string::npos )
						if( map_name_list[ i ].size() > max_size ) 
						{
							max_size = map_name_list[ i ].size();
							best_index = i;
						}

				//assert( -1 != best_index
				//	&& "Store::set_map_name_dir:�Ҳ�����Ч�ĵ�ͼĿ¼" );

				if( -1 == best_index )
					return string();
				else	
					return map_name_list[ best_index ];
			}

			bool Record::_get_map_name_list( vector<string>& map_name_list )
			{
				map_name_list.clear();
			
				if( bfs::is_empty( _map_dir ) )
				{
					assert( 0
						&& "Record::_get_map_name_list,��ͼ�����ڵ�Ŀ¼Ϊ��" );
					return false;
				}

				bfs::directory_iterator it( _map_dir );
				bfs::directory_iterator it_end;
				for( ; it != it_end; it++ )
					map_name_list.push_back(
					it->path().filename().string() );

				assert( !map_name_list.empty()
					&& "Record::_get_map_name_list:map_name_listδ�ܻ�ȡ����ͼ��" );

				return true;
			}

			bool Record::is_change_room()
			{
				_world.update_map_win_pos();

				if( !_last_map_pos.is_init() )
				{
					if( !_world.map_pos.is_invaild() )
						_last_map_pos = _world.map_pos;

					return false;
				}

				if( _world.map_pos.is_invaild() )
				{
					_world.update_boss_room_win_pos();
					if( !_world.boss_room_pos.is_invaild()
						&& _world.boss_room_pos.is_init()
						&& 	
						(
							( 
								abs( _world.boss_room_pos.x - _last_map_pos.x ) < 20
								&& _world.boss_room_pos.y == _last_map_pos.y 
							)
							||
							(
								abs( _world.boss_room_pos.y - _last_map_pos.y ) < 20
								&& _world.boss_room_pos.x == _last_map_pos.x
							)
						)
						&& !_is_boss_room
					)
					{
						//trace( "change_room:boss_room" );

						return true;
					}
					else
					{
						set_unleave();
						return false;
					}
				}
				else
				{
					if( _world.map_pos != _last_map_pos )
					{
						//trace( "change_room" );
						return true;
					}
					else
						return false;
				}
			}

			bool Record::update_last_map_pos()
			{
				_world.update_map_win_pos();
				_world.map_pos.init_assert();
				
				if( !_world.map_pos.is_invaild() )
				{
					_last_map_pos = _world.map_pos;
					return true;
				}
				else
					return false;

			}

			void Record::join()
			{
				//	����_current_room_ptr;
				*_current_room_ptr = Room();

				//	��ȡ��ǰ�����ָ��
				set_rfp();

				//	�������ƥ�䷿����ݲ�
				_current_room_ptr->_rfp.set_comp_scale( 0.9f );

				//	�Աȵ��ڷ����Ƿ���
				//	��exist_list����
				if( is_in_exist_list() )
				{
					*_current_room_ptr = best_sim_room();
					_current_room_ptr->set_unchanged();

					cout << "�÷������м�¼�ļ�";
					if( _current_room_ptr->_node_mgr.has_record_node() )
						cout << "�ͼ�¼��" << endl;
					else
						cout << ",��û�м�¼��" << endl;
				}
				else
				{
					//	�����ϰ���
					_world.update_target_group_map_pos();
					_current_room_ptr->_destroy_block.set_start(
						_world.target_group );

					_current_room_ptr->set_changed();

					cout << "�����·���" << endl;
				}
			}

			void Record::set_rfp()
			{
				_world.update_target_group_map_pos();
				_current_room_ptr->_rfp.set(
					_world.target_group );

				_current_room_ptr->_rfp.corrent_assert();
			}

			bool Record::is_in_exist_list()
			{

				float best_sim = 0;

				for( size_t i = 0; i < _exist_room_list_ptr->size(); i++ )
				{
					float sim =
						comp_sim( (*_exist_room_list_ptr)[ i ]._rfp, _current_room_ptr->_rfp );

					if( best_sim < sim )
						best_sim = sim;
				}

				if( best_sim > _current_room_ptr->_rfp.get_comp_scale() )
					return true;
				else
					return false;

			}

			Room& Record::best_sim_room()
			{
				assert( is_in_exist_list()
					&& "Record::best_sim_room:exist_list�����ڶ�Ӧ��room" );

				float best_sim = 0;
				size_t best_sim_index;

				for( size_t i = 0; i < _exist_room_list_ptr->size(); i++ )
				{
					float sim =
						comp_sim( (*_exist_room_list_ptr)[ i ]._rfp, _current_room_ptr->_rfp );

					if( best_sim < sim )
					{
						best_sim = sim;

						best_sim_index = i;
					}
				}

				return (*_exist_room_list_ptr)[ best_sim_index ];
			}
			
			void Record::update( Store& store )
			{
				_world.update_player_map_pos();

				if( _world.player.map_pos.is_init()
					&& !_world.player.map_pos.is_invaild()
					&& is_leave() )
					_last_player_pos = _world.player.map_pos;

				if( !_current_room_ptr->_destroy_block.is_set() )
				{
					_world.update_target_group_map_pos();
					
				//	if( _current_room_ptr->_destroy_block.is_end(
				//		_world.target_group	) )

					if( !right_state()
						|| _world.is_dungeon_end() )
					{
						_current_room_ptr->_destroy_block.set_end(
							_world.target_group );

						if( is_boss_room() )
							if( _current_room_ptr->is_changed()
								|| _world.is_dungeon_end() )
							{
								update_door_edge();

								store.save_room();
								store.save_room_png();

								trace( "end" );
							}
					}
				}
			}

			void Record::leave()
			{
				set_door();

			}

			void Record::set_door()
			{
				Door door;

				door.dir = _door_dir();

				if( door.dir == Dir::no )
				{
					cout << "warning:Dir::no" << endl;
					return;
				}

				_last_player_pos.init_assert();
				_last_player_pos.corrent_assert();
				door.pos = _last_player_pos;

				if( _current_room_ptr->_door_mgr.has_dir( door.dir ) )
					return;
				else
				{
					_current_room_ptr->set_changed();
					_current_room_ptr->_door_mgr.update( door );

					Node door_node;
					door_node.pos = door.pos;
					door_node.set_door_type();

					_current_room_ptr->_node_mgr.add_vertex( door_node );

				}
			}

			void Record::update_door_edge()
			{
				_current_room_ptr->_node_mgr.update_door_edge();
			}
	
			bool Record::is_leave()
			{
				return _is_leave;
			}

			void Record::set_leave()
			{
				_is_leave = true;
			}
			void Record::set_unleave()
			{
				_is_leave = false;
			}
				

			Dir Record::_door_dir()
			{
				_world.update_map_win_pos();
				//_world.map_pos.corrent_assert();
				_world.map_pos.init_assert();

				if( !_world.map_pos.is_invaild() )
				{
					assert( _world.map_pos != _last_map_pos
						&& "Record::_door_dir:������ʧ��,���ͼ�ı��,��ɫ����δ�ı�" );

					if( _last_map_pos.x == _world.map_pos.x )
					{
						if( _last_map_pos.y > _world.map_pos.y )
							return Dir::bottom;
						if( _last_map_pos.y < _world.map_pos.y )
							return Dir::top;
					}
					if( _last_map_pos.y == _world.map_pos.y )
					{
						if( _last_map_pos.x > _world.map_pos.x )
							return Dir::left;
						if( _last_map_pos.x < _world.map_pos.x )
							return Dir::right;
					}
				}

				if( _world.map_pos.is_invaild() )
				{
					_world.update_boss_room_win_pos();
					if( !_world.boss_room_pos.is_invaild()
						&& _world.boss_room_pos.is_init()
						&& 	
						(
							( 
								abs( _world.boss_room_pos.x - _last_map_pos.x ) < 20
								&& _world.boss_room_pos.y == _last_map_pos.y 
							)
							||
							(
								abs( _world.boss_room_pos.y - _last_map_pos.y ) < 20
								&& _world.boss_room_pos.x == _last_map_pos.x
							)
							)
							)
					{
						if( _last_map_pos.x == _world.boss_room_pos.x )
						{
							if( _last_map_pos.y > _world.boss_room_pos.y )
								return Dir::bottom;
							if( _last_map_pos.y < _world.boss_room_pos.y )
								return Dir::top;
						}
						if( _last_map_pos.y == _world.boss_room_pos.y )
						{
							if( _last_map_pos.x > _world.boss_room_pos.x )
								return Dir::left;
							if( _last_map_pos.x < _world.boss_room_pos.x )
								return Dir::right;
						}
					}
				}

				//	�������
				return Dir::no;
			}


			bool Record::is_boss_room()
			{
				return _is_boss_room;
			}

			//	ȷ�����Ƿ���boss����
			void Record::update_boss_room()
			{
				//	��ת���޸�.��boss�����boss��ǻ�����
				//	��������û����.����֮ǰ�ж��Ƿ���
				//	boss����ķ����Ѿ�ʧЧ��.

				//_world.update_map_win_pos();
				//_world.map_pos.corrent_assert();
				//_world.map_pos.init_assert();
				//_world.map_pos.invalid_assert();

				//if( _world.map_pos == _boss_room_pos )
				//	_is_boss_room = true;

				//	�޸�Ϊ,���boss��Ǵ���.���������ǲ�����
				//	����._last�����Ǻ�boss��ǵľ���Ϊһ������
				//	������ж�Ϊ������boss����
				_world.update_map_win_pos();
				if( _world.map_pos.is_invaild() )
				{
					_world.update_boss_room_win_pos();
					if( !_world.boss_room_pos.is_invaild()
						&& _world.boss_room_pos.is_init()
						&& 	
						(
							( 
								abs( _world.boss_room_pos.x - _last_map_pos.x ) < 20
								&& _world.boss_room_pos.y == _last_map_pos.y 
							)
							||
							(
								abs( _world.boss_room_pos.y - _last_map_pos.y ) < 20
								&& _world.boss_room_pos.x == _last_map_pos.x
							)
						)
					)
					{
						_is_boss_room = true;
						//trace( "_is_boss_room = true;" );
					}
				}
			}

			void Record::unset_boss_room()
			{
				_is_boss_room = false;
			}


			void Record::set_boss_room_pos()
			{
				_world.update_boss_room_win_pos();

				if( _world.boss_room_pos.is_invaild() )
				{
					_world.update_map_win_pos();
					if( !_world.map_pos.is_invaild() )
					{
						_world.map_pos.corrent_assert();
						_world.map_pos.init_assert();
						_boss_room_pos = _world.map_pos;
					}
					else
						assert( false
						&& "Record::set_boss_room_pos:����boss�������" );
				}
				else
				{
					_world.boss_room_pos.init_assert();
					_world.boss_room_pos.corrent_assert();
					_boss_room_pos = _world.boss_room_pos;
				}
			}

			///////////////////////////////
			//	Control

			Control::Control( World& world )
				:_world( world ),
				_record( _world ),
				_store( _world ),
				_input( world ),
				_input_thread( boost::ref( _input ) )
			{}

			void Control::init()
			{
				_input.set_current_room_ptr( current_room );
				_record.set_current_room_ptr( current_room );	
				_record.set_exist_room_list_ptr( exist_room_list );
				_store.set_current_room_ptr( current_room );
				_store.set_exist_room_list_ptr( exist_room_list );
			}

			void Control::operator()()
			{
				init();

				while( true )
				{
					if( _record.right_state()
						&& _record.map_name_is_changed()
						&& _record.is_corrent_map() )
					{
						_record.unset_boss_room();
						_store.set_map_name_dir();
						_store.load_room();

						_record.update_map_name();
						_record.set_boss_room_pos();
					}

					if( _record.is_change_room() )
					{

						//	���֮ǰ
						if( _record.is_join() )
						{
							//	���� 
							_record.set_door();

							if( current_room.is_changed() )
							{
								_record.update_door_edge();

								_store.save_room();
								_store.save_room_png();
							}

							current_room.set_unchanged();

							if( _record.is_in_exist_list() )
								_record.best_sim_room()
								= current_room;
							else
								exist_room_list.push_back(
								current_room );

							_record.update_last_map_pos();
							
							_record.set_leave();
							_record.set_unjoin();
						}

						_record.update_boss_room();

						//	��ʼ
						if( _record.is_corrent_map() )
						{
							_record.join();
							_record.set_join();
						}
					}

					if( _record.right_state() 
						&& _record.is_corrent_map() 
						&& _record.is_first_map() )
					{
#ifdef _DEBUG
						//	�����Ƿ�Ϊ��������
						//	������Ļ�л��й�
						bool is_found_529 = false;
						_world.update_target_group_map_pos();
						for( size_t i = 0; i < _world.target_group.size(); i++ )
							if( 529 == _world.target_group[ i ].id )
								is_found_529 = true;

						assert( is_found_529 
							&& "Control::operator()():is_first_map�߼�����" );
#endif

						_store.set_map_name_dir();
						_store.load_room();

						_record.update_map_name();

						_record.join();
						_record.set_join();
						_record.unset_first_map();

						_record.update_boss_room();
					}

					_record.update( _store );

					if( _input.is_show_pic() )
					{
						if( _record.is_corrent_map() )
						{
							if( current_room.is_changed() )
							{
								_record.update_door_edge();
								_store.save_room();
								_store.save_room_png();
							}

							string png_dir =
								current_room.get_filename( ".png" );
							std::system( png_dir.c_str() );
						}
						else
							cout << "�������ڵĵ�ͼ����ȷ" << endl;

						_input.set_show_pic_end();
					}
				}
			}

			/////////////////////////////////////////////////////////////
			//	RoomRecorder
			RoomRecorder::RoomRecorder( Dm& dm_, AttachIme& attach_ime_ )
				:_world( dm_, attach_ime_ ),
				_control( _world )
			{
				_control();
			}

_ROOM_END

//		}	//	namespace room
//	}	//	namespace map
//}	//	namespace dnf
