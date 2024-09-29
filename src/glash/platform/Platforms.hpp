#pragma once

#if _WIN32 == 1
	#if _WIN64 == 1
		#define GLASH_PLATFORM_WINDOWS 1
		#define GLASH_WINDOW_CLASS WindowsWindow
		#include "glash/platform/Windows/WindowsWindow.hpp"
	#else
		#error x86 not supported
	#endif
#else
	#error only Windows supported
#endif