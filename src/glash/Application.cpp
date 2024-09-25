#include "glash/Applicatoin.hpp"
#include "glash/Window.hpp"
#include "glash/Log.hpp"
#include "glash/events/Event.hpp"
namespace glash
{
	Application::Application()
	{
		
	}
	void Application::Run()
	{
		GLASH_CORE_TRACE("Trace inside {}", __FUNCTION__);
		GLASH_CORE_DEBUG("Debug inside {}", __FUNCTION__);
		GLASH_CORE_INFO("Info inside {}", __FUNCTION__);
		GLASH_CORE_ERROR("Error inside {}", __FUNCTION__);
		Event* event = nullptr;
		Window window(800, 600, "Window");
		while (!window.ShouldClose())
		{
			window.PollEvents();
		}
	}
}