#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <exception>
#include <algorithm>
using namespace std;

#include "random.h"

string gen_name( size_t base_size )
{
	string name_num; 
	string name_str; 
	name_str = "abcdefghijklmnopqrstuvwxyz";
	random_shuffle( name_str.begin(), name_str.end(), random() );
	string name = name_str.substr( 0, random() % ( base_size / 2 ) + base_size );

	name[ 0 ] = toupper( name[ 0 ] );

	return name;
}

void handle_def_name_list( vector<string>& def_name_list )
{
	string random_name;
	string main_random_name = "_A" + gen_name( 3 );
	string sub_random_name;
	vector<string> name_list;
	while( true )
	{
		for( size_t i = 0; i < def_name_list.size() + 3; i++ )
		{
			sub_random_name = gen_name( random() % 3 + 4 );
			random_name = main_random_name + sub_random_name;
			name_list.push_back( random_name );
		}
		
		sort( name_list.begin(), name_list.end() );
		unique( name_list.begin(), name_list.end() );

		if( name_list.size() > def_name_list.size() )
			break;
	}

	for( size_t i = 0; i < name_list.size(); i++ )
		cout << name_list[ i ] << endl;


	for( size_t i = 0, j = 0; i < def_name_list.size(); i++ )
		if( def_name_list[ i ].find( "#define ___000" ) != string::npos )
		{
			size_t pos = def_name_list[ i ].find_last_of( ' ' ) + 1;
			def_name_list[ i ] = def_name_list[ i ].substr( 0, pos ) + name_list[ j ];
			j++;
		}
}

int main()
	try
{


	ifstream ifile( "..\\imesample\\def_name.h" );
	if( !ifile )
		throw( exception( "Can not found file" ) );

	string str;
	vector<string> def_name_list;
	
	while( getline( ifile, str, '\n' ) )
		def_name_list.push_back( str );

	ifile.close();

	handle_def_name_list( def_name_list );

	ofstream ofile( "..\\imesample\\def_name.h" );
	if( !ofile )
		throw( exception( "Can not found file" ) );

	for( size_t i = 0; i < def_name_list.size(); i++ )
		ofile << def_name_list[ i ] << endl;

	ofile.close();

	return 0;
}
catch( exception& e )
{
	cerr << e.what() << endl;
}