#pragma once

#include <string>

#ifdef _WIN32
#define NOMINMAX 
#include <Windows.h>
#else
#include <dlfcn.h>
#endif


namespace Cine
{
	class FileDialogs
	{
	public:
		static std::filesystem::path OpenFile(const char* filter);
		static std::filesystem::path SaveFile(const char* filter);
	};

    class DynamicLibrary {
    public:

        DynamicLibrary() : handle(nullptr) {}

        ~DynamicLibrary() {
            unload();
        }

        bool load(const std::string& libraryName) {
#ifdef _WIN32
            handle = LoadLibrary(libraryName.c_str());
            if (!handle) {
                std::cerr << "Failed to load " << libraryName << std::endl;
                return false;
            }
#else
            handle = dlopen(libraryName.c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "Failed to load " << libraryName << ": " << dlerror() << std::endl;
                return false;
            }
#endif
            return true;
        }

        void unload() {
            if (handle) {
#ifdef _WIN32
                FreeLibrary(static_cast<HMODULE>(handle));
#else
                dlclose(handle);
#endif
                handle = nullptr;
            }
        }

        template <typename T>
        T getFunction(const std::string& functionName) {
#ifdef _WIN32
            return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(handle), functionName.c_str()));
#else
            return reinterpret_cast<T>(dlsym(handle, functionName.c_str()));
#endif
        }

    private:
        void* handle;
    };


}
