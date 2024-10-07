#pragma once

#if _WIN32 == 1
	#if _WIN64 == 1
		#define CINE_PLATFORM_WINDOWS 1
		#define CINE_PLATFORM_OPENGL 1
		#define CINE_RENDERER_API_CLASS OpenGLRendererAPI
		#define CINE_SHADER_CLASS OpenGLShader
		#define CINE_WINDOW_CLASS WindowsWindow
		#include "glash/platform/Windows/WindowsWindow.hpp"

	#else
		#error x86 not supported
	#endif
#else
	#error only Windows supported
#endif
