#pragma once

#include "glash/Core/Application.hpp"
#include "glash/Debug/Instrumentor.hpp"

extern Cine::Application* Cine::CreateApplication();

int main(int argc, char** argv)
{
	Cine::Log::Init();

	CINE_PROFILE_BEGIN_SESSION("Startup", "Startup-Session.json");
	Cine::Input::Init();
	Cine::Application* app = Cine::CreateApplication();
	CINE_PROFILE_END_SESSION();

	CINE_PROFILE_BEGIN_SESSION("Runtime", "Runtime-Session.json");
	app->Run();
	CINE_PROFILE_END_SESSION();

	delete app;
}