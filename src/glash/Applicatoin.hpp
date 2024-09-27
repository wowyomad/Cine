#pragma once
#include "glash_pch.hpp"
#include "glash/Core.hpp"

#include "glash/events/ApplicationEvent.hpp"
#include "glash/Window.hpp"

namespace glash
{
	class GLASH_API Application
	{
	public:
		Application();
		virtual ~Application() { };

		void Run();
		void OnEvent(Event& event);

	private:
		bool OnWindowCloseEvent(WindowCloseEvent& event);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
	};
	//Should be defined in Client
	Application* CreateApplication();
}