#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <sstream>
#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <iostream>
using namespace std;

#include <base.h>
#pragma comment( lib, "base.lib" )
#include <sync.h>
#include <log.h>

#ifdef WORLD_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif
//namespace dnf{
//	//	special名字空间为
//	namespace special{

_SPECIAL_BEGIN

	using namespace _DNF;
		///////////////////////////
		//	
		//	room文件对应的配置文件为rcfg
		//	map文件对应的配置文件为mcfg
		//

		////////////////////////////////////////////////
		//	人手工配置的特殊情况
		//	各种情况由
		//	CaseName来区别，
		//	CaseName是唯一的
		//	CaseData获取对应的参数
		//	格式如：
		//	20001@2.3
		//	@前面是CaseName的编号
		//	@后面是对应的参数
		enum _DllExport CaseName{
			//	无效的情况。用于初始化
			Invalid					=	00000,
			//	测试情况
			Test					=	00001,
			//	代表任意的情况
			AnyCase					=	00010,
			//----------------------------
			//	每个房间打完怪后需要休息
			//	休息时间由对应的参数获取
			SleepEachRoom			=	20001,
			//	开启躲怪,参数是代表多少秒后开始
			OpenHideEnemy			=	20002,
			//	设置自动躲怪
			//	第一个参数为x的判断范围
			//	第二个参数为y的判断范围
			//	第三个参数为移动的时间间隔,
			//	单位为毫秒
			OpenAutoHideEnemy		=	20010,
			//	设置击飞高度
			//	第一个参数为高度值
			SetHitHeightLimit		=	20020,
			//----------------------------
			//处理boss模式
			BossCase				=	30000,
				//	特殊处理Boss的情况1
				BossCase1				=	30001,
				//	开启躲boss技能,在boss房间会开启
				OpenHideBossSkill		=	30100,
				//	设置boss坐标偏移
				//	第一个参数为x坐标偏移
				//	第二个参数为y坐标偏移
				SetBossOffset			=	30200,
				//	开启BossKiller		
				OpenBossKiller			=	30300,
				//-----------------------------
				//	其他的特使情况
			MapCase					=	40000,
				//-------------------------------------
				//	赫顿玛尔旧街区的情况
				HeDunMaEr				=	40100,
					//	特雷泽盖的特殊情况处理
					TeLeiZeGai				=	40101,
					//	在特雷泽盖的房间中(内部设置)
					TeLeiZeGai_InRoom		=	40102,
				//	英雄冢
				YingXiongZhong			=	40200,
			//_____________________________


		};

		class _DllExport CaseData{
		public:

			CaseData();

			double& operator[]( size_t index );
			size_t size();
			bool empty();
			void clear();
			void add( double argv );

		private:
			vector<double> _data;
		};

		struct _DllExport CaseInfo{
			CaseName name;
			CaseData data;
			string file_name;
		};

		class _DllExport Config{
		public:
			Config();

			bool load( const string& file_name );
			bool has_case( CaseName case_name );
			
			bool has_case( const string& file_name, CaseName case_name = AnyCase );
			size_t get_case_index( CaseName case_name );
			size_t get_case_index( const string& file_name, CaseName case_name );
			size_t get_case_index( const string& file_name );
			CaseData& get_case_data( size_t case_info_index );
			string get_case_filename( size_t case_info_index );

			bool add_case( CaseInfo& case_info );
			bool del_case( CaseName case_name );

			size_t size();

			void clear();

			CaseInfo& operator[]( size_t index );

		private:
			void _set_case_info( const string& case_info_str, const string& file_name );
			//	str中只允许出现在check_letter中出现的字符
			bool _check_str( const string& check_letter, const string& str );
			void _set_case_data( const string& case_data_str, CaseInfo& case_info );

		protected:
			vector<CaseInfo> _case_info_vec;
		};

		//	处理全局的特殊情况
		//	使用内部的静态Config类
		class _DllExport Special{
			Special();
		public:
			static Config& get_cfg();
			//	根据Sync中存储的当前状态
			//	来判定当前的房间和地图是否设置了
			//	特殊情况
			static bool has_current_case( CaseName case_name );
			static CaseData& get_current_case_data( CaseName case_name );

		private:
			static Config _config;
		};


_SPECIAL_END

//	}	//	namespace cfg
//}	//	namespace dnf
//

#endif