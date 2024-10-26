#include "Shell.hpp"
#include "glash/glash_pch.hpp"

#include "glash/Core/Log.hpp"

namespace Cine
{
#if _WIN32
	bool Shell::RunPythonSilently(const std::string& scriptPath)
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

	bool Shell::RunPythonLoud(const std::string& scriptPath)
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
	bool Shell::OpenEditor(const std::filesystem::path& where)
	{
		HINSTANCE result = ShellExecuteA(NULL, "open", "code", where.string().c_str(), NULL, SW_HIDE);

		if ((int)result >= 32)
		{
			return true;
		}
		else
		{
			CINE_CORE_ASSERT(false, "Failed to open Visual Studio Code at {0}", where.string());
			return false;
		}
	}
	bool Shell::OpenExlorer(const std::filesystem::path& where)
	{
		HINSTANCE result = ShellExecuteA(NULL, "open", "explorer.exe", where.string().c_str(), NULL, SW_SHOWNORMAL);

		if ((int)result >= 32)
		{
			return true;
		}
		else
		{
			CINE_CORE_ASSERT(false, "Failed to open explorer at {0}", where.string());
			return false;
		}
	}
	bool Shell::CreateNewScript(const std::string& name)
	{
		std::string command = "./Assets/Plugin/creator.py ";
		command += name;
		return Shell::RunPythonLoud(command.c_str());
	}

	bool Shell::RunShellCommandLoud(const std::string& command)
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

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
			return false;
		}
	}

#endif // _WIN32
}


