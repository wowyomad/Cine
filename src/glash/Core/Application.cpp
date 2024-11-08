#include "glash/glash_pch.hpp"
#include "glash/Core/Application.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/RenderCommand.hpp"
#include "glash/Renderer/Renderer.hpp"
#include "glash/Renderer/Renderer2D.hpp"

#include "glash/ImGui/ImGuiLayer.hpp"

#include "glash/Scene/AssetManager.hpp"

//remove this
#include "GLFW/glfw3.h"

namespace Cine
{
	namespace Internal
	{

	}
	Application* Application::s_Instance = nullptr;

	Application& Application::Get()
	{
		return *s_Instance;
	}
	void Application::Set(Application* ptr)
	{
		if (!s_Instance)
		{
			s_Instance = ptr;
		}
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
		AssetManager::Init();


		RenderCommand::SetClearColor({ 0.15, 0.15, 0.15, 0.15 });

		Cine::debug::InitializeOpenGLDebug();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& event)
	{
		CINE_PROFILE_SCOPE("Application::OnEvent");

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
		Close();

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
				m_TickAccumulator += deltaTime;
				{
					CINE_PROFILE_SCOPE("LayerStack FixedUpdate");

					while (m_TickAccumulator > m_TickTime)
					{
						for (Layer* layer : m_LayerStack)
						{
							layer->OnFixedUpdate(m_TickTime);
						}
						m_TickAccumulator -= m_TickTime;
					}
				}

				{
					CINE_PROFILE_SCOPE("LayerStack Update");

					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(deltaTime);
					}
				}

				Internal::Input::ClearStates();
			}

#if CINE_IMGUI
			{
				if (m_UpdateUI)
				{
					CINE_PROFILE_SCOPE("ImGui Render");

					m_ImGuiLayer->Begin();
					for (Layer* layer : m_LayerStack)
					{
						layer->OnImGuiRender();
					}
					m_ImGuiLayer->End();
				}
				
			}
#endif
			if (m_UpdateUI)
			{
				m_Window->OnUpdate();
			}
			
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	ImGuiContext* Application::GetImGuiContext() const
	{
		return ImGui::GetCurrentContext();
	}

	void Application::GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_func, ImGuiMemFreeFunc* p_free_func, void** p_user_data) const
	{
		ImGui::GetAllocatorFunctions(p_alloc_func, p_free_func, p_user_data);
	}
	void Application::SetUpdateUI(bool update)
	{
		m_UpdateUI = update;
	}

}

