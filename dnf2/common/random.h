#ifndef _RANDOM_H_
#define _RANDOM_H_


//=====================class random======================

//	���������ǿ��Կ�ƽ̨.
//	��Ϊ��ʹ��time( 0 )����������
//	����������Ǳ�׼C++ͷ�ļ�����ĺ���.
//
//
//	_initseed �ǳ�ʼ������,�ڵ�һ��ʹ��ʱ
//	_initseed������Ϊtrue.����һ������,ֻ
//	������һ������.��������ǲ���Ҫ�ֶ�ȥ
//	��ɵ�.
//
//
//	ʹ������������ַ���:
//	����һ:
//	random rand;
//	rand();
//	�Ƽ����÷�.Ч�����;
//	
//	������:
//	random();
//	ֱ�ӷ��������,����ĵ���;
//
//	������:
//	random()();
//	����Ĳ��Ƽ����÷�;	


#include <ctime>
#include <cstdlib>

#include "common.h"
//#include <base.h>
//#pragma comment( lib, "base.lib" )

_DNF_BEGIN

class _DllExport random{
public:
	random();

	int operator()( int size ){ return ::rand() % size; }
	int operator()(){ return ::rand(); }
	operator int(){ return ::rand(); }
private:
	static bool _initseed;
};

_DNF_END
//========================================================


#endif