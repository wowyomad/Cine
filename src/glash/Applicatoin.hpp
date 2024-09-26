#pragma once
#include "glash_pch.hpp"
#include "glash/Core.hpp"

#include "Window.hpp"

namespace glash
{
	class GLASH_API Application
	{
	public:
		Application();
		virtual ~Application() { };

		virtual void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};
	//Should be defined in Client
	Application* CreateApplication();
}