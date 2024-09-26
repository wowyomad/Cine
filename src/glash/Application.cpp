#include "glash/Applicatoin.hpp"
#include "glash/Window.hpp"
#include "glash/Log.hpp"
#include "glash/events/Event.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"

namespace glash
{
	Application::Application()
	{
		
	}
	void Application::Run()
	{
		Window window(800, 600, "Window");
		while (!window.ShouldClose())
		{
			window.PollEvents();
		}
	}
}