#ifndef __MAP_H__
#define __MAP_H__

//#include "base.h"
#include "graph.h"
//#include "map_base.h"
//#include "archive_map.h"

#include <sstream>

#include <boost/graph/graphviz.hpp>
using namespace boost;

#include <map_base.h>
#pragma comment( lib, "world.lib" )

#ifdef MAP_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif


//	严重:图的节点类型和边类型的修改会改变hash_code

//namespace dnf{
//	namespace map{
_MAP_BEGIN
		using namespace _DNF;
		//	地图结构类
		//	继承自ShortestGraph类
		//	结合人工配置文件.
		class _DllExport Map : 
			public ShortestGraph<
			vecS,
			vecS,
			directedS,
			Pos,
			Road
			>{
		public:

			void save_dot( const string& file_name ) const;

			string get_file_name( const string& file_ext = "" );

			Dir next_road_dir( const vd_t& vd );
		};

_MAP_END

//	}	//	namespace map
//}	//	namespace dnf

#endif