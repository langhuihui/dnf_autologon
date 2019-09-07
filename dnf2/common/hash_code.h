#ifndef __HASH_CODE_H__
#define __HASH_CODE_H__



#include <cstdint>
#include <string>
#include <sstream>
using namespace std;

#include <boost/array.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/array.hpp>


//#include <base.h>
//#pragma comment( lib, "base.lib" )
//
#include "common.h"


_DNF_BEGIN
class HashCode;
_DNF_END

namespace boost {
	namespace serialization {


		template<class Archive>
		void serialize(Archive & ar, _DNF::HashCode& hash_code, const unsigned int version)
		{
			ar & hash_code._digest;
		}
	}
}

_DNF_BEGIN
class _DllExport HashCode{
	friend boost::serialization::access; 
public:
	HashCode();
	template <typename T>
	HashCode( const T& t )
	{
		set( t );
	}

	template <typename T>
	void set( const T& t )
	{
		boost::uuids::detail::sha1 sha1;
		ostringstream oss;
		boost::archive::text_oarchive boa( oss );
		boa << t;
		sha1.process_bytes( oss.str().c_str(), oss.str().size() );

		//cout << oss.str();

		uint32_t digest_arr[ 5 ];

		sha1.get_digest( digest_arr );

		copy( digest_arr, digest_arr + 5, _digest.begin() ); 
	}

	bool operator!=( const HashCode& hash_code ) const;
	bool operator==( const HashCode& hash_code ) const;

public:
	boost::array<uint32_t, 5> _digest;
};

_DNF_END

#endif