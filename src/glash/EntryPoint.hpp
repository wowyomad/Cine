#pragma once
#include "glash/Applicatoin.hpp"
#include "glash/Log.hpp"

extern glash::Application* glash::CreateApplication();

int main(int argc, char** argv)
{
	glash::Log::Init();
	glash::Log::GetClientLogger()->info("Info");
	auto app = glash::CreateApplication();
	app->Run();
	delete app;
}