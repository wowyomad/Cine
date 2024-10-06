#pragma once
#include "glash/Core/Application.hpp"
#include "glash/Core/Log.hpp"

extern Cine::Application* Cine::CreateApplication();

int main(int argc, char** argv)
{
	Cine::Log::Init();
	Cine::Input::Init();
	Cine::Application* app = Cine::CreateApplication();
	app->Run();
	delete app;
}