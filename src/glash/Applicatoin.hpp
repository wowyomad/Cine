#pragma once

namespace glash
{
	class Application
	{
	public:
		Application() = default;
		virtual ~Application() = default;

		virtual void Run();
	};

	//Should be defined in Client
	Application* CreateApplication();
}

