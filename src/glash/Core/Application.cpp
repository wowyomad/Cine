#include "glash/glash_pch.hpp"
#include "glash/Core/Application.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/RenderCommand.hpp"
#include "glash/Renderer/Renderer.hpp"

#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"

#include "glash/Core/KeyCodes.hpp"

#include "glash/ImGui/ImGuiLayer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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

	Application::Application(int dummy)
		: m_Running(false)
	{
		GLASH_LOG_TRACE(BUILD_STR);
		(void)dummy; //get rid of unused parameter warning

		GLASH_CORE_ASSERT(s_Instance == nullptr, "Application should be singleton");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(GLASH_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 0.15 });

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	ImGuiContext* Application::GetImGuiContext() const
	{
		return ImGui::GetCurrentContext();
	}

	void Application::GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) const
	{
		ImGui::GetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);
	}


}

