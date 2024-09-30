#pragma once

#if defined(_WIN32) || defined(_WIN64)
#define GLASH_API_EXPORT __declspec(dllexport)
#define GLASH_API_IMPORT __declspec(dllimport)
#endif


#ifdef GLASH_BUILD_DLL  // This is defined when building the DLL (in glash project)
#if defined(_WIN32) || defined(_WIN64)
#ifdef GLASH_EXPORT  // Defined during the DLL build to export symbols
#define BUILD_STR "DLL EXPORT"
#define GLASH_API GLASH_API_EXPORT
#else
#ifdef GLASH_IMPORT // Consumers of the DLL (like Sandbox) should use dllimport
#define BUILD_STR "DLL IMPORT"
#define GLASH_API GLASH_API_IMPORT
#endif
#endif
#else
// Non-Windows platforms (like Linux, macOS) can use default visibility
#define GLASH_API __attribute__((visibility("default")))
#define BUILD_STR "SHARED EXPORT"
#endif
#else  // Static library or no DLL
#define GLASH_API  // Empty macro for static builds
#define BUILD_STR "STATIC BUILD"
#endif

#ifdef GLASH_ENABLE_ASSERTS
    #define GLASH_CORE_ASSERT(x, ...) { if(!(x)) { GLASH_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); DEBUG_BREAK; } }
    #define GLASH_ASSERT(x, ...) { if(!(x)) { GLASH_LOG_ERROR("Assertion failed: {}", __VA_ARGS__); DEBUG_BREAK; } }
#else
    #define GLASH_CORE_ASSERT(x, ...)
    #define GLASH_ASSERT(x, ...)
#endif


template<typename T>
inline constexpr T BIT(T x) {
    static_assert(std::is_integral_v<T>, "BIT can only be used with integral types.");
    return T(1) << x;
}

namespace glash
{
    template <typename T, typename = void>
    struct has_to_string : std::false_type {};

    template <typename T>
    struct has_to_string<T, std::void_t<decltype(std::declval<T>().ToString())>> : std::true_type {};
}

//Requires class to implement 'std::string ToString() const',
//otherwise uses class name
#define STRING_FORMAT(ClassName)\
template <>																	\
struct fmt::formatter<glash::ClassName> {									\
    template <typename ParseContext>										\
    constexpr auto parse(ParseContext& ctx) {								\
        return ctx.begin();													\
    }																		\
																			\
    template <typename FormatContext>										\
    auto format(const glash::ClassName& event, FormatContext& ctx) const {	\
		if constexpr (glash::has_to_string<glash::ClassName>::value) {		\
			return fmt::format_to(ctx.out(), "{}", event.ToString());		\
		}																	\
		else {																\
			return fmt::format_to(ctx.out(), "{}", #ClassName);				\
		}																	\
    }																		\
};

#define GLASH_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#include "glash/Core/Log.hpp"
#include "glash/platform/Platforms.hpp"