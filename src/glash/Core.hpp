#pragma once

#if defined(GLASH_BUILD_SHARED) && defined(_WIN32)
	#ifdef GLASH_EXPORT
		#define GLASH_API __declspec(dllexport)
	#else
		#define GLASH_API __declspec(dllimport)
	#endif
#else
	#define GLASH_API
#endif