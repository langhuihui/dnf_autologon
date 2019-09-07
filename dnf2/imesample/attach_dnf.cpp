#include "attach_dnf.h"




#pragma data_seg( "SharedDataInDll" )

//-----dnf can write------
long CharacterAddr = 0;
//------------------------

bool start_set_player = false;
bool start_set_target = false;
bool end_set_player = false;
bool end_set_target = false;

//-----ime can write------

//	0  == 无异常
//	-1 == 内存读取异常
//	-2 == target_size > 1000
int err_code = 0;		
//Data player_data;
//boost::array<Data, 1000> target_data;

int player_data_arr[ 4 ] = { 0 };
int target_data_arr[ 4000 ] = { 0 };
long target_size = 0;
//------------------------

#pragma data_seg()
#pragma comment(linker, "/SECTION:SharedDataInDll,RWS")


Data& player_data = *(Data*)&player_data_arr[ 0 ];

typedef Data (*target_data_ptr_t)[ 1000 ];
typedef Data (&target_data_ref_t)[ 1000 ];
target_data_ref_t target_data 
	= *(target_data_ptr_t)&target_data_arr[ 0 ];

void dnf_loop()
{
	show( "dnf_loop" );
	while( true )
	{


		if( start_set_player && CharacterAddr )
		{
			start_set_player = false;


			show( "set_player_data();" );

			ostringstream oss;
			set_player_data( oss );
			show( oss.str().c_str() );

			oss.clear();
			oss.str( "" );

#ifdef _DEBUG
			//ostringstream oss;
			oss << "P(" 
				<< player_data.x 
				<< ","
				<< player_data.y
				<< ","
				<< player_data.z
				<< ")"
				<< "|E("
				<< err_code
				<< ")";

			oss << "data_addr" 
				<< &player_data;

			show( oss.str().c_str() );
#endif

			end_set_player = true;
			start_set_player = false;
		}


		if( start_set_target && CharacterAddr )
		{
			start_set_target = false;

			show( "set_target_data" );


			ostringstream oss;
			set_target_data( oss );
			show( oss.str().c_str() );

			oss.clear();
			oss.str( "" );
#ifdef _DEBUG
			//ostringstream oss;

			oss << "E("
				<< err_code
				<< ")T("
				<< target_size
				<< ")";

			for( size_t i = 0; i < target_size; i++ )
			{
				oss	<< "["
					<< i
					<< "]("
					<< target_data[ i ].x 
					<< ","
					<< target_data[ i ].y 
					<< ","
					<< target_data[ i ].z 
					<< ")";
			}

			show( oss.str().c_str() );
#endif

			end_set_target = true;
			start_set_target = false;
		}

		Sleep(1);
	}
}

void set_player_data( ostringstream& oss )
{
	__try
	{
		//int* CharacterAddr = (int*)0x1FA6978;

		char* addrData = (char*)*(long*)CharacterAddr;

#ifdef _DEBUG
		oss << "addrData"
			<< (long*)addrData
			<< "|";

#endif

		if(addrData > 0)
		{
			player_data.x = *(float*)(addrData + 0x1A4 );//0x1A8 );//0x188 + 4 + 4 );
			player_data.y = *(float*)(addrData + 0x1A8 );//0x1AC );//0x18C + 4 + 4 ); 
			player_data.z = *(float*)(addrData + 0x1AC );//0x1B0 );//0x190 + 4 + 4 );
		}
	}
	__except(1)
	{
		err_code = -1;
	}

}

void set_target_data( ostringstream& oss )
{
	__try
	{
		typedef long* LongPtr;
		typedef char* CharPtr;
		typedef float* FloatPtr;

		//LongPtr CharacterAddr = (LongPtr)0x1FA6978;


		CharPtr PlayerAddrData = (CharPtr)*(LongPtr)CharacterAddr;// + 0xBC;
#ifdef _DEBUG
		oss << "PlayerAddrData" 
			<< (LongPtr)PlayerAddrData 
			<< "|";
#endif

		CharPtr TargetListAddr = (CharPtr)*(LongPtr)(PlayerAddrData + 0xB8 );//0xBC);
#ifdef _DEBUG
		oss << "TargetListAddr" 
			<< (LongPtr)TargetListAddr 
			<< "|";
#endif

		CharPtr TargetAddr = (CharPtr)*(LongPtr)(TargetListAddr+0xc4);//0xB0);
		//第N个目标的如果地址
#ifdef _DEBUG
		oss << "TargetAddr" 
			<< (LongPtr)TargetAddr 
			<< "|";
#endif

		target_size = (*(LongPtr)(TargetListAddr+0xC8/*0xB4*/) - *(LongPtr)(TargetListAddr+0xC4/*0xB0*/))/4;
		if( target_size > 1000 )
			err_code = -2;

#ifdef _DEBUG

		//oss << "target_size:"
		//	<< target_size
		//	<< "|";
#endif


		for( size_t i = 0; i < target_size; i++ )
		{
			CharPtr nTargetAddr = (CharPtr)*(LongPtr)(TargetAddr + i*4);
#ifdef _DEBUG
			oss << "nTargetAddr"
				<< "(" << i << ")"
				<< (LongPtr)nTargetAddr 
				<< "|";
#endif

			target_data[ i ].id = *(LongPtr)(nTargetAddr+0x94);//0x8C);

#ifdef _DEBUG

			//oss << "target_data("
			//	<< i 
			//	<< ")"
			//	<< target_data[ i ].id
			//	<< "|";

#endif 

			//+0xA4  目标坐标位置入口
			CharPtr TargetxyzAddr = (CharPtr)*(LongPtr)(nTargetAddr + 0xA8 );// 0xAC );//0xA4);

#ifdef _DEBUG
			oss << "TargetxyzAddr" 
				<< "(" << i << ")"
				<< (LongPtr)TargetxyzAddr 
				<< "|";
#endif

			target_data[ i ].x = *(FloatPtr)(TargetxyzAddr+0x0C + 4);
			target_data[ i ].y = *(FloatPtr)(TargetxyzAddr+0x10 + 4);
			target_data[ i ].z = *(FloatPtr)(TargetxyzAddr+0x14 + 4);
		}

		//show( oss.str().c_str() );
	}
	__except(1)
	{
		err_code = -1;
	}

}



bool check_process( const string& process_name )
{
	
		// 获取当前pid；
	DWORD pid = GetCurrentProcessId(); 

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	PROCESSENTRY32 pe = { 0 };  
	pe.dwSize = sizeof(pe);


	Process32First(hSnapshot,&pe);

	while( true )
	{
		if( pe.th32ProcessID == pid )
		{
			string pe_szExeFile = pe.szExeFile;
			//show( pe_szExeFile );
			if( pe_szExeFile == process_name )
				return true;

		}
		if( !Process32Next( hSnapshot, &pe ) )
			break;
	}

	return false;
}

void show( const string& str )
{
#ifdef _DEBUG
	OutputDebugString( str.c_str() );
#endif
}

//-------------------------------------------------------------------------
//
//void get_player_mem( Data& player_data )
//{
//	__try
//	{
//		int* CharacterAddr = (int*)0x1FA6978;
//		char* addrData = (char*)*CharacterAddr;
//
//		if(addrData > 0)
//		{
//			player_data.x = *(float*)(addrData + 0x188 + 4 + 4);
//			player_data.y = *(float*)(addrData + 0x18C + 4 + 4); 
//			player_data.z = *(float*)(addrData + 0x190 + 4 + 4);
//		}
//	}
//	__except(1)
//	{
//		player_data.x = -1;
//		player_data.y = -1;
//		player_data.z = -1;
//	}
//}
//
//void get_target_mem( vector<Data>& target_group )
//{
//	__try
//	{
//		Data data;
//
//		typedef long* LongPtr;
//		typedef char* CharPtr;
//		typedef float* FloatPtr;
//
//		LongPtr CharacterAddr = (LongPtr)0x1FA6978;
//
//		CharPtr PlayerAddrData = (CharPtr)(*CharacterAddr);
//
//		CharPtr TargetListAddr = (CharPtr)*(LongPtr)(PlayerAddrData + 0xB4);
//
//		if( !*(LongPtr)TargetListAddr )
//			return;
//
//		CharPtr TargetAddr = (CharPtr)*(LongPtr)(TargetListAddr+0xB0);
//		//第N个目标的如果地址
//
//		long num = (*(LongPtr)(TargetListAddr+0xB4) - *(LongPtr)(TargetListAddr+0xB0))/4;
//
//		for( size_t i = 0; i < num; i++ )
//		{
//			CharPtr nTargetAddr = (CharPtr)*(LongPtr)(TargetAddr + i*4);
//
//			data.id = *(LongPtr)(nTargetAddr+0x8C);
//
//			//+0xA4  目标坐标位置入口
//			CharPtr TargetxyzAddr = (CharPtr)*(LongPtr)(nTargetAddr +0xA4);
//
//			data.x = *(FloatPtr)(TargetxyzAddr+0x0C + 4);
//			data.y = *(FloatPtr)(TargetxyzAddr+0x10 + 4);
//			data.z = *(FloatPtr)(TargetxyzAddr+0x14 + 4);
//
//			target_group.push_back( data );
//		}
//	}
//	__except(1)
//	{
//	}
//}
//
//void show_target_pos()
//{
//	vector<Data> target_group;
//
//	get_target_mem( target_group );
//
//	ostringstream oss; 
//
//	for( size_t i = 0; i < target_group.size(); i++ )
//		oss 
//		<< "["
//		<< target_group[ i ].id
//		<< "]"
//		<< "("
//		<< target_group[ i ].x 
//		<< "," 
//		<< target_group[ i ].y 
//		<< "," 
//		<< target_group[ i ].z 
//		<< ")";
//
//	show( oss.str().c_str() );
//}
//
//void show_player_pos()
//{
//	float x = -1;
//	float y = -1;
//	float z = -1;
//
//	get_player_mem( player_data );
//
//	ostringstream oss;
//
//	oss << "(" << player_data.x << "," << player_data.y << "," << player_data.z << ")" << endl;
//
//	string str;
//	
//	str += "-->";
//	str += oss.str();
//
//	show( str.c_str() );
//}
//
//void loop()
//{
//	char path[MAX_PATH];
//	memset( path, 0, MAX_PATH );
//	GetModuleFileName(NULL,path, MAX_PATH );
//
//	show( path );
//
//
//	for( size_t i = 0; i < 1000; i++ )
//	{
//		show_player_pos();
//		show_target_pos();
//	}
//	while( true )
//	{
//		show_player_pos();
//		show_target_pos();
//		show( "start_sleep" );
//		Sleep( 500 );
//		show( "end_sleep" );
//	}
//}


void attach_dnf()
{
	string gen_time = __DATE__;
	gen_time = gen_time + "\t" + __TIME__;
	show( gen_time );
	show( "attach_start" );
	if( check_process( "DNF.exe" ) )
	{
		show( "attach_dnf" );
		boost::thread thrd( dnf_loop );
	}
}