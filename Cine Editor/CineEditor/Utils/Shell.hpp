#pragma once

#include <string>
#include <filesystem>
#include <shellapi.h>

namespace Cine
{
	namespace Shell
	{
		bool RunPythonSilently(const std::string& scriptPath);
		bool RunPythonLoud(const std::string& scriptPath);

		bool OpenEditor(const std::filesystem::path& where);
		bool OpenExlorer(const std::filesystem::path& where);

		bool CreateNewScript(const std::string& name);


		bool RunShellCommandLoud(const std::string& command);
	}
}



