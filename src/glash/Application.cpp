#include "Core.hpp"

#include "glash/Applicatoin.hpp"
#include "glash/Window_OLD.hpp"
#include "glash/Log.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"

namespace glash
{
	Application::Application()
	{
		
	}
	void Application::Run()
	{
		Window_OLD window(800, 600, "Window");
		WindowFocusEvent focusEvent;

		EventDispatcher dispatcher(focusEvent);

		dispatcher.Dispatch<WindowFocusEvent>([](WindowFocusEvent& event) -> bool { LOG_INFO("Event dispatcher: {}", event);  return true;  }); //This works

		while (!window.ShouldClose())
		{
			window.PollEvents();
		}
	}
}

