#pragma once

#ifdef GLASH_PLATFORM_WINDOWS
#define GLASH_WINDOW_CLASS WindowsWindow
#include "glash/platform/Windows/WindowsWindow.hpp"
#else
#define GLASH_WINDOW_CLASS WindowsWindow
#include "glash/platform/Windows/WindowsWindow.hpp"
#endif