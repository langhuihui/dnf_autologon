#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <string>
#include <exception>
#include <utility>
#include <cassert>
using namespace std;


#include <boost/archive/binary_wiarchive.hpp>
#include <boost/archive/binary_woarchive.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/graph/adj_list_serialize.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

#include <hash_code.h>
#pragma comment( lib, "common.lib" )

#ifdef MAP_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

_DNF_BEGIN

///////////////////////////////////
//	��boost graph��ͨ�÷�װ
//	�ṩһ���ĸ��õĶȡ�
//	ʹboost graph�Ľӿ�
//	����ʹ�ã�����һЩ
//	��ݵĽӿ�
//	��Ӷ��ԣ��Ա����
//
//	ǰ�����������ܼ�����ͬ���Եıߺͽڵ�	
template <
	typename OutEdgeListS = vecS, // a Sequence or an AssociativeContainer
	typename VertexListS = vecS, // a Sequence or a RandomAccessContainer
	typename DirectedS = directedS,
	typename VertexProperty = no_property,
	typename EdgeProperty = no_property,
	typename GraphProperty = no_property,
	typename EdgeListS = listS
>
class Graph{
public:

	typedef adjacency_list<
		OutEdgeListS,
		VertexListS,
		DirectedS,
		VertexProperty,
		EdgeProperty,
		GraphProperty,
		EdgeListS
	> graph_t;

	typedef typename graph_traits<graph_t>::vertex_descriptor vertex_descriptor_t;
	typedef typename graph_traits<graph_t>::vertex_iterator vertex_iterator_t;
	typedef typename graph_traits<graph_t>::edge_descriptor edge_descriptor_t;
	typedef typename graph_traits<graph_t>::edge_iterator edge_iterator_t;
	typedef typename graph_traits<graph_t>::out_edge_iterator out_edge_iterator_t;

	typedef vertex_descriptor_t vd_t;
	typedef vertex_iterator_t vi_t;
	typedef pair<vi_t, vi_t> pvi_t;
	typedef edge_descriptor_t ed_t;
	typedef edge_iterator_t ei_t;
	typedef pair<ed_t, ed_t> pei_t;
	typedef out_edge_iterator_t oei_t;
	typedef pair<oei_t, oei_t> poei_t;

public:

	Graph()
		:_is_changed( false )
	{}
	
	//	����Ѿ����ڣ������������
	vd_t add_vertex( const VertexProperty& vertex_property )
	{
		assert( !has_vertex( vertex_property )
			&& "graph.add_vertex:vertex_property�Ѿ�����");
		vd_t vd = boost::add_vertex( _g );

		_is_changed = true;

		_g[ vd ] = vertex_property;

		return vd;
	}

	bool has_vertex( const VertexProperty& vertex_property )
	{
		if( !boost::num_vertices( _g ) )
			return false;
		for( pvi_t pvi = boost::vertices( _g );
			pvi.first != pvi.second; pvi.first++ )
			if( _g[ *pvi.first ] == vertex_property )
				return true;

		return false;
	}

	bool has_vd( const vd_t& vd )
	{
		if( !boost::num_vertices( _g ) )
			return false;
		for( pvi_t pvi = boost::vertices( _g );
			pvi.first != pvi.second; pvi.first++ )
			if( *pvi.first == vd )
				return true;

		return false;
	}

	vd_t get_vd( const VertexProperty& vertex_property )
	{
		assert( has_vertex( vertex_property )
			&& "graph.get_vd:�����ڵĽڵ�����");

		for( pvi_t pvi = boost::vertices( _g );
			pvi.first != pvi.second; pvi.first++ )
			if( _g[ *pvi.first ] == vertex_property )
				return *pvi.first;

		assert( false
			&& "graph.get_vd:�����ڵĽڵ�����");
	}

	void del_vertex( const vd_t& vd )
	{
		assert( has_vd( vd )
			&& "graph.del_vd:�����ڵ�vd" );

#ifdef _DEBUG
		size_t vertex_num = boost::num_vertices( _g );
		vertex_num--;
#endif 
		boost::clear_vertex( vd, _g );

		boost::remove_vertex( vd, _g );

#ifdef _DEBUG
		assert( boost::num_vertices( _g ) == vertex_num 
			&& "graph.del_vd:�ڵ�ɾ������������ȷ��" );
#endif

		//assert( !has_vd( vd )
		//	&& "graph.del_vd:��ɾ��֮����Ȼ����" );
	}

	void del_vertex( const VertexProperty& vertex_property )
	{
		del_vertex( get_vd( vertex_property ) );

		assert( !has_vertex( vertex_property )
			&& "graph.del_vertex:û��ɾ����ȫ��ͼ�д�����һ��vertex_property" );
	}


	VertexProperty& operator[]( vd_t vd )
	{
		assert( has_vd( vd ) 
			&& "graph.get_vertex_property:�����ڵ�vd" );
		
		_is_changed = true;

		return _g[ vd ];
	}

	//-----------------------
	//	edge	

	ed_t add_edge( 
		const vd_t& source_vd, 
		const vd_t& target_vd,
		const EdgeProperty& edge_property )
	{
		assert( has_vd( source_vd )
			&& "graph.add_edge:source_vd������" );
		assert( has_vd( target_vd )
			&& "graph.add_edge:target_vd������" );

		assert( !has_ed( source_vd, target_vd, edge_property )
			&& "graph.add_edge:Ҫ�����edge_property�Ѿ�����" );

		pair<ed_t,bool> add_edge_ret 
			= boost::add_edge( source_vd, target_vd, _g );

		_g[ add_edge_ret.first ] = edge_property;

		_is_changed = true;

		if( add_edge_ret.second )
			return add_edge_ret.first;
		else
			assert( false 
			&& "graph.add_edge:�޷������" );
	}

	ed_t add_edge( 
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property,
		const EdgeProperty& edge_property )
	{


		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		return add_edge( source_vd, target_vd, edge_property );
	}

	void add_biedge( 
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property,
		const EdgeProperty& edge_property )
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		add_edge( source_vd, target_vd, edge_property );
		if( !has_ed( target_vd, source_vd, edge_property ) )
			add_edge( target_vd, source_vd, edge_property );
			
	}

	bool has_edge(
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property,
		const EdgeProperty& edge_property 
		)
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		return has_ed(
			source_vd,
			target_vd,
			edge_property
			);
	}

	bool has_ed(	
		const vd_t& source_vd, 
		const vd_t& target_vd,
		const EdgeProperty& edge_property )
	{
		assert( has_vd( source_vd ) 
			&& has_vd( target_vd )
			&& "has_ed:source_vd����target_vd������" );

		if( !boost::out_degree( source_vd, _g ) )
			return false;

		poei_t poei = out_edges( source_vd, _g );
		for( ;poei.first != poei.second;
			poei.first++ )
			if( target( *poei.first, _g )
				== target_vd 
				&& _g[ *poei.first ]
				== edge_property )
				return true;

		return false;
	}

	ed_t get_ed( 
		const vd_t& source_vd,
		const vd_t& target_vd,
		const EdgeProperty& edge_property  )
	{	
		assert( has_vd( source_vd )
			&& "graph.get_ed:source_vd������" );
		assert( has_vd( target_vd )
			&& "graph.get_ed:target_vd������" );

		assert( has_ed( source_vd, target_vd, edge_property )
			&& "graph.get_ed:edge_property��Ӧ��ed������" );

		poei_t poei = out_edges( source_vd, _g );
		for( ;poei.first != poei.second;
			poei.first++ )
			if( target( *poei.first, _g )
				== target_vd 
				&& _g[ *poei.first ]
				== edge_property )
				return *poei.first;

		/*
		pair< ed_t, bool> pair_ret = edge( source_vd, target_vd, _g );
		if( pair_ret.second )
			return pair_ret.first;
		else
		{
			assert( false 
			&& "graph.get_ed:�Ҳ�����Ӧ��ed" );
			throw std::exception( "graph.get_ed:�Ҳ�����Ӧ��ed" );
		}
		*/
	}

	ed_t get_ed( 
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property,
		const EdgeProperty& edge_property
		)
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		return get_ed( source_vd, target_vd, edge_property );
	}

	void del_edge( 		
		const vd_t& source_vd, 
		const vd_t& target_vd,
		const EdgeProperty& edge_property
		)
	{
		assert( has_vd( source_vd )
			&& "graph.del_edge:source_vd������" );
		assert( has_vd( target_vd )
			&& "graph.del_edge:target_vd������" );

		assert( has_ed( source_vd, target_vd, edge_property )
			&& "graph.del_edge:edge_property��Ӧ�ı߲�����" );

		boost::remove_edge( get_ed( source_vd, target_vd, edge_property ), _g );
	}

	void del_edge( 	
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property,
		const EdgeProperty& edge_property )
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		del_edge( source_vd, target_vd, edge_property  );
	}

	void del_biedge( 	
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property,
		const EdgeProperty& edge_property )
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		del_edge( source_vd, target_vd, edge_property  );
		if( has_ed( target_vd, source_vd, edge_property ) )
			del_edge( target_vd, source_vd, edge_property );
	}

	//	ɾ��source_vd��target_vd֮�����еı�
	void del_edge( 		
		const vd_t& source_vd, 
		const vd_t& target_vd
		)
	{
		assert( has_vd( source_vd )
			&& "graph.del_edge:source_vd������" );
		assert( has_vd( target_vd )
			&& "graph.del_edge:target_vd������" );

		boost::remove_edge( source_vd , target_vd, _g );
	}

	void del_edge(
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property
		)
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		del_edge( source_vd, target_vd );
	}

	void del_biedge(
		const VertexProperty& source_vertex_property, 
		const VertexProperty& target_vertex_property
		)
	{
		vd_t source_vd = get_vd( source_vertex_property );
		vd_t target_vd = get_vd( target_vertex_property );

		del_edge( source_vd, target_vd );
		del_edge( target_vd, source_vd );
	}

	void del_biedge(
		const vd_t& source_vd,
		const vd_t& target_vd 
		)
	{
		del_edge( source_vd, target_vd );
		del_edge( target_vd, source_vd );
	}


	EdgeProperty& operator[]( ed_t ed )
	{
		_is_changed = true;

		return _g[ ed ];
	}

	void save( const string& file_name )
	{
		assert( !file_name.empty()
			&& "Graph.save:�ļ���Ϊ��" );

		ofstream ofs( file_name );
		if( !ofs )
			throw std::exception(
				"Graph.save:�޷��򿪱����ļ�" );

		boost::archive::text_oarchive boa( ofs );

		boa << _g;
		boa << _hash_code;
		boa << _is_changed;
		
#ifdef _DEBUG 

		ofs.close();

		ifstream ifs( file_name );
		assert( ifs 
			&& "Graph.save:�޷�����Ҫ��֤���ļ�" );

		boost::archive::text_iarchive bia( ifs );

		graph_t g;
		HashCode hash_code;
		bool is_changed;

		bia >> g;
		bia >> hash_code;
		bia >> is_changed;

		assert( boost::num_vertices( g ) == boost::num_vertices( _g ) 
			&& boost::num_edges( g ) == boost::num_edges( _g )
			&& "Graph.save:_g�洢����" );
		assert( hash_code == _hash_code 
			&& "Graph.save:_hash_code�洢����" );
		assert( is_changed == _is_changed
			&& "Graph.save:_is_changed�洢����" );

		ifs.close();
#endif 

	}

	void load( const string& file_name )
	{
		assert( !file_name.empty()
			&& "Graph.load:�ļ���Ϊ��" );

		ifstream ifs( file_name );
		if( !ifs )
			throw std::exception(
				"Graph.save:�޷��������ļ�" );

		boost::archive::text_iarchive bia( ifs );

		bia >> _g;
		bia >> _hash_code;
		bia >> _is_changed;

#ifdef _DEBUG
		assert( boost::num_vertices( _g )
			|| boost::num_edges( _g ) 
			|| _hash_code != HashCode() 
			&& "Graph.load:���ļ������ͼ����Ϊ�գ������Ǵ����" );
#endif
	}

	bool empty()
	{
		if( !boost::num_vertices( _g ) )
			return true;

		return false;
	}

	void clear()
	{
		_g.clear();
		_is_changed = false;
		_hash_code = HashCode();
	}

protected:

	void _update_hash_code() const
	{
		_hash_code.set( _g );
		_is_changed = false;
	}

public:

	bool operator!=( const Graph& graph ) const
	{
		return !operator==( graph );
	}


	bool operator==( const Graph& graph ) const
	{
		if( _is_changed )
			_update_hash_code();

		if( graph._is_changed )
			graph._update_hash_code();

		if( _hash_code == graph._hash_code )
		{
			assert( boost::num_vertices( _g )
				== boost::num_vertices( graph._g )
				&& "Graph.opeartor==:�Ƚϴ��󣬶����������ԣ����������" );

			assert( boost::num_edges( _g )
				== boost::num_edges( graph._g ) 
				&& "Graph.opeartor==:�Ƚϴ��󣬱��������ԣ����Ƿ������" );

			return true;
		}
		else
			return false;
	}

	string hash_code_str()
	{
			if( _is_changed )
				_update_hash_code();

			assert( boost::num_vertices( _g )
				&& "Map::get_file_name:_gΪ��,������С��ͼû��" );
			assert( _hash_code != HashCode()
				&& "Map::get_file_name:_hash_codeΪ��,������С��ͼû��" );

			ostringstream oss;

			oss << hex << setw( 8 )				
			<< _hash_code._digest[ 0 ];

			return oss.str();
	}

protected:

	graph_t _g;

	//	���ڶԱȵ�hash��
	mutable HashCode _hash_code;
	mutable bool _is_changed;
};



///////////////////////////////////////////////////////////////
//	�ܸ�����̱�weight_map�����·����ͼ
template <
	typename OutEdgeListS = vecS, // a Sequence or an AssociativeContainer
	typename VertexListS = vecS, // a Sequence or a RandomAccessContainer
	typename DirectedS = directedS,
	typename VertexProperty = no_property,
	typename EdgeProperty = no_property,
	typename GraphProperty = no_property,
	typename EdgeListS = listS
>
class ShortestGraph : 
	public Graph<
		OutEdgeListS,
		VertexListS,
		DirectedS,
		VertexProperty,
		EdgeProperty,
		GraphProperty,
		EdgeListS
	>{
public:
	
	/////////////////////////////////////////////////
	//	2.�����·���ķ���,���뿪ʼ�ͽ����Ľڵ�.
private:
	void _set_shortest_path( const vd_t& source_vd )
	{
		assert( has_vd( source_vd )
			&& "ShortestGraph._set_shortest_path:�Ҳ���source_vd" );

		assert( boost::num_vertices( _g ) > 1
			&& "ShortestGraph._set_shortest_path:ͼ�ڵ�����2" );

		assert( boost::num_edges( _g ) > 0 
			&& "ShortestGraph._set_shortest_path:ͼ������1" );

		_shortest_path_vd.clear();
		_shortest_path_vd.assign( 
			num_vertices( _g ),
			graph_traits<graph_t>::null_vertex()
			);

		dijkstra_shortest_paths(
			_g,
			source_vd , 
			weight_map(
			get( &EdgeProperty::weight, _g ) )
			.predecessor_map( &_shortest_path_vd[0] )
			);

		assert( !_shortest_path_vd.empty()
			&& "ShortestGraph._set_shortest_path:���·������ʧ��" );
	}

	vd_t _predecessor( const vd_t& v )
	{
		return _shortest_path_vd[ v ];
	}

public:

	bool set_shortest_path(
		const vd_t& start_vd,
		const vd_t& end_vd
		)
	{
		assert( has_vd( start_vd )
			&& "ShortestGraph._set_shortest_path:�Ҳ���start_vd" );

		assert( has_vd( end_vd )
			&& "ShortestGraph._set_shortest_path:�Ҳ���end_vd" );
		
		_set_shortest_path( start_vd );

		if( _shortest_path_vd.empty() )
			return false;

		vector<vertex_descriptor_t> vd_vec;
		vd_vec.push_back( end_vd );

		vertex_descriptor_t v = end_vd;
		while(true)
		{

			if ( _predecessor( v ) == v )
				break;
			else
			{
				v = _predecessor( v );
				vd_vec.push_back( v );
			}
		}

		_shortest_path_vd.clear();

		for( vector<vd_t>::reverse_iterator it = vd_vec.rbegin();
			it != vd_vec.rend(); it++ )
			_shortest_path_vd.push_back( *it );

		
		assert( _shortest_path_vd.size() > 1
			&& "ShortestGraph.set_shortest_path:\
			   ���·������ʧ��,vd_vec,\
			   ����������֮�䲻���ڿ��Ե���ı�." );

		//	set_shortest_path_edges;

		if( _shortest_path_vd.size() < 2 )
			return false;

		_shortest_path_ed.clear();

		pair<edge_descriptor_t, bool> p_ret;

		for( size_t i = 0; i < _shortest_path_vd.size() - 1; i++ )
		{
			p_ret = edge( _shortest_path_vd[ i ], _shortest_path_vd[ i + 1 ], _g );

			assert( p_ret.second 
				&& "ShortestGraph.set_shortest_path:��ȡ��ʧ��" );

			_shortest_path_ed.push_back( p_ret.first );
		}

		assert( !_shortest_path_ed.empty()
			&& "ShortestGraph.set_shortest_path:���·������ʧ��,ed_vec" );

		return true;
	}


	void set_shortest_path(
		const VertexProperty& start_vertex_property,
		const VertexProperty& end_vertex_property 
		)
	{
		set_shortest_path( 
			get_vd( start_vertex_property ),
			get_vd( end_vertex_property )
		);
	}

	const vector<vd_t>& get_shortest_path_vd()
	{
		assert( _shortest_path_vd.size() > 1 
			&& "ShortestGraph.get_shortest_path_vd:\
			��ȡʧ�ܣ��ڵ������������нڵ���С��2" );
		return _shortest_path_vd;
	}

	const vector<ed_t>& get_shortest_path_ed()
	{
		assert( !_shortest_path_ed.empty()
			&& "ShortestGraph.get_shortest_path_ed:\
			��ȡʧ�ܣ�������������Ϊ��" );

		return _shortest_path_ed;
	}

protected:

	vector<vd_t> _shortest_path_vd;
	vector<ed_t> _shortest_path_ed;

};

_DNF_END

#endif