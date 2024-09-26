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
		m_Window = std::unique_ptr<Window>(Window::Create());	
	}
	void Application::Run()
	{
		m_Running = true;
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}

