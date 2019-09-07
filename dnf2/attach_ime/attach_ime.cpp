#include "attach_ime.h"

_DNF_BEGIN

AttachIme::AttachIme()
	:base_addr( 0x2B110E0 ),
	ime_path("C:\\windows\\system32\\QQPinyin.ime"),
	ime_name("QQ拼音输入法")
#ifdef _DEBUG
	,err_code_cnt( 0 )
#endif
{
#ifdef	_USE_DM
#else
	init();
#endif
}

void AttachIme::install()
{


	HKL hkl = (HKL)0;

	if( !find_ime_hkl() )
	{
		hkl = ImmInstallIME( ime_path.c_str(), ime_name.c_str() );

		assert( hkl && "安装输入法失败" );
		if( !hkl )
			throw exception( "安装输入法失败" );
	}
}


HKL AttachIme::find_ime_hkl()
{
	size_t n = GetKeyboardLayoutList( 0, NULL ); //( ime_name, ime_path );

	vector<HKL> hkl_vec(n);

	GetKeyboardLayoutList( n, hkl_vec.data());


	string buffer( 256, 0 );

	HKL hkl = (HKL)0;

	for( size_t i = 0; i < n; i++ )
	{
		//ImmGetDescription( hkl_vec[ i ], (LPWSTR)buffer.data(), buffer.size() );
		ImmGetIMEFileName( hkl_vec[ i ], (LPSTR)buffer.data(), buffer.size() );

		if( 
			"QQPINYIN.IME" == string( buffer.c_str() ) ||
			"QQPinyin.ime" == string( buffer.c_str() )
			)
			hkl = hkl_vec[ i ];
	}

	return hkl;
}

HKL AttachIme::find_en_hkl()
{
	size_t n = GetKeyboardLayoutList( 0, NULL ); //( ime_name, ime_path );

	vector<HKL> hkl_vec(n);

	GetKeyboardLayoutList( n, hkl_vec.data());


	string buffer( 256, 0 );

	HKL hkl = (HKL)0;
	/*
	for( size_t i = 0; i < n; i++ )
	{
		//ImmGetDescription( hkl_vec[ i ], (LPWSTR)buffer.data(), buffer.size() );
		ImmGetIMEFileName( hkl_vec[ i ], (LPSTR)buffer.data(), buffer.size() );

		if( 
			"QQPINYIN.IME" == string( buffer.c_str() ) ||
			"QQPinyin.ime" == string( buffer.c_str() )
			)
			hkl = hkl_vec[ i ];
	}
*/
	ImmGetIMEFileName( hkl_vec[ 0 ], (LPSTR)buffer.data(), buffer.size() );
	hkl = hkl_vec[ 0 ];

	return hkl;
}

void AttachIme::set_ime()
{
	HKL hkl = find_ime_hkl();

	assert( hkl 
		&& "AttachIme::set_ime:hkl为0" );

	SystemParametersInfo(  
		SPI_SETDEFAULTINPUTLANG,  
		0,  
		&hkl,  
		SPIF_SENDWININICHANGE); 
}
void AttachIme::set_en()
{
	HKL hkl = find_en_hkl();

	assert( hkl 
		&& "AttachIme::set_en:hkl为0" );

	SystemParametersInfo(  
		SPI_SETDEFAULTINPUTLANG,  
		0,  
		&hkl,  
		SPIF_SENDWININICHANGE); 
}

void AttachIme::init()
{
	string ime_path = "C:\\windows\\system32\\QQPinyin.ime";
	//string ime_path = "C:\\Windows\\SysWOW64\\imesample.ime";
	//string ime_name = "google ime";
	string ime_name = "QQ拼音输入法";

	//system( "copy imesample.dll C:\\windows\\system32\\imesample.ime" );
	HMODULE h_module = ::LoadLibrary( ime_path.c_str() );
	assert( h_module && "载入输入法dll失败" );
	if( !h_module )
		throw exception( "载入输入法dll失败" );

	install();

	/*
	HKL hkl = ImmInstallIME( ime_path.c_str(), ime_name.c_str() );

	if( hkl )
		SystemParametersInfo(  
		SPI_SETDEFAULTINPUTLANG,  
		0,  
		&hkl,  
		SPIF_SENDWININICHANGE);  
	else
	{
		size_t n = GetKeyboardLayoutList( 0, NULL ); //( ime_name, ime_path );

		vector<HKL> hkl_vec(n);

		GetKeyboardLayoutList( n, hkl_vec.data());

		string buffer( 256, 0 );

		for( size_t i = 0; i < n; i++ )
		{
			//ImmGetDescription( hkl_vec[ i ], (LPWSTR)buffer.data(), buffer.size() );
			ImmGetIMEFileName( hkl_vec[ i ], (LPSTR)buffer.data(), buffer.size() );

			if( 
				"QQPINYIN.IME" == string( buffer.c_str() ) ||
				"QQPinyin.ime" == string( buffer.c_str() )
				)
				hkl = hkl_vec[ i ];
		}

		assert( hkl && "安装输入法失败" );
		if( !hkl )
			throw exception( "安装输入法失败" );

		SystemParametersInfo(  
			SPI_SETDEFAULTINPUTLANG,  
			0,  
			&hkl,  
			SPIF_SENDWININICHANGE);  
	}
	*/

	///////////////////////////////////////////////////////////////////////////////

	size_t offset = 20;

	CharacterAddrPtr = (long*)GetProcAddress( h_module, (char*)( offset + 1 ) );//"CharacterAddr" );

	assert( CharacterAddrPtr && "CharacterAddrPtr错误" );
	if( !CharacterAddrPtr )
		throw exception( "CharacterAddrPtr错误" );

	*CharacterAddrPtr = base_addr;

	start_set_player_ptr = (bool*)GetProcAddress( h_module, (char*)( offset + 2 ) );//"start_set_player" );
	assert( start_set_player_ptr && "start_set_player_ptr错误" );
	if( !start_set_player_ptr )
		throw exception( "start_set_player_ptr错误" );


	start_set_target_ptr = (bool*)GetProcAddress( h_module, (char*)( offset + 3 ) );//"start_set_target" );
	assert( start_set_target_ptr && "start_set_target_ptr错误" );
	if( !start_set_target_ptr )
		throw exception( "start_set_target_ptr错误" );


	end_set_player_ptr = (bool*)GetProcAddress( h_module, (char*)( offset + 4 ) );//"end_set_player" );
	assert( end_set_player_ptr && "end_set_player_ptr错误" );
	if( !end_set_player_ptr )
		throw exception( "end_set_player_ptr错误" );

	end_set_target_ptr = (bool*)GetProcAddress( h_module, (char*)( offset + 5 ) );//"end_set_target" );
	assert( end_set_target_ptr && "end_set_target_ptr错误" );
	if( !end_set_target_ptr )
		throw exception( "end_set_target_ptr错误" );

	//	0  == 无异常
	//	-1 == 内存读取异常
	//	-2 == target_size > 1000
	err_code_ptr = (int*)GetProcAddress( h_module, (char*)( offset + 6 ) );//"err_code" );;	
	assert( err_code_ptr && "err_code_ptr错误" );
	if( !err_code_ptr )
		throw exception( "err_code_ptr错误" );


	player_data_ptr = (_Data*)GetProcAddress( h_module, (char*)( offset + 7 ) );// "player_data_arr" );
	assert( player_data_ptr && "player_data_ptr错误" );
	if( !player_data_ptr )
		throw exception( "player_data_ptr错误" );


	target_data_ptr = (target_data_ptr_t)GetProcAddress( h_module, (char*)( offset + 8 ) );//"target_data_arr" );
	assert( target_data_ptr && "target_data_ptr错误" );
	if( !target_data_ptr )
		throw exception( "target_data_ptr错误" );


	target_size_ptr = (long*)GetProcAddress( h_module, (char*)( offset + 9 ) );//"target_size" );
	assert( target_size_ptr && "target_size_ptr错误" );
	if( !target_size_ptr )
		throw exception( "target_size_ptr错误" );

}


//void AttachIme::update_player_data( Player& player )
const _Data& AttachIme::update_player_data()
{
	boost::mutex::scoped_lock lock( player_mutex );
	//player_mutex.lock();

	//assert( !*start_set_player_ptr 
	//	&& "start_set_player异常" );
	//assert( !*end_set_player_ptr	//	end_set == false
	//	&& "player:end_set异常" );

	*end_set_player_ptr = false;

#ifdef _DEBUG
	int i = 0;
#endif

	*start_set_player_ptr = true;

	while( true )
	{
		if( *end_set_player_ptr )
		{
			*end_set_player_ptr = false;

			//	assert( *err_code_ptr != -1 
			//		&& "读取内存异常" );

#ifdef _DEBUG
			if( -1 == *err_code_ptr )
			{
				err_code_cnt++;
			}
			if( err_code_cnt > 10 )
			{
				assert( false
					&& "连续10次读取内存异常" );

				err_code_cnt = 0;
			}
#endif

			//player.map_pos.x = player_data_ptr->x;
			//player.map_pos.y = player_data_ptr->y;
			//player.map_pos.z = player_data_ptr->z;

			//player.map_pos.corrent_assert();

			*err_code_ptr = 0;
			break;
		}

		Sleep(1);

#ifdef _DEBUG
		i++;
		if( i > 2000 )
		{
			assert( false &&
				"update_player_data超时" );

			i = 0;
		}
#endif
	}

	return *player_data_ptr;

	//player_mutex.unlock();
}


//void AttachIme::update_target_data( vector<Target>& target_vec )
const _Data* AttachIme::update_target_data( size_t& target_size ) 
{
	boost::mutex::scoped_lock lock( target_mutex );
	//target_mutex.lock();

	assert( !*start_set_target_ptr 
		&& "start_set_target异常" );
	assert( !*end_set_target_ptr 
		&& "end_set_target异常" );

	*end_set_target_ptr = false;

#ifdef _DEBUG
	int i = 0;
#endif

	*start_set_target_ptr = true;

	while( true )
	{
		if( *end_set_target_ptr )
		{
			*end_set_target_ptr = false;

			//assert( *err_code_ptr != -1 
			//	&& "读取内存异常" );

#ifdef _DEBUG
			if( -1 == *err_code_ptr )
			{
				err_code_cnt++;
			}
			if( err_code_cnt > 10 )
			{
				assert( false
					&& "连续10次读取内存异常" );

				err_code_cnt = 0;
			}
#endif

			assert( *err_code_ptr != -2
				&& "target数目超出范围" );

			target_size = *target_size_ptr;

			//while(!target_vec.empty())
			//	target_vec.pop_back();

			//target_vec.resize( *target_size_ptr );

			//for( size_t i = 0; i < *target_size_ptr; i++ )
			//{
			//	//	处理33的特殊情况
			//	if( 33 == (*target_data_ptr)[ i ].id 
			//		&&
			//			(
			//				(*target_data_ptr)[ i ].x < 0 
			//				|| (*target_data_ptr)[ i ].x > 3000 
			//				|| (*target_data_ptr)[ i ].y < 0 
			//				|| (*target_data_ptr)[ i ].y > 3000 
			//				|| (*target_data_ptr)[ i ].z < 0 
			//				|| (*target_data_ptr)[ i ].z > 3000 
			//			)
			//		)
			//	{
			//		target_vec[ i ].id = 33;
			//		target_vec[ i ].map_pos.x = 0;
			//		target_vec[ i ].map_pos.y = 0;
			//		target_vec[ i ].map_pos.z = 0;
			//	}
			//	else
			//	{
			//		target_vec[ i ].id = (*target_data_ptr)[ i ].id;
			//		target_vec[ i ].map_pos.x = (*target_data_ptr)[ i ].x;
			//		target_vec[ i ].map_pos.y = (*target_data_ptr)[ i ].y;
			//		target_vec[ i ].map_pos.z = (*target_data_ptr)[ i ].z;
			//	}
			//}

			*err_code_ptr = 0;
#ifdef _DEBUG
			//for( size_t i = 0; i < target_vec.size(); i++ )
			//{
			//	target_vec[ i ].map_pos.corrent_assert();
			//	assert( target_vec[ i ].id > -1 
			//		&& "update_target_data->id设置异常" );
			//}
#endif
			break;
		}

		Sleep(1);

#ifdef _DEBUG
		i++;

		if( i > 2000 )
		{
			assert( false &&
				"update_player_data超时" );

			i = 0;
		}
#endif
	}

	//for( size_t i = 0; i < target_vec.size(); i++ )
	//{
	//	target_vec[ i ].map_pos.corrent_assert();
	//	assert( target_vec[ i ].id >= 0 
	//		&& "update_target_data->id异常" );
	//}

	return (const _Data*)target_data_ptr;

	//target_mutex.unlock();
}

_DNF_END