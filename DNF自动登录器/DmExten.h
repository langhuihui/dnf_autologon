#pragma once

//#include "log.h"
#include "Cdmsoft.h"
#include <afxwin.h>
#include <string>
#include <cmath>
//#define GetRandom( min, max ) ((rand() % (int)(((max)+1) - (min))) + (min))

#include <cassert>



class CDmExten
{
public:
	// 大漠插件创建对象
	Cdmsoft& dm;

	// 程序所在路径
	CString MyPath;

	CPoint m_PointSafe;
	long GameHwnd;

public:
	CDmExten( Cdmsoft& dm );
	~CDmExten(void);



	// 绑定状态
	bool Bind_ret;
	CRect m_WinArea;

	CRect m_WinLeftArea;
	CRect m_WinRightArea;
	CRect m_WinTopArea;
	CRect m_WinBottomArea;

	CRect m_WinLeftTopArea;
	CRect m_WinLeftBottomArea;
	CRect m_WinRightTopArea;
	CRect m_WinRightBottomArea;


	BOOL ActivateWin(long winHwnd);

	long BindWinEx(long winHwnd);

	// 设置应用程序路径，默认AppPath\data为大漠资源文件夹
	long SetAppPath(CString AppPath);
	// 设置默认的字库文件，文件必须是TXT格式，利用大漠的工具提取的字库
	bool SetWordTxt(CString FileName);


	// 鼠标移动至CPoint指定的点（需要DM先绑定指定窗口，否则移动当前鼠标）
	long MouseMoveTo(CPoint nXY);
	//在指定点便宜指定量单击，并回到安全点
	bool MouseLeftClick(CPoint ClickPoint, int dX = 0, int dY = -1, bool safe = true);
	bool MouseRightClick(CPoint ClickPoint, int dX = 0, int dY = -1, bool safe = false);
	bool MouseLeftClick(CRect *Area, int dX = 0, int dY = -1, bool safe = true);
	bool MouseRightClick(CRect *Area, int dX = 0, int dY = -1, bool safe = false);

	bool MouseWheelUp(void);
	bool MouseWheelDown(void);	
	bool MouseLeftDoubleClick(CPoint *ClickPoint);
	//鼠标左键拖动操作
	bool MouseLeftDrag(CPoint startPoint, CPoint endPoint, bool Shift = false);
	//鼠标左键拖动操作(iX,iY)相对起点的偏移量
	bool MouseLeftDrag(CPoint startPoint, int iX, int iY);

	bool MouseLeftCtrlClick(CPoint _op);
	bool MouseLeftCtrlClick(void);
	//在一定区域内找COLOR
	CPoint FindColor(CRect *Area, CString color_name, double sim = 0.9, int dir = 0);

	// 在Area区域寻找指定的图片，返回找到的图片左上角坐标
	CPoint FindPic(CRect *Area, CString pic_name, CString delta_color, double sim = 0.9, int dir = 0);
	//在Area 查找图片是否存在，存在则返回true;否则返回false 
	//dir:查找方向 0: 从左到右,从上到下 1: 从左到右,从下到上 2: 从右到左,从上到下 3: 从右到左, 从下到上,PIC_NAME 可以是|区分开的多个文件
	bool CmpPic(CRect *Area, CString pic_name, CString delta_color, double sim = 0.9, int dir = 0);
	// Area:选择区域， WordColoe字体颜色，sim相似度
	CString GetPicStr(CRect Area, CString WordColor, double sim = 0.9);
	// 在CRect指定的区域内查找字符串，返回找到的字串坐上角坐标
	CPoint FindStr(CRect Area, CString str, CString color_format, double sim = 0.9);
	// 利用DM.findstrfaste快速寻找字符串，找到返回真，否则返回假
	bool CmpStr(CRect Area, CString str, CString color_format, double sim = 0.9);
	// 比较指定坐标和给定的颜色是否匹配
	bool CmpColor(CPoint ColorPoint, CString ColorStr, double sim = 0.9);

	bool PrintArea(CRect *box, CString picname);
	CString GetColor(CPoint nXY);

	int GetRandomInt(int min, int max) const;

	bool UnBindWin(void);

	long DnfReadMemoryInt(long _addr);

	float DnfReadMemoryfloat(long _addr);
	CString IntToAdr(const long _val);

	CPoint StrToCPoint(CString DmStr);
	bool TestCRectArea(CRect *_r);
	bool TestCPoint(CPoint *_p);

	void KeyDown(CString str);
	void KeyUp(CString str);
	void KeyDown(int key);
	void KeyUp(int key);

	void PressKey(CString str);
	void PressKey(int _key);

	bool GetMousePos( long& x, long& y );


	//Logger log;
};



