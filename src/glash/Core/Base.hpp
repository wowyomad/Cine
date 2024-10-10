#pragma once


#if defined(_WIN32) || defined(_WIN64)
    #define GLASH_API_EXPORT __declspec(dllexport)
    #define GLASH_API_IMPORT __declspec(dllimport)
#else
    #define GLASH_API_EXPORT
    #define GLASH_API_IMPORT
#endif

#ifndef GLASH_BUILD_DLL
    #define GLASH_BUILD_DLL 0
#endif
#ifndef GLASH_EXPORT
    #define GLASH_EXPORT 0
#endif
#ifndef GLASH_IMPORT
    #define GLASH_IMPORT 0
#endif
#ifndef GLASH_ENABLE_ASSERTS
    #define GLASH_ENABLE_ASSERTS 0
#endif
#ifndef GLASH_ENABLE_DEBUG
    #define GLASH_ENABLE_DEBUG 0
#endif

#if GLASH_BUILD_DLL  // This is defined when building the DLL (in glash project)
    #if defined(_WIN32) || defined(_WIN64)
        #if GLASH_EXPORT  // Defined during the DLL build to export symbols
            #define BUILD_STR "DLL EXPORT"
            #define GLASH_API GLASH_API_EXPORT
        #else
            #if GLASH_IMPORT // Consumers of the DLL (like Sandbox) should use dllimport
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

#if CINE_ENABLE_ASSERTS
    #define CINE_CORE_ASSERT(x, ...) { if(!(x)) { GLASH_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); DEBUG_BREAK; } }
    #define CINE_ASSERT(x, ...) { if(!(x)) { GLASH_LOG_ERROR("Assertion failed: {}", __VA_ARGS__); DEBUG_BREAK; } }
#else
    #define CINE_CORE_ASSERT(x, ...)
    #define CINE_ASSERT(x, ...)
#endif


template<typename T>
inline constexpr T BIT(T x) {
    static_assert(std::is_integral_v<T>, "BIT can only be used with integral types.");
    return T(1) << x;
}

namespace Cine
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
struct fmt::formatter<Cine::ClassName> {									\
    template <typename ParseContext>										\
    constexpr auto parse(ParseContext& ctx) {								\
        return ctx.begin();													\
    }																		\
																			\
    template <typename FormatContext>										\
    auto format(const Cine::ClassName& event, FormatContext& ctx) const {	\
		if constexpr (Cine::has_to_string<Cine::ClassName>::value) {		\
			return fmt::format_to(ctx.out(), "{}", event.ToString());		\
		}																	\
		else {																\
			return fmt::format_to(ctx.out(), "{}", #ClassName);				\
		}																	\
    }																		\
};

#define CINE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Cine
{
    template <class T>
    using Scope = std::unique_ptr<T>;
    
    template <class T>
    using Ref = std::shared_ptr<T>;

    template <class T, class ...Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <class T, class ...Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}

#include "glash/Core/Log.hpp"
#include "glash/platform/Platforms.hpp"
