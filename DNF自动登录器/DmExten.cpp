#include "stdafx.h"
#include "DmExten.h"


CDmExten::CDmExten( Cdmsoft& dm_ )
	:dm( dm_ )
	, Bind_ret(false)
	, MyPath(_T(""))
	, GameHwnd(0)
	//log( "memory.log" )
{

	::CoInitialize(NULL);
	CLSID clsid;
//	HRESULT hr=CLSIDFromProgID(dm_object_name_wstr,&clsid);
	//HRESULT hr=CLSIDFromProgID(OLESTR("dm.dmsoft"),&clsid);
	HRESULT hr=CLSIDFromProgID(OLESTR("asd.jkl"),&clsid);


	dm.CreateDispatch(clsid);
	
	assert( S_OK == hr 
		&& "初始化dm的com组件失败" );

	CString des="";
	::GetCurrentDirectory(MAX_PATH,des.GetBuffer(MAX_PATH));
	des.ReleaseBuffer();
	MyPath=des;

}

CDmExten::~CDmExten(void)
{

}

bool CDmExten::UnBindWin(void)
{
	if(1 == dm.UnBindWindow())
	{
		Bind_ret = false;
		return true;
	}
	return false;
}



// 鼠标移动至CPoint指定的点（需要DM先绑定指定窗口，否则移动当前鼠标）
long CDmExten::MouseMoveTo(CPoint nXY)
{
	long rValue=dm.MoveTo(nXY.x, nXY.y);
	Sleep(50);
	return rValue;
}


// Area:选择区域， WordColoe字体颜色，sim相似度
CString CDmExten::GetPicStr(CRect Area, CString WordColor, double sim)
{
	CString str=dm.Ocr(Area.left,Area.top,Area.right,Area.bottom ,WordColor,sim);
	return str;
}


bool CDmExten::CmpPic(CRect *Area, CString pic_name, CString delta_color, double sim, int dir)
{

	VARIANT x,y;
	long r = dm.FindPic(Area->left,Area->top,Area->right,Area->bottom ,pic_name,delta_color,sim,dir,&x,&y);
	/*
	CPoint p =StrToCPoint( dm.FindPicE(Area->left,Area->top,Area->right,Area->bottom ,pic_name,delta_color,sim,dir));
	if(p.x > 1) 
	{
		return true;
	}*/

	if(r > -1) return true;
	return false;
}


// 设置应用程序路径，默认AppPath\data为大漠资源文件夹
long CDmExten::SetAppPath(CString AppPath)
{
	long dm_ret = 0;
	if(AppPath.GetLength()>0) dm_ret= dm.SetPath(AppPath+"\\data");
	TRACE("创游戏对象，设定数据文件路径 "+AppPath+"\\data\n");
	return dm_ret;

}

BOOL CDmExten::ActivateWin(long winHwnd)
{
	BOOL _ret = dm.SetWindowState(winHwnd,12);
	Sleep(50);
	return _ret;
}

long CDmExten::BindWinEx(long winHwnd)
{
	GameHwnd = winHwnd;
	//long bind_ret=dm.BindWindowEx(winHwnd,"dx2","dx","dx","dx.public.memory",101);
	long bind_ret = dm.BindWindowEx(winHwnd,"dx2","normal","normal","dx.public.memory",101);
	//long bind_ret=dm.BindWindowEx(winHwnd,"dx2","normal","dx","dx.public.memory",0);
	if(1==bind_ret)  //"normal","normal","normal","dx.public.memory",101
	{
		Bind_ret=true;

		dm.DmGuard(1,"memory");
		dm.MoveWindow(winHwnd,-10,0);
		dm.FreeProcessMemory(winHwnd);
		//TR
	}
	else
	{
		long  err = dm.DmGetLastError();
		CString errStr;
		errStr.Format("绑定游戏窗口失败：错误号[%d]",err);
		Bind_ret=false;

		MessageBox((HWND)winHwnd,errStr,"绑定错误号",MB_OK);
	}

	dm.SetPicPwd("lh7812119");
	dm.SetDictPwd("lh7812119");

	//dm.SetShowErrorMsg(0);
	//获取窗口大小
	CRect rect;
	::GetClientRect((HWND)winHwnd,&rect);
	m_WinArea =rect;// CRect(0, 0,rect.Width()-16, rect.Height()-38);
	
	m_WinLeftArea = CRect(0, 0, m_WinArea.right/2, m_WinArea.bottom);
	m_WinRightArea = CRect(m_WinArea.right/2, 0, m_WinArea.right, m_WinArea.bottom);
	
	m_WinTopArea = m_WinArea;
	m_WinTopArea.bottom = m_WinArea.bottom/2;
	
	m_WinBottomArea = m_WinArea;
	m_WinBottomArea.top = m_WinArea.bottom/2;

	m_WinLeftTopArea = m_WinLeftArea;
	m_WinLeftTopArea.bottom = m_WinLeftArea.bottom/2;

	m_WinLeftBottomArea = m_WinLeftArea;
	m_WinLeftBottomArea.top = m_WinLeftArea.bottom/2;

	m_WinRightTopArea = m_WinRightArea;
	m_WinRightTopArea.bottom = m_WinRightArea.bottom/2;

	m_WinRightBottomArea = m_WinRightArea;
	m_WinRightBottomArea.top = m_WinRightArea.bottom/2;

	return bind_ret;
}

bool CDmExten::PrintArea(CRect *box, CString picname)
{
	CString name = picname + ".bmp";
	long _ret = dm.Capture(box->left, box->top, box->right, box->bottom, name);
	TRACE(name + "已保存成功\n");
	return false;
}


bool CDmExten::MouseLeftDoubleClick(CPoint *ClickPoint)
{
	dm.MoveTo(ClickPoint->x, ClickPoint->y);
	Sleep(50);
	if(1 == dm.LeftDoubleClick())
	{
		return true;
	}
	return false;
}

bool CDmExten::MouseLeftClick(CPoint ClickPoint, int dX, int dY, bool safe)
{
	
	long X,Y;
	VARIANT x,y;

	dm.GetCursorPos(&x,&y);
	X = x.intVal ;
	Y = y.intVal ;

	//dm.LeftUp();
	//TRACE("获取鼠标原来位置:(%d,%d)\n",X,Y );

	Sleep(30);
	long _ret = dm.MoveTo(ClickPoint.x+dX + GetRandomInt(0,5)-2, ClickPoint.y+dY + GetRandomInt(0,1)-1);
	if(0 == _ret) return false;
	Sleep(150);
	_ret = dm.LeftClick();
	if(0 == _ret) return false;
	Sleep(50);


	if(safe)
	{
		_ret = dm.MoveTo(X, Y);

		/*
		int x = X + GetRandomInt(0,50) -25;
		int y = Y + GetRandomInt(0,50) -25;

		if(x < 0 || x > m_WinArea.left -2)
		{
			x = m_PointSafe.x;
		}
		if( y < 0 || y > m_WinArea.bottom -2)
		{
			y=m_PointSafe.y;
		}
		dm.MoveTo(x, y);*/

		Sleep(50);
	}


	//TRACE("鼠标在（%d, %d）单击完成\n",ClickPoint.x+dX,ClickPoint.y+dY);
	return true;


}
bool CDmExten::MouseWheelDown(void)
{
	if(1 == dm.WheelDown()) return true;
	return false;
}
bool CDmExten::MouseWheelUp(void)
{
	if(1 == dm.WheelUp()) return true;
	return false;
}
bool CDmExten::MouseRightClick(CPoint ClickPoint, int dX, int dY, bool safe)
{
	long X,Y;
	VARIANT x,y;

	dm.GetCursorPos(&x,&y);
	X = x.intVal ;
	Y = y.intVal ;

	if(-1 == dY) dY=dX;
	//if(dX < 0 || dY < 0) return false;

	ClickPoint.x+=dX;
	ClickPoint.y+=dY;

	long _ret = dm.MoveTo(ClickPoint.x+ GetRandomInt(0,5)-2, ClickPoint.y+ GetRandomInt(0,1)-1);
	if(0 == _ret) return false;
	Sleep(60);
	_ret = dm.RightClick();
	if(0 == _ret) return false;
	Sleep(50);

	if(safe)
	{
		_ret = dm.MoveTo(X, Y);
		if(0 == _ret) return false;
		//
		Sleep(50);
	}
	//else
	//{
	//	dm.MoveTo(m_PointSafe.x, m_PointSafe.y);
	//	Sleep(50);
	//}
	return true;


}

bool CDmExten::MouseLeftDrag(CPoint startPoint, int iX, int iY)
{
	if(1 == dm.MoveTo(startPoint.x + GetRandomInt(0,5)-2,startPoint.y + GetRandomInt(0,5)-2))
	{
		dm.LeftDown();
		Sleep(50);
		dm.MoveTo(startPoint.x +iX, startPoint.y +iY);
		Sleep(100);
		dm.LeftUp();

		return true;
	}

	return false;
}
bool CDmExten::MouseLeftDrag(CPoint startPoint, CPoint endPoint, bool Shift)
{
	if(1 == dm.MoveTo(startPoint.x + GetRandomInt(0,5)-2 ,startPoint.y + GetRandomInt(0,1)-1))
	{
		//dm.LeftClick();
		Sleep(20);
		if(Shift) dm.KeyDown(16);
		Sleep(20);
		dm.LeftDown();
		Sleep(100);
		dm.MoveTo(endPoint.x, endPoint.y);
		Sleep(100);
		dm.LeftUp();
		Sleep(20);
		if(Shift) dm.KeyUp(16);
		Sleep(20);
		return true;
	}

	return false;
}

bool CDmExten::MouseLeftCtrlClick(void)
{
	dm.KeyDown(17);
	Sleep(50);
	dm.LeftClick();
	Sleep(30);
	dm.KeyUp(17);
	Sleep(20);
	return true;

}
bool CDmExten::MouseLeftCtrlClick(CPoint _op)
{
	if(_op.x > 1)
	{
		dm.MoveTo(_op.x+ GetRandomInt(0,5)-2, _op.y+ GetRandomInt(0,1)-1);
		Sleep(50);
		dm.KeyDown(17);
		Sleep(20);
		dm.LeftClick();
		Sleep(20);
		dm.KeyUp(17);
		Sleep(20);
		return true;


	}
	return false;
}


CString CDmExten::GetColor(CPoint nXY)
{
	return dm.GetColor(nXY.x, nXY.y);
}
int CDmExten::GetRandomInt(int min, int max) const
{
	return (rand() % (max - min)) + min ;
}


bool CDmExten::MouseLeftClick(CRect *Area, int dX, int dY, bool safe)
{
	long X,Y;
	VARIANT x,y;

	dm.GetCursorPos(&x,&y);
	X = x.intVal ;
	Y = y.intVal ;

	if(-1 == dY) 
	{
		dY=dX;
	}
	dm.LeftUp();
	long _ret = dm.MoveTo(Area->left +dX, Area->top +dY);
	if(0 == _ret) return false;
	Sleep(100);
	_ret = dm.LeftClick();
	if(0 == _ret) return false;
	Sleep(100);

	if(safe)
	{
		_ret = dm.MoveTo(X, Y);
		
		if(0 == _ret) return false;


	//	dm.MoveTo(m_PointSafe.x, m_PointSafe.y);
		Sleep(50);
	}

	//TRACE("鼠标在（%d, %d）单击完成\n", ClickPoint.x, ClickPoint.y);
	return true;
}
bool CDmExten::MouseRightClick(CRect *Area, int dX, int dY, bool safe)
{
	long X,Y;
	VARIANT x,y;

	dm.GetCursorPos(&x,&y);
	X = x.intVal ;
	Y = y.intVal ;

	if(-1 == dY) 
	{
		dY=dX;
	}

	long _ret = dm.MoveTo(Area->left +dX, Area->top +dY);
	if(0 == _ret) return false;
	Sleep(100);
	_ret = dm.RightClick();
	if(0 == _ret) return false;
	Sleep(100);

	if(safe)
	{
		_ret = dm.MoveTo(X, Y);
		if(0 == _ret) return false;
		///dm.MoveTo(m_PointSafe.x, m_PointSafe.y);
		Sleep(100);
	}


	//TRACE("鼠标在（%d, %d）单击完成\n", ClickPoint.x, ClickPoint.y);
	return true;
}


CString CDmExten::IntToAdr(const long _val)
{
	CString str;
	str.Format("%x",_val);
	return str;
}

long CDmExten::DnfReadMemoryInt(long _addr)
{
	//log << "dm.ReadInt->start" << endl;
	return dm.ReadInt(GameHwnd, IntToAdr(_addr) ,0);  //读取已解密的数据
}

float CDmExten::DnfReadMemoryfloat(long _addr)
{
	//log << "dm.ReadFloat->start" << endl;
	return dm.ReadFloat(GameHwnd, IntToAdr(_addr));  //读取已解密的数据
}

bool CDmExten::TestCRectArea(CRect *_r)
{
	if(_r->left >= _r->right || _r->top >= _r->bottom ) return false;
	if(_r->left <0 || _r->top <0 || _r->right<0 || _r->bottom<0 ) return false;
//	if(_r->right > m_WinArea.right || _r->bottom > m_WinArea.bottom) return false;

	return true;
}

CPoint CDmExten::FindPic(CRect *Area, CString pic_name, CString delta_color, double sim, int dir)
{
	if(TestCRectArea(Area))
	{
		CString strXY=dm.FindPicE(Area->left,Area->top,Area->right,Area->bottom ,pic_name,delta_color,sim,dir);
		CPoint nXY=StrToCPoint(strXY);

		return nXY;
	}
	return CPoint(-1,-1);
}

// 将大漠部分函数返回的"0|100|100"格式的字符穿转化为CPoint格式
CPoint CDmExten::StrToCPoint(CString DmStr)
{
	CString Pos[3];
	CPoint rValue;
	int n,m;
	n=DmStr.Find('|');
	m=DmStr.ReverseFind('|');

	Pos[0] = DmStr.Left(n);
	Pos[2] = DmStr.Right(DmStr.GetLength()-m-1);
	Pos[1] = DmStr.Mid(n+1,m-n-1);

	rValue.x = atoi(Pos[1]);
	rValue.y = atoi(Pos[2]);

	return rValue;
}

void CDmExten::KeyDown(CString str)
{

	dm.KeyDownChar(str);
}
void CDmExten::KeyUp(CString str)
{
	dm.KeyUpChar(str);
}

void CDmExten::KeyDown(int key)
{

	dm.KeyDown(key);

}
void CDmExten::KeyUp(int key)
{
	dm.KeyUp(key);
}

void CDmExten::PressKey(CString str)
{
	dm.KeyPressChar(str);
}
void CDmExten::PressKey(int _key)
{
	dm.KeyPress(_key);
}

bool CDmExten::GetMousePos( long& x, long& y )
{
	VARIANT _x, _y;

	if( dm.GetCursorPos( &_x, &_y ) )
	{
		x = _x.iVal;
		y = _y.iVal;
		return true;
	}
	else
		return false;
}

