#ifndef _RANDOM_H_
#define _RANDOM_H_


//=====================class random======================

//	这个随机数是可以跨平台.
//	因为他使用time( 0 )来设置种子
//	这个函数不是标准C++头文件里面的函数.
//
//
//	_initseed 是初始化种子,在第一次使用时
//	_initseed会设置为true.这样一个程序,只
//	会设置一次种子.这个部分是不需要手动去
//	完成的.
//
//
//	使用这个类有三种方法:
//	方法一:
//	random rand;
//	rand();
//	推荐的用法.效率最高;
//	
//	方法二:
//	random();
//	直接返回随机数,方便的调用;
//
//	方法三:
//	random()();
//	怪异的不推荐的用法;	


#include <ctime>
#include <cstdlib>

//#include <base.h>
//#pragma comment( lib, "base.lib" )


class random{
public:
	random();

	int operator()( int size ){ return ::rand() % size; }
	int operator()(){ return ::rand(); }
	operator int(){ return ::rand(); }
private:
	static bool _initseed;
};

//========================================================


#endif