#pragma once


#if defined(_MSC_VER)


#if defined(RAGE_SHARED_LIB)
		#if defined(RAGE_EXPORTS)
				#define RageAPI __declspec(dllexport)
		#else
				#define RageAPI __declspec(dllimport)
		#endif
#else
		#define RageAPI
#endif


#if defined(RAGE_SHARED_LIB)
		#define FOUNDATION_SHARED_LIB
#endif

#if defined(FOUNDATION_NDEBUG)
		#define RAGE_NDEBUG
#else
		#define RAGE_DEBUG
#endif

#include "Foundation.h"
#pragma comment(lib, "Foundation.lib")

#if defined(min)
#undef min

#endif


#pragma warning(disable : 4267)
#pragma warning(disable : 4505)
#pragma warning(disable : 4310)

#endif