#pragma once

#include <string>
#include "glash/Core/Log.hpp"

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

        DynamicLibrary() : m_Handle(nullptr) {}

        ~DynamicLibrary() {
            Unload();
        }

        bool Load(const std::filesystem::path& libraryName) 
        {
            m_Name = libraryName.string();
            CINE_CORE_WARN("Loading library {0}", m_Name);
#ifdef _WIN32
            m_Handle = LoadLibrary(m_Name.c_str());
            if (!m_Handle) {
                CINE_CORE_WARN("Failed to load ", m_Name);
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

        void Unload() {
            if (m_Handle)
            {
                CINE_CORE_WARN("Unloading library {0}", m_Name);
#ifdef _WIN32
                FreeLibrary(static_cast<HMODULE>(m_Handle));
#else
                dlclose(handle);
#endif
                m_Handle = nullptr;
            }
        }

        template <typename T>
        T GetFunction(const std::string& functionName) {
#ifdef _WIN32
            return reinterpret_cast<T>(GetProcAddress(static_cast<HMODULE>(m_Handle), functionName.c_str()));
#else
            return reinterpret_cast<T>(dlsym(handle, functionName.c_str()));
#endif
        }

    private:
        void* m_Handle = nullptr;
        std::string m_Name;
    };


}
