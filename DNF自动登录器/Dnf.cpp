#include "stdafx.h"
#include "Dnf.h"


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

	CharacterAddr = 0x1FC7548; //人物基址，可有参数进行传递后初始化
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
	//log << "-------GetPlayerPoint->start-------" << endl;
	long addrData = DnfReadMemoryInt(CharacterAddr);

	if(addrData > 0)
	{
		x = DnfReadMemoryfloat(addrData + 0x188 + 4 + 4);
		y = DnfReadMemoryfloat(addrData + 0x18C + 4 + 4); 
		z = DnfReadMemoryfloat(addrData + 0x190 + 4 + 4); 

		//log << "-------GetPlayerPoint->end-------" << endl;

		return true;
	}

	//log << "-------GetPlayerPoint->end--------" << endl;

	return false;
}

bool CDnf::GetTargetInfor(int _n, int &_id,int& x, int& y, int& z)
{
	//log << "-------GetTargetInfor->start-------" << endl;


	if(_n < 0) _n =0;

	long PlayerAddrData = DnfReadMemoryInt(CharacterAddr);

	if(PlayerAddrData >0)
	{
		//地图内目标区域怪物清单入口
		long TargetListAddr = DnfReadMemoryInt(PlayerAddrData + 0xB4);
		long TargetAddr = DnfReadMemoryInt(TargetListAddr+0xB0);

		//第N个目标的如果地址
		long nTargetAddr = DnfReadMemoryInt(TargetAddr + _n*4);

		//+0x8C  目标ID
		_id =  DnfReadMemoryInt(nTargetAddr+0x8C);

		//+0xA4  目标坐标位置入口
		long TargetxyzAddr = DnfReadMemoryInt(nTargetAddr +0xA4);


		x = DnfReadMemoryfloat(TargetxyzAddr+0x0C + 4);
		y = DnfReadMemoryfloat(TargetxyzAddr+0x10 + 4);
		z = DnfReadMemoryfloat(TargetxyzAddr+0x14 + 4);


		//log << "-------GetTargetInfor->end-------" << endl;

		return true;
	}


	//log << "-------GetTargetInfor->end-------" << endl;

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
	//log << "----GetTargerNum->start----" << endl;

	long PlayerAddrData = DnfReadMemoryInt(CharacterAddr);

	//地图内目标区域怪物清单入口
	long TargetListAddr = DnfReadMemoryInt(PlayerAddrData + 0xB4);

	if( !TargetListAddr )
	{
		//log << "----GetTargerNum->start----" << endl;
		return 0;
	}

	long num = ( DnfReadMemoryInt(TargetListAddr+0xB4) - DnfReadMemoryInt(TargetListAddr+0xB0))/4;

	//log << "----GetTargerNum->end----" << endl;

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

bool CDnf::GetBigMapPoint( int&x, int& y )
{
	CRect area;
	area.left = 0;
	area.top = 0;
	area.right = 800;
	area.bottom = 600;	

	CPoint point = FindPic( &area, "big_map.bmp", "050505" );

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
	CPoint point = FindPic( &m_WinArea, pic_name.c_str(), "010101" );

	x = point.x;
	y = point.y;

	if( point.x == -1 && point.y == -1 )
		return false;

	return true;
}


/////////////////////////////////////////////
//------------------skill e------------------
bool CDnf::has_e()
{
	bool ret = dm.CmpColor( 593,575, "202020", 1.0 );
	if( !ret )
		return false;
	else
		return true;
}

bool CDnf::is_e_cooldown()
{
	bool ret = dm.CmpColor( 604, 547, "32ffff", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

bool CDnf::is_start_e()
{
	bool ret = true;
	ret = dm.CmpColor( 585, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 584, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 586, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;

	
	return false;
}

bool CDnf::is_cannot_use_e()
{
	bool ret = dm.CmpColor( 593, 575, "808080", 1.0 );
	if( !ret )
		return true;
	else
		return false;
}

bool CDnf::is_e_use_finish()
{
	bool ret = dm.CmpColor( 586, 546, "32ffff", 1.0 );
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
	ret = dm.CmpColor( 541, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 540, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 542, 586, "ff5d27", 1.0 );
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
	ret = dm.CmpColor( 627, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 626, 586, "ff5d27", 1.0 );
	if( !ret )
		return true;
	ret = dm.CmpColor( 628, 586, "ff5d27", 1.0 );
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
	return has_pic( "easy.bmp", 681,472,797,499 );
}

bool CDnf::has_pic_mid()
{
	return has_pic( "mid.bmp" ,681,472,797,499 );
}

bool CDnf::has_pic_hard()
{
	return has_pic( "hard.bmp" ,681,472,797,499 );
}

bool CDnf::has_pic( const string& file_name, 
		long x1, long y1, long x2, long y2 )
{

	CRect area;
	area.left = x1;
	area.top = y1;
	area.right = x2;
	area.bottom = y2;

	CPoint point = FindPic( &area, file_name.c_str(), "010101" );

	if( point.x == -1 && point.y == -1 )
		return false;

	return true;
}


bool CDnf::has_pic_big_map_check()
{
	return has_pic( "big_map_check.bmp", 568,24,729,52 );
}

bool CDnf::has_pic_dungeon_end()
{
	return has_pic( "dungeon_end.bmp", 588,44,704,69 );
}

bool CDnf::has_pic_reward()
{
	return has_pic( "reward.bmp", 274,28,497,91 );
}

bool CDnf::has_pic_score()
{
	return has_pic( "score.bmp", 549,26,691,96 );
}