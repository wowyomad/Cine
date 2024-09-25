#include "glash/Log.hpp"

namespace glash
{
	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_logger_mt("GLASH");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_logger_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}