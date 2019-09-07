#include "watch.h"

_WATCH_BEGIN

OcrHandler::OcrHandler( World& world_ )
	:_world( world_ ),
	_is_start( false ),
	_cmd( _buffer[ 0 ] ),
	_key_value( _buffer[ 1 ] ),
	_resolver( _io_service ),
	_query( _load_ip(), _load_port() ),
	_socket( _io_service )
{
	init();
	_frame_data.fill( 0 );
}
void OcrHandler::init()
{
	_endpoint_iter = _resolver.resolve( _query );
}

void OcrHandler::start( const Pos& ocr_pos )
{
	glog( "OcrHandler::start(" );
	_ocr_pos = ocr_pos;

	_is_start = true;
	_is_end = false;

	_io_service.reset();
	connect();

	//_io_service.run();

	boost::thread( 
		boost::bind( &boost::asio::io_service::run, boost::ref( this->_io_service ) )
		);

}


bool OcrHandler::is_start()
{
	return _is_start;
}

void OcrHandler::end()
{
	glog( "OcrHandler::end()" );

	_is_start = false;
	_is_end = true;

	Sleep( 1000 );
	disconnect();

	_io_service.reset();
}

void OcrHandler::handle_frame_data()
{
	//	二值化图像到
	//	_frame_data

	_frame_data[ 0 ] = 2;

	size_t* addr = (size_t*)_world.dnf.dm.GetScreenData(
			_ocr_pos.x, _ocr_pos.y, 
			_ocr_pos.x + _width, 
			_ocr_pos.y + _height );

	for( size_t i = 0; i < _width * _height / 8; i++ )
	{
		_frame_data[ i + 1 ] = _8_dword_to_byte( &addr[ i * 8 ] );
	}
}

unsigned char OcrHandler::_arbg2gray( size_t argb )
{
	unsigned char gray;

	gray = ( argb & 0xff ) * 0.11 +
		( ( argb >> 8 ) & 0xff ) * 0.59 +
		( ( argb >> 16 ) & 0xff ) * 0.30;

	return gray;
}

unsigned char OcrHandler::_gray_cache2bin()
{
	unsigned char bin;

	for( size_t i = 0; i < _gray_cache.size(); i++ )
		if( _gray_cache[ i ] > 127 )
			_bin_buffer[ 7 - i ] = 1;
		else
			_bin_buffer[ 7 - i ] = 0;
		

	return _bin_buffer.to_ulong() & 0xff;
}

unsigned char OcrHandler::_8_dword_to_byte( size_t* _8_dword_addr )
{
	for( size_t i = 0; i < 8; i++ )
		_gray_cache[ i ] = _arbg2gray( _8_dword_addr[ i ] );

	return _gray_cache2bin();
}



void OcrHandler::connect()
{
	glog( "OcrHandler::connecnt" );
	boost::asio::async_connect(
		_socket, _endpoint_iter,
		boost::bind( &OcrHandler::ready, this,
		boost::asio::placeholders::error )
		);
}

void OcrHandler::ready(const boost::system::error_code& error)
{
	if( !error )
	{
		glog( "OcrHandler::ready" );
		//	接受就绪消息
		//	由ready_finish处理
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
			_buffer, _buffer.size() ),
			boost::bind( &OcrHandler::ready_finish,
						this,
						boost::asio::placeholders::error
						)
			);
	}
	else
	{
		glog ( "连接验证码服务器失败" );
		disconnect();
	}
}



void OcrHandler::ready_finish(const boost::system::error_code& error)
{
	if( !error )
	{
		glog( "OcrHandler::ready_finish" );

		if( _CMD::READY == _cmd )
		{
			//	调用send发送图像
			_is_pause = false;

			handle_frame_data();

			boost::asio::async_write(
				_socket,
				boost::asio::buffer( _frame_data, _frame_data.size() ),
				boost::bind( &OcrHandler::send,
							this,
							boost::asio::placeholders::error )
							);
		}

		//	调用receive接受消息
		boost::asio::async_read(
			_socket,
			boost::asio::buffer(
			_buffer, _buffer.size() ),
			boost::bind( &OcrHandler::receive,
						this,
						boost::asio::placeholders::error )
						);
	}
	else
	{
		glog( "等待验证码服务器就绪失败" );
		disconnect();
	}
}

void OcrHandler::send( const boost::system::error_code& error)
{
	if( !error )
	{
		glog( "OcrHandler::send" );

		if( !_is_pause )
		{
			Sleep( 50 );

			if( !_is_end )

			{
				handle_frame_data();
				//	发送数据
				boost::asio::async_write(
					_socket,
					boost::asio::buffer( _frame_data, _frame_data.size() ),
					boost::bind( &OcrHandler::send,
					this,
					boost::asio::placeholders::error )
					);
			
			}
			else
				disconnect();
		}
	}
	else if( !_is_end )
	{
		glog( "发送图像到验证码服务器失败" );
		disconnect();
	}
	else
		disconnect();

}

void OcrHandler::receive( const boost::system::error_code& error )
{
	if( !error )
	{
		glog( "OcrHandler::receive" );

		//	根据收到的消息进行不同的操作
		if( _CMD::READY	== _cmd )
			_is_pause = false;
		if( _CMD::KEY	== _cmd )
			_world.player.key_press( 
			_to_vk_code( _key_value )
			);
		if( _CMD::PAUSE  == _cmd )
			_is_pause = true;
		if( _CMD::ENTRY  == _cmd )
			_press_entry();

		if( !_is_end )
		{

			boost::asio::async_read(
				_socket,
				boost::asio::buffer(
				_buffer, _buffer.size() ),
				boost::bind( &OcrHandler::receive,
				this,
				boost::asio::placeholders::error )
				);
		}
		else
			disconnect();

		//	操作完成之后继续接受
	}
	else if( !_is_end )
	{
		glog( "接受验证码按键失败" );
		disconnect();
	}
	else
		disconnect();
}

void OcrHandler::disconnect()
{
	glog( "OcrHandler::disconnect" );
	_socket.close();
	_io_service.stop();
	//_clear_io_service();
}

void OcrHandler::_clear_io_service()
{
	//_io_service.stop();
 //   _io_service.~io_service();
 //   new( &_io_service ) boost::asio::io_service;
}

string OcrHandler::_load_ip()
{
	string ip;
	
	ifstream ifile( "data\\ocr_server_ip.txt" );
	if( !ifile )
	{
		cout << "Can not open data\\ocr_server_ip.txt" << endl;
	}
	
	getline( ifile, ip, ':' );

	return ip;
}

string OcrHandler::_load_port()
{
	string port;

	ifstream ifile( "data\\ocr_server_ip.txt" );
	if( !ifile )
	{
		cout << "Can not open data\\ocr_server_ip.txt" << endl;
	}
	
	getline( ifile, port, ':' );
	getline( ifile, port, ':' );

	return port;
}

unsigned char& OcrHandler::_to_vk_code(
	unsigned char& key )
{
	if( key >= 34 
		&& key < 44 )
		key += 14;
	else if( key >= 44 
		&& key < 71 )
		key += 21;
	else if( key >= 74 
		&& key < 84 )
		key -= 26;

	return key;
}

void OcrHandler::_press_entry()
{
	//	按确认键.
	//	补完后,删除该注释
	Pos ensure_pos;
	ensure_pos;

	ensure_pos.x = _ocr_pos.x + 237;
	ensure_pos.y = _ocr_pos.y + 300;

	//cout << "ocr_pos:" 
	//	<< _ocr_pos.x << ","
	//	<< _ocr_pos.y << endl;

	//cout << "ensure_pos:" 
	//	<< ensure_pos.x << ","
	//	<< ensure_pos.y << endl;

	_world.player.LB( ensure_pos );
	_world.player.LB( ensure_pos );
	_world.player.LB( ensure_pos );
}

////////////////////////////////////////
Watch::Watch( World& world_ )
:_world( world_ ),
_ocr_handler( world_ ),
_is_send_safe_mode( false ),
_sleep_timer_limit( 60 )
{}

void Watch::operator()()
{
	while( true )
	{
		//glog( "watch_start" );
		if( Sync::is_start_watch() )
			update();
		//glog( "watch_end" );
	}
}

void Watch::update()
{
	_world.update_player_win_pos();
	//cout 
	//	<< "player.win_pos:"
	//	<< _world.player.win_pos.x << ','
	//	<< _world.player.win_pos.y
	//	<< endl;
	//glog( "watch_update:start" );
	if( _world.has_unusual() )
	{
		Message::unusual();
		trace( "Message::unusual()" );
		Sync::set_stop_all();
		_world.handle_unusual();
		//_world.dm.unbind_wnd();
	}

	if( _world.has_dis_connect() )
	{
		Sync::set_stop_all();
		_world.handle_dis_connect();
		_world.dm.unbind_wnd();
		::exit(0);
	}

	//if( _world.has_safe_mode() )
	if( Sync::is_safe_mode() )
	{
		if( !_is_send_safe_mode )
		{
			Message::safemode();
			Sync::set_safe_mode();
			_is_send_safe_mode = true;
		}
		
		_world.handle_safe_mode();
	}

	if( Sync::is_return_town() )
	{
		_world.attach_ime.set_ime();

		Sync::set_stop_all();
		_world.to_big_map();

		Message::restart_without_login();

		_world.dm.unbind_wnd();
		::exit(0);
	}
	
	if( _world.is_dead() )
	{
		//_world.player.key_press( 0x58 );
		Sync::set_stop_all();
		Sleep( 12000 );
		Message::restart_without_login();
		_world.dm.unbind_wnd();
		::exit(0);
	}

	if( _sleep_timer.elapsed() > _sleep_timer_limit )
	{
		trace( "_sleep_timer" );
		if( !_world.has_process( "DNF.exe" ) )
		{
			Sync::set_stop_all();
			Message::restart_all();
		}

		_sleep_timer.restart();
	}

	check_close();
	check_control();
	check_ocr();

	Sleep( 2000 );
	//glog( "watch::update:end" );
}

void Watch::set_sleep_timer_limit( size_t sleep_time )
{
	_sleep_timer_limit = sleep_time;
}

void Watch::check_close()
{
	if( _world.dnf.dm.GetKeyState( VK_CONTROL ) &&
		_world.dnf.dm.GetKeyState( 0x4B ) ) // ctrl+k
	{
		trace( "close" );
		Sync::set_stop_all();
		::exit(0);
	}
}

void Watch::pause()
{
	//	需修正,全局对stop_all的相应

	Sync::set_stop_all();
	Message::pluse( 0x100000, 2 );
	cout << "pause" << endl;
}

void Watch::continue_run()
{
	Sync::set_nonstop_all();
	cout << "run" << endl;
}

void Watch::check_control()
{
	if( _world.dnf.dm.GetKeyState( VK_CONTROL ) &&
		_world.dnf.dm.GetKeyState( 0x50 ) ) // ctrl+p
	{
		if( Sync::is_stop_all() )
			continue_run();
		else
			pause();
	}
}


bool Watch::is_find_ocr()
{
	if( get_ocr_pos().is_invaild() )
		return false;
	else
		return true;
}

Pos Watch::get_ocr_pos()
{
	Pos ocr_pos;
	ocr_pos.set_invaild();

	//	查找pos的过程
	//	补完后删除该注释

	_world.dnf.GetPicPos(
		ocr_pos.x, ocr_pos.y,
		"ocr_left_top_sign.bmp",
		"030303",100,100,700,500 );

	//cout << "ocr_pos:" 
	//	<< ocr_pos.x << ","
	//	<< ocr_pos.y << endl;

	if( ocr_pos.is_invaild() )
		return ocr_pos;

	Pos ensure_pos;
	ensure_pos.set_invaild();

	_world.dnf.GetPicPos(
		ensure_pos.x, ensure_pos.y,
		"ocr_ensure_sign.bmp",
		"030303",
		ocr_pos.x + 85,
		ocr_pos.y + 204,
		ocr_pos.x + 239,
		ocr_pos.y + 251
		);
	
	//cout << "ensure_pos:" 
	//	<< ensure_pos.x << ","
	//	<< ensure_pos.y << endl;

	if( ensure_pos.is_invaild() )
		ocr_pos.set_invaild();

	return ocr_pos;
}

void Watch::check_ocr()
{
	if( is_find_ocr() &&
		!_ocr_handler.is_start()
		)
	{
		glog( "find_ocr()" );
		pause();
		_ocr_handler.start(
			get_ocr_pos()
			);
	}

	if( !is_find_ocr()
		&& _ocr_handler.is_start() )
	{
		glog( "finish_ocr" );
		_ocr_handler.end();
		continue_run();
	}
}



_WATCH_END