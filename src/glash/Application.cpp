#include "Core.hpp"

#include "glash/Applicatoin.hpp"
#include "glash/Window_OLD.hpp"
#include "glash/Log.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"


#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace glash
{
	Application::Application()
		: m_Running(false)
	{
		m_Window = std::unique_ptr<Window>(Window::Create());

		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		glClearColor(0.25, 1.0, 0.5, 1.0);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResizeEvent));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.IsHandled())
			{
				break;
			}
			(*it)->OnEvent(event);
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushLayer(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& event)
	{
		m_Running = false;

		return true;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		
		return true;
	}

	void Application::Run()
	{
		m_Running = true;
		while (m_Running)
		{
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}
	
}

