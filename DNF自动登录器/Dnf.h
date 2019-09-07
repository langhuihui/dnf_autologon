#pragma once
#include "dmexten.h"
#include <string>
#include <cassert>
#include <vector>
#include <sstream>
using namespace std;

const long ID_ENEMY = 529;  //0x211
const long ID_THING = 289;  //0x121

const LPCTSTR COLOR_MIN = (LPCTSTR)"303030";
const LPCTSTR COLOR_MIN05 = (LPCTSTR)"050505";

const LPCTSTR PIC_DOOR_LEVEL = (LPCTSTR)"door2.bmp"; 
const LPCTSTR PIC_DOOR_VERTICAL = (LPCTSTR)"door1.bmp|door11.bmp";  //vertical
const LPCTSTR PIC_CHARACTER = (LPCTSTR)"character.bmp";  //用黄低代表的角色模型


//	带坐标的文字
//	用于dm.OcrEx的返回
struct PosChar{
	//	因为汉字是2个char
	//	所以这里string来存储
	//	单个的字
	string ch;
	CPoint pos;
};

struct PosStr{
	string str;
	CPoint pos;
};


struct sDnfDoorData
{
	CPoint _left;
	CPoint _top;
	CPoint _right;
	CPoint _bottom;
};


class CDnf :
	public CDmExten
{
public:
	CDnf( Cdmsoft& dm_ );
	~CDnf(void);

	//	初始化地址数据
	void InitializeAddrData(void);
	void InitializeDoorData(/*sDnfDoorData *_door*/);

public:

	bool GetCharacterLevel(int &_level);

	bool GetPlayerPoint(int &x, int &y, int &z);

	long GetTargerNum(void);
	bool GetTargetInfor(int _n, int &_id, int &x, int &y, int &z);

	bool UpDoorData(sDnfDoorData *_door);

	//获取门的坐标，n代表门的 方向位置0left1top2right3bottom, xy为返回的门的坐标
	bool GetDoorPoint(int _n, CPoint *_xy);
	bool GetPlayPoint(CPoint *_xy);
	bool CharacterMoveStart(int _n, bool _bq =  false);

	bool CharacterMoveEnd(int _n);

public:

	CString strDoorData(sDnfDoorData *_door);


	/////////////////////////////////////////////////////

	bool GetWinPlayerPoint( int& x, int& y );
	bool GetMapPoint( int&x, int& y );
	bool GetBigMapPoint( int&x, int& y );
	bool GetBossRoomPoint( int&x, int& y );


	vector<PosChar>& GetPosCharList(
		vector<PosChar>& pos_char_list,
		long x1, long y1, long x2, long y2,
		const string& color_format,
		float sim = 1.0 
		);

	vector<PosStr>& GetPosStrList(
		vector<PosStr>& pos_str_list,
		long x1, long y1, long x2, long y2,
		const string& color_format,
		float sim = 1.0 
		);


	string GetMapName();

	//-------------------------------------------
	bool GetLeftDownSign( int&x, int&y );
	bool GetLeftDownReverseSign( int&x, int&y );
	bool GetLeftUpSign( int&x, int&y );
	bool GetLeftUpReverseSign( int&x, int&y );
	bool GetRightDownSign( int&x, int&y );
	bool GetRightDownReverseSign( int&x, int&y );
	bool GetRightUpSign( int&x, int&y );
	bool GetRightUpReverseSign( int&x, int&y );
	bool GetMiddleSign( int&x, int&y );
	bool GetMiddleReverseSign( int&x, int&y );

	bool _get_sign( int& x, int& y, const string& pic_name );

	void check_left_down_sign_corrent( int x, int y );
	void check_left_up_sign_corrent( int x, int y);
	void check_right_down_sign_corrent( int x, int y);
	void check_right_up_sign_corrent( int x, int y );
	void check_middle_sign_corrent( int x, int y );

	//-------------------------------------------


	//------------------skill e------------------

	bool has_e();	//
	bool is_e_cooldown();
	bool is_e_use_finish();
	bool is_start_e();
	bool is_cannot_use_e();


	//-------------------------------------------

	//------------------skill q------------------

	bool is_q_cooldown();
	bool is_start_q();
	bool is_cannot_use_q();	//


	//-------------------------------------------

	//------------------skill r------------------

	bool has_r();
	bool is_start_r();
	bool is_r_use_finish();
	bool is_cannot_use_r();

	//-------------------------------------------

	////////////////////////////////////////////////////

	bool has_pic_easy();
	bool has_pic_mid();
	bool has_pic_hard();
	bool has_pic( const string& file_name, 
		long x1, long y1, long x2, long y2 );

	bool has_pic_big_map_check();
	bool has_pic_dungeon_end();
	bool has_pic_reward();
	bool has_pic_score();

private:

	long CharacterAddr;

	int nMoveVal[4];

	CRect area_door_left;
	CRect area_door_top;
	CRect area_door_right;
	CRect area_door_bottom;

	CRect area_character;

};

