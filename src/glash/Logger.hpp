#pragma once
#include "glash/glash_pch.hpp" 
#include "glash/Enums.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/pattern_formatter.h"



#ifndef NO_LOGGING

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak();
#else
#define DEBUG_BREAK
#endif


#ifndef FORMAT_DUBUG_MESSAGE
#define FORMAT_DEBUG_MESSAGE "\"{}\" in {} at {}:{}"
#endif

#ifndef __FILENAME__
#define __FILENAME__ std::filesystem::path(__FILE__).filename().string().c_str()
#endif

#define ASSERT(x) if((x)) DEBUG_BREAK;
#define GLCall(x)\
        glash::GLClearErrors();\
        (x);\
        ASSERT(glash::GLLogCall(#x, __FILENAME__, __LINE__))


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
#define LOG_INFO(msg, ...)
#define LOG_ERROR(msg, ...)
#define LOG_WARN(msg, ...)
#define LOG_DEBUG(msg, ...)
#define LOG_INFO_EX(msg, func, line, file)
#define LOG_ERROR_EX(msg, func, line, file)
#define LOG_WARN_EX(msg, func, line, file)
#define LOG_DEBUG_EX(msg, func, line, file)

#define GLCall(x)\
        (x);
#endif
#include "glash/IndexBuffer.hpp"
namespace glash
{
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
		while (glGetError() != GL_NO_ERROR)
		{

		}

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

	inline GLint GLGetStatus(GLuint object, GLStatus status)
	{
		char log[512];
		GLint param = 0;


		switch (status)
		{
		case PROGRAM_LINK:
			glGetProgramiv(object, status, &param);
			break;
		case GLShaderType: case SHADER_COMPILE:
			glGetShaderiv(object, status, &param);
			break;
		default:
			param = 0;
			LOG_DEBUG("Uknown status");
		}
		return param;
	}

	inline void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam)
	{
		std::string sourceStr;
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Other"; break;
		}

		std::string typeStr;
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

		std::string severityStr;
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         severityStr = "High"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "Medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          severityStr = "Low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
		}

		LOG_ERROR("[OpenGL Debug] Source: {}, Type: {}, Severity: {}, Message: {}",
			sourceStr, typeStr, severityStr, message);
	}

	inline void initLogger()
	{
		spdlog::set_level(spdlog::level::debug);
	}

	inline void InitializeOpenGLDebug()
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDebugMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}



