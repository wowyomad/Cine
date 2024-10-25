#include "Shell.hpp"
#include "glash/glash_pch.hpp"

#include "glash/Core/Log.hpp"

namespace Cine
{
#if _WIN32
	bool Shell::RunPythonScriptSilently(const std::string& scriptPath)
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow = SW_HIDE;

		si.hStdOutput = NULL;
		si.hStdError = NULL;

		std::string command = "python " + scriptPath;

		char cmdLine[256];
		strncpy(cmdLine, command.c_str(), sizeof(cmdLine));
		cmdLine[sizeof(cmdLine) - 1] = '\0';

		if (CreateProcessA(NULL, cmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return true;
		}
		else
		{
			CINE_CORE_ASSERT(false, "Failed to run script {0} with error code {1}", scriptPath, GetLastError());
			return false;
		}
	}

	bool Shell::RunPythonScriptLoud(const std::string& scriptPath)
	{
		

		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		std::string command = "python " + scriptPath;

		char cmdLine[256];
		strncpy(cmdLine, command.c_str(), sizeof(cmdLine));
		cmdLine[sizeof(cmdLine) - 1] = '\0';

		if (CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		{
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return true;
		}
		else
		{
			CINE_CORE_ASSERT(false, "Failed to run script {0} with error code {1}", scriptPath, GetLastError());
			return false;
		}
	}
	bool Shell::CreateNewScript(const std::string& name)
	{
		std::string command = "./Assets/Plugin/creator.py ";
		command += name;
		return Shell::RunPythonScriptLoud(command.c_str());
	}
#endif // _WIN32
}


