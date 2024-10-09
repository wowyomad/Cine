#include "glash/glash_pch.hpp"
#include "glash/Core/Application.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/RenderCommand.hpp"
#include "glash/Renderer/Renderer.hpp"
#include "glash/Renderer/Renderer2D.hpp"

#include "glash/ImGui/ImGuiLayer.hpp"

//remove this
#include "GLFW/glfw3.h"

namespace Cine
{
	Application* Application::s_Instance = nullptr;

	Application& Application::Get()
	{
		return *s_Instance;
	}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	Application::Application(int dummy)
		: m_Running(false)
	{
		CINE_LOG_TRACE(BUILD_STR);
		(void)dummy; //get rid of unused parameter warning

		CINE_CORE_ASSERT(s_Instance == nullptr, "Application should be singleton");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(CINE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		Renderer2D::Init();

		RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 0.15 });

		Cine::debug::InitializeOpenGLDebug();

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

		dispatcher.Dispatch<WindowCloseEvent>(CINE_BIND_EVENT_FN(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(CINE_BIND_EVENT_FN(Application::OnWindowResizeEvent));
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
		 if (event.GetWidth() == 0 && event.GetHeight() == 0)
		 {
			 m_Minimized = true;
		 }
		 else
		 {
			 m_Minimized = false;
		 }
		return false;
	}

	void Application::Run()
	{
		m_Running = true;
		
		while (m_Running)
		{
			float time = glfwGetTime();
			Timestep deltaTime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				m_Window->SetTitle(std::to_string(deltaTime.Milleseconds()));
				m_TickAccumulator += deltaTime;

				while (m_TickAccumulator > m_TickTime)
				{
					for (Layer* layer : m_LayerStack)
					{
						layer->OnFixedUpdate(m_TickTime);
					}
					m_TickAccumulator -= m_TickTime;
				}

				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(deltaTime);
				}

				Input::ClearKeyStates();
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

