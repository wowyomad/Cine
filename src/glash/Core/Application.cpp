#include "glash/glash_pch.hpp"
#include "glash/Core/Application.hpp"

#include "glash/Core/Log.hpp"

#include "glash/Renderer/RendererAPI.hpp"

#include "glash/events/KeyEvent.hpp"
#include "glash/events/ApplicationEvent.hpp"
#include "glash/events/MouseEvent.hpp"

#include "glash/ImGui/ImGuiLayer.hpp"

namespace glash
{
	Application* Application::s_Instance = nullptr;

	Application& Application::GetAPI()
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

		m_ImGuiLayer = new ImGuiLayer();

		PushOverlay(m_ImGuiLayer);

		float verticesSquare[] = {
			-0.75f, -0.75f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.75f,  0.75f, 0.0f,		1.0f, 0.0f, 0.0f,
			-0.75f,  0.75f, 0.0f,		1.0f, 0.0f, 0.0f,
		};
		float verticesTriangle[] = {
			-0.5f, -0.5f, 0.0f,		1.0f, 0.5f, 0.5f,
			 0.5f, -0.5f, 0.0f,		0.5f, 1.0f, 0.5f,
			 0.0f,  0.5f, 0.0f,		0.5f, 0.5f, 1.0f,
		};
		unsigned int indicesSquare[] = {
			0, 1, 2,
			2, 3, 0
		};
		unsigned int indicesTriangle[] = {
			0, 1, 2,
		};

		uint32_t vertexBuffer;
		m_VertexArrayTriangle = VertexArray::Create();
		m_VertexArraySquare = VertexArray::Create();
		
		auto SquareVertexBuffer = VertexBuffer::Create(verticesSquare, sizeof(verticesSquare));
		auto SquareIndexBuffer = IndexBuffer::Create(indicesSquare, sizeof(indicesSquare) / sizeof(float));

		auto TriangleVertexBuffer = VertexBuffer::Create(verticesTriangle, sizeof(verticesTriangle));
		auto TriangleIndexBuffer = IndexBuffer::Create(indicesTriangle, sizeof(indicesTriangle) / sizeof(float));

		BufferLayout SquareLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color" }
		};
		SquareVertexBuffer->SetLayout(SquareLayout);
	
		BufferLayout TriangleLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Color" }
		};

		SquareVertexBuffer->SetLayout(SquareLayout);
		TriangleVertexBuffer->SetLayout(TriangleLayout);

		m_VertexArraySquare->AddVertexBuffer(SquareVertexBuffer);
		m_VertexArraySquare->SetIndexBuffer(SquareIndexBuffer);
		m_VertexArrayTriangle->AddVertexBuffer(TriangleVertexBuffer);
		m_VertexArrayTriangle->SetIndexBuffer(TriangleIndexBuffer);

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

			m_VertexArraySquare->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArraySquare->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (void*)0);

			m_VertexArrayTriangle->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArrayTriangle->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, (void*)0);

			

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

