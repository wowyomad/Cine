#pragma once
#include "glash/glash_pch.hpp" 
#include "gl/glew.h" //can't find 'defines' without it(???)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/pattern_formatter.h"



#ifndef NDEBUG

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak();
#else
#define DEBUG_BREAK
#endif

#define __FILENAME__ std::filesystem::path(__FILE__).filename().string().c_str()

#define ASSERT(x) if((x)) DEBUG_BREAK;
#define GlCall(x)\
        glash::GlClearErrors();\
        (x);\
        ASSERT(glash::GlLogCall(#x, __FILENAME__, __LINE__))


#define LOG_DEBUG(msg, ...) \
    spdlog::debug("{} ({}:{}) {}", fmt::format(msg, ##__VA_ARGS__), __func__, __LINE__, __FILENAME__)

#define LOG_LIBRARY_DEBUG(msg, ...) \
    LOG_DEBUG(msg, ##__VA_ARGS__)

#define LOG_INFO(msg, ...) \
    spdlog::info("{} ({}:{}) {}", fmt::format(msg, ##__VA_ARGS__), __func__, __LINE__, __FILENAME__)

#define LOG_LIBRARY_INFO(msg, ...) \
    LOG_INFO(msg, ##__VA_ARGS__)

#define LOG_ERROR(msg, ...) \
    spdlog::error("{} ({}:{}) {}", fmt::format(msg, ##__VA_ARGS__), __func__, __LINE__, __FILENAME__)

#define LOG_LIBRARY_ERROR(msg, ...) \
    LOG_ERROR(msg, ##__VA_ARGS__)

#define LOG_WARN(msg, ...) \
    spdlog::warn("{} ({}:{}) {}", fmt::format(msg, ##__VA_ARGS__), __func__, __LINE__, __FILENAME__)

#define LOG_LIBRARY_WARN(msg, ...) \
    LOG_WARN(msg, ##__VA_ARGS__)


namespace glash
{
	enum GLStatus
	{
		SHADER_COMPILE = GL_COMPILE_STATUS,
		SHADER_INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH,
		SHADER_SOURCE_LENGTH = GL_SHADER_SOURCE_LENGTH,
		SHADER_TYPE = GL_SHADER_TYPE,
		PROGRAM_LINK = GL_LINK_STATUS,
		PROGRAM_VALIDATE = GL_VALIDATE_STATUS,
		PROGRAM_INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH,
		PROGRAM_ATTACHED_SHADERS = GL_ATTACHED_SHADERS,
		PROGRAM_ACTIVE_UNIFORMS = GL_ACTIVE_UNIFORMS,
		PROGRAM_ACTIVE_ATTRIBUTES = GL_ACTIVE_ATTRIBUTES

	};


	inline void GlClearErrors()
	{
		while (glGetError() != GL_NO_ERROR)
		{

		}

	}

	inline bool GlLogCall(const char* function, const char* file, int line)
	{
		bool hasErrorOccured = false;
		while (GLenum error = glGetError())
		{
			fmt::println("GL_ERROR ({}) in {}:{} {}", error, file, line, function);
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
		default:
			param = 0;
			LOG_DEBUG(fmt::runtime("Uknown status"));
			throw std::runtime_error("Uknown status");
		}
		return param;
	}




	inline void initLogger()
	{
		spdlog::set_level(spdlog::level::debug);
	}
}


#else
#define LOG_INFO(msg, ...)
#define LOG_LIBRARY_INFO(msg, ...)
#define LOG_ERROR(msg, ...)
#define LOG_LIBRARY_ERROR(msg, ...)
#define LOG_WARN(msg, ...)
#define LOG_LIBRARY_WARN(msg, ...)
#define LOG_DEBUG(msg, ...)
#define LOG_LIBRARY_DEBUG(msg, ...)

#define GLCall(x)\
        (x);

namespace glash
{
	inline void initLogger()
	{

	}
}
#endif



