#include <log.h>
#include <hash_code.h>
#include <random.h>
#pragma comment( lib, "common.lib" )


using namespace _DNF;

int main()
{
	_DNF::trace( "ok" );
	int rand = random();
	trace( rand );

	string str = "2236";

	HashCode hash_code( str );

	trace( hash_code._digest[ 0 ] );

	system( "pause" );



	return 0;
}