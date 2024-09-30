#pragma once
#include "glash/Core/Application.hpp"
#include "glash/Core/Log.hpp"

extern glash::Application* glash::CreateApplication();

int main(int argc, char** argv)
{
	glash::Log::Init();
	glash::Input::Init();
	GLASH_LOG_INFO(BUILD_STR);
	glash::Application* app = glash::CreateApplication();
	app->Run();
	delete app;
}