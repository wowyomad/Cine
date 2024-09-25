#pragma once
#include "glash/Applicatoin.hpp"

extern glash::Application* glash::CreateApplication();

int main(int argc, char** argv)
{
	auto app = glash::CreateApplication();
	app->Run();
	delete app;
}