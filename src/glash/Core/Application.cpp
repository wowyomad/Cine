#include "glash/glash_pch.hpp"
#include "glash/Core/Application.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/Renderer.hpp"

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

	Application::Application(int dummy)
		: m_Running(false)
	{
		GLASH_LOG_TRACE(BUILD_STR);
		(void)dummy; //get rid of unused parameter warning

		GLASH_CORE_ASSERT(s_Instance == nullptr, "Application should be singleton");
		s_Instance = this;

		m_Window = std::unique_ptr<GLASH_WINDOW_CLASS>(Window::Create());
		m_Window->SetEventCallback(GLASH_BIND_EVENT_FN(Application::OnEvent));

		m_Renderer = CreateRef<Renderer>(RendererAPI::OpenGL);

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		const uint64_t stride = 6 * sizeof(float);
		float vertices[6 * 3] ={
			-0.5f, -0.5f, 0.0f,	0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
			0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
		};
		unsigned int indices[3] = {
			0, 1, 2 
		};

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);


		uint32_t vertexBuffer;

		m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(float));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)12);
	

		m_Shader = Shader::Create("resources/shaders/shader.shader");

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
			glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, (void*)0);

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

