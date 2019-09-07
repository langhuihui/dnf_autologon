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
	// ��Į�����������
	Cdmsoft& dm;

	// ��������·��
	CString MyPath;

	CPoint m_PointSafe;
	long GameHwnd;

public:
	CDmExten( Cdmsoft& dm );
	~CDmExten(void);



	// ��״̬
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

	// ����Ӧ�ó���·����Ĭ��AppPath\dataΪ��Į��Դ�ļ���
	long SetAppPath(CString AppPath);
	// ����Ĭ�ϵ��ֿ��ļ����ļ�������TXT��ʽ�����ô�Į�Ĺ�����ȡ���ֿ�
	bool SetWordTxt(CString FileName);


	// ����ƶ���CPointָ���ĵ㣨��ҪDM�Ȱ�ָ�����ڣ������ƶ���ǰ��꣩
	long MouseMoveTo(CPoint nXY);
	//��ָ�������ָ�������������ص���ȫ��
	bool MouseLeftClick(CPoint ClickPoint, int dX = 0, int dY = -1, bool safe = true);
	bool MouseRightClick(CPoint ClickPoint, int dX = 0, int dY = -1, bool safe = false);
	bool MouseLeftClick(CRect *Area, int dX = 0, int dY = -1, bool safe = true);
	bool MouseRightClick(CRect *Area, int dX = 0, int dY = -1, bool safe = false);

	bool MouseWheelUp(void);
	bool MouseWheelDown(void);	
	bool MouseLeftDoubleClick(CPoint *ClickPoint);
	//�������϶�����
	bool MouseLeftDrag(CPoint startPoint, CPoint endPoint, bool Shift = false);
	//�������϶�����(iX,iY)�������ƫ����
	bool MouseLeftDrag(CPoint startPoint, int iX, int iY);

	bool MouseLeftCtrlClick(CPoint _op);
	bool MouseLeftCtrlClick(void);
	//��һ����������COLOR
	CPoint FindColor(CRect *Area, CString color_name, double sim = 0.9, int dir = 0);

	// ��Area����Ѱ��ָ����ͼƬ�������ҵ���ͼƬ���Ͻ�����
	CPoint FindPic(CRect *Area, CString pic_name, CString delta_color, double sim = 0.9, int dir = 0);
	//��Area ����ͼƬ�Ƿ���ڣ������򷵻�true;���򷵻�false 
	//dir:���ҷ��� 0: ������,���ϵ��� 1: ������,���µ��� 2: ���ҵ���,���ϵ��� 3: ���ҵ���, ���µ���,PIC_NAME ������|���ֿ��Ķ���ļ�
	bool CmpPic(CRect *Area, CString pic_name, CString delta_color, double sim = 0.9, int dir = 0);
	// Area:ѡ������ WordColoe������ɫ��sim���ƶ�
	CString GetPicStr(CRect Area, CString WordColor, double sim = 0.9);
	// ��CRectָ���������ڲ����ַ����������ҵ����ִ����Ͻ�����
	CPoint FindStr(CRect Area, CString str, CString color_format, double sim = 0.9);
	// ����DM.findstrfaste����Ѱ���ַ������ҵ������棬���򷵻ؼ�
	bool CmpStr(CRect Area, CString str, CString color_format, double sim = 0.9);
	// �Ƚ�ָ������͸�������ɫ�Ƿ�ƥ��
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



