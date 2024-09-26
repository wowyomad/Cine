#include "Core.hpp"

#include "glash/Applicatoin.hpp"
#include "glash/Window_OLD.hpp"
#include "glash/Log.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"

#include "GLFW/glfw3.h"
namespace glash
{
	Application::Application()
		: m_Running(false)
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		glClearColor(0.25, 1.0, 0.5, 1.0);
	}
	void Application::Run()
	{
		m_Running = true;
		while (m_Running)
		{
			m_Window->OnUpdate();
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
}

