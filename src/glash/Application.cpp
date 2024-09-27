#include "Core.hpp"

#include "glash/Applicatoin.hpp"
#include "glash/Window_OLD.hpp"
#include "glash/Log.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace glash
{
	Application::Application()
		: m_Running(false)
	{
		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		glClearColor(0.25, 1.0, 0.5, 1.0);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowCloseEvent));
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;

		return true;
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

