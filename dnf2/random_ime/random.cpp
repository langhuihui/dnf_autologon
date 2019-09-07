#include "random.h"


bool random::_initseed = false;

random::random()
{
	if( !_initseed )
	{
		::srand( ( unsigned int )time( 0 ) );
		_initseed = true;
	}
}
