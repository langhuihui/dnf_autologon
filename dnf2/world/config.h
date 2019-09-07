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
//	//	special���ֿռ�Ϊ
//	namespace special{

_SPECIAL_BEGIN

	using namespace _DNF;
		///////////////////////////
		//	
		//	room�ļ���Ӧ�������ļ�Ϊrcfg
		//	map�ļ���Ӧ�������ļ�Ϊmcfg
		//

		////////////////////////////////////////////////
		//	���ֹ����õ��������
		//	���������
		//	CaseName������
		//	CaseName��Ψһ��
		//	CaseData��ȡ��Ӧ�Ĳ���
		//	��ʽ�磺
		//	20001@2.3
		//	@ǰ����CaseName�ı��
		//	@�����Ƕ�Ӧ�Ĳ���
		enum _DllExport CaseName{
			//	��Ч����������ڳ�ʼ��
			Invalid					=	00000,
			//	�������
			Test					=	00001,
			//	������������
			AnyCase					=	00010,
			//----------------------------
			//	ÿ���������ֺ���Ҫ��Ϣ
			//	��Ϣʱ���ɶ�Ӧ�Ĳ�����ȡ
			SleepEachRoom			=	20001,
			//	�������,�����Ǵ���������ʼ
			OpenHideEnemy			=	20002,
			//	�����Զ����
			//	��һ������Ϊx���жϷ�Χ
			//	�ڶ�������Ϊy���жϷ�Χ
			//	����������Ϊ�ƶ���ʱ����,
			//	��λΪ����
			OpenAutoHideEnemy		=	20010,
			//	���û��ɸ߶�
			//	��һ������Ϊ�߶�ֵ
			SetHitHeightLimit		=	20020,
			//----------------------------
			//����bossģʽ
			BossCase				=	30000,
				//	���⴦��Boss�����1
				BossCase1				=	30001,
				//	������boss����,��boss����Ὺ��
				OpenHideBossSkill		=	30100,
				//	����boss����ƫ��
				//	��һ������Ϊx����ƫ��
				//	�ڶ�������Ϊy����ƫ��
				SetBossOffset			=	30200,
				//	����BossKiller		
				OpenBossKiller			=	30300,
				//-----------------------------
				//	��������ʹ���
			MapCase					=	40000,
				//-------------------------------------
				//	�ն�����ɽ��������
				HeDunMaEr				=	40100,
					//	������ǵ������������
					TeLeiZeGai				=	40101,
					//	��������ǵķ�����(�ڲ�����)
					TeLeiZeGai_InRoom		=	40102,
				//	Ӣ��ڣ
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
			//	str��ֻ���������check_letter�г��ֵ��ַ�
			bool _check_str( const string& check_letter, const string& str );
			void _set_case_data( const string& case_data_str, CaseInfo& case_info );

		protected:
			vector<CaseInfo> _case_info_vec;
		};

		//	����ȫ�ֵ��������
		//	ʹ���ڲ��ľ�̬Config��
		class _DllExport Special{
			Special();
		public:
			static Config& get_cfg();
			//	����Sync�д洢�ĵ�ǰ״̬
			//	���ж���ǰ�ķ���͵�ͼ�Ƿ�������
			//	�������
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