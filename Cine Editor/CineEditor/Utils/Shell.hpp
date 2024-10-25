#pragma once

namespace Cine
{
	namespace Shell
	{
		bool RunPythonScriptSilently(const std::string& scriptPath);
		bool RunPythonScriptLoud(const std::string& scriptPath);

		bool CreateNewScript(const std::string& name);
	}
}



