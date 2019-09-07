#include "hash_code.h"

_DNF_BEGIN

HashCode::HashCode()
{
	_digest.fill( 0 );
}

bool HashCode::operator!=( const HashCode& hash_code ) const
{
	for( size_t i = 0; i < _digest.size(); i++ )
		if( _digest[ i ] != hash_code._digest[ i ] )
			return true;
	return false;
}

bool HashCode::operator==( const HashCode& hash_code ) const
{
	return !operator!=( hash_code );
}

_DNF_END
