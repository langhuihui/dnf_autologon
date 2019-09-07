
#include "Dnf.h"

_DNF_BEGIN

CDnf::CDnf( Cdmsoft& dm_ )
	:CDmExten( dm_ )
{
	//nMoveVal = -1;

	//InitializeAddrData();
	//InitializeDoorData();
}

CDnf::~CDnf(void)
{
}


void CDnf::InitializeAddrData(void)
{
	dm.SetDict(0,"song9.txt");
	
	
	CharacterAddr = 0x2B110E0; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x2B0E000; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x2AA4160; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x2A96FE8; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x2A19D60; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x2A5AD58; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x2A20DE8; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x201AAA0; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x1FC7548; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x1FB2F60; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x1FD4D20; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x1FD3D20; //人物基址，可有参数进行传递后初始化
	//CharacterAddr = 0x1FB2A80; //人物基址，可有参数进行传递后初始化
	//	CharacterAddr = 0x1FA6978; //人物基址，可有参数进行传递后初始化
	//	CharacterAddr = 0x1F72970; //人物基址，可有参数进行传递后初始化
	//	CharacterAddr = 0x1F35DB8; //人物基址，可有参数进行传递后初始化
	//	CharacterAddr = 0x1F31D58; //人物基址，可有参数进行传递后初始化
	//	CharacterAddr = 0x1F00D50; //人物基址，可有参数进行传递后初始化
	//	CharacterAddr = 0x1ED85F8; //人物基址，可有参数进行传递后初始化

}


bool CDnf::GetCharacterLevel(int &_level)
{
	long addrData = DnfReadMemoryInt(CharacterAddr);

	if(addrData > 0)
	{

		return true;
	}
	return false;
}

bool CDnf::GetPlayerPoint(int &x, int &y, int &z)
{
	log << "-------GetPlayerPoint->start-------" << endl;
	long addrData = DnfReadMemoryInt(CharacterAddr);

	if(addrData > 0)
	{
		x = DnfReadMemoryfloat(addrData + 0x188 + 4 + 4);
		y = DnfReadMemoryfloat(addrData + 0x18C + 4 + 4); 
		z = DnfReadMemoryfloat(addrData + 0x190 + 4 + 4); 

		log << "-------GetPlayerPoint->end-------" << endl;

		return true;
	}

	log << "-------GetPlayerPoint->end--------" << endl;

	return false;
}

bool CDnf::GetTargetInfor(int _n, int &_id,int& x, int& y, int& z)
{
	log << "-------GetTargetInfor->start-------" << endl;


	if(_n < 0) _n =0;

	long PlayerAddrData = DnfReadMemoryInt(CharacterAddr); // + 0xBC

	if(PlayerAddrData >0)
	{
		//地图内目标区域怪物清单入口
		long TargetListAddr = DnfReadMemoryInt(PlayerAddrData + 0xB4);
		long TargetAddr = DnfReadMemoryInt(TargetListAddr+0xB0);

		//第N个目标的如果地址
		long nTargetAddr = DnfReadMemoryInt(TargetAddr + _n*4);

		//+0x8C  目标ID
		_id =  DnfReadMemoryInt(nTargetAddr+0x8C); //0x94

		//+0xA4  目标坐标位置入口
		long TargetxyzAddr = DnfReadMemoryInt(nTargetAddr +0xA4); //0xAC


		x = DnfReadMemoryfloat(TargetxyzAddr+0x0C + 4);
		y = DnfReadMemoryfloat(TargetxyzAddr+0x10 + 4);
		z = DnfReadMemoryfloat(TargetxyzAddr+0x14 + 4);


		log << "-------GetTargetInfor->end-------" << endl;

		return true;
	}


	log << "-------GetTargetInfor->end-------" << endl;

	// 坐标位置偏移
	/*
	int x = dm.ReadInt(hwnd, Int16ToAdr(nXYStructAdr +0x0C) ,0);
	int y = dm.ReadInt(hwnd, Int16ToAdr(nXYStructAdr +0x10) ,0);
	int z = dm.ReadInt(hwnd, Int16ToAdr(nXYStructAdr +0x14) ,0);
	*/

	return false;
}

long CDnf::GetTargerNum(void)
{
	log << "----GetTargerNum->start----" << endl;

	long PlayerAddrData = DnfReadMemoryInt(CharacterAddr);

	//地图内目标区域怪物清单入口
	long TargetListAddr = DnfReadMemoryInt(PlayerAddrData + 0xB4);

	if( !TargetListAddr )
	{
		log << "----GetTargerNum->start----" << endl;
		return 0;
	}

	long num = ( DnfReadMemoryInt(TargetListAddr+0xB4) - DnfReadMemoryInt(TargetListAddr+0xB0))/4;

	log << "----GetTargerNum->end----" << endl;

	return num;
}


void CDnf::InitializeDoorData(/*sDnfDoorData *_door*/)
{
	CPoint p(-1,-1);

	int iBottom = m_WinArea.bottom-70;;
	int iTop = m_WinArea.top + 145;

	area_door_left = CRect(m_WinArea.left, iTop, 125, iBottom);
	area_door_top = CRect(m_WinArea.left, iTop, m_WinArea.right, 230);
	area_door_right = CRect(m_WinArea.right-155, iTop, m_WinArea.right, iBottom);
	area_door_bottom =  CRect(m_WinArea.left, iBottom -125, m_WinArea.right, iBottom);



	area_character = m_WinArea;
	area_character.top = m_WinArea.top+250;
	area_character.bottom = m_WinArea.bottom-70;

	/*
	_door->_left = p;
	_door->_top = p;
	_door->_right = p;
	_door->_bottom =p;
	*/
	/*
	PrintArea(&area_door_left,"left");
	PrintArea(&area_door_top,"top");
	PrintArea(&area_door_right,"right");
	PrintArea(&area_door_bottom,"bottom");
	*/
}

bool CDnf::GetDoorPoint(int _n, CPoint *_xy)
{
	switch(_n)
	{
	case 0:
		*_xy = FindPic(&area_door_left, PIC_DOOR_LEVEL, COLOR_MIN, 0.9, 0);
		if(_xy->x >1) return true;
		//break;
	case 1:
		*_xy = FindPic(&area_door_top, PIC_DOOR_VERTICAL, COLOR_MIN, 0.9, 0);
		if(_xy->x >1) return true;
		//break;
	case 2:
		*_xy = FindPic(&area_door_right, PIC_DOOR_LEVEL, COLOR_MIN, 0.9, 0);
		if(_xy->x >1) return true;
		//break;
	case 3:
		*_xy = FindPic(&area_door_bottom, PIC_DOOR_VERTICAL, COLOR_MIN, 0.9, 0);
		if(_xy->x >1) return true;
		//break;
	default:
		return false;
	}

	return false;
}

bool CDnf::GetPlayPoint(CPoint *_xy)
{
	//PIC_CHARACTER ,area_character
	*_xy = FindPic(&area_character, PIC_CHARACTER, COLOR_MIN05, 1.0, 0);

	if(_xy->x >1)
	{
		_xy->x -= 30;
		_xy->y += 80;
		return true;
	}

	return false;

}
bool CDnf::CharacterMoveEnd(int _n)
{
	if(_n < 0 || _n > 3) return false;
	if(nMoveVal[_n] < 1) return false;
	KeyUp(nMoveVal[_n]);
	return true;
}
bool CDnf::CharacterMoveStart(int _n, bool _bq)
{
	switch(_n)
	{
	case 0:
		if(_bq)
		{
			PressKey(65);
			Sleep(300);
		}
		KeyDown(65);
		nMoveVal[0] = 65;

		return true;

	case 1:
		KeyDown(87);
		nMoveVal[1] = 87;
		return true;

	case 2:
		if(_bq)
		{
			PressKey(68);
			Sleep(300);
		}
		KeyDown(68);
		nMoveVal[2] = 68;

		return true;

	case 3:
		KeyDown(83);
		nMoveVal[3] = 83;
		return true;

	default:
		return false;
	}
	return false;
}

bool CDnf::UpDoorData(sDnfDoorData *_door)
{
	//if(_door->_left.x <1 )
	{
		_door->_left = FindPic(&area_door_left, PIC_DOOR_LEVEL, COLOR_MIN, 0.9, 0);
	}

	//if(	_door->_top.x < 1)
	{
		_door->_top  = FindPic(&area_door_top, PIC_DOOR_VERTICAL, COLOR_MIN, 0.9, 0);
	}

	//if(	_door->_right.x <1 )
	{
		_door->_right  = FindPic(&area_door_right, PIC_DOOR_LEVEL, COLOR_MIN, 0.9, 0);

	}

	//if(_door->_bottom.x < 1)
	{
		_door->_bottom = FindPic(&area_door_bottom, PIC_DOOR_VERTICAL, COLOR_MIN, 0.9, 0);
	}


	return false;
}


CString CDnf::strDoorData(sDnfDoorData *_door)
{
	CString strl,strt,strr,strb;

	strl.Format("左门(%d,%d)|", _door->_left.x,  _door->_left.y );
	strt.Format("上门(%d,%d)|", _door->_top.x,   _door->_top.y );
	strr.Format("右门(%d,%d)|", _door->_right.x,  _door->_right.y );
	strb.Format("下门(%d,%d) ", _door->_bottom.x, _door->_bottom.y );

	return strl+strt+strr+strb;
}

/*
bool CDnf::GetWinPlayerPoint( int& x, int& y)
{
//log << "findPic_start" << endl;
CPoint point = FindPic( &m_WinArea, "lv.bmp", "252525" );
//log << "findPic_end" << endl;

//if( -1 == y &&
//	-1 == y )
//	log << "no find" << endl;

x = point.x;
y = point.y;


return true;
}
*/

/////////////////////////////////////////////////////

void CDnf::a_down()
{
	::keybd_event( 0x41,0x1E,0,0);
}
void CDnf::a_up()
{
	::keybd_event( 0x41,0x1E,KEYEVENTF_KEYUP,0);
}
void CDnf::d_down()
{
	::keybd_event( 0x44,0x20,0,0);
}
void CDnf::d_up()
{
	::keybd_event( 0x44,0x20,KEYEVENTF_KEYUP,0);
}
void CDnf::s_down()
{
	::keybd_event( 0x53,0x1F,0,0);
}
void CDnf::s_up()
{
	::keybd_event( 0x53,0x1F,KEYEVENTF_KEYUP,0);
}
void CDnf::w_down()
{
	::keybd_event( 0x57,0x11,0,0);
}
void CDnf::w_up()
{
	::keybd_event( 0x57,0x11,KEYEVENTF_KEYUP,0);
}

void CDnf::key_down( long vk )
{
	::keybd_event( vk, ::MapVirtualKey( vk, 0 ) ,0,0 );
}

void CDnf::key_up( long vk )
{
	::keybd_event( vk, ::MapVirtualKey( vk, 0 ),KEYEVENTF_KEYUP,0 );
}

void CDnf::key_click( long vk )
{
	key_down( vk );
	Sleep( 10 );
	key_up( vk );
}

void CDnf::left_click( long x, long y )
{
	dm.MoveTo( x, y );
	Sleep( 10 );
	dm.LeftClick();
}

void CDnf::check_left_down_sign_corrent( int x, int y )
{
//	assert( 
//		!( GetLeftDownSign( x, y )
//		&& GetLeftDownReverseSign( x, y ) )
//		&& "left_down_sign错误" ); 
}

void CDnf::check_left_up_sign_corrent( int x, int y )
{
//	assert( 
//		!( GetLeftUpSign( x, y )
//		&& GetLeftUpReverseSign( x, y ) )
//		&& "left_down_sign错误" ); 
}
void CDnf::check_right_down_sign_corrent( int x, int y )
{
//	assert( 
//		!( GetRightDownSign( x, y )
//		&& GetRightDownReverseSign( x, y ) )
//		&& "left_down_sign错误" ); 
}
void CDnf::check_right_up_sign_corrent( int x, int y )
{
//	assert( 
//		!( GetRightUpSign( x, y )
//		&& GetRightUpReverseSign( x, y ) )
//		&& "left_down_sign错误" ); 
}
void CDnf::check_middle_sign_corrent( int x, int y )
{
//	assert( 
//		!( GetMiddleSign( x, y )
//		&& GetMiddleReverseSign( x, y ) )
//		&& "left_down_sign错误" ); 
}

bool CDnf::GetWinPlayerPoint( int& x, int& y)
{
	if( GetLeftUpSign( x, y ) )
	{
		x += 144;
		y += 392;

		check_left_up_sign_corrent( x, y );

		return true;
	}
	if( GetLeftUpReverseSign( x, y ) )
	{
		x -= 116;
		y += 392;

		check_left_up_sign_corrent( x, y );

		return true;
	}

	if( GetRightUpSign( x, y ) )
	{
		x -= 116;
		y += 391;

		check_right_up_sign_corrent( x, y );

		return true;
	}

	if( GetRightUpReverseSign( x, y ) )
	{
		x += 145;
		y += 391;

		check_right_up_sign_corrent( x, y );

		return true;
	}

	if( GetMiddleSign( x, y ) )
	{
		x += 13;
		y += 213;
		
		check_middle_sign_corrent( x, y );

		return true;
	}

	if( GetMiddleReverseSign( x, y ) )
	{
		x += 14;
		y += 213;

		check_middle_sign_corrent( x, y );

		return true;
	}

	if( GetLeftDownSign( x, y ) )
	{
		x += 144;
		y += 33;

		check_left_down_sign_corrent( x, y );
		
		return true;
	}
	if( GetLeftDownReverseSign( x, y ) )
	{
		x -= 117;
		y += 33;

		check_left_down_sign_corrent( x, y );

		return true;
	}

	if( GetRightDownSign( x, y ) )
	{
		x -= 116;
		y += 31;

		check_right_down_sign_corrent( x, y );

		return true;
	}

	if( GetRightDownReverseSign( x, y ) )
	{
		x += 144;
		y += 31;

		check_right_down_sign_corrent( x, y );

		return true;
	}




	return false;
}


bool CDnf::GetWinPlayerPoint2( int& x, int& y )
{
	_iss.str(
		(LPCTSTR)dm.FindPicE(
		0,0,799,599,
		"\
		top_left.bmp|\
		top_left_r.bmp|\
		top_right.bmp|\
		top_right_r.bmp|\
		bottom_left.bmp|\
		bottom_left_r.bmp|\
		bottom_right.bmp|\
		bottom_right_r.bmp|\
		mid_left.bmp|\
		mid_left_r.bmp|\
		mid_right.bmp|\
		mid_right_r.bmp|\
		mid_mid.bmp|\
		mid_mid_r.bmp\
		", 
		"404040", 
		1.0, 
		0
		)
		);

	//istringstream iss( str );

	//string buf_str;
	
	//cout << _iss.str() << endl;

	getline( _iss, _buf_str, '|' );
	if( "-1" == _buf_str )
	{
		x = -1;
		y = -1;
		return false;
	}

	//_iss_buf.str( _buf_str );
	//
	//_iss_buf >> _id;

	_id = _buf_str;
	//cout << _buf_str << endl;

	//_iss_buf.str( "" );
	//_iss_buf.clear();

	getline( _iss, _buf_str, '|' );

	_iss_buf.str( _buf_str );
	_iss_buf >> x;

	_iss_buf.str( "" );
	_iss_buf.clear();

	getline( _iss, _buf_str, '|' );

	_iss_buf.str( _buf_str );
	_iss_buf >> y;

	_iss_buf.str( "" );
	_iss_buf.clear();

	_iss.str( "" );
	_iss.clear();

	if( "0" == _id )
	{
		x += 150;
		y += 400;

		return true;
	}
	if( "1" == _id )
	{
		x += 150;
		y += 400;

		return true;
	}
	if( "2" == _id )
	{
		x -= 148;
		y += 400;

		return true;
	}
	if( "3" == _id )
	{
		x -= 148;
		y += 400;

		return true;
	}
	if( "4" == _id )
	{
		x += 150;
		y += 3;

		return true;
	}
	if( "5" == _id )
	{
		x += 150;
		y += 3;

		return true;
	}
	if( "6" == _id )
	{
		x -= 148;
		y += 3;

		return true;
	}
	if( "7" == _id )
	{
		x -= 148;
		y += 3;

		return true;
	}
	if( "8" == _id )
	{
		x += 150;
		y += 201;

		return true;
	}
	if( "9" == _id )
	{
		x += 150;
		y += 201;

		return true;
	}
	if( "10" == _id )
	{
		x -= 148;
		y += 201;

		return true;
	}
	if( "11" == _id )
	{
		x -= 148;
		y += 201;

		return true;
	}
	if( "12" == _id )
	{
		x += 0;
		y += 201;

		return true;
	}
	if( "13" == _id )
	{
		x += 1;
		y += 201;

		return true;
	}


	return false;
}

bool CDnf::GetChairPoint( int& x, int& y)
{
	CRect area;
	area.left = 573;
	area.top = 23;
	area.right = 800;
	area.bottom = 210;

	CPoint point = FindPic( &area, "chair.bmp", "202020", 1.0 );

	x = point.x + 5;
	y = point.y + 4;

	return true;
}

bool CDnf::GetMapPoint( int& x, int& y)
{
	CRect area;
	area.left = 573;
	area.top = 23;
	area.right = 806;
	area.bottom = 210;

	CPoint point = FindPic( &area, "map.bmp", "101010" );

	x = point.x;
	y = point.y;

	return true;
}

bool CDnf::GetWeakPoint( int& x, int& y )
{
	CRect area;

	//	大转移修改
	/*
	area.left = 264;
	area.top = 526;
	area.right = 530;
	area.bottom = 556;	
	*/
	area.left = 317;
	area.top = 478;
	area.right = 528;
	area.bottom = 552;	

	CPoint point = FindPic( &area, "weak.bmp", "050505" );

	x = point.x;
	y = point.y;

	return true;
}

bool CDnf::GetBigMapPoint( int&x, int& y )
{
	CRect area;
	area.left = 0;
	area.top = 0;
	area.right = 800;
	area.bottom = 600;	

	//CPoint point = FindPic( &area, "big_map.bmp", "050505" );
	CPoint point = FindPic( &area, "big_map2.bmp", "050505" );

	x = point.x;
	y = point.y;

	return true;
}

bool CDnf::GetBossRoomPoint( int& x, int& y)
{
	CRect area;
	area.left = 573;
	area.top = 23;
	area.right = 806;
	area.bottom = 210;

	CPoint point = FindPic( &area, "boss_room.bmp", "101010" ); 

	x = point.x;
	y = point.y;

	return true;
}

void CDnf::GetPicPos( int& x, int& y,
	const string& file_name, 
	const string& delta_color, 
	int x1, int y1, int x2, int y2 )
{
	CRect area;
	area.left = x1;
	area.top = y1;
	area.right = x2;
	area.bottom = y2;

	CPoint point = FindPic( &area, file_name.c_str(), delta_color.c_str() ); 

	x = point.x;
	y = point.y;
}

//---------------------------------------------
//	得到更词组,以空格隔开
//

vector<PosChar>& CDnf::GetPosCharList(
		vector<PosChar>& pos_char_list,
		long x1, long y1, long x2, long y2,
		const string& color_format,
		float sim
		)
{
	pos_char_list.clear();
	string ret_str = dm.OcrEx( x1, y1, x2, y2, color_format.c_str(), sim );

	istringstream iss( ret_str );
	string char_str;
	PosChar pos_char;
	while( getline( iss, char_str, '|' ) )
	{
		istringstream char_iss( char_str );
		string str;
		getline( char_iss, str, '$' ); 
		pos_char.ch = str;

		istringstream pos_iss;
		getline( char_iss, str, '$' ); 
		pos_iss.str( str );
		pos_iss >> pos_char.pos.x;
		pos_iss.clear();
		getline( char_iss, str, '$' ); 
		pos_iss.str( str );
		pos_iss >> pos_char.pos.y;
		
		pos_char_list.push_back( pos_char );
	}

	return pos_char_list;
}

vector<PosStr>& CDnf::GetPosStrList(
	vector<PosStr>& pos_str_list,
	long x1, long y1, long x2, long y2,
	const string& color_format,
	float sim 
		)
{
	pos_str_list.clear();
	vector<PosChar> pos_char_list;
	GetPosCharList( 
		pos_char_list,
		x1, y1, x2, y2,
		color_format,
		sim
		);

	if( pos_char_list.size() < 2 )
		return pos_str_list;
	else
	{
		PosStr pos_str_buffer;
		pos_str_buffer.pos = pos_char_list[ 0 ].pos;
		pos_str_buffer.str = pos_char_list[ 0 ].ch;

		for( size_t i = 1; i < pos_char_list.size(); i++ )
		{
			size_t last_char_size = pos_char_list[ i - 1 ].ch.size();

			size_t line_dis = abs( pos_char_list[ i ].pos.y - pos_char_list[ i - 1 ].pos.y );

			bool is_cut = false;

			if( line_dis > 12 )
				is_cut = true;

			size_t char_dis = pos_char_list[ i ].pos.x - pos_char_list[ i - 1 ].pos.x;

			if( 1 == last_char_size 
				&& char_dis >= 9 )
				is_cut = true;
			if( 2 == last_char_size
				&& char_dis >= 14 )
				is_cut = true;


			if( is_cut )
			{
				pos_str_list.push_back( pos_str_buffer );
				pos_str_buffer.str = pos_char_list[ i ].ch;
				pos_str_buffer.pos = pos_char_list[ i ].pos;
			}
			else
			{
				pos_str_buffer.str += pos_char_list[ i ].ch;
			}

			if( ( pos_char_list.size() - 1 ) == i )
				pos_str_list.push_back( pos_str_buffer );
		}
	}

	return pos_str_list;
}

string CDnf::GetMapName()
{
	string str = dm.Ocr( 557,2,768,20,"c8c2b4-404040", 1.0 );

	size_t pos = str.find( "练习" );
	if( pos != string::npos )
		str.replace( pos, 4, "" );

	pos = str.find( "普通级" );
	if( pos != string::npos )
		str.replace( pos, 6, "" );

	pos = str.find( "困难级" );
	if( pos != string::npos )
		str.replace( pos, 6, "" );

	pos = str.find( "王者级" );
	if( pos != string::npos )
		str.replace( pos, 6, "" );

	return str;
}


bool CDnf::GetLeftDownSign( int&x, int&y )
{
	return _get_sign( x, y, "left_down.bmp" );
}

bool CDnf::GetLeftDownReverseSign( int&x, int&y )
{
	return _get_sign( x, y,  "left_down_reverse.bmp" );
}
bool CDnf::GetLeftUpSign( int&x, int&y )
{
	return _get_sign( x, y,  "left_up.bmp" );
}
bool CDnf::GetLeftUpReverseSign( int&x, int&y )
{
	return _get_sign( x, y,  "left_up_reverse.bmp" );
}
bool CDnf::GetRightDownSign( int&x, int&y )
{
	return _get_sign( x, y,  "right_down.bmp" );
}
bool CDnf::GetRightDownReverseSign( int&x, int&y )
{
	return _get_sign( x, y,  "right_down_reverse.bmp" );
}
bool CDnf::GetRightUpSign( int&x, int&y )
{
	return _get_sign( x, y,  "right_up.bmp" );
}
bool CDnf::GetRightUpReverseSign( int&x, int&y )
{
	return _get_sign( x, y,  "right_up_reverse.bmp" );
}
bool CDnf::GetMiddleSign( int&x, int&y )
{
	return _get_sign( x, y,  "middle.bmp" );
}
bool CDnf::GetMiddleReverseSign( int&x, int&y )
{
	return _get_sign( x, y, "middle_reverse.bmp" );
}

bool CDnf::_get_sign( int& x, int& y, const string& pic_name )
{
	//CPoint point = FindPic( &m_WinArea, pic_name.c_str(), "010101" );
	CPoint point = FindPic( &m_WinArea, pic_name.c_str(), "101010" );

	x = point.x;
	y = point.y;

	if( point.x == -1 && point.y == -1 )
		return false;

	return true;
}


/////////////////////////////////////////////
//------------------skill e------------------
//	是否学习了e技能
bool CDnf::has_e()
{
	bool ret = dm.CmpColor( 593,575, "202020-050505", 1.0 );
	if( !ret )
		return false;
	else
		return true;
}
//	e的冷却条是否完全恢复
bool CDnf::is_e_cooldown()
{
	bool ret = dm.CmpColor( 604, 547, "32ffff-050505", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}
//	是否激活e
bool CDnf::is_start_e()
{
	bool ret = true;
	ret = dm.CmpColor( 585, 586, "ff5522-050505", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 584, 586, "ff5522-050505", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 586, 586, "ff5522-050505", 1.0 );
	if( !ret )
		return true;

	
	return false;
}

//	是否e处于不能使用状态,即跳起或者正在被怪攻击
//	而导致e不能使用
bool CDnf::is_cannot_use_e()
{
	bool ret = dm.CmpColor( 593, 575, "808080-050505", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

//	释放e已经小于40%左右,而导致他不进行第二下攻击
bool CDnf::is_e_use_finish()
{
	bool ret = dm.CmpColor( 586, 546, "32ffff-050505", 1.0 );
	if( !ret )
		return false;
	else
		return true;
}


//------------------skill q------------------

bool CDnf::is_q_cooldown()
{
	bool ret = dm.CmpColor( 568, 547, "ff3232", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

bool CDnf::is_start_q()
{
	bool ret = true;
	ret = dm.CmpColor( 541, 586, "ff5522-101010", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 540, 586, "ff5522-101010", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 542, 586, "ff5522-101010", 1.0 );
	if( !ret )
		return true;

	
	return false;
}

bool CDnf::is_cannot_use_q()
{
	bool ret = dm.CmpColor( 554, 575, "888888", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

//-------------------------------------------


//------------------skill r------------------
bool CDnf::has_r()
{
	bool ret = dm.CmpColor( 625,577, "1b1b1b", 1.0 );
	if( !ret )
		return false;
	else
		return true;
}

bool CDnf::is_start_r()
{
	bool ret = true;
	ret = dm.CmpColor( 627, 586, "ff5522-101010", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 626, 586, "ff5522-101010", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 628, 586, "ff5522-101010", 1.0 );
	if( !ret )
		return true;
	
	return false;
}

bool CDnf::is_r_use_finish()
{
	bool ret = dm.CmpColor( 614, 547, "111111", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

bool CDnf::is_cannot_use_r()
{
	bool ret = dm.CmpColor( 625, 577, "707070", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}


bool CDnf::has_pic_easy()
{
	//	大转移修改
	return has_pic( "easy.bmp", 660,472,797,499 );
}

bool CDnf::has_pic_mid()
{
	return has_pic( "mid.bmp" ,660,472,797,499 );
}

bool CDnf::has_pic_hard()
{
	return has_pic( "hard.bmp" ,660,472,797,499 );
}

bool CDnf::has_pic( const string& file_name, 
		long x1, long y1, long x2, long y2, const string& delta_color )
{

	CRect area;
	area.left = x1;
	area.top = y1;
	area.right = x2;
	area.bottom = y2;

	CPoint point = FindPic( &area, file_name.c_str(), delta_color.c_str() );

	if( point.x == -1 && point.y == -1 )
		return false;

	return true;
}


bool CDnf::has_pic_big_map_check()
{
	//return has_pic( "big_map_check.bmp", 568,24,729,52 );
	return has_pic( "big_map_check1.bmp", 568,24,729,52 );
}

bool CDnf::has_pic_dungeon_end()
{
	return has_pic( "dungeon_end.bmp", 588,44,704,69 );
}

bool CDnf::has_pic_reward()
{
	//	大转移修改
	//return has_pic( "reward.bmp", 274,28,497,91 );
	return has_pic( "reward1.bmp", 284,35,488,93 );
}

bool CDnf::has_pic_score()
{
	return has_pic( "score.bmp", 549,26,691,96 );
}

bool CDnf::has_pic_m( long x1, long y1, long x2, long y2 )
{
	return has_pic( "m.bmp", x1, y1, x2, y2 );
}

bool CDnf::has_pic_no_m( long x1, long y1, long x2, long y2 )
{
	return has_pic( "no_m.bmp", x1, y1, x2, y2 );
}

bool CDnf::has_color_m( long x, long y )
{
	//	大转移修改
	//return has_pos_color( x, y, "eef708-050505" );//"ffe663-050505" );
	return has_pos_color( x, y, "eef708-202020" );//"ffe663-050505" );
}

bool CDnf::has_color_no_m( long x, long y)
{
	//	大转移修改
	//return has_pos_color( x, y, "555555-050505" );//"515151-050505" );
	return has_pos_color( x, y, "515151-020202" );//"515151-050505" );
}

bool CDnf::has_color_c( long x, long y )
{
	return has_pos_color( x, y, "52e652-050505" );
}

bool CDnf::has_pos_color( long x, long y, const string& color )
{
	bool ret = dm.CmpColor( x, y, color.c_str(), 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

bool CDnf::has_ocr_str( 
	const string& key_str,
	long x1, long y1, long x2, long y2,
	const string& color,
	float sim
	)
{
	_ocr_buf = dm.Ocr(
		x1, y1, x2, y2,
		color.c_str(), 
		sim );
	if( _ocr_buf.find( key_str ) 
		!= string::npos )
		return true;
	else
		return false;
}

bool CDnf::is_ocr_str( 
	const string& key_str,
	long x1, long y1, long x2, long y2,
	const string& color,
	float sim
	)
{
	_ocr_buf = dm.Ocr(
		x1, y1, x2, y2,
		color.c_str(), 
		sim );
	if( _ocr_buf == key_str )
		return true;
	else
		return false;
}



_DNF_END