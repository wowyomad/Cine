#include "Sandbox2DApp.hpp"
#include "glash/Core/EntryPoint.hpp"

Cine::Application* Cine::CreateApplication()
{
	return new SandboxApp();
}