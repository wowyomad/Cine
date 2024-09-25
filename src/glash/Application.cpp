#include "glash/Applicatoin.hpp"
#include "glash/Window.hpp"
#include "glash/Log.hpp"
namespace glash
{
	void Application::Run()
	{
		Window window(800, 600, "Window");
		while (!window.ShouldClose())
		{
			window.PollEvents();
		}
	}
}