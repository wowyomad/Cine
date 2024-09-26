#pragma once
#include "glash/Applicatoin.hpp"
#include "glash/Log.hpp"

extern glash::Application* glash::CreateApplication();

int main(int argc, char** argv)
{
	glash::Log::Init();
	GLASH_LOG_INFO(BUILD_STR);
	auto app = glash::CreateApplication();
	app->Run();
	delete app;
}