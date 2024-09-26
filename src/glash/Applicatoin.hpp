#pragma once
#include "glash/Core.hpp"

namespace glash
{
	class GLASH_API Application
	{
	public:
		Application();
		virtual ~Application() {};

		virtual void Run();
	};

	//Should be defined in Client
	Application* CreateApplication();
}