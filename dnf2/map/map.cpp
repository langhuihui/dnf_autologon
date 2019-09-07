#include "map.h"

//namespace dnf{
//	namespace map{

_MAP_BEGIN



		void Map::save_dot( const string& file_name ) const
		{
			assert( !file_name.empty()
				&& "Map.save_dot:保存的文件名为空" );

			assert( boost::num_vertices( _g ) 
				&& "Map.save_dot:_g节点为空" );

			ofstream ofs( file_name );

			if( !ofs )
				throw std::exception( "Map.save_dot:打不开保存文件" );

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
						pos_x_vec.push_back( _g[ *(vp.first) ].x );
						pos_y_vec.push_back( _g[ *(vp.first) ].y );
					}

					min_pos_x = *min_element( pos_x_vec.begin(), pos_x_vec.end() );
					//min_pos_y = *min_element( pos_y_vec.begin(), pos_y_vec.end() );
					//max_pos_x = *max_element( pos_x_vec.begin(), pos_x_vec.end() );
					max_pos_y = *max_element( pos_y_vec.begin(), pos_y_vec.end() );
				}

				void operator()(std::ostream& out, const vd_t& v ) const 
				{
					Pos pos;
					pos.x = _g[ v ].x - min_pos_x;
					pos.y = max_pos_y - _g[ v ].y;

					float x = (float)pos.x / 8.f;
					float y = (float)pos.y / 8.f;

					out << "[shape = polygon, sides = 4, label=\""
						<< _g[ v ].x// - min_pos_x
						<< ','
						<< _g[ v ].y// - min_pos_y
						<< " ["
						<< v
						<< "]"
						<< "\", pos = \""
						<< x
						<< ','
						<< y
						<< "!\" ]" ;
				}

				int min_pos_x;
				//int min_pos_y;
				//int max_pos_x;
				int max_pos_y;

				const graph_t& _g;
			};

			struct edge_writer {
				edge_writer(
					const graph_t& g
					)
					:_g( g )
				{}

				void operator()(std::ostream& out, const ed_t& e ) const
				{
					//out << "[ label = \""
					//	<< _g[ e ].dir
					//	<< "\" "
					//	<< "]" ;
				}
				const graph_t& _g;
			};

			struct graph_writer {
				void operator()(std::ostream& out) const {
					out << "graph [bgcolor=white size=\"20,20\"]" << endl;
					out << "node [shape=circle color=black]" << endl;
					//out << "edge [style=dashed]" << endl;
				}
			};

			write_graphviz(
				ofs, 
				_g,  
				vertex_writer( _g ),
				edge_writer( _g ),
				graph_writer()
				);
		}

		string Map::get_file_name( const string& file_ext )
		{
			//if( _is_changed )
			//	_update_hash_code();

			//assert( boost::num_vertices( _g )
			//	&& "Map::get_file_name:_g为空,可能是小地图没打开" );
			//assert( _hash_code != HashCode()
			//	&& "Map::get_file_name:_hash_code为空,可能是小地图没打开" );

			//ostringstream oss;

			//oss << hex << setw( 8 )				
			//<< _hash_code._digest[ 0 ] 
			////<< _hash_code._digest[ 1 ]	
			////<< _hash_code._digest[ 2 ]	
			////<< _hash_code._digest[ 3 ]	
			////<< _hash_code._digest[ 4 ]
			//;

			if( "" == file_ext )
				return hash_code_str() + ".map";
			else
				return hash_code_str() + file_ext;

			//return oss.str();
		}

		Dir Map::next_road_dir( const vd_t& vd )
		{
			assert( _shortest_path_vd.size() > 1 
				&& "Map::next_road_dir:shortest_path_vd不正确" );

			assert( vd != _shortest_path_vd.back()
				&& "Map::next_road_dir:vd不能为最后一个vd" );


			Pos p1, p2;
			int next_index = -1;

			p1 = _g[ vd ];

			for( size_t i = 0; i < _shortest_path_vd.size(); i++ )
				if( _shortest_path_vd[ i ] == vd )
					next_index = i + 1;

			assert( -1 != next_index 
				&& "Map::next_road_dir:vd在_shortest_path中不存在" );

			p2 = _g[ _shortest_path_vd[ next_index ] ];
				

			Dir dir = Dir::no;

			p1.init_assert();
			p1.corrent_assert();
			p2.init_assert();
			p2.corrent_assert();

			assert( p1 != p2
				&& "Map::next_road_dir: p1 == p2 错误" );

			if( p1.x == p2.x )
			{
				if( p1.y > p2.y )
					dir = Dir::bottom;
				if( p1.y < p2.y )
					dir = Dir::top;
			}
			else if( p1.y == p2.y )
			{
				if( p1.x > p2.x )
					dir = Dir::left;
				if( p1.x < p2.x )
					dir = Dir::right;
			}
			else
				assert( false
				&& "Map::next_road_dir: p1和p2不为同一水平或垂直" );

			assert( Dir::no != dir
				&& "Map::next_road_dir: dir == Dir::no错误" );
							
			return dir;
		}

_MAP_END

//	}	//	namespace map
//}	//	namespace dnf