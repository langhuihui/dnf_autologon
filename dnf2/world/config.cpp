#include "config.h"

//namespace dnf{
//	namespace special{

_SPECIAL_BEGIN

		/////////////////////////////////////////////////
		CaseData::CaseData(){}

		double& CaseData::operator[]( size_t index )
		{
			assert( index < size() 
				&& index >= 0
				&& "CaseData::operator[]:index超出范围" );

			return _data[ index ];
		}

		size_t CaseData::size()
		{
			return _data.size();
		}

		bool CaseData::empty()
		{
			return _data.empty();
		}

		void CaseData::clear()
		{
			return _data.clear();
		}

		void CaseData::add( double argv )
		{
			_data.push_back( argv );
		}


		/////////////////////////////////////////////////
		Config::Config(){}


		//	这个函数允许文件不存在
		//	所以不用断言来判断参数
		bool Config::load( const string& file_name )
		{
			//cout << "case_size: " << size() << endl;
			//for( size_t i = 0; i < size(); i++ )
			//	cout << get_case_filename( i ) << endl;

			ifstream ifs( file_name );
			if( !ifs )
				return false;

			if( file_name.empty() )
				return false;

			string buffer;

			while( getline( ifs, buffer, '\n' ) )
				_set_case_info( buffer, file_name );

			return true;
		}
		bool Config::has_case( CaseName case_name )
		{
			if( _case_info_vec.empty() )
				return false;

			for( size_t i = 0; i < _case_info_vec.size(); i++ )
				if( _case_info_vec[ i ].name == case_name )
					return true;

			return false;
		}

		bool Config::has_case( const string& file_name, CaseName case_name )
		{
			if( file_name.empty() )
				return false;

			if( _case_info_vec.empty() )
				return false;

			//output( "Config::has_case:file_name" );
			//output( file_name );

			if( AnyCase == case_name )
			{
				for( size_t i = 0; i < _case_info_vec.size(); i++ )
					//if( file_name == _case_info_vec[ i ].file_name  )
					if( _case_info_vec[ i ].file_name.find( file_name ) 
						!= string::npos )
					{
						//output( "Config::has_case.true1" );
						//output( _case_info_vec[ i ].file_name );
						return true;
					}
			}
			else
			{
				for( size_t i = 0; i < _case_info_vec.size(); i++ )
					//if( file_name == _case_info_vec[ i ].file_name  
					if( _case_info_vec[ i ].file_name.find( file_name )
						!= string::npos
						&& case_name == _case_info_vec[ i ].name )
					{
						//output( "Config::has_case.true2" );
						//output( _case_info_vec[ i ].file_name );
						return true;
					}
			}


			return false;
		}

		size_t Config::get_case_index( const string& file_name )
		{

			assert( !file_name.empty()
				&& "Config::get_case_index:file_name为空" );

			assert( !_case_info_vec.empty()
				&& "Config::get_case_index:_case_info_vec为空" );

			for( size_t i = 0; i < _case_info_vec.size(); i++ )
				if( _case_info_vec[ i ].file_name == file_name )
					return i;

		}

		size_t Config::get_case_index( CaseName case_name )
		{
			assert( has_case( case_name )
				&& "Config::get_case_index:不存在的case_name" );

			for( size_t i = 0; i < _case_info_vec.size(); i++ )
				if( _case_info_vec[ i ].name == case_name )
					return i;
		}

		size_t Config::get_case_index( const string& file_name,
			CaseName case_name )
		{
			assert( has_case( file_name, case_name )
				&& "Config::get_case_index:不存在的case_name" );

			for( size_t i = 0; i <_case_info_vec.size(); i++ )
				if( case_name == _case_info_vec[ i ].name
					//&& file_name == _case_info_vec[ i ].file_name )
					&& _case_info_vec[ i ].file_name.find( 
					file_name ) != string::npos 
					)
					return i;
		}

		CaseData& Config::get_case_data( size_t case_info_index )
		{
			assert( case_info_index < size()
				&& case_info_index >= 0 
				&& "Config::get_case_data:case_info_index超出范围" );

			return _case_info_vec[ case_info_index ].data;
		}

		string Config::get_case_filename( size_t case_info_index )
		{
			assert( case_info_index < size()
				&& case_info_index >= 0
				&& "Config::get_case_filename:case_info_index超出范围" );

			assert( !_case_info_vec[ case_info_index ].file_name.empty()
				&& "Config::get_case_filename:file_name为空" );

			return _case_info_vec[ case_info_index ].file_name;
		}

		bool Config::add_case( CaseInfo& case_info )
		{
			assert( !has_case( case_info.name ) 
				&& "Config::add_case:has_this_case" );
			
			if( has_case( case_info.name ) )
				return false;

			_case_info_vec.push_back( case_info );

			return true;
		}

		bool Config::del_case( CaseName case_name )
		{
			assert( has_case( case_name )
				&& "Config::del_case:has_no_this_case" );

			if( !has_case( case_name ) )
				return false;
		
			for( vector<CaseInfo>::iterator it = _case_info_vec.begin();
				it != _case_info_vec.end(); )
				if( case_name == it->name )
					it = _case_info_vec.erase( it );
				else
					it++;

			return true;
		}

		size_t Config::size()
		{  
			return _case_info_vec.size();
		}

		void Config::clear()
		{
			_case_info_vec.clear();
		}

		CaseInfo& Config::operator[]( size_t index )
		{
			assert( index < size() 
				&& index >= 0
				&& "Config::operator[]:index超出范围" );

			return _case_info_vec[ index ];
		}

		void Config::_set_case_info( const string& case_info_str, const string& file_name )
		{
			if( '\"' ==  case_info_str[ 0 ]
			|| case_info_str.empty() )
				return ;

			assert( _check_str( "-0123456789., @\"", case_info_str ) 
				&&	" Config::_set_case_info:载入文件格式错误:含有不非法的字符" );

			CaseInfo case_info;
			case_info.name = CaseName::Invalid;

			size_t pos = case_info_str.find( '@' );

			istringstream iss( case_info_str );
			//如果没找到
			if( string::npos == pos )
			{
				assert( _check_str( "0123456789", case_info_str ) 
					&& "Config::_set_case_info:case_name格式无效" );

				size_t case_name;
				iss >> case_name;
				case_info.name = (CaseName)case_name;
				case_info.file_name = file_name;

				_case_info_vec.push_back( case_info );
			}
			else	
			{
				string case_name_str = case_info_str.substr( 0, pos );

				assert( _check_str( "0123456789", case_name_str ) 
					&& "Config::_set_case_info:case_name格式无效" );

				size_t case_name;
				iss >> case_name;
				case_info.name = (CaseName)case_name;

				case_info.file_name = file_name;

				string info_data_str = case_info_str.substr( pos+1, case_info_str.size() - pos );

				_set_case_data( info_data_str, case_info );

				_case_info_vec.push_back( case_info );
			}
		}


		bool Config::_check_str( const string& check_letter, const string& str )
		{
			assert( !check_letter.empty()
				&& !str.empty() 
				&& "Config::_check_str:参数为空" );

			for( size_t i = 0; i < str.size(); i++ )
				if( check_letter.find( str[ i ] )
					== string::npos )
					return false;

			return true;
		}

		void Config::_set_case_data( const string& case_data_str, CaseInfo& case_info )
		{
			assert( case_info.name != CaseName::Invalid 
				&& "Config::_set_case_data:CaseName无效" );
			assert( !case_data_str.empty()
				&& "Config::_set_case_data:case_data_str为空" );

			assert( _check_str( "-0123456789.,", case_data_str )
				&& "Config::_set_case_data:case_data_str格式错误" );

			istringstream iss( case_data_str );

			istringstream data_stream;

			string str;

			double argv = 0.f;

			while( getline( iss, str, ',' ) )
			{
				assert( _check_str( "-0123456789.", str )
					&& "Config::_set_case_data:case_data_str:参数格式错误" );

				data_stream.clear();
				data_stream.str( str );
				data_stream >> argv;

				case_info.data.add( argv );
			}
		}

		Config& Special::get_cfg()
		{
			return _config;
		}

		bool Special::has_current_case( CaseName case_name )
		{
			//output( "Special::has_current_case" );
			//output( case_name );
			//output( 
			//	Sync::get_current_map_id() + ".mcfg" );
			//output( 
			//	Sync::get_current_room_id() + ".rcfg" );
			return 
				_config.has_case( 
				Sync::get_current_map_id() + ".mcfg",
				case_name
				)
				||
				_config.has_case(
				Sync::get_current_room_id() + ".rcfg",
				case_name
				);
		}

		CaseData& Special::get_current_case_data( CaseName case_name )
		{
			//for( size_t i = 0; i < _config.size(); i++ )
			//{
			//	output( "特殊情况" );
			//	output( _config[ i ].name );
			//	output( _config[ i ].file_name );
			//}

			if( _config.has_case(
				Sync::get_current_room_id() + ".rcfg",
				case_name )
				)
			{
				return _config.get_case_data(
					_config.get_case_index(
						Sync::get_current_room_id() + ".rcfg",
						case_name )
						);
			}
			
			if( _config.has_case(
				Sync::get_current_map_id() + ".mcfg",
				case_name )
				)
			{
				return _config.get_case_data(
					_config.get_case_index(
						Sync::get_current_map_id() + ".mcfg",
						case_name )
						);
			}
			
			assert( false
				&& "Special::get_current_case_data:不存在的case_name" );
		}

		Config Special::_config;

_SPECIAL_END

//
//	}	//	namespace cfg
//}	//	namespace dnf