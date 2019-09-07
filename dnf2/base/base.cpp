#include "base.h"

_DNF_BEGIN

__int64 str2int64( const string& str )
{
	istringstream iss( str );
	__int64 i;
	iss >> i;
	return i;
}


int str2int( const string& str )
{
	istringstream iss( str );
	int i;
	iss >> i;
	return i;
}


string int2str( int i )
{
	ostringstream oss;
	oss << i;
	return oss.str();
}
string __int642str( __int64 i )
{
	ostringstream oss;
	oss << i;
	return oss.str();
}

bool check_only_letter( const string& check_letter, const string& str )
{
	assert( !check_letter.empty()
		&& !str.empty() 
		&& "check_only_letter:参数为空" );

	for( size_t i = 0; i < str.size(); i++ )
		if( check_letter.find( str[ i ] )
			== string::npos )
			return false;

	return true;
}

bool check_no_letter( const string& check_letter, const string& str )
{
	assert( !check_letter.empty()
		&& !str.empty() 
		&& "check_no_letter:参数为空" );

	for( size_t i = 0; i < str.size(); i++ )
		if( check_letter.find( str[ i ] )
			!= string::npos )
			return false;

	return true;
}




/*

wstring AnsiToUnicode(const string& buf )
{
	int len = ::MultiByteToWideChar(CP_ACP, 0, buf.c_str(), -1, NULL, 0);
    if (len == 0) return L"";

    std::vector<wchar_t> unicode(len);
    ::MultiByteToWideChar(CP_ACP, 0, buf.c_str(), -1, &unicode[0], len);

    return &unicode[0];
}

string UnicodeToAnsi(const wstring& buf)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, buf.c_str(), -1, NULL, 0, NULL, NULL);
    if (len == 0) return "";

    std::vector<char> utf8(len);
    ::WideCharToMultiByte(CP_ACP, 0, buf.c_str(), -1, &utf8[0], len, NULL, NULL);

    return &utf8[0];
}

wstring Utf8ToUnicode(const string& buf)
{
	int len = ::MultiByteToWideChar(CP_UTF8, 0, buf.c_str(), -1, NULL, 0);
    if (len == 0) return L"";

    std::vector<wchar_t> unicode(len);
	::MultiByteToWideChar(CP_UTF8, 0, buf.c_str(), -1, &unicode[0], len);

    return &unicode[0];
}

string UnicodeToUtf8(const wstring& buf)
{
    int len = ::WideCharToMultiByte(CP_UTF8, 0, buf.c_str(), -1, NULL, 0, NULL, NULL);
    if (len == 0) return "";

    std::vector<char> utf8(len);
    ::WideCharToMultiByte(CP_UTF8, 0, buf.c_str(), -1, &utf8[0], len, NULL, NULL);

    return &utf8[0];
}


string AnsiToUtf8( const string& buf )
{
	return UnicodeToUtf8( AnsiToUnicode( buf ) );
}

string Utf8ToAnsi( const string& buf )
{
	return UnicodeToAnsi( Utf8ToUnicode( buf ) );
}

*/

_DNF_END