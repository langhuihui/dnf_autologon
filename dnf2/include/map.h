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


//	����:ͼ�Ľڵ����ͺͱ����͵��޸Ļ�ı�hash_code

//namespace dnf{
//	namespace map{
_MAP_BEGIN
		using namespace _DNF;
		//	��ͼ�ṹ��
		//	�̳���ShortestGraph��
		//	����˹������ļ�.
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