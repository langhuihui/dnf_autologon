#include "random.h"

_DNF_BEGIN

bool random::_initseed = false;

random::random()
{
	if( !_initseed )
	{
		::srand( ( unsigned int )time( 0 ) );
		_initseed = true;
	}
}

_DNF_END