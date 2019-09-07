#ifndef __WATCH_H__
#define __WATCH_H__


#include <sync.h>
#include <world.h>
#include <message.h>
#pragma comment( lib, "world.lib" )


//	OcrHandler��Ҫ�õ�ͷ�ļ�
//	���������ֵ����
#include <bitset>
#include <boost/asio.hpp>

#ifdef GAME_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

//	�۲��������Ϸ״̬
//	�Ա������̸߳�����Ϸ״̬
//	�����и���
//	��ص�����
//	�ͱ���������,��С��ͼ���ر�,���Զ���

using boost::asio::ip::tcp;

_WATCH_BEGIN

	using namespace _DNF;


	//	������������ʱ��������
	//	��Ϊ����������߳�
	//	�Ժ�滮�����޸�Ҳ����
	//	

	class _DllExport OcrHandler{


		static const size_t _width = 400;
		static const size_t _height = 300;
		
	public:
		OcrHandler( World& world_ );

		void init();


		void start( const Pos& ocr_pos );
		bool is_start();

		void end();

	private:

		bool _is_start;
		bool _is_end;
		Pos _ocr_pos;

		//______________ͼ����_______________
	public:
		void handle_frame_data();

	private:

		unsigned char _arbg2gray( size_t argb );

		unsigned char _gray_cache2bin();
		
		unsigned char _8_dword_to_byte( size_t* _8_dword_addr );

	private:
		std::array< unsigned char, _width * _height / 8  + 1> _frame_data;
		
		bitset<8> _bin_buffer;

		std::array<unsigned char, 8> _gray_cache;

		//______________��������_______________
	public:

		void init_asio();

		void connect();

		void ready( const boost::system::error_code& error );

		void ready_finish( const boost::system::error_code& error ); 

		void send( const boost::system::error_code& error );

		void receive( const boost::system::error_code& error );

		void disconnect();

	private:
		
		string _load_ip();
		string _load_port();
		unsigned char& _to_vk_code( unsigned char& key );
		void _press_entry();

		void _clear_io_service();

	private:
		boost::asio::io_service _io_service;
		tcp::resolver _resolver;
		tcp::resolver::query _query;
		tcp::resolver::iterator _endpoint_iter;

		tcp::socket _socket;

		bool _is_pause;

		static const size_t _buffer_len = 2;

		std::array<unsigned char, _buffer_len> _buffer;

		unsigned char& _cmd;
		unsigned char& _key_value;

		enum _CMD{
			READY		 = 1,
			KEY			 = 2,
			PAUSE		 = 3,
			ENTRY		 = 4
		};

	private:


		World& _world;
	};

	class _DllExport Watch{
	public:
		Watch( World& world_ );

		void operator()();
		void update();

		void set_sleep_timer_limit( size_t sleep_time );

		void check_close();

		void pause();
		void continue_run();

		void check_control();


		bool is_find_ocr();
		Pos get_ocr_pos();
		void check_ocr();

	private:
		World& _world;

		OcrHandler _ocr_handler;

		boost::timer _sleep_timer;
		size_t _sleep_timer_limit;

		bool _is_send_safe_mode;
	};

_WATCH_END


#endif