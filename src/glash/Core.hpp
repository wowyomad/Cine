#pragma once

#ifdef GLASH_BUILD_DLL  // This is defined when building the DLL (in glash project)
#if defined(_WIN32) || defined(_WIN64)
#ifdef GLASH_EXPORT  // Defined during the DLL build to export symbols
#define BUILD_STR "DLL EXPORT"
#define GLASH_API __declspec(dllexport)
#else
#ifdef GLASH_IMPORT // Consumers of the DLL (like Sandbox) should use dllimport
#define BUILD_STR "DLL IMPORT"
#define GLASH_API __declspec(dllimport)
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



template<typename T>
inline constexpr T BIT(T x) {
    static_assert(std::is_integral_v<T>, "BIT can only be used with integral types.");
    return T(1) << x;
}

