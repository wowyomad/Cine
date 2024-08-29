#pragma once
#include "glash/glash_pch.hpp" 
#ifndef NDEBUG
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/pattern_formatter.h"

namespace glash
{
    inline void initLogger()
    {
        spdlog::set_level(spdlog::level::debug);
    }
}

#define __FILENAME__ std::filesystem::path(__FILE__).filename().string()

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
#else
#define LOG_INFO(msg, ...)
#define LOG_LIBRARY_INFO(msg, ...)
#define LOG_ERROR(msg, ...)
#define LOG_LIBRARY_ERROR(msg, ...)
#define LOG_WARN(msg, ...)
#define LOG_LIBRARY_WARN(msg, ...)
#define LOG_DEBUG(msg, ...)
#define LOG_LIBRARY_DEBUG(msg, ...)

namespace glash
{
    inline void initLogger()
    {
        
    }
}
#endif
