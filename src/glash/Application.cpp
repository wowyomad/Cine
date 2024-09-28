#include "glash/glash_pch.hpp"
#include "glash/Applicatoin.hpp"

#include "glash/Log.hpp"
#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"
#include "glash/ImGui/ImGuiLayer.hpp"


namespace glash
{
	Application* Application::s_Instance = nullptr;

	Application& Application::Get()
	{
		return *s_Instance;
	}

	GLASH_WINDOW_CLASS& Application::GetWindow()
	{
		return *m_Window;
	}

	Application::Application()
		: m_Running(false)
	{
		GLASH_CORE_ASSERT(s_Instance == nullptr, "Applicatoin should be singleton");
		s_Instance = this;

		m_Window = std::unique_ptr<GLASH_WINDOW_CLASS>(Window::Create());
		m_Window->SetEventCallback(GLASH_BIND_EVENT_FN(Application::OnEvent));

		glClearColor(0.5, 0.2, 1, 1.0);
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (event.IsHandled())
			{
				break;
			}	
			(*it)->OnEvent(event);
		}

		dispatcher.Dispatch<WindowCloseEvent>(GLASH_BIND_EVENT_FN(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(GLASH_BIND_EVENT_FN(Application::OnWindowResizeEvent));


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

		return false;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& event)
	{
		return false;
	}

	void Application::Run()
	{
		m_Running = true;
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}

}

