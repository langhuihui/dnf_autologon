#ifndef __LOGIN_BASE_H__
#define __LOGIN_BASE_H__


#include <dm.h>
#pragma comment(lib, "dm.lib")

#ifdef LOGIN_EXPORTS
#define _DllExport __declspec(dllexport)
#else
#define _DllExport __declspec(dllimport)
#endif

_DNF_BEGIN

	

_DNF_END

#endif