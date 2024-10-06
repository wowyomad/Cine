#pragma once
#include "glash/glash_pch.hpp" 

#include "glash/Core/Core.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/pattern_formatter.h"

#include <string_view>

namespace Cine
{
#if GLASH_ENABLE_DEBUG
	#ifdef _MSC_VER
		#define DEBUG_BREAK __debugbreak()
	#else
		#define DEBUG_BREAK
	#endif
	#ifndef FORMAT_DEBUG_MESSAGE
		#define FORMAT_DEBUG_MESSAGE "\"{}\" in {} at {}:{}"
	#endif

	#ifndef __FILENAME__
		#define __FILENAME__ std::filesystem::path(__FILE__).filename().string()
	#endif

	#define ASSERT(x) if(!(x)) DEBUG_BREAK;
	#define GLCall(x)\
			Cine::debug::g_Func = __func__;\
			Cine::debug::g_Line = __LINE__;\
			Cine::debug::g_FileName = __FILENAME__;\
			x;\
			if (Cine::debug::g_HasErrorOccured) DEBUG_BREAK;\
			Cine::debug::g_HasErrorOccured = false;
	#define GLASH_CORE_TRACE(msg, ...)\
		Cine::Log::GetCoreLogger()->trace(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_CORE_DEBUG(msg, ...)\
		Cine::Log::GetCoreLogger()->debug(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_CORE_INFO(msg, ...)\
		Cine::Log::GetCoreLogger()->info(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_CORE_ERROR(msg, ...)\
		Cine::Log::GetCoreLogger()->error(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_CORE_ERROR_EX(msg, func, filename, line, ...)\
		Cine::Log::GetCoreLogger()->error(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), func, filename, line)
	#define GLASH_LOG_TRACE(msg, ...)\
		Cine::Log::GetClientLogger()->trace(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_LOG_DEBUG(msg, ...)\
		Cine::Log::GetClientLogger()->debug(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_LOG_INFO(msg, ...)\
		Cine::Log::GetClientLogger()->info(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	#define GLASH_LOG_ERROR(msg, ...)\
		Cine::Log::GetClientLogger()->error(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	
	#define LOG_DEBUG(msg, ...) \
				spdlog::debug(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	
	#define LOG_DEBUG_EX(msg, func, filename, line, ...) \
				spdlog::debug(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), func, filename, line)
	
	#define LOG_INFO(msg, ...) \
				spdlog::info(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	
	#define LOG_INFO_EX(msg, func, filename, line, ...) \
				spdlog::info(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), func, filename, line)
	
	#define LOG_ERROR(msg, ...) \
				spdlog::error(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	
	#define LOG_ERROR_EX(msg, func, filename, line, ...) \
				spdlog::error(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), func, filename, line)
	
	#define LOG_WARN(msg, ...) \
				spdlog::warn(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), __func__, __FILENAME__, __LINE__)
	
	#define LOG_WARN_EX(msg, func, filename, line, ...) \
			spdlog::warn(FORMAT_DEBUG_MESSAGE, fmt::format(msg, ##__VA_ARGS__), func, filename, line)

#else
	#define GLASH_CORE_TRACE(msg, ...)
	#define GLASH_CORE_DEBUG(msg, ...)
	#define GLASH_CORE_INFO(msg, ...)
	#define GLASH_CORE_ERROR(msg, ...)
	#define GLASH_LOG_TRACE(msg, ...)
	#define GLASH_LOG_DEBUG(msg, ...)
	#define GLASH_LOG_INFO(msg, ...)
	#define GLASH_LOG_ERROR(msg, ...)
	#define DEBUG_BREAK
	#define GLCall(x) (x)
	#define ASSERT(x)
	#define LOG_DEBUG(msg, ...)
	#define LOG_DEBUG_EX(msg, func, filename, line, ...)
	#define LOG_INFO(msg, ...)
	#define LOG_INFO_EX(msg, func, filename, line, ...)
	#define LOG_ERROR(msg, ...)
	#define LOG_ERROR_EX(msg, func, filename, line, ...)
	#define LOG_WARN(msg, ...)
	#define LOG_WARN_EX(msg, func, filename, line, ...)
#endif
	class GLASH_API Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger(); 
		static std::shared_ptr<spdlog::logger>& GetClientLogger();

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
	namespace debug
	{
		inline bool g_HasErrorOccured = false;
		inline std::string_view g_Func = "";
		inline std::string g_FileName = "";
		inline size_t g_Line = 0;

		inline const char* GetGLErrorString(GLenum error)
		{
			switch (error)
			{
			case GL_INVALID_ENUM:                  return "INVALID_ENUM";
			case GL_INVALID_VALUE:                 return "INVALID_VALUE";
			case GL_INVALID_OPERATION:             return "INVALID_OPERATION";
			case GL_STACK_OVERFLOW:                return "STACK_OVERFLOW";
			case GL_STACK_UNDERFLOW:               return "STACK_UNDERFLOW";
			case GL_OUT_OF_MEMORY:                 return "OUT_OF_MEMORY";
			case GL_INVALID_FRAMEBUFFER_OPERATION: return "INVALID_FRAMEBUFFER_OPERATION";
			default:                               return "UNKNOWN_ERROR";
			}
		}

		inline void GLClearErrors()
		{
			while (glGetError() != GL_NO_ERROR) {}
		}

		inline bool GLLogCall(const char* function, const char* file, int line)
		{
			bool hasErrorOccured = false;
			while (GLenum error_code = glGetError())
			{
				const std::string error_name = GetGLErrorString(error_code);
				LOG_ERROR_EX("[GL_ERROR 0x{:0X}: {}]", function, file, line, error_code, error_name);
				hasErrorOccured = true;
			}
			return hasErrorOccured;
		}

		inline void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam)
		{
			g_HasErrorOccured = true;
			std::string_view sourceStr, typeStr, severityStr;
			switch (source)
			{
			case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Other"; break;
			}

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behavior"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behavior"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
			}

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:         severityStr = "High"; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "Medium"; break;
			case GL_DEBUG_SEVERITY_LOW:          severityStr = "Low"; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
			}

			GLASH_CORE_ERROR_EX("[OpenGL Debug] Source: {}, Type: {}, Severity: {}, Message: {}",
				g_Func, g_FileName, g_Line, sourceStr, typeStr, severityStr, message);
		}

		inline void InitializeOpenGLDebug()
		{
#ifdef GLASH_ENABLE_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugMessageCallback, nullptr);
#endif
		}
	}
	inline GLint GLGetStatus(GLuint object, uint32_t status)
	{
		char log[512];
		GLint param = 0;

		switch (status)
		{
		case GL_LINK_STATUS:
			GLCall(glGetProgramiv(object, status, &param));
			break;
		case GL_SHADER_TYPE: case GL_COMPILE_STATUS:
			GLCall(glGetShaderiv(object, status, &param));
			break;
		default:
			param = 0;
			LOG_DEBUG("Uknown status");
		}
		return param;
	}
}
