#include "Sandbox.hpp"
#include "glash/Core/EntryPoint.hpp"


Application* Cine::CreateApplication()
{
	return new SandboxApp();
}